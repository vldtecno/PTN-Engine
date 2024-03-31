/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
 * Copyright (c) 2021 Kale Evans
 * Copyright (c) 2024 Eduardo Valgôde
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "PTN_Engine/ManagerBase.h"
#include "PTN_Engine/Transition.h"
#include <shared_mutex>

namespace ptne
{

using SharedPtrTransition = std::shared_ptr<Transition>;
using WeakPtrTransition = std::weak_ptr<Transition>;

//!
//! \brief The TransitionsManager contains and manages all the transitions.
//!
class TransitionsManager : private ManagerBase<Transition>
{
public:
	~TransitionsManager() = default;
	TransitionsManager() = default;
	TransitionsManager(const TransitionsManager &) = delete;
	TransitionsManager(TransitionsManager &&) = delete;
	TransitionsManager &operator=(const TransitionsManager &) = delete;
	TransitionsManager &operator=(TransitionsManager &&) = delete;

	std::vector<std::vector<ArcProperties>> getArcsProperties() const;

	//!
	//! \brief Remove all transitions from the container.
	//!
	void clear();

	//!
	//! \brief collectEnabledTransitionsRandomly
	//! \return A vector of weak pointers to the enabled transitions.
	//!
	std::vector<WeakPtrTransition> collectEnabledTransitionsRandomly() const;

	bool contains(const std::string &itemName) const;

	SharedPtrTransition getTransition(const std::string &transitionName) const;

	std::vector<TransitionProperties> getTransitionsProperties() const;

	bool hasTransition(const std::string &name) const;

	void insert(std::shared_ptr<Transition> transition);

private:
	//! Shared mutex to synchronize the access to the items(readers-writer lock).
	mutable std::shared_mutex m_itemsMutex;
};

} // namespace ptne
