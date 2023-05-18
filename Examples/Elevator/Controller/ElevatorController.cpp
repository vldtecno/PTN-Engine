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

#include <algorithm>
#include <iostream>
#include <mutex>

using namespace std;

namespace
{
void printFloorList(const unordered_set<int> &floors)
{
	if (floors.empty())
	{
		cout << "Empty" << endl;
		return;
	}
	for (const auto &d : floors)
	{
		cout << d << " ";
	}
	cout << endl;
}
} // namespace

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
{
}

void ElevatorController::checkPetriNetPointer() const
{
	if (m_pPetriNet == nullptr)
	{
		throw runtime_error("Petri net not available.");
	}
}

void ElevatorController::openDoors()
{
	m_pPetriNet->openDoors();
}

void ElevatorController::closeDoors()
{
	m_pPetriNet->closeDoors();
}

bool ElevatorController::callElevatorUp(const int floor)
{
	{
		unique_lock<shared_mutex> l(m_mutex);
		if (floor < s_bottomFloor || floor >= s_topFloor)
		{
			return false;
		}
		m_toAddToDestination = floor;
	}
	m_pPetriNet->callButtonUp();
	return true;
}

bool ElevatorController::callElevatorDown(const int floor)
{
	{
		unique_lock<shared_mutex> l(m_mutex);
		if (floor <= s_bottomFloor || floor > s_topFloor)
		{
			return false;
		}
		m_toAddToDestination = floor;
	}
	m_pPetriNet->callButtonDown();
	return true;
}

bool ElevatorController::setDestinationFloor(const int floor)
{
	{
		unique_lock<shared_mutex> l(m_mutex);
		if (floor < s_bottomFloor || floor > s_topFloor)
		{
			return false;
		}
		m_toAddToDestination = floor;
	}
	m_pPetriNet->destinationButton();
	return true;
}

void ElevatorController::stop()
{
	m_pPetriNet->stop();
}

// Actions
void ElevatorController::addDestination1()
{
	unique_lock<shared_mutex> l(m_mutex);
	m_nextTravelDestinations.erase(m_toAddToDestination);
	m_destinations.insert(m_toAddToDestination);
	printDestinations();
}

void ElevatorController::addDestination2()
{
	unique_lock<shared_mutex> l(m_mutex);
	if (m_destinations.contains(m_toAddToDestination))
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
	unique_lock<shared_mutex> l(m_mutex);
	m_floorsWaitingToGoDown.insert(m_toAddToDestination);
	printWaitingGoDown();
}

void ElevatorController::addWaitingToGoUp()
{
	unique_lock<shared_mutex> l(m_mutex);
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
	unique_lock<shared_mutex> l(m_mutex);
	cout << "Removed " << m_currentFloor << " from destinations" << endl;
	auto it = m_destinations.find(m_currentFloor);
	if (it != m_destinations.end())
	{
		m_destinations.erase(it);
	}
}

void ElevatorController::removeCurrentFromWaitingToGoDown()
{
	unique_lock<shared_mutex> l(m_mutex);
	size_t i = m_floorsWaitingToGoDown.erase(m_currentFloor);
	if (i > 0)
	{
		cout << "Removed " << m_currentFloor << " from waiting to go down" << endl;
	}
}

void ElevatorController::removeCurrentFromWaitingToGoUp()
{
	unique_lock<shared_mutex> l(m_mutex);
	size_t i = m_floorsWaitingToGoUp.erase(m_currentFloor);
	if (i > 0)
	{
		cout << "Removed " << m_currentFloor << " from waiting to go up" << endl;
	}
}

void ElevatorController::rotateLists()
{
	unique_lock<shared_mutex> l(m_mutex);
	swap(m_destinations, m_nextTravelDestinations);
	m_destinations.erase(m_currentFloor);
}

void ElevatorController::increaseFloor()
{
	unique_lock<shared_mutex> l(m_mutex);
	cout << "Ascending... " << endl;
	++m_currentFloor;
	printCurrentFloor();
}

void ElevatorController::decreaseFloor()
{
	unique_lock<shared_mutex> l(m_mutex);
	cout << "Descending... " << endl;
	--m_currentFloor;
	printCurrentFloor();
}

void ElevatorController::mergeGoingUpGTCurrent()
{
	unique_lock<shared_mutex> l(m_mutex);
	mergeToDestinations(m_floorsWaitingToGoDown, false);
}

void ElevatorController::mergeMinGoingUp()
{
	unique_lock<shared_mutex> l(m_mutex);
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
	unique_lock<shared_mutex> l(m_mutex);
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
	unique_lock<shared_mutex> l(m_mutex);
	m_destinations.insert(m_nextTravelDestinations.begin(), m_nextTravelDestinations.end());
	m_nextTravelDestinations.clear();

	m_destinations.erase(m_currentFloor);
}

