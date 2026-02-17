# Code Review - Full Codebase (2026-02-17)

## 🎯 Summary

C++20 SDL2 game engine bootstrapping with Window, Renderer, Texture, Drawable/Animation
hierarchy, and keyboard Input. The architecture is clean and simple for the project stage.
Two critical bugs are present: `IMG_Init` is never called (silent texture-load failure) and
all SDL resource wrappers violate the Rule of Five (double-free on copy). Four more high-priority
issues cover dead code, misnamed parameters, misleading API, and animation state being mutated
inside `draw()`.

---

## ⚠️ Critical Issues

- **`main.cpp:34`** — `IMG_Init()` is never called before `IMG_LoadTexture()`
  - Impact: PNG loading silently returns null on platforms that require explicit format
    initialization, causing a throw in `Texture::Texture()` with no indication of why.
  - Fix:
    ```cpp
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);  // add after SDL_Init
    // ... at teardown:
    IMG_Quit();
    SDL_Quit();
    ```

- **`Renderer.hpp:22`, `Window.hpp:22`, `Texture.hpp:16`** — Rule of Five violated on all SDL resource wrappers
  - Impact: Compiler-generated copy constructor shallow-copies the raw pointer. Any copy of
    `Renderer`, `Window`, or `Texture` leads to a double-free (undefined behaviour / crash) when
    both objects are destroyed.
  - Fix: delete copy and move in all three classes:
    ```cpp
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&)                 = delete;
    Renderer& operator=(Renderer&&)      = delete;
    ```
    Apply identically to `Window` and `Texture`.

---

## 🔧 High Priority

- **`Animation.hpp:38`** — `draw()` mutates animation state (mixing update and render)
  - Why: Calling `draw()` twice in one frame (e.g., for debug overlays, multi-pass rendering)
    double-advances the animation. The renderer should not be responsible for advancing game state.
  - Suggestion: Separate update from render.
    ```cpp
    auto update() -> void {
        frame_counter_++;
        if (frame_counter_ >= frames_per_a_frame) {
            current_a_frame_ = (current_a_frame_ + 1) % frames_;
            frame_counter_ = 0;
        }
        setFrameAtTexture(current_a_frame_ * animation_frame_size_.width_x, 0);
    }

    auto draw(SDL_Renderer* renderer) -> void override {
        Drawable::draw(renderer);
    }
    ```
    Call `player->update()` in the game loop before `renderer.draw(*player)`.

- **`Drawable.hpp:21-25`, `Drawable.hpp:34`** — `DrawableType` enum and `type_` field are dead code
  - Why: `type_` is written in constructors but never read anywhere. The `friend class Renderer`
    declaration exists but `Renderer` never accesses private members directly — it only calls the
    public `draw()`. Both are noise that will mislead future readers.
  - Suggestion: Remove the enum, the `type_` field, and the `friend` declaration entirely.

- **`Animation.hpp:28`** — `fps` parameter name is wrong
  - Why: `fps` conventionally means frames per second. Here it means "render frames to wait before
    advancing one animation frame" — the opposite concept. Passing `18` with the name `fps` implies
    18 FPS, but it means "advance every 18 render frames."
  - Suggestion: Rename to `frame_duration` or `ticks_per_frame`:
    ```cpp
    Animation(SDL_Texture* texture, const int frames, const int ticks_per_frame,
              const Size frame_size, const Size& size, const Position& position)
    ```

- **`main.cpp:50-53`** — `player_pos` is a duplicate of state already inside `player`
  - Why: Position is stored in both `player_pos` (a local `Position`) and inside the `player`
    object's `location_frame_`. They can diverge. `setPosition` is called unconditionally every
    frame even when no input occurred.
  - Suggestion: Remove `player_pos` and expose a `move(dx, dy)` method on `Drawable`, or read the
    position back from the object:
    ```cpp
    case InputEvent::Up:   player->move(0, -2); break;
    case InputEvent::Down: player->move(0,  2); break;
    // ...
    // no setPosition call needed after the switch
    ```

---

## 🚀 Modernization Opportunities

- **`Renderer.hpp`, `Window.hpp`, `Texture.hpp`** — Raw owning pointers instead of `unique_ptr` with custom deleter
  - Modern approach: express sole ownership with the type system:
    ```cpp
    struct SdlRendererDeleter {
        void operator()(SDL_Renderer* r) const { SDL_DestroyRenderer(r); }
    };
    std::unique_ptr<SDL_Renderer, SdlRendererDeleter> renderer_;
    ```
    The destructor, copy-delete, and move semantics are then handled automatically — Rule of Five
    for free.
  - Why it matters: Eliminates the manual destructor, removes the Rule of Five violation, and
    makes ownership semantically explicit.

- **`Input.hpp:29`** — `std::optional<InputEvent>` with no `std::nullopt` path
  - Modern approach: The function always returns a value. Either remove the `optional`:
    ```cpp
    auto get() -> InputEvent;
    ```
    Or return `std::nullopt` when no key is pressed and remove `InputEvent::None`:
    ```cpp
    auto get() -> std::optional<InputEvent> {
        if (movingDown)  return InputEvent::Down;
        if (movingLeft)  return InputEvent::Left;
        if (movingRight) return InputEvent::Right;
        if (movingUp)    return InputEvent::Up;
        return std::nullopt;
    }
    ```
  - Why it matters: `optional` signals "may have no value." Wrapping a value that always exists
    defeats the purpose and forces callers to unwrap for no reason.

