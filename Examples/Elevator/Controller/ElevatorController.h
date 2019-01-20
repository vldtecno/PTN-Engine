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

#include <memory>
#include <unordered_set>


class IElevatorPetriNet;

//! Elevator controller class
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

	//! Command to open elevator doors.
	void openDoors();

	//! Command to close elevator doors.
	void closeDoors();

	//! Command to call elevator to go up from a given floor.
	bool callElevatorUp(const int floor);

	//! Command to call elevator to go down from a given floor.
	bool callElevatorDown(const int floor);

	//! Command to press a floor button in the elevator.
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

	//! Floor where the elevator currently is.
	int m_currentFloor;

	//! Floor pressed in the button.
	int m_toAddToDestination;

	//! List of destinations in the current travel.
	std::unordered_set<int> m_destinations;
	
	//! List of destinations for the next travel.
	std::unordered_set<int> m_nextTravelDestinations;

	//! List of destinations that want to catch the elevator to go up.
	std::unordered_set<int> m_floorsWaitingToGoUp;

	//! List of destinations that want to catch the elevator to go down.
	std::unordered_set<int> m_floorsWaitingToGoDown;

	//! Merge set to m_destinations and delete the merged items.
	void mergeToDestinations(std::unordered_set<int>& toAdd, const bool lessThan);

	//! Remove current floor from m_floorsWaitingToGoDown
	void removeCurrentFromWaitingToGoDown();

	//! Remove current floor from m_floorsWaitingToGoUp
	void removeCurrentFromWaitingToGoUp();

	//Information
	void printCurrentFloor() const;
	void printDestinations() const;
	void printNextDestinations() const;
	void printWaitingGoDown() const;
	void printWaitingGoUp() const;
	void printFloorList(const std::unordered_set<int>& floors) const;
	void printSchedule() const;
	
	////////////////////
	//Methods used by the petri net
	
	//Actions
	void removeDestinationGU();
	void removeDestinationGD();
	void removeDestination();
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
	void processedLists();
	void elevatorStopped();
	void elevatorMoving();
	void doorsAreOpen();
	void doorsAreClosed();
	void goingUp();
	void goingDown();

	//Conditions
	bool isFloorNotInList() const;
	bool isFloorInList() const;
	bool isDestinationListNotEmpty() const;
	bool isDestinationListEmpty() const;
	bool isMarkedFloorNotCurrentFloor() const;
	bool isMarkedFloorGreaterThanCurrentFloor() const;
	bool isMarkedFloorSmallerThanCurrentFloor() const;
	bool isMinSmallerThanCurrent() const;
	bool isMinGreaterThanCurrent() const;
	bool isMaxSmallerThanCurrent() const;
	bool isMaxGreaterThanCurrent() const;

};

