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

#include "Controller/ElevatorController.h"
#include "Controller/IElevatorPetriNet.h"
#include "PTN_Engine/PTN_Engine.h"

//! Implements PTN Engine net defining the phone menu behaviour.
/*!
 * The behaviour is defined in the constructor.
 */
class ElevatorPetriNet:
		public IElevatorPetriNet,
		private ptne::PTN_Engine
{
public:

	/*!
	 * Constructor.
	 * \param ptrDispatcher Shared pointer to the controller.
	 */
	ElevatorPetriNet(std::shared_ptr<ElevatorController> ptrMenuController);

	//Events

	void closeDoors() override;

	void openDoors() override;

	void callButtonUp() override;
	
	void callButtonDown() override;

	void destinationButton() override;


private:

	void createInputPlaces();
	void createCabinePlaces(std::shared_ptr<ElevatorController> ptrController);
	void createArrivalPlaces(std::shared_ptr<ElevatorController> ptrController);
	void createButtonPressPlaces(std::shared_ptr<ElevatorController> ptrController);
	void createSimulationPlaces(std::shared_ptr<ElevatorController> ptrController);

	void createCabineTransitions(std::shared_ptr<ElevatorController> ptrController);
	void createSimulationTransitions(std::shared_ptr<ElevatorController> ptrController);
	void createArrivingFloorTransitions(std::shared_ptr<ElevatorController> ptrController);
	void createButtonPressingTransitions(std::shared_ptr<ElevatorController> ptrController);
	void createCallingButtonTransitions(std::shared_ptr<ElevatorController> ptrController);

	void incrementAndExecute(const std::string& inputPlace, const bool debug = false);



};
