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
#include <entt/entity/registry.hpp>


namespace will_engine {

class BaseSystem {

private:
    entt::registry *registry_ = nullptr;  // All entities/components here


public:
    BaseSystem() = default;

    [[nodiscard]] auto getRegistry() const -> entt::registry * { return registry_; }
    auto setRegistry(entt::registry *registry) -> void { registry_ = registry; }
    [[nodiscard]] auto isRegisterSet() const -> bool { return registry_ != nullptr; }
};

}  // namespace will_engine
