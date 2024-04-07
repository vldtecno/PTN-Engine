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

#include "PTN_Engine/TransitionsManager.h"
#include "PTN_Engine/Transition.h"
#include <algorithm>
#include <mutex>
#include <random>

namespace ptne
{
using namespace std;

TransitionsManager::~TransitionsManager() = default;
TransitionsManager::TransitionsManager() = default;

bool TransitionsManager::contains(const string &itemName) const
{
	shared_lock itemsGuard(m_itemsMutex);
	return ManagerBase<Transition>::contains(itemName);
}

void TransitionsManager::insert(shared_ptr<Transition> transition)
{
	unique_lock itemsGuard(m_itemsMutex);
	ManagerBase<Transition>::insert(transition);
}

void TransitionsManager::clear()
{
	unique_lock itemsGuard(m_itemsMutex);
	ManagerBase<Transition>::clear();
}

vector<weak_ptr<Transition>> TransitionsManager::collectEnabledTransitionsRandomly() const
{
	shared_lock transitionsGuard(m_itemsMutex);

	vector<weak_ptr<Transition>> enabledTransitions;
	for (const auto &[_, transition] : m_items)
	{
		if (transition->isEnabled())
		{
			enabledTransitions.push_back(transition);
		}
	}

	// TO DO check performance
	random_device randomDevice;
	mt19937_64 seed(randomDevice());
	ranges::shuffle(enabledTransitions, seed);
	return enabledTransitions;
}

SharedPtrTransition TransitionsManager::getTransition(const string &transitionName) const
{
	shared_lock itemsGuard(m_itemsMutex);
	return ManagerBase<Transition>::getItem(transitionName);
}

vector<TransitionProperties> TransitionsManager::getTransitionsProperties() const
{
	shared_lock itemsGuard(m_itemsMutex);
	vector<TransitionProperties> transitionsProperties;
	for (const auto &[_, transition] : m_items)
	{
		transitionsProperties.push_back(transition->getTransitionProperties());
	}
	return transitionsProperties;
}

} // namespace ptne
