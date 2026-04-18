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

#include <utility>
#include <boost/sml.hpp>
#include "BaseSystem.hpp"

namespace will_engine {

class IStateMachine : public BaseSystem {
public:
    virtual ~IStateMachine() = default;
    virtual auto getEntityId() -> entt::entity = 0;
    virtual auto tick() -> void = 0;
};

template <typename TransitionTable>
class BaseStateMachine : public IStateMachine {
    entt::entity entity_;
    boost::sml::sm<TransitionTable> sm_;

protected:
    template <typename TEvent>
    auto process(TEvent event) -> void {
        sm_.process_event(event);
    }

    template <typename TState>
    [[nodiscard]] auto isState() const -> bool {
        return sm_.is(boost::sml::state<TState>);
    }

public:
    BaseStateMachine() = delete;
    explicit BaseStateMachine(entt::entity id)
        : entity_(id), sm_(static_cast<IStateMachine &>(*this)) {}
    BaseStateMachine(entt::entity id, TransitionTable table)
        : entity_(id), sm_(std::move(table), static_cast<IStateMachine &>(*this)) {}
    ~BaseStateMachine() override = default;

    auto getEntityId() -> entt::entity override { return entity_; }
};
}  // namespace will_engine
