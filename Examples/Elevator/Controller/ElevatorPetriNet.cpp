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

#include "Controller/ElevatorPetriNet.h"
#include "Controller/ElevatorController.h"

using namespace ptne;
using namespace std;

ElevatorPetriNet::ElevatorPetriNet(ElevatorController &elevatorController)
: PTN_Engine(ACTIONS_THREAD_OPTION::JOB_QUEUE)
, m_elevatorController(elevatorController)
{
	// Create places
	createInputPlaces();
	createCabinePlaces();
	createArrivalPlaces();
	createButtonPressPlaces();
	createSimulationPlaces();

	// CreateTransitions
	createCabineTransitions();
	createSimulationTransitions();
	createArrivingFloorTransitions();
	createButtonPressingTransitions();
	createCallingButtonTransitions();
}

ElevatorPetriNet::~ElevatorPetriNet() = default;

void ElevatorPetriNet::closeDoors()
{
	incrementInput("CloseDoors");
}

void ElevatorPetriNet::openDoors()
{
	incrementInput("OpenDoors");
}

void ElevatorPetriNet::callButtonUp()
{
	incrementInput("CallButtonUp");
}

void ElevatorPetriNet::callButtonDown()
{
	incrementInput("CallButtonDown");
}

void ElevatorPetriNet::destinationButton()
{
	incrementInput("DestinationButton");
}

void ElevatorPetriNet::incrementInput(const string &inputPlace, const bool)
{
	incrementInputPlace(inputPlace);
}

void ElevatorPetriNet::execute(const bool debug)
{
	PTN_Engine::execute(debug);
}

void ElevatorPetriNet::stop()
{
	PTN_Engine::stop();
}

void ElevatorPetriNet::createInputPlaces()
{
	createPlace({ .name="CloseDoors",
				  .input=true });
	createPlace({ .name="OpenDoors",
				  .input=true });
	createPlace({ .name="CallButtonUp",
				  .input=true });
	createPlace({ .name="CallButtonDown",
				  .input=true });
	createPlace({ .name="DestinationButton",
				  .input=true });
}

void ElevatorPetriNet::createCabinePlaces()
{
	createPlace({ .name = "Stopped",
				  .onEnterAction = bind_front(&ElevatorController::elevatorStopped, &m_elevatorController) });
	createPlace({ .name = "Stopped_",
				  .initialNumberOfTokens = 1 });
	createPlace({ .name="Moving",
				  .onEnterAction=bind_front(&ElevatorController::elevatorMoving, &m_elevatorController)});
	createPlace({ .name="Moving_"});
	createPlace({ .name="DoorsOpened", .initialNumberOfTokens=1,
				  .onEnterAction=bind_front(&ElevatorController::doorsAreOpen, &m_elevatorController)});
	createPlace({ .name="DoorsClosed",
				  .onEnterAction=bind_front(&ElevatorController::doorsAreClosed, &m_elevatorController)});
	createPlace({ .name="DoorsClosed_"});

	createPlace({ .name="RemoveFromListGU",
				  .onEnterAction=bind_front(&ElevatorController::removeDestinationGU, &m_elevatorController) });
	createPlace({ .name="RemoveFromListGD",
				  .onEnterAction=bind_front(&ElevatorController::removeDestinationGD, &m_elevatorController) });
	createPlace({ .name="ArrivedDestination" });

	createPlace({ .name="ProcessLists" });
	createPlace({ .name="ProcessedLists",
				  .initialNumberOfTokens=1,
				  .onEnterAction=bind_front(&ElevatorController::processedLists, &m_elevatorController) });

	createPlace({ .name="GoingUp",
				  .onEnterAction=bind_front(&ElevatorController::goingUp, &m_elevatorController) });
	createPlace({ .name="GoingUp_" });
	createPlace({ .name="GoingDown",
				  .onExitAction=bind_front(&ElevatorController::goingDown, &m_elevatorController) });
	createPlace({ .name="GoingDown_" });
}

