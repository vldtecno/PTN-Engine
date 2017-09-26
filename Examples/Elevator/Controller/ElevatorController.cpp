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

#include "Controller/ElevatorController.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include "Controller/ElevatorPetriNet.h"

#include <iostream>
#include <algorithm>

using namespace std;

ElevatorController::ElevatorController():
	m_pPetriNet{nullptr},
	m_currentFloor(s_bottomFloor),
	m_toAddToDestination(s_bottomFloor),
	m_goingUp(false),
	m_goingDown(false),
	m_destinations1(),
	m_destinations2(),
	m_waitingToGoUp(),
	m_waitingToGoDown()
{

}

ElevatorController::~ElevatorController()
{}

void ElevatorController::initialize()
{
	if(m_pPetriNet)
	{
		return;
	}

	m_pPetriNet = move(
		PtrPetriNet(new ElevatorPetriNet(shared_from_this()))
		);
}

void ElevatorController::openDoors()
{
	if (!m_pPetriNet)
	{
		throw runtime_error("Petri net not available.");
	}
	m_pPetriNet->openDoors();
}

void ElevatorController::closeDoors()
{
	if (!m_pPetriNet)
	{
		throw runtime_error("Petri net not available.");
	}
	m_pPetriNet->closeDoors();
}


bool ElevatorController::callElevatorUp(const int floor)
{
	if (floor < s_bottomFloor || floor >= s_topFloor)
	{
		return false;
	}
	if (!m_pPetriNet)
	{
		throw runtime_error("Petri net not available.");
	}
	m_toAddToDestination = floor;
	m_pPetriNet->callButtonUp();
	return true;
}

bool ElevatorController::callElevatorDown(const int floor)
{
	if (floor <= s_bottomFloor || floor > s_topFloor)
	{
		return false;
	}
	if (!m_pPetriNet)
	{
		throw runtime_error("Petri net not available.");
	}
	m_toAddToDestination = floor;
	m_pPetriNet->callButtonDown();
	return true;
}

bool ElevatorController::setDestinationFloor(const int floor)
{
	if (floor < s_bottomFloor || floor > s_topFloor)
	{
		return false;
	}
	if (!m_pPetriNet)
	{
		throw runtime_error("Petri net not available.");
	}
	m_toAddToDestination = floor;
	m_pPetriNet->destinationButton();
	return true;
}

//
//
//bool ElevatorController::dlCallElevatorUp(const int floor)
//{
//	bool addedDestination = false;
//	if (m_goingUp && !m_goingDown)
//	{
//		if (floor > m_currentFloor)
//		{
//			addedDestination = addDestination1(floor);
//		}
//		else if (floor < m_currentFloor)
//		{
//			addedDestination = addWaitingToGoUp(floor);
//		}
//	}
//	else if (m_goingDown && !m_goingUp)
//	{
//		addedDestination = addDestination2(floor);
//	}
//	else if (!m_goingUp && !m_goingDown)
//	{
//		addedDestination = addDestination1(floor);
//	}
//	else
//	{
//		throw runtime_error("Messed up.");
//	}
//	return addedDestination;
//
//}
//
//bool ElevatorController::dlCallElevatorDown(const int floor)
//{
//	bool addedDestination = false;
//	if (m_goingDown && !m_goingUp)
//	{
//		if (floor < m_currentFloor)
//		{
//			addedDestination = addDestination1(floor);
//		}
//		else if (floor > m_currentFloor)
//		{
//			addedDestination = addWaitingToGoDown(floor);
//		}
//	}
//	else if (m_goingUp && !m_goingDown)
//	{
//		addedDestination = addDestination2(floor);
//	}
//	else if (!m_goingUp && !m_goingDown)
//	{
//		addedDestination = addDestination1(floor);
//	}
//	else
//	{
//		throw runtime_error("Messed up.");
//	}
//	return addedDestination;
//
//}
//
//
//bool ElevatorController::dlSetDestinationFloor(const int floor)
//{
//	bool addedDestination = false;
//	if (m_goingUp && !m_goingDown)
//	{
//		if (floor > m_currentFloor)
//		{
//			addedDestination = addDestination1(floor);
//		}
//		else if (floor < m_currentFloor)
//		{
//			addedDestination = addDestination2(floor); //no replace
//		}
//	}
//	else if (m_goingDown && !m_goingUp)
//	{
//		if (floor < m_currentFloor)
//		{
//			addedDestination = addDestination1(floor);
//		}
//		else if (floor > m_currentFloor)
//		{
//			addedDestination = addDestination2(floor); //no replace
//		}
//	}
//	else if (!m_goingUp && !m_goingDown)
//	{
//		addedDestination = addDestination1(floor);
//	}
//	else
//	{
//		throw runtime_error("Messed up.");
//	}
//	return addedDestination;
//
//}
//
//void ElevatorController::dlArrivedFloor(const int floor)
//{
//	m_destinations1.erase(floor);
//	if (list1IsEmpty())
//	{
//		m_endTravel = true;
//	}
//}
//
//
//void ElevatorController::dlGoUp()
//{
//	m_goingUp = true;
//	m_endTravel = false;
//
//	if (list1IsEmpty())
//	{
//		rotateLists();
//	}
//
//	mergeWaitingToGoUp();
//	mergeMaxWaitingToGoDown();
//
//
//
//}
//
//void ElevatorController::dlGoDown()
//{
//	m_goingDown = true;
//	m_endTravel = false;
//
//	if (list1IsEmpty())
//	{
//		rotateLists();
//	}
//
//	mergeWaitingToGoDown();
//	mergeMinWaitingToGoUp();
//
//
//}
//
//bool ElevatorController::addDestination1(const int floor)
//{
//	if (m_destinations1.find(floor) != m_destinations1.end())
//	{
//		m_destinations1.insert(floor);
//		return true;
//	}
//	return false;
//}
//
//bool ElevatorController::addDestination2(const int floor)
//{
//	if (m_destinations2.find(floor) != m_destinations2.end())
//	{
//		m_destinations2.insert(floor);
//		return true;
//	}
//	return false;
//}
//
//bool ElevatorController::addWaitingToGoDown(const int floor)
//{
//	if (m_waitingToGoDown.find(floor) != m_waitingToGoDown.end())
//	{
//		m_waitingToGoDown.insert(floor);
//		return true;
//	}
//	return false;
//}
//
//bool ElevatorController::addWaitingToGoUp(const int floor)
//{
//	if (m_waitingToGoUp.find(floor) != m_waitingToGoUp.end())
//	{
//		m_waitingToGoUp.insert(floor);
//		return true;
//	}
//	return false;
//}


