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

#pragma once

#include "PTN_Engine/Transition.h"
#include <shared_mutex>

namespace ptne
{
class IExporter;

/*!
 * \brief The TransitionsManagedContainer class
 */
class TransitionsManagedContainer
{
public:
	TransitionsManagedContainer() = default;
	~TransitionsManagedContainer() = default;
	TransitionsManagedContainer(const TransitionsManagedContainer &) = delete;
	TransitionsManagedContainer(TransitionsManagedContainer &&) = delete;
	TransitionsManagedContainer &operator=(const TransitionsManagedContainer &) = delete;
	TransitionsManagedContainer &operator=(const TransitionsManagedContainer &&) = delete;

	/*!
	 * \brief Insert a new transition.
	 * \param transition
	 */
	void insert(std::shared_ptr<Transition> transition);

	/*!
	 * \brief collectEnabledTransitionsRandomly
	 * \return A vector of weak pointers to the enabled transitions.
	 */
	std::vector<std::weak_ptr<Transition>> collectEnabledTransitionsRandomly() const;

	/*!
	 * \brief clear
	 */
	void clear();

	/*!
	 * \brief export_
	 * \param exporter
	 * \param places
	 * \param conditions
	 */
	void export_(IExporter &exporter) const;

private:
	/*!
	 * Shared mutex to synchronize the access to the transitions(readers-writer lock).
	 */
	mutable std::shared_mutex m_transitionsMutex;

	/*!
	 * \brief Vector with the transitions.
	 */
	std::vector<std::shared_ptr<Transition>> m_transitions;
};

} // namespace ptne
