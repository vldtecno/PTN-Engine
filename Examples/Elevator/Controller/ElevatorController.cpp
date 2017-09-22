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

using namespace std;

ElevatorController::ElevatorController():
	m_pPetriNet{nullptr}
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

}

void ElevatorController::closeDoors()
{

}


bool ElevatorController::callElevatorUp(const int floor)
{
	if (floor < s_bottomFloor || floor >= s_topFloor)
	{
		return false;
	}
	return true;
}

bool ElevatorController::callElevatorDown(const int floor)
{
	if (floor <= s_bottomFloor || floor > s_topFloor)
	{
		return false;
	}
	return true;
}

bool ElevatorController::setDestinationFloor(const int floor)
{
	if (floor < s_bottomFloor || floor > s_topFloor)
	{
		return false;
	}
	return true;
}









