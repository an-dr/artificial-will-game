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
#include <cstdint>

namespace will_engine {
    struct Size {
        uint32_t width_x;
        uint32_t height_y;
        uint32_t depth_z;

        Size(const uint32_t w_x, const uint32_t h_y) : width_x(w_x), height_y(h_y), depth_z(0) {
        }

        Size(const uint32_t w_x, const uint32_t h_y, const uint32_t d_z) : width_x(w_x), height_y(h_y), depth_z(d_z) {
        }
    };
}
