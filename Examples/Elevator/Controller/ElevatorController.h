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
#include <unordered_set>


class IElevatorPetriNet;

//! Example of a controller class
/*!
 *
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
	int m_toAddToDestination;
	
	bool m_goingUp;
	bool m_goingDown;

	std::unordered_set<int> m_destinations1;
	std::unordered_set<int> m_destinations2;
	std::unordered_set<int> m_waitingToGoUp;
	std::unordered_set<int> m_waitingToGoDown;

	//! The state machine of the controller.
	PtrPetriNet m_pPetriNet;

	
	//Actions

	void removeDestinationGU();
	void removeDestinationGD();
	void removeDestination();
	void removeWaitingToGoDown();
	void removeWaitingToGoUp();
	void rotateLists();

	void addDestination1();
	void addDestination2();
	void addWaitingToGoDown();
	void addWaitingToGoUp();
	void increaseFloor();
	void decreaseFloor();

	void mergeGoingUpGTCurrent();
	void mergeMinGoingUp();
	void mergeMaxGoingDown();
	void mergePostponedToCurrent();
	void mergeGoingDownSTCurrent();

	

	
	//info
	void processedLists();
	void elevatorStopped();
	void elevatorMoving();
	void doorsAreOpen();
	void doorsAreClosed();
	void goingUp();
	void goingDown();

	void printCurrentFloor() const;
	void printDestinations() const;
	void printNextDestinations() const;
	void printWaitingGoDown() const;
	void printWaitingGoUp() const;
	void printFloorList(const std::unordered_set<int>& floors) const;
	void printSchedule() const;



	//Conditions
	bool isFloorNotInList() const;
	bool isFloorInList() const;
	bool isDestinationListNotEmpty() const;
	bool isDestinationListEmpty() const;
	bool isMarkedFloorNotCurrentFloor() const;
	bool isMarkedFloorCurrentFloor() const;
	bool isMarkedFloorGreaterThanCurrentFloor() const;
	bool isMarkedFloorSmallerThanCurrentFloor() const;
	bool isMinSmallerThanCurrent() const;
	bool isMinGreaterThanCurrent() const;
	bool isMaxSmallerThanCurrent() const;
	bool isMaxGreaterThanCurrent() const;

};

template class ptne::Action<ElevatorController>;

//!
using ControllerAction = ptne::Action<ElevatorController>;
using ControllerActionPtr = std::shared_ptr<ControllerAction>;

template class ptne::ActivationCondition<ElevatorController>;

//!
using FireCondition = ptne::ActivationCondition<ElevatorController>;

