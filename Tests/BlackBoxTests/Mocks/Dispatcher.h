/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
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

#include "PTN_Engine/ActivationCondition.h"
#include "PTN_Engine/Action.h"


//! Mock/fake controller class
/*!
 * This is the controller of a package dispatcher that
 * has two channels available: A and B.
 * When a package arrives it can either use one or the other.
 * Required to inherit from std::enable_shared_from_this, in order
 * to provide the PTN Engine with a shared pointer to the controller.
 */
class Dispatcher: public std::enable_shared_from_this<Dispatcher>
{
private:

	//! Interface for Petri nets controlling the logic of this dispatcher
	class IDispatcherPetriNet;

	//! Alternately uses channel a and b to dispatch packages.
	class RoundRobinPetriNet;

	//! Randomly choses a channel to dispatch a package.
	class FreeChoicePetriNet;

	//! Petri net using weighted arcs.
	class WeightedPetriNet;

	//! For testing purposes only
	friend class FixturePetriNet;

	//! Action to be performed when waiting a package.
	void actionWaitPackage();

	//! Action to be performed when using channel A.
	void actionChannelA();

	//! Action to be performed when using channel B.
	void actionChannelB();

	//! Action to be performed when channel A is selected.
	void actionSelectA();

	//! Action to be performed when channel B is selected.
	void actionSelectB();

	//! Action to be performed when no longer waiting a package.
	void onLeaveWaitPackage();

	//! Action to be performed when no longer on channel A.
	void onLeaveChannelA();

	//! Action to be performed when no longer on channel B.
	void onLeaveChannelB();

	//! Action to be performed when channel A is not longer selected.
	void onLeaveSelectChannelA();

	//! Action to be performed when channel B is no longer selected.
	void onLeaveSelectChannelB();

	/*!
	 * If the counter can be reset.
	 * \return if the counter can be reset.
	 */
	bool resetCounter() const;

	using PtrRoundRobinPetriNet = std::unique_ptr<RoundRobinPetriNet>;
	using PtrFreeChoicePetriNet = std::unique_ptr<FreeChoicePetriNet>;
	using PtrWeightedPetriNet = std::unique_ptr<WeightedPetriNet>;


	//! Petri net that defines and controls the dispatcher business logic.
	std::unique_ptr<IDispatcherPetriNet> m_pPetriNet;

	//! If the counter can be reset.
	bool m_resetCounter;

public:

	Dispatcher();

	~Dispatcher();

	//! Necessary to have an initialize method at this point.
	/*!
	 * Passes a shared pointer from "this" to the PTN Engine net.
	 * "shared_from_this" cannot be called in the constructor.
	 */
	void initialize();

	/*!
	 * Set the flag to reset the counter.
	 * \param resetCounter Reset counter on or off.
	 */
	void setResetCounter(const bool resetCounter);

	//! Puts a token in the input place and triggers the net to perform one dispatch operation.
	void dispatch();

	//! Flag that indicates it is waiting for a package.
	bool m_isWaitingPackage;

	//! Flag that indicates it is using channel A.
	bool m_isUsingChannelA;

	//! Flag that indicates it is using channel B.
	bool m_isUsingChannelB;

	//! Flag that indicates channel A is selected.
	bool m_isChannelASelected;

	//! Flag that indicates channel B is selected.
	bool m_isChannelBSelected;

	//! Select round robin Petri net.
	void setRoundRobinMode();

	//! Select free choice Petri net.
	void setFreeChoiceMode();

	//! Select weighted Petri net.
	void setWeightedPN();

};

template class ptne::Action<Dispatcher>;
using DispatcherAction = ptne::Action<Dispatcher>;

template class ptne::ActivationCondition<Dispatcher>;
using DispatcherFireCondition = ptne::ActivationCondition<Dispatcher>;

#include "Mocks/RoundRobinPetriNet.h"
#include "Mocks/FreeChoicePetriNet.h"
#include "Mocks/WeightedPetriNet.h"
