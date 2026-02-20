// *************************************************************************
//
// Copyright (c) 2026 Andrei Gramakov. All rights reserved.
//
// This file is licensed under the terms of the MIT license.
// For a copy, see: https://opensource.org/licenses/MIT
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

/*

✅ Window owns Renderer - SDL2 requires this, destroyed together
✅ Systems are objects - structs with state, not free functions
✅ NO system inheritance - no ISystem base class, no polymorphism
✅ Different system signatures - each takes what it needs
✅ Game::run() encapsulates loop - main.cpp just creates Game and calls run()
✅ World owns registry - all entity data lives here
✅ Components are POD - plain structs, no methods that modify state

main.cpp
cppint main(int argc, char* argv[]) {
    try {
        Game game;
        game.run();  // That's it
    } catch (const std::exception& e) {
        // Error handling
        return 1;
    }
    return 0;
}
 *
 */

#pragma once
#include <memory>
#include "Window.hpp"
#include "systems/Input.hpp"
#include "systems/Renderer.hpp"


namespace will_engine {
    class Game {
        Window window_;

        Input sys_input_;
        Renderer sys_renderer_;
    };
} // will_engine
