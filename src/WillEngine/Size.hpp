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
    struct Size {
        int width_x;
        int height_y;
        int depth_z;

        Size(const int w_x, const int h_y) : width_x(w_x), height_y(h_y), depth_z(0) {
        }

        Size(const int w_x, const int h_y, const int d_z) : width_x(w_x), height_y(h_y), depth_z(d_z) {
        }
    };
}
