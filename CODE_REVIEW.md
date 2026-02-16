# Code Review - Full Codebase 2026-02-16

## Summary
An early-stage 2D game engine ("WillEngine") wrapping SDL2, with animation, input, and rendering. The codebase is small (~200 LOC) but has **3 critical resource leak bugs**, several Rule-of-Five violations that will cause crashes on copy, frame-rate-dependent gameplay, and misleading API naming. box2d is linked but unused.

## Critical Issues

- **Renderer.hpp: no destructor** - `SDL_Renderer*` is never destroyed. Every time a `Renderer` is created and goes out of scope, the GPU resource leaks permanently.
  - Impact: GPU resource leak, eventual resource exhaustion in long sessions
  - Fix:
  ```cpp
  ~Renderer() {
      if (renderer_) SDL_DestroyRenderer(renderer_);
  }
  ```

- **Texture.hpp: no destructor** - `SDL_Texture*` is never destroyed. Every loaded texture leaks.
  - Impact: VRAM leak, one per texture load
  - Fix:
  ```cpp
  ~Texture() {
      if (texture_) SDL_DestroyTexture(texture_);
  }
  ```

- **Window.hpp / Renderer.hpp / Texture.hpp: Rule-of-Five violations** - `Window` has a destructor but no deleted/defined copy/move operations. `Renderer` and `Texture` have neither. If any of these are accidentally copied, you get double-free crashes or dangling pointers.
  - Impact: Undefined behavior on copy; `const auto window = Window(...)` in main.cpp:37 works only because of copy elision (guaranteed in C++17) but the class is still unsafe
  - Fix: Delete copy, define move for all three:
  ```cpp
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&& other) noexcept : window_(std::exchange(other.window_, nullptr)) {}
  Window& operator=(Window&&) = delete;
  ```

## High Priority

- **main.cpp:64-81 / Input.hpp:40-50** - Movement is not delta-time based. `pos.x += 2` runs once per frame, so speed is directly proportional to frame rate. At 60fps VSync = 120px/s; at 144Hz = 288px/s.
  - Why: Gameplay will feel completely different on different monitors
  - Suggestion: Track `SDL_GetTicks64()` delta and multiply:
  ```cpp
  auto now = SDL_GetTicks64();
  float dt = (now - lastTick) / 1000.0f;
  lastTick = now;
  // ...
  pos.x += static_cast<int>(speed * dt);
  ```

- **Animation.hpp:36-42** - Animation frame advancement is frame-rate dependent (`frame_counter_++` per render frame). Animation will play 2.4x faster on a 144Hz monitor vs 60Hz.
  - Why: Same root cause as movement - no delta time
  - Suggestion: Use elapsed time instead of frame counting

- **Animation.hpp:29** - Constructor parameter `fps` is assigned to `frames_per_a_frame`, but `fps` means "frames per second" to any reader. The actual semantic is "render frames to wait before advancing one animation frame" - the exact opposite of what `fps` implies.
  - Why: A caller passing `60` thinking "60 fps animation" would get an animation that advances once every 60 render frames (1 frame per second at 60Hz)
  - Suggestion: Rename to `frame_delay` or `frames_per_animation_step`, or better yet, accept actual FPS and compute the delay internally

- **Window.hpp:29 / Renderer.hpp:33** - Constructors call `SDL_Quit()` on failure before throwing. A constructor should not tear down global state it doesn't own.
  - Why: If anything else depends on SDL being initialized, it's now broken. The caller (main) should decide cleanup policy.
  - Suggestion: Just `throw` without calling `SDL_Quit()`. Let main handle cleanup:
  ```cpp
  if (renderer_ == nullptr) {
      throw std::runtime_error("Cannot create a renderer: " + std::string(SDL_GetError()));
  }
  ```

- **Input.hpp:31** - `get()` returns `std::optional<InputEvent>` but never returns `std::nullopt`. It always returns a value (including `InputEvent::None`). The optional wrapper is meaningless.
  - Why: Misleading API - caller checks `.has_value()` in main.cpp:64 but it's always true
  - Suggestion: Return `InputEvent` directly, or use `std::nullopt` for the "no input" case instead of `InputEvent::None`

- **Input.hpp:40-50** - Single-direction-only input. The `if/else if` chain means pressing Down+Right gives only Down. Diagonal movement is impossible, and priority (Down > Left > Right > Up) is arbitrary.
  - Why: Standard 2D movement should support simultaneous axes
  - Suggestion: Return a velocity vector or a bitmask of active directions

## Modernization Opportunities

- **Drawable.hpp:26 / Texture.hpp:22** - Raw `SDL_Texture*` with no ownership semantics. In C++23 you should use RAII wrappers. A `std::unique_ptr` with a custom deleter is idiomatic:
  - Modern approach:
  ```cpp
  struct SdlTextureDeleter {
      void operator()(SDL_Texture* t) const { SDL_DestroyTexture(t); }
  };
  using UniqueTexture = std::unique_ptr<SDL_Texture, SdlTextureDeleter>;
  ```
  - Why it matters: Eliminates entire class of resource leak bugs

