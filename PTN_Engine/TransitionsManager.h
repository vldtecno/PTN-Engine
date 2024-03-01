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

#include "PTN_Engine/Transition.h"
#include "PTN_Engine/ManagerBase.h"
#include <shared_mutex>

namespace ptne
{
using SharedPtrTransition = std::shared_ptr<Transition>;

//!
//! \brief The TransitionsManager class
//!
class TransitionsManager : private ManagerBase<Transition>
{
public:
	//!
	//! \brief contains
	//! \param itemName
	//! \return
	//!
	bool contains(const std::string &itemName) const;

	//!
	//! \brief Insert a new transition.
	//! \param transition
	//!
	void insert(std::shared_ptr<Transition> transition);

	//!
	//! \brief clear
	//!
	void clear();

	//!
	//! \brief collectEnabledTransitionsRandomly
	//! \return A vector of weak pointers to the enabled transitions.
	//!
	std::vector<std::weak_ptr<Transition>> collectEnabledTransitionsRandomly() const;

	//!
	//! \brief hasTransition
	//! \param name
	//! \return
	//!
	bool hasTransition(const std::string &name) const;

	//!
	//! \brief getTransition
	//! \param transitionName
	//! \return
	//!
	SharedPtrTransition getTransition(const std::string &transitionName) const;

	//!
	//! \brief getTransitionsProperties
	//! \return
	//!
	std::vector<TransitionProperties> getTransitionsProperties() const;

	//!
	//! \brief getArcsProperties
	//! \return
	//!
	std::vector<std::vector<ArcProperties>> getArcsProperties() const;

private:
	//!
	//! Shared mutex to synchronize the access to the items(readers-writer lock).
	//!
	mutable std::shared_mutex m_itemsMutex;
};

} // namespace ptne