void ElevatorController::mergeWaitingToGoUp()
{
	m_destinations1.insert(m_waitingToGoUp.begin(), m_waitingToGoUp.end());
}

void ElevatorController::mergeWaitingToGoDown()
{
	m_destinations1.insert(m_waitingToGoDown.begin(), m_waitingToGoDown.end());
}

void ElevatorController::mergeMaxWaitingToGoDown()
{
	if (m_waitingToGoDown.empty())
	{
		return;
	}
	auto it = max_element(m_destinations1.begin(), m_destinations1.end());
	const int maxFloor = *it;	
	m_destinations1.insert(maxFloor);
	m_waitingToGoDown.erase(it);
}

void ElevatorController::mergeMinWaitingToGoUp()
{
	if (m_waitingToGoUp.empty())
	{
		return;
	}
	auto it = min_element(m_destinations1.begin(), m_destinations1.end());
	const int minFloor = *it;	
	m_destinations1.insert(minFloor);
	m_waitingToGoUp.erase(it);
}



//Actions
void ElevatorController::addDestination1()
{
	if (m_destinations1.find(m_toAddToDestination) != m_destinations1.cend())
	{
		m_destinations1.insert(m_toAddToDestination);
	}
}

void ElevatorController::addDestination2()
{
	if (m_destinations2.find(m_toAddToDestination) != m_destinations2.cend())
	{
		m_destinations2.insert(m_toAddToDestination);
	}
}

void ElevatorController::addWaitingToGoDown()
{
	if (m_waitingToGoDown.find(m_toAddToDestination) != m_waitingToGoDown.cend())
	{
		m_waitingToGoDown.insert(m_toAddToDestination);
	}
}

void ElevatorController::addWaitingToGoUp()
{
	if (m_waitingToGoUp.find(m_toAddToDestination) != m_waitingToGoUp.cend())
	{
		m_waitingToGoUp.insert(m_toAddToDestination);
	}
}

void ElevatorController::removeDestination()
{
	auto it = m_destinations1.find(m_currentFloor);
	if (it != m_destinations1.end())
	{
		m_destinations1.erase(it);
	}
}

void ElevatorController::rotateLists()
{
	swap(m_destinations1, m_destinations2);
}

void ElevatorController::processWaitingToGoUp()
{
	mergeMaxWaitingToGoDown();
	mergeWaitingToGoUp();
}

void ElevatorController::processWaitingToGoDown()
{
	mergeMinWaitingToGoUp();
	mergeWaitingToGoDown();
}

void ElevatorController::increaseFloor()
{
	++m_currentFloor;
}

void ElevatorController::decreaseFloor()
{
	--m_currentFloor;
}


bool ElevatorController::isFloorNotInList() const
{
	return !isFloorInList();
}

bool ElevatorController::isFloorInList() const
{
	return m_destinations1.find(m_toAddToDestination) != m_destinations1.cend();
}

bool ElevatorController::isDestinationListNotEmpty() const
{
	return !isDestinationListEmpty();
}

bool ElevatorController::isDestinationListEmpty() const
{
	return m_destinations1.empty();
}

bool ElevatorController::isMarkedFloorNotCurrentFloor() const
{
	return !isMarkedFloorCurrentFloor();
}

bool ElevatorController::isMarkedFloorCurrentFloor() const
{
	return m_toAddToDestination == m_currentFloor;
}

bool ElevatorController::isMarkedFloorGreaterThanCurrentFloor() const
{
	return m_toAddToDestination > m_currentFloor;
}

bool ElevatorController::isMarkedFloorSmallerThanCurrentFloor() const
{
	return m_toAddToDestination < m_currentFloor;
}

//Info

void ElevatorController::elevatorStopped()
{
	cout << "Elevator stopped" << endl;
}

void ElevatorController::elevatorMoving()
{
	cout << "Elevator moving" << endl;
}

void ElevatorController::doorsAreOpen()
{
	cout << "Elevator doors are open" << endl;
}

void ElevatorController::doorsAreClosed()
{
	cout << "Elevator doors are closed" << endl;
}

void ElevatorController::arrivedFloor()
{
	cout << "Arrived floor: " << m_currentFloor << endl;
}

void ElevatorController::hasDestination()
{
	cout << "Elevator has destination" << endl;
}


