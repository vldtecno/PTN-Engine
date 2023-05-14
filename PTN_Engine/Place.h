/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2023 Eduardo Valgôde
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

#include <atomic>
#include <functional>
#include <memory>
#include <shared_mutex>

namespace ptne
{

class IExporter;
class IPTN_EnginePlace;

/*!
 * \brief Implements a place of a Petri net.
 */
class Place final
{
public:
	using ActionFunction = std::function<void(void)>;

	/*!
	 * Place constructor.
	 * \param initialNumberOfTokens The number of tokens the place contains originally.
	 * \param onEnterAction The action to be performed when a token enters the place.
	 * \param onExitAction The action to be performed when a token leaves the place.
	 * \param input Flag that marks the place as an input place.
	 */
	Place(IPTN_EnginePlace &parent,
		  std::string name,
		  const size_t initialNumberOfTokens,
		  ActionFunction onEnterAction,
		  ActionFunction onExitAction,
		  const bool input = false);

	/*!
	 * \brief Place
	 * \param parent
	 * \param name
	 * \param initialNumberOfTokens
	 * \param onEnterActionName
	 * \param onEnterAction
	 * \param onExitActionName
	 * \param onExitAction
	 * \param input
	 */
	Place(IPTN_EnginePlace &parent,
		  std::string name,
		  const size_t initialNumberOfTokens,
		  const std::string &onEnterActionName,
		  ActionFunction onEnterAction,
		  const std::string &onExitActionName,
		  ActionFunction onExitAction,
		  const bool input = false);

	~Place() = default;
	Place(const Place &) = delete;
	Place(Place &&) = delete;
	Place &operator=(Place &) = delete;
	Place &operator=(Place &&) = delete;

	/*!
	 * \brief getName
	 * \return place name
	 */
	std::string getName() const;

	/*!
	 * \brief Increase number of tokens and call on enter action.
	 * \param tokens - number of tokens to increase.
	 */
	void enterPlace(const size_t tokens = 1);

	/*!
	 * \brief Decrease number of tokens and call on exit action.
	 * \param tokens - number of tokens to decrease.
	 */
	void exitPlace(const size_t tokens = 1);

	/*!
	 * Set the number of tokens in the place.
	 * \param tokens Number of tokens to be set.
	 */
	void setNumberOfTokens(const size_t tokens);

	/*!
	 * Return the number of tokens.
	 * \return The number of tokens in the place.
	 */
	size_t getNumberOfTokens() const;

	/*!
	 * If the place is flagged as input.
	 * \return If the place is an input place.
	 */
	bool isInputPlace() const;

	/*!
	 * \brief getOnEnterActionName
	 * \return The label name of the on enter action.
	 */
	const std::string getOnEnterActionName() const;

	/*!
	 * \brief getOnExitActionName
	 * \return the label name of the on exit action.
	 */
	const std::string getOnExitActionName() const;

	/*!
	 * \brief Tells if an "onEnter" action, belonging to the place, is being executed.
	 * \return true if an "onEnter" action being executed.
	 */
	bool isOnEnterActionInExecution() const;

	/*!
	 * \brief Whether an on enter action can be triggered or not.
	 * \param value
	 */
	void blockStartingOnEnterActions(const bool value);

	/*!
	 * \brief Export a place.
	 * \param exporter - Object that can export a place.
	 */
	void export_(IExporter &exporter) const;

private:
	/*!
	 * Increase number of tokens in the place.
	 * \param tokens Number of tokens to be added. Must be at least 1.
	 */
	void increaseNumberOfTokens(const size_t tokens = 1);

	/*!
	 * Decrease number of tokens in the place.
	 * \param tokens Number of tokens to be removed. Must be at least 1.
	 */
	void decreaseNumberOfTokens(const size_t tokens = 1);

	/*!
	 * \brief Execute the action according to the configuration.
	 * \param action to be executed
	 */
	void executeAction(const ActionFunction &action, std::atomic<size_t> &);

	//! Reference to the PTN Engine instance, to which the place belongs.
	IPTN_EnginePlace &m_ptnEngine;

	//! Name of the place used to identify it.
	std::string m_name;

	//! A label for the on enter action.
	std::string m_onEnterActionName;

	//! Function to be called when a token enters the place.
	const ActionFunction m_onEnterAction;

	//! A label for the on exite action.
	std::string m_onExitActionName;

	//! Function to be called when a token leaves the place.
	const ActionFunction m_onExitAction;

	//! Number of tokens in the place.
	size_t m_numberOfTokens;

	//! Flag that determines if the place can be added tokens from outside the net.
	bool m_isInputPlace;

	//! Shared mutex to synchronize calls, allowing simultaneous reads (readers-writer lock).
	mutable std::shared_mutex m_mutex;

	//! Counter of on enter functions being executed.
	std::atomic<size_t> m_onEnterActionsInExecution = 0;

	//! Counter of on exit functions being executed.
	std::atomic<size_t> m_onExitActionsInExecution = 0;

	//! Flag to block triggering on enter actions.
	std::atomic<bool> m_blockStartingOnEnterActions = false;
};

} // namespace ptne
