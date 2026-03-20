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
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


namespace will_engine::shapes {

struct Rect {
    glm::ivec2 a, b;
};

struct Box {
    glm::ivec3 a, b;
};

}  // namespace will_engine::shapes
