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

#include "BaseStateMachine.hpp"
#include "BaseSystem.hpp"

namespace will_engine {
class SystemState : public BaseSystem {

    std::vector<std::unique_ptr<IStateMachine>> state_machines_;

public:
    SystemState() = default;

    void add(std::unique_ptr<IStateMachine> machine) {
        machine->setRegistry(getRegistry());
        state_machines_.push_back(std::move(machine));
    }

    void setStateMachineRegistry() {
        for (auto &sm : state_machines_) {
            sm->setRegistry(getRegistry());
        }
    }

    void process() const {
        for (auto &sm : state_machines_) {
            sm->tick();
        }
    }
};
}  // namespace will_engine
