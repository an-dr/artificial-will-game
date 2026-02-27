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
#include <string>

#include "../Point2d.hpp"

namespace will_engine
{
    struct ComponentTexture
    {
        std::string name;
        Point2d rect_a;
        Point2d rect_b;
    };
}
