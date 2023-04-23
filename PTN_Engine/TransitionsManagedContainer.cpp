/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
 * Copyright (c) 2021 Kale Evans
 * Copyright (c) 2023 Eduardo Valgôde
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

#include "PTN_Engine/TransitionsManagedContainer.h"
#include "PTN_Engine/IExporter.h"
#include "PTN_Engine/Transition.h"
#include <mutex>
#include <random>

namespace ptne
{
using namespace std;

vector<weak_ptr<Transition>> TransitionsManagedContainer::collectEnabledTransitionsRandomly() const
{
	shared_lock<shared_mutex> transitionsGuard(m_transitionsMutex);
	vector<weak_ptr<Transition>> enabledTransitions;
	for (const auto &transition : m_transitions)
	{
		if (transition->isEnabled())
		{
			enabledTransitions.push_back(transition);
		}
	}

	// TODO check performance
	random_device randomDevice;
	mt19937_64 seed(randomDevice());
	shuffle(enabledTransitions.begin(), enabledTransitions.end(), seed);
	return enabledTransitions;
}

void TransitionsManagedContainer::export_(IExporter &exporter) const
{
	shared_lock<shared_mutex> transitionsGuard(m_transitionsMutex);
	for (const auto &transition : m_transitions)
	{
		transition->export_(exporter);
	}
}

void TransitionsManagedContainer::insert(shared_ptr<Transition> transition)
{
	unique_lock<shared_mutex> transitionsGuard(m_transitionsMutex);
	m_transitions.emplace_back(transition);
}

void TransitionsManagedContainer::clear()
{
	unique_lock<shared_mutex> transitionsGuard(m_transitionsMutex);
	m_transitions.clear();
}

} // namespace ptne
