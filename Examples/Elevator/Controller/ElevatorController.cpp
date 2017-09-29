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
	m_minimumFloor(s_bottomFloor-1),
	m_maximumFloor(s_topFloor+1),
	m_goingUp(false),
	m_goingDown(false),
	m_destinations1(),
	m_destinations2(),
	m_waitingToGoUp(),
	m_waitingToGoDown()
{}

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

void ElevatorController::mergeWaitingToGoUp()
{
	m_destinations1.insert(m_waitingToGoUp.begin(), m_waitingToGoUp.end());
	m_waitingToGoUp.clear();
}

void ElevatorController::mergeWaitingToGoDown()
{
	m_destinations1.insert(m_waitingToGoDown.begin(), m_waitingToGoDown.end());
	m_waitingToGoDown.clear();
}

void ElevatorController::mergeMaxWaitingToGoDown()
{
	if (m_waitingToGoDown.empty())
	{
		return;
	}
	auto it = max_element(m_waitingToGoDown.begin(), m_waitingToGoDown.end());
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
	auto it = min_element(m_waitingToGoUp.begin(), m_waitingToGoUp.end());
	const int minFloor = *it;	
	m_destinations1.insert(minFloor);
	m_waitingToGoUp.erase(it);
}

//Actions
void ElevatorController::addDestination1()
{
	if (m_destinations2.find(m_toAddToDestination) != m_destinations2.end())
	{
		m_destinations2.erase(m_toAddToDestination);
	}
	if (m_waitingToGoDown.find(m_toAddToDestination) != m_waitingToGoDown.end())
	{
		m_waitingToGoDown.erase(m_toAddToDestination);
	}
	if (m_waitingToGoUp.find(m_toAddToDestination) != m_waitingToGoUp.end())
	{
		m_waitingToGoUp.erase(m_toAddToDestination);
	}

	m_destinations1.insert(m_toAddToDestination);

	printDestinations();
}

void ElevatorController::addDestination2()
{
	if (m_destinations1.find(m_toAddToDestination) != m_destinations1.end())
	{
		return;
	}

	if (m_waitingToGoDown.find(m_toAddToDestination) != m_waitingToGoDown.end())
	{
		m_waitingToGoDown.erase(m_toAddToDestination);
	}
	if (m_waitingToGoUp.find(m_toAddToDestination) != m_waitingToGoUp.end())
	{
		m_waitingToGoUp.erase(m_toAddToDestination);
	}

	m_destinations2.insert(m_toAddToDestination);
	
	printNextDestinations();
}

void ElevatorController::addWaitingToGoDown()
{
	if (m_destinations1.find(m_toAddToDestination) != m_destinations1.end())
	{
		return;
	}

	m_waitingToGoDown.insert(m_toAddToDestination);
	printWaitingGoDown();
}

void ElevatorController::addWaitingToGoUp()
{
	if (m_destinations1.find(m_toAddToDestination) != m_destinations1.end())
	{
		return;
	}

	m_waitingToGoUp.insert(m_toAddToDestination);
	printWaitingGoUp();
}

void ElevatorController::removeDestination()
{
	auto it = m_destinations1.find(m_currentFloor);
	if (it != m_destinations1.end())
	{
		m_destinations1.erase(it);
	}
	//printDestinations();
}

void ElevatorController::rotateLists()
{
	swap(m_destinations1, m_destinations2);
	
	//printDestinations();
	//printNextDestinations();
}

void ElevatorController::processWaitingToGoUp()
{
	mergeMaxWaitingToGoDown();
	mergeWaitingToGoUp();

	if (!m_destinations1.empty())
	{
		m_minimumFloor = *min_element(m_destinations1.begin(), m_destinations1.end());
	}

	//printDestinations();
}

void ElevatorController::processWaitingToGoDown()
{
	mergeMinWaitingToGoUp();
	mergeWaitingToGoDown();

	if (!m_destinations1.empty())
	{
		m_maximumFloor = *max_element(m_destinations1.begin(), m_destinations1.end());
	}

	//printDestinations();
}

void ElevatorController::increaseFloor()
{
	++m_currentFloor;
}

void ElevatorController::decreaseFloor()
{
	--m_currentFloor;
}

