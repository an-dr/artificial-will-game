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

#include "BaseSystem.hpp"

namespace will_engine {
class BaseStateMashine : public BaseSystem {
    entt::entity entity_;

public:
    BaseStateMashine() = delete;
    explicit BaseStateMashine(entt::entity id) : entity_(id) {}

    auto getEntittyId() const { return entity_; }
    virtual auto tick() -> void = 0;
};
}  // namespace will_engine