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

#pragma once

namespace will_engine {
    struct Position {
        int x;
        int y;
        int z;

        Position(const int x, const int y, const int z) : x(x), y(y), z(z) {
        }

        Position(const int x, const int y) : x(x), y(y), z(0) {
        }
    };
}