void ElevatorController::setMinimum()
{
	if (m_toAddToDestination < m_minimumFloor || m_minimumFloor == s_bottomFloor-1)
	{
		m_minimumFloor = m_toAddToDestination;
	}
}

void ElevatorController::setMaximum()
{
	if (m_toAddToDestination > m_maximumFloor || m_maximumFloor == s_topFloor+1)
	{
		m_maximumFloor = m_toAddToDestination;
	}
}

void ElevatorController::resetMinimum()
{
	m_minimumFloor = s_bottomFloor - 1;
}

void ElevatorController::resetMaximum()
{
	m_maximumFloor = s_topFloor + 1;
}

bool ElevatorController::isFloorNotInList() const
{
	return !isFloorInList();
}

bool ElevatorController::isFloorInList() const
{
	return m_destinations1.find(m_currentFloor) != m_destinations1.cend();
}

bool ElevatorController::isDestinationListNotEmpty() const
{
	return !isDestinationListEmpty();
}

bool ElevatorController::hasDestinationGreaterThanCurrent() const
{
	return isDestinationListNotEmpty() && m_currentFloor <= *min_element(m_destinations1.begin(), m_destinations1.end());
}

bool ElevatorController::hasDestinationSmallerThanCurrent() const
{
	return isDestinationListNotEmpty() && m_currentFloor >= *max_element(m_destinations1.begin(), m_destinations1.end());
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

bool ElevatorController::isGreaterThanMinimumFloor() const
{
	return m_toAddToDestination > m_minimumFloor;
}

bool ElevatorController::isSmallerThanMaximumFloor() const
{
	return m_toAddToDestination < m_maximumFloor;
}

bool ElevatorController::conditionAux1_T1() const
{
	return isMarkedFloorGreaterThanCurrentFloor() &&
		isSmallerThanMaximumFloor();
}

bool ElevatorController::conditionAux1_T2() const
{
	return !conditionAux1_T1();
}

bool ElevatorController::conditionAux2_T1() const
{
	return isMarkedFloorSmallerThanCurrentFloor() &&
		isGreaterThanMinimumFloor();
}

bool ElevatorController::conditionAux2_T2() const
{
	return !conditionAux2_T1();
}

bool ElevatorController::conditionAux3_T1() const
{
	return isMarkedFloorGreaterThanCurrentFloor() &&
		isSmallerThanMaximumFloor();
}

bool ElevatorController::conditionAux3_T2() const
{
	return !conditionAux3_T1();
}

bool ElevatorController::conditionAux4_T1() const
{
	return isMarkedFloorSmallerThanCurrentFloor() &&
		isGreaterThanMinimumFloor();
}

bool ElevatorController::conditionAux4_T2() const
{
	return !conditionAux4_T1();
}

//Info

void ElevatorController::elevatorStopped()
{
	cout << "Elevator stopped" << endl;
	printDestinations();
	printNextDestinations();
}

void ElevatorController::elevatorMoving()
{
	cout << "Elevator moving" << endl;
	printDestinations();
	printNextDestinations();
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
	printCurrentFloor();
}

void ElevatorController::goingUp()
{
	cout << "Going up " << endl;
}

void ElevatorController::goingDown()
{
	cout << "Going down " << endl;
}

void ElevatorController::printCurrentFloor() const
{
	cout << "Current floor: " << m_currentFloor << endl;
}

void ElevatorController::printDestinations() const
{
	cout << "Destinations: ";
	printFloorList(m_destinations1);
}

void ElevatorController::printNextDestinations() const
{
	cout << "Next travel destinations: ";
	printFloorList(m_destinations2);
}

void ElevatorController::printWaitingGoDown() const
{
	cout << "Floors with people waiting to go down: ";
	printFloorList(m_waitingToGoDown);
}

void ElevatorController::printWaitingGoUp() const
{
	cout << "Floors with people waiting to go down: ";
	printFloorList(m_waitingToGoUp);
}

void ElevatorController::printFloorList(const unordered_set<int>& floors) const
{
	if (floors.empty())
	{
		cout << "Empty" << endl;
		return;
	}
	for (const auto& d : floors)
	{
		cout << d << " ";
	}
	cout << endl;
}


