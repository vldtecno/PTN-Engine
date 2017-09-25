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
#include <vector>
#include <set>


class IElevatorPetriNet;

//! Example of a controller class
/*!
 * Controls the display of a simplified hypothetical phone's menus,
 * using only three keys.
 */
class ElevatorController: public std::enable_shared_from_this<ElevatorController>
{
	friend class ElevatorPetriNet;

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

	int m_currentFloor;
	int m_destinationFloor;
	int m_upCallerFloor;
	int m_downCallerFloor;
	
	
	bool m_goingUp;
	bool m_goingDown;
	bool m_endTravel;

	std::set<int> m_destinations1;
	std::set<int> m_destinations2;
	std::set<int> m_waitingToGoUp;
	std::set<int> m_waitingToGoDown;

	bool dlCallElevatorUp(const int floor);
	bool dlCallElevatorDown(const int floor);
	bool dlSetDestinationFloor(const int floor);
	void dlArrivedFloor(const int floor);
	void dlGoUp();
	void dlGoDown();

	bool addDestination1(const int floor);
	bool addDestination2(const int floor);
	bool addWaitingToGoDown(const int floor);
	bool addWaitingToGoUp(const int floor);
	bool list1IsEmpty() const;

	void rotateLists();
	void mergeWaitingToGoUp();
	void mergeWaitingToGoDown();
	void mergeMaxWaitingToGoDown();
	void mergeMinWaitingToGoUp();


	//! The state machine of the controller.
	PtrPetriNet m_pPetriNet;

	
	//Actions





	//Conditions


};

template class ptne::Action<ElevatorController>;

//!
using Action = ptne::Action<ElevatorController>;

template class ptne::ActivationCondition<ElevatorController>;

//!
using FireCondition = ptne::ActivationCondition<ElevatorController>;

