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

#include "Controller/ElevatorController.h"
#include "Controller/ElevatorPetriNet.h"

#include <iostream>
#include <algorithm>

using namespace std;

ElevatorController::ElevatorController()
	: m_pPetriNet(nullptr)
	, m_currentFloor(s_bottomFloor)
	, m_toAddToDestination(s_bottomFloor)
	, m_destinations()
	, m_nextTravelDestinations()
	, m_floorsWaitingToGoUp()
	, m_floorsWaitingToGoDown()
{
	m_pPetriNet = make_unique<ElevatorPetriNet>(*this);
	m_pPetriNet->execute(false);
}

ElevatorController::~ElevatorController()
{}

void ElevatorController::checkPetriNetPointer() const
{
	if (m_pPetriNet == nullptr)
	{
		throw runtime_error("Petri net not available.");
	}
}

void ElevatorController::openDoors()
{
	checkPetriNetPointer();
	m_pPetriNet->openDoors();
}

void ElevatorController::closeDoors()
{
	checkPetriNetPointer();
	m_pPetriNet->closeDoors();
}

bool ElevatorController::callElevatorUp(const int floor)
{
	if (floor < s_bottomFloor || floor >= s_topFloor)
	{
		return false;
	}
	checkPetriNetPointer();
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
	checkPetriNetPointer();
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
	checkPetriNetPointer();
	m_toAddToDestination = floor;
	m_pPetriNet->destinationButton();
	return true;
}

void ElevatorController::stop()
{
	m_pPetriNet->stop();
}

//Actions
void ElevatorController::addDestination1()
{
	m_nextTravelDestinations.erase(m_toAddToDestination);
	m_destinations.insert(m_toAddToDestination);

	printDestinations();
}

void ElevatorController::addDestination2()
{
	if (m_destinations.find(m_toAddToDestination) != m_destinations.end())
	{
		return;
	}
	m_floorsWaitingToGoDown.erase(m_toAddToDestination);
	m_floorsWaitingToGoUp.erase(m_toAddToDestination);

	m_nextTravelDestinations.insert(m_toAddToDestination);
	
	printNextDestinations();
}

void ElevatorController::addWaitingToGoDown()
{
	m_floorsWaitingToGoDown.insert(m_toAddToDestination);
	printWaitingGoDown();
}

void ElevatorController::addWaitingToGoUp()
{
	m_floorsWaitingToGoUp.insert(m_toAddToDestination);
	printWaitingGoUp();
}

void ElevatorController::removeDestinationGU()
{
	removeDestination();
	removeCurrentFromWaitingToGoUp();
}

void ElevatorController::removeDestinationGD()
{
	removeDestination();
	removeCurrentFromWaitingToGoDown();
}

void ElevatorController::removeDestination()
{
	cout << "Removed " << m_currentFloor << " from destinations" << endl;
	auto it = m_destinations.find(m_currentFloor);
	if (it != m_destinations.end())
	{
		m_destinations.erase(it);
	}
}

void ElevatorController::removeCurrentFromWaitingToGoDown()
{
	size_t i = m_floorsWaitingToGoDown.erase(m_currentFloor);
	if (i > 0)
	{
		cout << "Removed " << m_currentFloor << " from waiting to go down" << endl;
	}
}

void ElevatorController::removeCurrentFromWaitingToGoUp()
{
	size_t i = m_floorsWaitingToGoUp.erase(m_currentFloor);
	if (i > 0)
	{
		cout << "Removed " << m_currentFloor << " from waiting to go up" << endl;
	}
}

void ElevatorController::rotateLists()
{
	swap(m_destinations, m_nextTravelDestinations);	
	m_destinations.erase(m_currentFloor);
}

void ElevatorController::increaseFloor()
{
	cout << "Ascending... " << endl;
	++m_currentFloor;
	printCurrentFloor();
}

void ElevatorController::decreaseFloor()
{
	cout << "Descending... " << endl;
	--m_currentFloor;
	printCurrentFloor();
}

void ElevatorController::mergeGoingUpGTCurrent()
{
	mergeToDestinations(m_floorsWaitingToGoDown, false);
}

void ElevatorController::mergeMinGoingUp()
{
	if (m_floorsWaitingToGoUp.empty())
	{
		return;
	}
	const int minWaiting = *min_element(m_floorsWaitingToGoUp.begin(), m_floorsWaitingToGoUp.end());
	m_destinations.insert(minWaiting);
	m_floorsWaitingToGoUp.erase(minWaiting);

	m_nextTravelDestinations.insert(m_floorsWaitingToGoUp.begin(), m_floorsWaitingToGoUp.end());
	m_floorsWaitingToGoUp.clear();

}