void ElevatorController::mergeGoingDownSTCurrent()
{
	unique_lock<shared_mutex> l(m_mutex);
	mergeToDestinations(m_floorsWaitingToGoDown, true);
}

void ElevatorController::mergeToDestinations(unordered_set<int> &toAdd, const bool lessThan)
{
	bool deleted = false;
	do
	{
		deleted = false;
		auto floorIt =
		std::find_if(toAdd.cbegin(), toAdd.cend(),
					 [this, &lessThan](const int floor)
					 { return (lessThan && floor < m_currentFloor) || (!lessThan && floor > m_currentFloor); });
		if (floorIt != toAdd.cend())
		{
			auto &floor = *floorIt;
			m_destinations.insert(floor);
			toAdd.erase(floor);
			deleted = true;
		}
	} while (deleted);

	m_destinations.erase(m_currentFloor);
}

void ElevatorController::processedLists() const
{
	printSchedule();
}

bool ElevatorController::isFloorNotInList() const
{
	return !isFloorInList();
}

bool ElevatorController::isFloorInList() const
{
	shared_lock<shared_mutex> l(m_mutex);
	return m_destinations.contains(m_currentFloor);
}

bool ElevatorController::isDestinationListNotEmpty() const
{
	return !isDestinationListEmpty();
}

bool ElevatorController::isDestinationListEmpty() const
{
	shared_lock<shared_mutex> l(m_mutex);
	return m_destinations.empty();
}

bool ElevatorController::isMarkedFloorNotCurrentFloor() const
{
	shared_lock<shared_mutex> l(m_mutex);
	return m_toAddToDestination != m_currentFloor;
}

bool ElevatorController::isMarkedFloorGreaterThanCurrentFloor() const
{
	shared_lock<shared_mutex> l(m_mutex);
	return m_toAddToDestination > m_currentFloor;
}

bool ElevatorController::isMarkedFloorSmallerThanCurrentFloor() const
{
	shared_lock<shared_mutex> l(m_mutex);
	return m_toAddToDestination < m_currentFloor;
}

bool ElevatorController::isMinSmallerThanCurrent() const
{
	shared_lock<shared_mutex> l(m_mutex);
	if (m_destinations.empty())
	{
		return false;
	}
	int minFloor = *min_element(m_destinations.cbegin(), m_destinations.cend());
	return minFloor < m_currentFloor;
}

bool ElevatorController::isMinGreaterThanCurrent() const
{
	shared_lock<shared_mutex> l(m_mutex);
	if (m_destinations.empty())
	{
		return false;
	}
	int minFloor = *min_element(m_destinations.cbegin(), m_destinations.cend());
	return minFloor > m_currentFloor;
}

bool ElevatorController::isMaxSmallerThanCurrent() const
{
	shared_lock<shared_mutex> l(m_mutex);
	if (m_destinations.empty())
	{
		return false;
	}
	int maxFloor = *max_element(m_destinations.cbegin(), m_destinations.cend());
	return maxFloor < m_currentFloor;
}

bool ElevatorController::isMaxGreaterThanCurrent() const
{
	shared_lock<shared_mutex> l(m_mutex);
	if (m_destinations.empty())
	{
		return false;
	}
	int maxFloor = *max_element(m_destinations.cbegin(), m_destinations.cend());
	return maxFloor > m_currentFloor;
}


// Info

void ElevatorController::elevatorStopped() const
{
	unique_lock<shared_mutex> l(m_mutex);
	cout << "Elevator stopped" << endl;
}

void ElevatorController::elevatorMoving() const
{
	printSchedule();
}

void ElevatorController::doorsAreOpen() const
{
	unique_lock<shared_mutex> l(m_mutex);
	cout << "Elevator doors are open" << endl;
}

void ElevatorController::doorsAreClosed() const
{
	unique_lock<shared_mutex> l(m_mutex);
	cout << "Elevator doors are closed" << endl;
}

void ElevatorController::goingUp() const
{
	unique_lock<shared_mutex> l(m_mutex);
	cout << "Scheduled to go up." << endl;
}

void ElevatorController::goingDown() const
{
	unique_lock<shared_mutex> l(m_mutex);
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

void ElevatorController::printSchedule() const
{
	shared_lock<shared_mutex> l(m_mutex);
	cout << endl;
	cout << "Scheduled floors: " << endl;
	printDestinations();
	printNextDestinations();
	printWaitingGoDown();
	printWaitingGoUp();
	cout << endl;
}