- **Size.hpp / Position.hpp** - These are plain aggregates that could be `struct Size { int width_x; int height_y; int depth_z = 0; };` with no user-declared constructors, gaining aggregate initialization, structured bindings, and `operator<=>` for free in C++23.
  - Modern approach:
  ```cpp
  struct Position {
      int x = 0;
      int y = 0;
      int z = 0;
  };
  // Usage: Position{100, 200} works via aggregate init
  ```
  - Why it matters: Less boilerplate, compiler-generated special members, designated initializers (`Position{.x=100, .y=200}`)

- **main.cpp:1** - Uses SDL2 (`#include <SDL.h>`). SDL3 has been stable since 2025, with better API design, GPU API, and modern C practices. Not urgent, but worth planning for.
  - Why it matters: SDL2 is now in maintenance mode

- **main.cpp:4** - `#include <print>` is included but never used. This is a C++23 feature - if you have it available, use `std::println` instead of `spdlog` for simple console output, or remove the include.

- **Drawable.hpp:34** - `friend class Renderer` breaks encapsulation. Renderer only calls `draw()` which is public. The friend declaration is unnecessary.
  - Modern approach: Remove `friend class Renderer;` - it grants access to all private members when none is needed

## Improvements

- **main.cpp:33** - Error reporting uses `std::cerr` for SDL_Init failure but `spdlog` everywhere else. At this point spdlog is already initialized (line 30), so use it consistently:
  ```cpp
  spdlog::critical("SDL_Init Error: {}", SDL_GetError());
  ```

- **Texture.hpp:28** - Error message `"No image"` is unhelpful. Include the file path and SDL error:
  ```cpp
  throw std::runtime_error("Failed to load texture '" + file_path + "': " + IMG_GetError());
  ```

- **Texture.hpp:33** - `getSdlTexture()` should be `const`:
  ```cpp
  [[nodiscard]] SDL_Texture* getSdlTexture() const { return texture_; }
  ```

- **Drawable.hpp:70** - `draw()` is virtual but `Drawable` lacks a virtual destructor... wait, it has `virtual ~Drawable() = default;` on line 45. Good. But `draw()` should probably be pure virtual or at least the base `Drawable` class should document that it's meant to be a concrete class too.

- **main.cpp:52-57** - The event loop handles only `SDL_QUIT`. Common events like `SDL_KEYDOWN` for escape-to-quit are missing. Currently the only way to close is the window X button.

## Technical Debt

- **box2d dependency** - box2d is in `vcpkg.json`, linked in `CMakeLists.txt`, and `#include`d in main.cpp but never used anywhere. It adds build time and binary size for zero benefit.
  - Reason: Likely planned for future physics integration
  - Future work: Remove until actually needed, or add a TODO in CMakeLists.txt

- **Header-only engine** - All WillEngine classes are defined entirely in `.hpp` files with no `.cpp` files. This is fine at current scale but will cause recompilation of everything on any header change as the project grows.
  - Reason: Acceptable for a small prototype
  - Future work: Split into headers and implementation files when the engine grows

- **No game loop timing** - No fixed timestep, no interpolation, no frame cap (beyond VSync). Physics and gameplay are entirely tied to render rate.
  - Reason: Early prototype stage
  - Future work: Implement a proper fixed-timestep game loop (e.g., "Fix Your Timestep" pattern)

- **Drawable ownership model** - `Drawable` holds a raw `SDL_Texture*` it doesn't own. Lifetime coupling between `Texture` and `Drawable` is implicit and enforced only by programmer discipline. If `Texture` dies first, `Drawable` has a dangling pointer.
  - Reason: Simple initial design
  - Future work: Use `std::shared_ptr` for shared textures, or a resource manager with handles

- **cmake-build-profile-arm64-debug in repo** - The `.gitignore` covers `cmake-build-*` but the `cmake-build-profile-arm64-debug/vcpkg_installed/` directory appears to exist in the working tree. Verify it's not tracked.

## Positives

- **Clean namespace usage** - All engine types are in `will_engine` namespace, preventing global pollution
- **`#pragma once`** on all headers - consistent include guard strategy
- **`[[nodiscard]]`** on `getSdlRenderer()` and trailing return type on `getSdlWindow()` - good use of modern attributes where applied
- **spdlog integration** - Proper dual-sink logging (console + file) from the start is mature infrastructure thinking
- **vcpkg for dependencies** - Reproducible dependency management with a pinned baseline
- **`const` parameters in constructors** - Consistent use of `const` for value parameters shows intent
- **Separation of Window/Renderer/Texture/Drawable** - Even at this early stage, the decomposition into focused classes is clean and follows single-responsibility

## Verdict
- [ ] Approved - ship it
- [ ] Approved with minor comments
- [x] Changes required
- [ ] Major rework needed

The resource leaks (Renderer, Texture destructors) and Rule-of-Five violations are real bugs that will cause problems. Fix those first. The delta-time issue is the next priority since it makes the game unplayable at non-60Hz refresh rates. The API naming (`fps` parameter) should be addressed before more code is built on top of it. The architecture is clean for an early prototype - the foundation is solid once the RAII issues are resolved.