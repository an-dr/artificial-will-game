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

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


namespace will_engine {

struct ComponentTexture {
    std::string name;
    glm::ivec2 frame_position;
    glm::ivec3 frame_size;
    unsigned int frames_total;
    float frame_current; // float to progress smoothly
    unsigned int fps;
};

} // will_engine