void ElevatorPetriNet::createArrivalPlaces()
{
	createPlace({ .name = "Ready",
				  .initialNumberOfTokens=1 });

	createPlace({ .name="SwapGD",
				  .onEnterAction=bind_front(&ElevatorController::rotateLists, &m_elevatorController)});
	createPlace({ .name="SwapGU",
				  .onEnterAction=bind_front(&ElevatorController::rotateLists, &m_elevatorController)});

	createPlace({ .name="MergeGoingUpGTCurrent",
				  .onEnterAction=bind_front(&ElevatorController::mergeGoingUpGTCurrent, &m_elevatorController) });
	createPlace({ .name="MergeMinGoingUp1",
				  .onEnterAction=bind_front(&ElevatorController::mergeMinGoingUp, &m_elevatorController) });
	createPlace({ .name="MergeMaxGoingDown1",
				  .onEnterAction=bind_front(&ElevatorController::mergeMaxGoingDown, &m_elevatorController) });
	createPlace({ .name="MergePostponedToCurrent11",
				  .onEnterAction=bind_front(&ElevatorController::mergePostponedToCurrent, &m_elevatorController) });
	createPlace({ .name="MergePostponedToCurrent12",
				  .onEnterAction=bind_front(&ElevatorController::mergePostponedToCurrent, &m_elevatorController) });
	createPlace({ .name="MergeGoingDownSTCurrent",
				  .onEnterAction=bind_front(&ElevatorController::mergeGoingDownSTCurrent, &m_elevatorController) });
	createPlace({ .name="MergeMaxGoingDown2",
				  .onEnterAction=bind_front(&ElevatorController::mergeMaxGoingDown, &m_elevatorController) });
	createPlace({ .name="MergeMinGoingUp2",
				  .onEnterAction=bind_front(&ElevatorController::mergeMinGoingUp, &m_elevatorController) });
	createPlace({ .name="MergePostponedToCurrent21",
				  .onEnterAction=bind_front(&ElevatorController::mergePostponedToCurrent, &m_elevatorController) });
	createPlace({ .name="MergePostponedToCurrent22",
				  .onEnterAction=bind_front(&ElevatorController::mergePostponedToCurrent, &m_elevatorController) });
}

void ElevatorPetriNet::createButtonPressPlaces()
{
	createPlace({ .name = "AddToTravel",
				  .onEnterAction=bind_front(&ElevatorController::addDestination1, &m_elevatorController) });
	createPlace({ .name = "AddToNextTravel",
				  .onEnterAction=bind_front(&ElevatorController::addDestination2, &m_elevatorController) });
	createPlace({ .name = "WaitToGoUp",
				  .onEnterAction=bind_front(&ElevatorController::addWaitingToGoUp, &m_elevatorController) });
	createPlace({ .name = "WaitToGoDown",
				  .onEnterAction=bind_front(&ElevatorController::addWaitingToGoDown, &m_elevatorController) });
	createPlace({ .name="D1"});
	createPlace({ .name="D2"});
	createPlace({ .name="D3"});
	createPlace({ .name="D4"});
	createPlace({ .name="D5"});
}

void ElevatorPetriNet::createSimulationPlaces()
{
	createPlace({ .name="IncreaseFloor",
				  .onEnterAction=bind_front(&ElevatorController::increaseFloor, &m_elevatorController)});
	createPlace({ .name="DecreaseFloor",
				  .onEnterAction=bind_front(&ElevatorController::decreaseFloor, &m_elevatorController)});
}

