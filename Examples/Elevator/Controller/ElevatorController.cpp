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
	cout << "Added destination"<< m_toAddToDestination << endl;
	m_destinations1.insert(m_toAddToDestination);

	cout << "Destinations: ";
	for (const auto& d : m_destinations1)
	{
		cout << d << ", " ;
	}
	cout << endl;
}

void ElevatorController::addDestination2()
{
	cout << "Added next destination" << m_toAddToDestination << endl;
	m_destinations2.insert(m_toAddToDestination);

	cout << "Next destinations: ";
	for (const auto& d : m_destinations2)
	{
		cout << d << ", ";
	}
	cout << endl;
}

void ElevatorController::addWaitingToGoDown()
{
	cout << "Added waiting to go down: " << m_toAddToDestination << endl;
	m_waitingToGoDown.insert(m_toAddToDestination);

	cout << "Waiting to go down: ";
	for (const auto& d : m_waitingToGoDown)
	{
		cout << d << ", ";
	}
	cout << endl;
}

void ElevatorController::addWaitingToGoUp()
{
	cout << "Added waiting to go up: " << m_toAddToDestination << endl;

	m_waitingToGoUp.insert(m_toAddToDestination);

	cout << "Waiting to go up: ";
	for (const auto& d : m_waitingToGoUp)
	{
		cout << d << ", ";
	}
	cout << endl;
}

void ElevatorController::removeDestination()
{
	cout << "Removed destination: " << m_currentFloor << endl;

	auto it = m_destinations1.find(m_currentFloor);
	if (it != m_destinations1.end())
	{
		m_destinations1.erase(it);
	}

	cout << "Destinations: ";
	for (const auto& d : m_destinations1)
	{
		cout << d << ", ";
	}
	cout << endl;
}

void ElevatorController::rotateLists()
{
	cout << "Rotated lists " << endl;
	swap(m_destinations1, m_destinations2);
	cout << "Destinations: ";
	for (const auto& d : m_destinations1)
	{
		cout << d << ", ";
	}
	cout << endl;
	cout << "Next destinations: ";
	for (const auto& d : m_destinations2)
	{
		cout << d << ", ";
	}	
	cout << endl;
}

void ElevatorController::processWaitingToGoUp()
{
	cout << "processWaitingToGoUp" << endl;
	mergeMaxWaitingToGoDown();
	mergeWaitingToGoUp();

	cout << "Destinations: ";
	for (const auto& d : m_destinations1)
	{
		cout << d << ", ";
	}
	cout << endl;
}

void ElevatorController::processWaitingToGoDown()
{
	cout << "processWaitingToGoDown" << endl;
	mergeMinWaitingToGoUp();
	mergeWaitingToGoDown();

	cout << "Destinations: ";
	for (const auto& d : m_destinations1)
	{
		cout << d << ", ";
	}
	cout << endl;
}

void ElevatorController::increaseFloor()
{
	cout << "Increase floor" << endl;
	++m_currentFloor;
	printCurrentFloor();
}

void ElevatorController::decreaseFloor()
{
	cout << "Decrease floor" << endl;
	--m_currentFloor;
	printCurrentFloor();
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

void ElevatorController::printCurrentFloor() const
{
	cout << "Current floor: " << m_currentFloor << endl;
}

void ElevatorController::goingUp()
{
	cout << "Going up: " << endl;
}

void ElevatorController::goingDown()
{
	cout << "Going down: " << endl;
}


