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


//!Base class for a PTN Engine net that controls the ElevatorController.
class IElevatorPetriNet
{
public:

	virtual ~IElevatorPetriNet(){}

	virtual void closeDoors() = 0;
	
	virtual void openDoors() = 0;

	virtual void callButtonUp() = 0;

	virtual void callButtonDown() = 0;

	virtual void destinationButton() = 0;

};
