/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2024 Eduardo Valgôde
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

#include "PTN_Engine/PTN_Engine.h"
#include <atomic>
#include <functional>
#include <shared_mutex>

namespace ptne
{

class IPTN_EnginePlace;

//!
//! \brief Implements a place of a Petri net.
//!
class Place final
{
public:
	using ActionFunction = std::function<void(void)>;

	~Place();
	Place(IPTN_EnginePlace &parent, const PlaceProperties &placeProperties);
	Place(const Place &) = delete;
	Place(Place &&) = delete;
	Place &operator=(Place &) = delete;
	Place &operator=(Place &&) = delete;

	//!
	//! \brief Whether an on enter action can be triggered or not.
	//! \param value
	//!
	void blockStartingOnEnterActions(const bool value);

	//!
	//! \brief Increase number of tokens and call on enter action.
	//! \param tokens - number of tokens to increase.
	//!
	void enterPlace(const size_t tokens = 1);

	//!
	//! \brief Decrease number of tokens and call on exit action.
	//! \param tokens - number of tokens to decrease.
	//!
	void exitPlace(const size_t tokens = 1);

	//!
	//! \brief getName
	//! \return place name
	//!
	std::string getName() const;

	//!
	//! Return the number of tokens.
	//! \return The number of tokens in the place.
	//!
	size_t getNumberOfTokens() const;

	//!
	//! \brief getOnEnterActionName
	//! \return The label name of the on enter action.
	//!
	std::string getOnEnterActionName() const;

	//!
	//! \brief getOnExitActionName
	//! \return the label name of the on exit action.
	//!
	std::string getOnExitActionName() const;

	//!
	//! If the place is flagged as input.
	//! \return If the place is an input place.
	//!
	bool isInputPlace() const;

	//!
	//! \brief Tells if an "onEnter" action, belonging to the place, is being executed.
	//! \return true if an "onEnter" action being executed.
	//!
	bool isOnEnterActionInExecution() const;

	//!
	//! \brief placeProperties
	//! \return
	//!
	PlaceProperties placeProperties() const;

	//!
	//! Set the number of tokens in the place.
	//! \param tokens Number of tokens to be set.
	//!
	void setNumberOfTokens(const size_t tokens);

private:
	//!
	//! Decrease number of tokens in the place.
	//! \param tokens Number of tokens to be removed. Must be at least 1.
	//!
	void decreaseNumberOfTokens(const size_t tokens = 1);

	//!
	//! \brief Execute the action according to the configuration.
	//! \param action to be executed
	//!
	void executeAction(const ActionFunction &action, std::atomic<size_t> &);

	//!
	//! Increase number of tokens in the place.
	//! \param tokens Number of tokens to be added. Must be at least 1.
	//!
	void increaseNumberOfTokens(const size_t tokens = 1);

	//! Reference to the PTN Engine instance, to which the place belongs.
	IPTN_EnginePlace &m_ptnEngine;

	//! Flag to block triggering on enter actions.
	std::atomic<bool> m_blockStartingOnEnterActions = false;

	//! Flag that determines if the place can be added tokens from outside the net.
	bool m_isInputPlace = false;

	//! Shared mutex to synchronize calls, allowing simultaneous reads (readers-writer lock).
	mutable std::shared_mutex m_mutex;

	//! Name of the place used to identify it.
	std::string m_name;

	//! Number of tokens in the place.
	size_t m_numberOfTokens = 0;

	//! Function to be called when a token enters the place.
	const ActionFunction m_onEnterAction = nullptr;

	//! A label for the on enter action.
	std::string m_onEnterActionName;

	//! Counter of on enter functions being executed.
	std::atomic<size_t> m_onEnterActionsInExecution = 0;

	//! Function to be called when a token leaves the place.
	const ActionFunction m_onExitAction = nullptr;

	//! A label for the on exite action.
	std::string m_onExitActionName;

	//! Counter of on exit functions being executed.
	std::atomic<size_t> m_onExitActionsInExecution = 0;
};

} // namespace ptne