- **`Position.hpp`, `Size.hpp`** — Aggregate structs without constructors in C++20
  - Modern approach: Use designated initialisers consistently instead of positional construction:
    ```cpp
    auto player_pos = Position{.x = 100, .y = 100};
    auto frame_size = Size{.width_x = 64, .height_y = 64};
    ```
    This removes the ambiguity from `Size(0, 0)` (is the missing `depth_z` intentional?) and makes
    all call sites self-documenting.
  - Why it matters: Designated initialisers are C++20, this project already uses C++20 (`<print>`
    is included).

- **`main.cpp:4-5`** — `#include <print>` included but `std::print` never used; `<box2d/box2d.h>` included but unused
  - Modern approach: Remove dead includes. If `std::print` is intended to replace `std::cerr`,
    finish the migration:
    ```cpp
    std::print(stderr, "SDL_Init Error: {}\n", SDL_GetError());
    ```
  - Why it matters: Unused includes increase compilation time and confuse readers about
    dependencies.

---

## 💡 Improvements

- **`Drawable.hpp:65-69`** — `setFrameAtTexture(x, y)` (2-arg overload) only sets position, not size
  - Better approach: The name implies setting a full frame region but only half the rect is set.
    Rename the 2-arg version to `setTextureOrigin(x, y)` to make the distinction clear.

- **`Texture.hpp:25`** — `getSdlTexture()` is not `const`-qualified
  - Better approach:
    ```cpp
    auto getSdlTexture() const -> SDL_Texture* { return texture_; }
    ```
    Without `const`, a `const Texture&` cannot provide its texture to the renderer.

- **`Texture.hpp:20`** — Error message omits the file path
  - Better approach:
    ```cpp
    throw std::runtime_error("Failed to load texture: " + file_path + " — " + IMG_GetError());
    ```

- **`Animation.hpp:22-24`** — Inconsistent member naming
  - `frames_` uses `trailing_underscore_` convention; `current_a_frame`, `frame_counter_`, and
    `frames_per_a_frame` do not. Apply trailing underscores uniformly:
    `current_a_frame_`, `frame_counter_`, `frames_per_a_frame_`.

- **`main.cpp:28`** — `using namespace will_engine` at file scope
  - Limits flexibility (name conflicts if a third-party library also has `Window`, `Renderer`,
    etc.). Prefix explicitly or limit to function scope.

- **`main.cpp:2`** — Stale `// Add this` comment on `#include <SDL_image.h>`
  - Remove it; it was a TODO that has been resolved.

---

## 📝 Technical Debt

- **Game loop / timing** — Movement is `+= 2` per frame, tied entirely to vsync (60 Hz assumed)
  - Reason: Acceptable while only one platform and one refresh rate are targeted.
  - Future work: Introduce a `deltaTime` (seconds since last frame) and multiply all velocities by
    it. Consider a fixed-timestep accumulator pattern for physics.

- **All engine code is header-only**
  - Reason: Convenient for a small single-target project.
  - Future work: As the engine grows, move implementations to `.cpp` files to reduce recompilation
    cascades. Consider precompiled headers for the SDL/spdlog includes.

- **Input system returns one event per frame, no diagonal movement**
  - Reason: Priority chain is the simplest implementation.
  - Future work: Return a `std::bitset<4>` or a small action-set to support simultaneous
    directions and multi-action input.

- **`Position::z` and `Size::depth_z` are carried everywhere but unused**
  - Reason: Possibly reserved for 3D or z-ordering.
  - Future work: Either use them (z-ordering for draw calls) or remove them to keep the 2D API
    clean. If kept, document the intent.

---

## ✅ Positives

- **RAII wrappers for all SDL resources** — `Window`, `Renderer`, and `Texture` each own their SDL
  object and destroy it in the destructor. No SDL resource is left floating in `main`.

- **`[[nodiscard]]` on all getters** — `getSdlRenderer()`, `getSdlWindow()` are correctly marked,
  preventing silent discard of returned resources.

- **`shared_ptr` for texture lifetime** — Textures are kept alive via `shared_ptr` and the raw
  pointer is shared down to `Drawable`. This prevents use-after-free if a texture outlives its
  drawable.

- **`spdlog` with both console and file sinks from the start** — Logging infrastructure is in
  place before the engine is complex. This pays dividends during debugging.

- **`virtual draw()` with correct `override`** — The polymorphic render interface is set up
  correctly and the `override` keyword is present, so any future signature mismatch in a subclass
  will be a compile error.

- **`namespace will_engine` on all engine code** — Clean separation from user code and third-party
  libraries.

---

## 🏁 Verdict

- [ ] Approved - ship it
- [ ] Approved with minor comments
- [x] Changes required
- [ ] Major rework needed

Fix the two critical bugs (`IMG_Init` and Rule of Five) before this runs reliably on any target.
The high-priority items (update/draw separation, dead code, `fps` rename, duplicated position
state) are straightforward and clean up the API surface before it grows. Modernization items
(`unique_ptr` deleters, proper `optional` usage, designated initialisers) are low-effort and
significantly improve correctness guarantees.