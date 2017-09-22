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


class IElevatorPetriNet;

//! Example of a controller class
/*!
 * Controls the display of a simplified hypothetical phone's menus,
 * using only three keys.
 */
class ElevatorController: public std::enable_shared_from_this<ElevatorController>
{
public:

	//! Constructor.
	ElevatorController();

	~ElevatorController();

	//! Necessary to have an initialize method at this point.
	/*!
	 * Passes a shared pointer from "this" to the PTN Engine net.
	 * "shared_from_this" cannot be called in the constructor.
	 */
	void initialize();	

	void openDoors();

	void closeDoors();

	bool callElevatorUp(const int floor);

	bool callElevatorDown(const int floor);

	bool setDestinationFloor(const int floor);

	static const int s_bottomFloor = 0;
	static const int s_topFloor = 10;



private:

	//! Base (nested) class for a Petri net based state machine.
	class IControllerPetriNet;

	//! One concrete (nested) class of a Petri net based state machine.
	class MenuStateMachine;


	using PtrPetriNet = std::unique_ptr<IElevatorPetriNet>;

	//! The state machine of the controller.
	PtrPetriNet m_pPetriNet;


};

template class ptne::Action<ElevatorController>;

//!
using Action = ptne::Action<ElevatorController>;

template class ptne::ActivationCondition<ElevatorController>;

//!
using FireCondition = ptne::ActivationCondition<ElevatorController>;