void ElevatorController::mergeMaxGoingDown()
{
	if (m_floorsWaitingToGoDown.empty())
	{
		return;
	}
	const int maxWaiting = *max_element(m_floorsWaitingToGoDown.begin(), m_floorsWaitingToGoDown.end());
	m_destinations.insert(maxWaiting);
	m_floorsWaitingToGoDown.erase(maxWaiting);

	m_nextTravelDestinations.insert(m_floorsWaitingToGoDown.begin(), m_floorsWaitingToGoDown.end());
	m_floorsWaitingToGoDown.clear();

}

void ElevatorController::mergePostponedToCurrent()
{
	m_destinations.insert(m_nextTravelDestinations.begin(), m_nextTravelDestinations.end());
	m_nextTravelDestinations.clear();

	m_destinations.erase(m_currentFloor);
}

void ElevatorController::mergeGoingDownSTCurrent()
{
	mergeToDestinations(m_floorsWaitingToGoDown, true);
}

void ElevatorController::mergeToDestinations(unordered_set<int>& toAdd, const bool lessThan)
{
	bool deleted = false;
	do
	{
		deleted = false;
		auto floorIt = std::find_if(toAdd.cbegin(), toAdd.cend(), [this, &lessThan](const int floor) {
						return (lessThan && floor < m_currentFloor) || (!lessThan && floor > m_currentFloor);
					 });
		if (floorIt != toAdd.cend())
		{
			auto& floor = *floorIt;
			m_destinations.insert(floor);
			toAdd.erase(floor);
			deleted = true;
		}
	} while (deleted);

	m_destinations.erase(m_currentFloor);
}

void ElevatorController::processedLists()
{
	printSchedule();
}

bool ElevatorController::isFloorNotInList() const
{
	return !isFloorInList();
}

bool ElevatorController::isFloorInList() const
{
	return m_destinations.find(m_currentFloor) != m_destinations.cend();
}

bool ElevatorController::isDestinationListNotEmpty() const
{
	return !isDestinationListEmpty();
}

bool ElevatorController::isDestinationListEmpty() const
{
	return m_destinations.empty();
}

bool ElevatorController::isMarkedFloorNotCurrentFloor() const
{
	return m_toAddToDestination != m_currentFloor;
}

bool ElevatorController::isMarkedFloorGreaterThanCurrentFloor() const
{
	return m_toAddToDestination > m_currentFloor;
}

bool ElevatorController::isMarkedFloorSmallerThanCurrentFloor() const
{
	return m_toAddToDestination < m_currentFloor;
}

bool ElevatorController::isMinSmallerThanCurrent() const
{
	if (m_destinations.empty())
	{
		return false;
	}
	int minFloor = *min_element(m_destinations.begin(), m_destinations.end());
	return minFloor < m_currentFloor;
}

bool ElevatorController::isMinGreaterThanCurrent() const
{
	if (m_destinations.empty())
	{
		return false;
	}
	int minFloor = *min_element(m_destinations.begin(), m_destinations.end());
	return minFloor > m_currentFloor;
}

bool ElevatorController::isMaxSmallerThanCurrent() const
{
	if (m_destinations.empty())
	{
		return false;
	}
	int maxFloor = *max_element(m_destinations.begin(), m_destinations.end());
	return maxFloor < m_currentFloor;
}

bool ElevatorController::isMaxGreaterThanCurrent() const
{
	if (m_destinations.empty())
	{
		return false;
	}
	int maxFloor = *max_element(m_destinations.begin(), m_destinations.end());
	return maxFloor > m_currentFloor;
}


//Info

void ElevatorController::elevatorStopped()
{
	cout << "Elevator stopped" << endl;
}

void ElevatorController::elevatorMoving()
{
	printSchedule();
}

void ElevatorController::doorsAreOpen()
{
	cout << "Elevator doors are open" << endl;
}

void ElevatorController::doorsAreClosed()
{
	cout << "Elevator doors are closed" << endl;
}

void ElevatorController::goingUp()
{
	cout << "Scheduled to go up." << endl;
}

void ElevatorController::goingDown()
{
	cout << "Scheduled to go down." << endl;
}

void ElevatorController::printCurrentFloor() const
{
	cout << "Current floor: " << m_currentFloor << endl;
}

void ElevatorController::printDestinations() const
{
	cout << "Current travel destinations: ";
	printFloorList(m_destinations);
}

void ElevatorController::printNextDestinations() const
{
	cout << "Next travel destinations: ";
	printFloorList(m_nextTravelDestinations);
}

void ElevatorController::printWaitingGoDown() const
{
	cout << "Floors with people waiting to go down: ";
	printFloorList(m_floorsWaitingToGoDown);
}

void ElevatorController::printWaitingGoUp() const
{
	cout << "Floors with people waiting to go up: ";
	printFloorList(m_floorsWaitingToGoUp);
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

void ElevatorController::printSchedule() const
{
	cout << endl;
	cout << "Scheduled floors: " << endl;
	printDestinations();
	printNextDestinations();
	printWaitingGoDown();
	printWaitingGoUp();
	cout << endl;
}
