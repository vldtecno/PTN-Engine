/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2024 Eduardo Valgôde
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

class ElevatorController;

//! Implements PTN Engine net defining the phone menu behaviour.
class ElevatorPetriNet : public IElevatorPetriNet, private ptne::PTN_Engine
{
public:
	~ElevatorPetriNet() override;

	//!
	//! Constructor.
	//! \param ptrDispatcher Shared pointer to the controller.
	//!
	explicit ElevatorPetriNet(ElevatorController &elevatorController);
	ElevatorPetriNet(const ElevatorPetriNet &) = delete;
	ElevatorPetriNet(ElevatorPetriNet &&) = delete;
	ElevatorPetriNet &operator=(ElevatorPetriNet &) = delete;
	ElevatorPetriNet &operator=(ElevatorPetriNet &&) = delete;

	// Events

	void closeDoors() override;

	void openDoors() override;

	void callButtonUp() override;

	void callButtonDown() override;

	void destinationButton() override;

	void execute(const bool debug) override;

	void stop() override;

private:
	void createInputPlaces();
	void createCabinePlaces();
	void createArrivalPlaces();
	void createButtonPressPlaces();
	void createSimulationPlaces();

	void createCabineTransitions();
	void createSimulationTransitions();
	void createArrivingFloorTransitions();
	void createButtonPressingTransitions();
	void createCallingButtonTransitions();

	void incrementInput(const std::string &inputPlace, const bool debug = false);

	ElevatorController &m_elevatorController;
};