void ElevatorPetriNet::createCabineTransitions()
{
	createTransition({ .name = "T1",
					   .activationArcs = { { .placeName = "CloseDoors" },
										   { .placeName = "Stopped_" },
										   { .placeName = "DoorsOpened" } },
					   .destinationArcs = { { .placeName = "Stopped_" },
											{ .placeName = "DoorsClosed" } } });

	createTransition({ .name = "T2",
					   .activationArcs = { { .placeName = "OpenDoors" },
										   { .placeName = "Stopped_" },
										   { .placeName = "DoorsClosed_" } },
					   .destinationArcs = { { .placeName = "Stopped_" },
											{ .placeName = "DoorsOpened" } } });

	createTransition({ .name = "T3",
					   .activationArcs = { { .placeName = "Stopped_" },
										   { .placeName = "DoorsClosed_" },
										   { .placeName = "ProcessedLists" } },
					   .destinationArcs = { { .placeName = "DoorsClosed_" },
											{ .placeName = "Moving" } },
					   .inhibitorArcs = { { .placeName = "OpenDoors" } },
					   .additionalConditions{ bind_front(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T4",
					   .activationArcs = { { .placeName = "ArrivedDestination" },
										   { .placeName = "Moving_" } },
					   .destinationArcs = { { .placeName = "Stopped" },
											{ .placeName = "OpenDoors" } } });

	// Trick to prevent read arcs from executing multiple times one place
	createTransition({ .name = "T5",
					   .activationArcs = { { .placeName = "DoorsClosed" } },
					   .destinationArcs = { { .placeName = "DoorsClosed_" } } });
	createTransition({ .name = "T6",
					   .activationArcs = { { .placeName = "Moving" } },
					   .destinationArcs = { { .placeName = "Moving_" } } });
	createTransition({ .name = "T7",
					   .activationArcs = { { .placeName = "Stopped" } },
					   .destinationArcs = { { .placeName = "Stopped_" } } });
	createTransition({ .name = "T8",
					   .activationArcs = { { .placeName = "GoingUp" } },
					   .destinationArcs = { { .placeName = "GoingUp_" } } });
	createTransition({ .name = "T9",
					   .activationArcs = { { .placeName = "GoingDown" } },
					   .destinationArcs = { { .placeName = "GoingDown_" } } });
}

void ElevatorPetriNet::createSimulationTransitions()
{
	createTransition({ .name = "T10",
					   .activationArcs = { { .placeName = "GoingUp_" },
										   { .placeName = "Ready" },
										   { .placeName = "Moving_" } },
					   .destinationArcs = { { .placeName = "Moving_" },
											{ .placeName = "GoingUp_" },
											{ .placeName = "IncreaseFloor" } },
					   .inhibitorArcs = { { .placeName = "ArrivedDestination" } } });

	createTransition({ .name = "T11",
					   .activationArcs = { { .placeName = "GoingDown_" },
										   { .placeName = "Ready" },
										   { .placeName = "Moving_" } },
					   .destinationArcs = { { .placeName = "Moving_" },
											{ .placeName = "GoingDown_" },
											{ .placeName = "DecreaseFloor" } },
					   .inhibitorArcs = { { .placeName = "ArrivedDestination" } } });

	createTransition({ .name = "T12",
					   .activationArcs = { { .placeName = "IncreaseFloor" } },
					   .destinationArcs = { { .placeName = "Ready" } },
					   .additionalConditions={ bind_front(&ElevatorController::isFloorNotInList, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T13",
					   .activationArcs = { { .placeName = "DecreaseFloor" } },
					   .destinationArcs = { { .placeName = "Ready" } },
					   .additionalConditions={ bind_front(&ElevatorController::isFloorNotInList, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T14",
					   .activationArcs = { { .placeName = "IncreaseFloor" } },
					   .destinationArcs = { { .placeName = "RemoveFromListGU" } },
					   .additionalConditions={ bind_front(&ElevatorController::isFloorInList, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T15",
					   .activationArcs = { { .placeName = "RemoveFromListGU" } },
					   .destinationArcs = { { .placeName = "ProcessLists" },
											{ .placeName = "ArrivedDestination" },
											{ .placeName = "Ready" } } });

	createTransition({ .name = "T16",
					   .activationArcs = { { .placeName = "DecreaseFloor" } },
					   .destinationArcs = { { .placeName = "RemoveFromListGD" } },
					   .additionalConditions={ bind_front(&ElevatorController::isFloorInList, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T17",
					   .activationArcs = { { .placeName = "RemoveFromListGD" } },
					   .destinationArcs = { { .placeName = "ProcessLists" },
											{ .placeName = "ArrivedDestination" },
											{ .placeName = "Ready" } } });
}

void ElevatorPetriNet::createArrivingFloorTransitions()
{
	// Arriving a floor going down.

	createTransition({ .name = "T18",
					   .activationArcs = { { .placeName = "Ready" },
										   { .placeName = "ProcessLists" },
										   { .placeName = "GoingDown_" },
										   { .placeName = "DoorsClosed_" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "GoingDown_" },
											{ .placeName = "DoorsClosed_" },
											{ .placeName = "ProcessedLists" } },
					   .inhibitorArcs = { { .placeName = "OpenDoors" },
										  { .placeName = "ArrivedDestination" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T19",
					   .activationArcs = { { .placeName = "Ready" },
										   { .placeName = "ProcessLists" },
										   { .placeName = "GoingDown_" },
										   { .placeName = "DoorsClosed_" } },
					   .destinationArcs = { { .placeName = "DoorsClosed_" },
											{ .placeName = "SwapGD" } },
					   .inhibitorArcs = { { .placeName = "OpenDoors" },
										  { .placeName = "ArrivedDestination" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T20",
					   .activationArcs = { { .placeName = "SwapGD" } },
					   .destinationArcs = { { .placeName = "MergeGoingUpGTCurrent" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T21",
					   .activationArcs = { { .placeName = "SwapGD" } },
					   .destinationArcs = { { .placeName = "MergeMinGoingUp1" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T22",
					   .activationArcs = { { .placeName = "MergeGoingUpGTCurrent" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" },
											{ .placeName = "GoingUp" } } });

	createTransition({ .name = "T23",
					   .activationArcs = { { .placeName = "MergeMinGoingUp1" } },
					   .destinationArcs = { { .placeName = "MergeMaxGoingDown1" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T24",
					   .activationArcs = { { .placeName = "MergeMinGoingUp1" } },
					   .destinationArcs = { { .placeName = "GoingDown" },
											{ .placeName = "Ready" },
											{ .placeName = "ProcessedLists" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMinSmallerThanCurrent, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T25",
					   .activationArcs = { { .placeName = "MergeMinGoingUp1" } },
					   .destinationArcs = { { .placeName = "MergePostponedToCurrent11" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMinGreaterThanCurrent, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T26",
					   .activationArcs = { { .placeName = "MergeMaxGoingDown1" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T27",
					   .activationArcs = { { .placeName = "MergeMaxGoingDown1" } },
					   .destinationArcs = { { .placeName = "MergePostponedToCurrent12" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController),
											   bind_front(&ElevatorController::isMaxSmallerThanCurrent, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T28",
					   .activationArcs = { { .placeName = "MergeMaxGoingDown1" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" },
											{ .placeName = "GoingUp" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController),
											   bind_front(&ElevatorController::isMaxGreaterThanCurrent, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T29",
					   .activationArcs = { { .placeName = "MergePostponedToCurrent11" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" },
											{ .placeName = "GoingUp" } } });

	createTransition({ .name = "T30",
					   .activationArcs = { { .placeName = "MergePostponedToCurrent12" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" },
											{ .placeName = "GoingDown" } } });


	// Arriving a floor going up.

	createTransition({ .name = "T31",
					   .activationArcs = { { .placeName = "Ready" },
										   { .placeName = "ProcessLists" },
										   { .placeName = "GoingUp_" },
										   { .placeName = "DoorsClosed_" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "GoingUp_" },
											{ .placeName = "DoorsClosed_" },
											{ .placeName = "ProcessedLists" } },
					   .inhibitorArcs = { { .placeName = "OpenDoors" },
										  { .placeName = "ArrivedDestination" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T32",
					   .activationArcs = { { .placeName = "Ready" },
										   { .placeName = "ProcessLists" },
										   { .placeName = "GoingUp_" },
										   { .placeName = "DoorsClosed_" } },
					   .destinationArcs = { { .placeName = "DoorsClosed_" },
											{ .placeName = "SwapGU" } },
					   .inhibitorArcs = { { .placeName = "OpenDoors" },
										  { .placeName = "ArrivedDestination" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T33",
					   .activationArcs = { { .placeName = "SwapGU" } },
					   .destinationArcs = { { .placeName = "MergeGoingDownSTCurrent" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T34",
					   .activationArcs = { { .placeName = "SwapGU" } },
					   .destinationArcs = { { .placeName = "MergeMaxGoingDown2" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T35",
					   .activationArcs = { { .placeName = "MergeGoingDownSTCurrent" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" },
											{ .placeName = "GoingDown" } } });

	createTransition({ .name = "T36",
					   .activationArcs = { { .placeName = "MergeMaxGoingDown2" } },
					   .destinationArcs = { { .placeName = "MergeMinGoingUp2" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T37",
					   .activationArcs = { { .placeName = "MergeMaxGoingDown2" } },
					   .destinationArcs = { { .placeName = "GoingUp" },
											{ .placeName = "Ready" },
											{ .placeName = "ProcessedLists" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMaxGreaterThanCurrent, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T38",
					   .activationArcs = { { .placeName = "MergeMaxGoingDown2" } },
					   .destinationArcs = { { .placeName = "MergePostponedToCurrent21" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMaxSmallerThanCurrent, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T39",
					   .activationArcs = { { .placeName = "MergeMinGoingUp2" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListEmpty, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T40",
					   .activationArcs = { { .placeName = "MergeMinGoingUp2" } },
					   .destinationArcs = { { .placeName = "MergePostponedToCurrent22" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController),
											   bind_front(&ElevatorController::isMinGreaterThanCurrent, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T41",
					   .activationArcs = { { .placeName = "MergeMinGoingUp2" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" },
											{ .placeName = "GoingDown" } },
					   .additionalConditions={ bind_front(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController),
											   bind_front(&ElevatorController::isMinSmallerThanCurrent, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T42",
					   .activationArcs = { { .placeName = "MergePostponedToCurrent21" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" },
											{ .placeName = "GoingDown" } } });

	createTransition({ .name = "T43",
					   .activationArcs = { { .placeName = "MergePostponedToCurrent22" } },
					   .destinationArcs = { { .placeName = "Ready" },
											{ .placeName = "ProcessedLists" },
											{ .placeName = "GoingUp" } } });
}

void ElevatorPetriNet::createButtonPressingTransitions()
{
	createTransition({ .name = "T44",
					   .activationArcs = { { .placeName = "DestinationButton" },
										   { .placeName = "Ready" } },
					   .destinationArcs = { { .placeName = "D3" } },
					   .inhibitorArcs = { { .placeName = "GoingUp_" },
										  { .placeName = "GoingDown_" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T45",
					   .activationArcs = { { .placeName = "DestinationButton" },
										   { .placeName = "GoingUp_" },
										   { .placeName = "Ready" } },
					   .destinationArcs = { { .placeName = "D1" },
											{ .placeName = "GoingUp_" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T46",
					   .activationArcs = { { .placeName = "DestinationButton" },
										   { .placeName = "GoingDown_" },
										   { .placeName = "Ready" } },
					   .destinationArcs = { { .placeName = "D2" },
											{ .placeName = "GoingDown_" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T47",
					   .activationArcs = { { .placeName = "D1" } },
					   .destinationArcs = { { .placeName = "AddToTravel" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T48",
					   .activationArcs = { { .placeName = "D1" } },
					   .destinationArcs = { { .placeName = "AddToNextTravel" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T49",
					   .activationArcs = { { .placeName = "D2" } },
					   .destinationArcs = { { .placeName = "AddToNextTravel" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T50",
					   .activationArcs = { { .placeName = "D2" } },
					   .destinationArcs = { { .placeName = "AddToTravel" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T51",
					   .activationArcs = { { .placeName = "D3" } },
					   .destinationArcs = { { .placeName = "GoingUp" },
											{ .placeName = "AddToTravel" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T52",
					   .activationArcs = { { .placeName = "D3" } },
					   .destinationArcs = { { .placeName = "GoingDown" },
											{ .placeName = "AddToTravel" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T53",
					   .activationArcs = { { .placeName = "AddToNextTravel" } },
					   .destinationArcs = { { .placeName = "Ready" } } });

	createTransition({ .name = "T54",
					   .activationArcs = { { .placeName = "AddToTravel" } },
					   .destinationArcs = { { .placeName = "Ready" } } });
}

void ElevatorPetriNet::createCallingButtonTransitions()
{
	// Calling the elevator to go up.

	createTransition({ .name = "T55",
					   .activationArcs = { { .placeName = "CallButtonUp" },
										   { .placeName = "Ready" } },
					   .destinationArcs = { { .placeName = "D3" } },
					   .inhibitorArcs = { { .placeName = "GoingUp_" },
										  { .placeName = "GoingDown_" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T56",
					   .activationArcs = { { .placeName = "CallButtonUp" },
										   { .placeName = "GoingUp_" },
										   { .placeName = "Ready" } },
					   .destinationArcs = { { .placeName = "D4" },
											{ .placeName = "GoingUp_" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T57",
					   .activationArcs = { { .placeName = "CallButtonUp" },
										   { .placeName = "GoingDown_" },
										   { .placeName = "Ready" } },
					   .destinationArcs = { { .placeName = "WaitToGoUp" },
											{ .placeName = "GoingDown_" } } });

	createTransition({ .name = "T58",
					   .activationArcs = { { .placeName = "D4" } },
					   .destinationArcs = { { .placeName = "WaitToGoUp" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T59",
					   .activationArcs = { { .placeName = "D4" } },
					   .destinationArcs = { { .placeName = "AddToTravel" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T60",
					   .activationArcs = { { .placeName = "WaitToGoUp" } },
					   .destinationArcs = { { .placeName = "Ready" } } });


	// Calling the elevator to go down.

	createTransition({ .name = "T61",
					   .activationArcs = { { .placeName = "CallButtonDown" },
										   { .placeName = "Ready" } },
					   .destinationArcs = { { .placeName = "D3" } },
					   .inhibitorArcs = { { .placeName = "GoingUp_" },
										  { .placeName = "GoingDown_" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T62",
					   .activationArcs = { { .placeName = "CallButtonDown" },
										   { .placeName = "GoingDown_" },
										   { .placeName = "Ready" } },
					   .destinationArcs = { { .placeName = "D5" },
											{ .placeName = "GoingDown_" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T63",
					   .activationArcs = { { .placeName = "CallButtonDown" },
										   { .placeName = "GoingUp_" },
										   { .placeName = "Ready" } },
					   .destinationArcs = { { .placeName = "WaitToGoDown" },
											{ .placeName = "GoingUp_" } } });

	createTransition({ .name = "T64",
					   .activationArcs = { { .placeName = "D5" } },
					   .destinationArcs = { { .placeName = "AddToTravel" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T65",
					   .activationArcs = { { .placeName = "D5" } },
					   .destinationArcs = { { .placeName = "WaitToGoDown" } },
					   .additionalConditions={ bind_front(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					   .requireNoActionsInExecution=true });

	createTransition({ .name = "T66",
					   .activationArcs = { { .placeName = "WaitToGoDown" } },
					   .destinationArcs = { { .placeName = "Ready" } } });
}
