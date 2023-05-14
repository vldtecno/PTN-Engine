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

void ElevatorPetriNet::incrementInput(const string &inputPlace, const bool debug)
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
	createPlace("CloseDoors", 0, true);
	createPlace("OpenDoors", 0, true);
	createPlace("CallButtonUp", 0, true);
	createPlace("CallButtonDown", 0, true);
	createPlace("DestinationButton", 0, true);
}

void ElevatorPetriNet::createCabinePlaces()
{
	createPlace("Stopped", 0, bind(&ElevatorController::elevatorStopped, &m_elevatorController));
	createPlace("Stopped_", 1);
	createPlace("Moving", 0, bind(&ElevatorController::elevatorMoving, &m_elevatorController));
	createPlace("Moving_", 0);
	createPlace("DoorsOpened", 1, bind(&ElevatorController::doorsAreOpen, &m_elevatorController));
	createPlace("DoorsClosed", 0, bind(&ElevatorController::doorsAreClosed, &m_elevatorController));
	createPlace("DoorsClosed_", 0);

	createPlace("RemoveFromListGU", 0, bind(&ElevatorController::removeDestinationGU, &m_elevatorController));
	createPlace("RemoveFromListGD", 0, bind(&ElevatorController::removeDestinationGD, &m_elevatorController));
	createPlace("ArrivedDestination", 0);

	createPlace("ProcessLists", 0);
	createPlace("ProcessedLists", 1, bind(&ElevatorController::processedLists, &m_elevatorController));

	createPlace("GoingUp", 0, bind(&ElevatorController::goingUp, &m_elevatorController));
	createPlace("GoingUp_", 0);
	createPlace("GoingDown", 0, bind(&ElevatorController::goingDown, &m_elevatorController));
	createPlace("GoingDown_", 0);
}

void ElevatorPetriNet::createArrivalPlaces()
{
	createPlace("Ready", 1);

	createPlace("SwapGD", 0, bind(&ElevatorController::rotateLists, &m_elevatorController));
	createPlace("SwapGU", 0, bind(&ElevatorController::rotateLists, &m_elevatorController));

	createPlace("MergeGoingUpGTCurrent", 0,
				bind(&ElevatorController::mergeGoingUpGTCurrent, &m_elevatorController));
	createPlace("MergeMinGoingUp1", 0, bind(&ElevatorController::mergeMinGoingUp, &m_elevatorController));
	createPlace("MergeMaxGoingDown1", 0, bind(&ElevatorController::mergeMaxGoingDown, &m_elevatorController));
	createPlace("MergePostponedToCurrent11", 0,
				bind(&ElevatorController::mergePostponedToCurrent, &m_elevatorController));
	createPlace("MergePostponedToCurrent12", 0,
				bind(&ElevatorController::mergePostponedToCurrent, &m_elevatorController));

	createPlace("MergeGoingDownSTCurrent", 0,
				bind(&ElevatorController::mergeGoingDownSTCurrent, &m_elevatorController));
	createPlace("MergeMaxGoingDown2", 0, bind(&ElevatorController::mergeMaxGoingDown, &m_elevatorController));
	createPlace("MergeMinGoingUp2", 0, bind(&ElevatorController::mergeMinGoingUp, &m_elevatorController));
	createPlace("MergePostponedToCurrent21", 0,
				bind(&ElevatorController::mergePostponedToCurrent, &m_elevatorController));
	createPlace("MergePostponedToCurrent22", 0,
				bind(&ElevatorController::mergePostponedToCurrent, &m_elevatorController));
}

void ElevatorPetriNet::createButtonPressPlaces()
{
	createPlace("AddToTravel", 0, bind(&ElevatorController::addDestination1, &m_elevatorController));
	createPlace("AddToNextTravel", 0, bind(&ElevatorController::addDestination2, &m_elevatorController));
	createPlace("WaitToGoUp", 0, bind(&ElevatorController::addWaitingToGoUp, &m_elevatorController));
	createPlace("WaitToGoDown", 0, bind(&ElevatorController::addWaitingToGoDown, &m_elevatorController));
	createPlace("D1", 0);
	createPlace("D2", 0);
	createPlace("D3", 0);
	createPlace("D4", 0);
	createPlace("D5", 0);
}

void ElevatorPetriNet::createSimulationPlaces()
{
	createPlace("IncreaseFloor", 0, bind(&ElevatorController::increaseFloor, &m_elevatorController));
	createPlace("DecreaseFloor", 0, bind(&ElevatorController::decreaseFloor, &m_elevatorController));
}

void ElevatorPetriNet::createCabineTransitions()
{
	createTransition({ "CloseDoors", "Stopped_", "DoorsOpened" }, { "Stopped_", "DoorsClosed" });

	createTransition({ "OpenDoors", "Stopped_", "DoorsClosed_" }, { "Stopped_", "DoorsOpened" });

	createTransition({ "Stopped_", "DoorsClosed_", "ProcessedLists" }, { "DoorsClosed_", "Moving" },
					 { "OpenDoors" },
					 { bind(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) }, true);

	createTransition({ "ArrivedDestination", "Moving_" }, { "Stopped", "OpenDoors" });

	// Trick to prevent read arcs from executing multiple times one place
	createTransition({ "DoorsClosed" }, { "DoorsClosed_" });
	createTransition({ "Moving" }, { "Moving_" });
	createTransition({ "Stopped" }, { "Stopped_" });
	createTransition({ "GoingUp" }, { "GoingUp_" });
	createTransition({ "GoingDown" }, { "GoingDown_" });
}

void ElevatorPetriNet::createSimulationTransitions()
{
	createTransition({ "GoingUp_", "Ready", "Moving_" }, { "Moving_", "GoingUp_", "IncreaseFloor" },
					 vector<string>{ "ArrivedDestination" });

	createTransition({ "GoingDown_", "Ready", "Moving_" }, { "Moving_", "GoingDown_", "DecreaseFloor" },
					 vector<string>{ "ArrivedDestination" });

	createTransition({ "IncreaseFloor" }, { "Ready" },
					 { bind(&ElevatorController::isFloorNotInList, &m_elevatorController) }, true);

	createTransition({ "DecreaseFloor" }, { "Ready" },
					 { bind(&ElevatorController::isFloorNotInList, &m_elevatorController) }, true);

	createTransition({ "IncreaseFloor" }, { "RemoveFromListGU" },
					 { bind(&ElevatorController::isFloorInList, &m_elevatorController) }, true);

	createTransition({ "RemoveFromListGU" }, { "ProcessLists", "ArrivedDestination", "Ready" });

	createTransition({ "DecreaseFloor" }, { "RemoveFromListGD" },
					 { bind(&ElevatorController::isFloorInList, &m_elevatorController) }, true);

	createTransition({ "RemoveFromListGD" }, { "ProcessLists", "ArrivedDestination", "Ready" });
}

void ElevatorPetriNet::createArrivingFloorTransitions()
{
	// Arriving a floor going down.

	createTransition({ "Ready", "ProcessLists", "GoingDown_", "DoorsClosed_" },
					 { "Ready", "GoingDown_", "DoorsClosed_", "ProcessedLists" },
					 { "OpenDoors", "ArrivedDestination" },
					 { bind(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) }, true);

	createTransition({ "Ready", "ProcessLists", "GoingDown_", "DoorsClosed_" }, { "DoorsClosed_", "SwapGD" },
					 { "OpenDoors", "ArrivedDestination" },
					 { bind(&ElevatorController::isDestinationListEmpty, &m_elevatorController) }, true);

	createTransition({ "SwapGD" }, { "MergeGoingUpGTCurrent" },
					 { bind(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) }, true);

	createTransition({ "SwapGD" }, { "MergeMinGoingUp1" },
					 { bind(&ElevatorController::isDestinationListEmpty, &m_elevatorController) }, true);

	createTransition({ "MergeGoingUpGTCurrent" }, { "Ready", "ProcessedLists", "GoingUp" });

	createTransition({ "MergeMinGoingUp1" }, { "MergeMaxGoingDown1" },
					 { bind(&ElevatorController::isDestinationListEmpty, &m_elevatorController) }, true);

	createTransition({ "MergeMinGoingUp1" }, { "GoingDown", "Ready", "ProcessedLists" },
					 { bind(&ElevatorController::isMinSmallerThanCurrent, &m_elevatorController) }, true);

	createTransition({ "MergeMinGoingUp1" }, { "MergePostponedToCurrent11" },
					 { bind(&ElevatorController::isMinGreaterThanCurrent, &m_elevatorController) }, true);

	createTransition({ "MergeMaxGoingDown1" }, { "Ready", "ProcessedLists" },
					 { bind(&ElevatorController::isDestinationListEmpty, &m_elevatorController) }, true);

	createTransition({ "MergeMaxGoingDown1" }, { "MergePostponedToCurrent12" },
					 { bind(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController),
					   bind(&ElevatorController::isMaxSmallerThanCurrent, &m_elevatorController) },
					 true);

	createTransition({ "MergeMaxGoingDown1" }, { "Ready", "ProcessedLists", "GoingUp" },
					 { bind(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController),
					   bind(&ElevatorController::isMaxGreaterThanCurrent, &m_elevatorController) },
					 true);

	createTransition({ "MergePostponedToCurrent11" }, { "Ready", "ProcessedLists", "GoingUp" });

	createTransition({ "MergePostponedToCurrent12" }, { "Ready", "ProcessedLists", "GoingDown" });


	// Arriving a floor going up.

	createTransition({ "Ready", "ProcessLists", "GoingUp_", "DoorsClosed_" },
					 { "Ready", "GoingUp_", "DoorsClosed_", "ProcessedLists" },
					 { "OpenDoors", "ArrivedDestination" },
					 { bind(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) }, true);

	createTransition({ "Ready", "ProcessLists", "GoingUp_", "DoorsClosed_" }, { "DoorsClosed_", "SwapGU" },
					 { "OpenDoors", "ArrivedDestination" },
					 { bind(&ElevatorController::isDestinationListEmpty, &m_elevatorController) }, true);

	createTransition({ "SwapGU" }, { "MergeGoingDownSTCurrent" },
					 { bind(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController) }, true);

	createTransition({ "SwapGU" }, { "MergeMaxGoingDown2" },
					 { bind(&ElevatorController::isDestinationListEmpty, &m_elevatorController) }, true);

	createTransition({ "MergeGoingDownSTCurrent" }, { "Ready", "ProcessedLists", "GoingDown" });

	createTransition({ "MergeMaxGoingDown2" }, { "MergeMinGoingUp2" },
					 { bind(&ElevatorController::isDestinationListEmpty, &m_elevatorController) }, true);

	createTransition({ "MergeMaxGoingDown2" }, { "GoingUp", "Ready", "ProcessedLists" },
					 { bind(&ElevatorController::isMaxGreaterThanCurrent, &m_elevatorController) }, true);

	createTransition({ "MergeMaxGoingDown2" }, { "MergePostponedToCurrent21" },
					 { bind(&ElevatorController::isMaxSmallerThanCurrent, &m_elevatorController) }, true);

	createTransition({ "MergeMinGoingUp2" }, { "Ready", "ProcessedLists" },
					 { bind(&ElevatorController::isDestinationListEmpty, &m_elevatorController) }, true);

	createTransition({ "MergeMinGoingUp2" }, { "MergePostponedToCurrent22" },
					 { bind(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController),
					   bind(&ElevatorController::isMinGreaterThanCurrent, &m_elevatorController) },
					 true);

	createTransition({ "MergeMinGoingUp2" }, { "Ready", "ProcessedLists", "GoingDown" },
					 { bind(&ElevatorController::isDestinationListNotEmpty, &m_elevatorController),
					   bind(&ElevatorController::isMinSmallerThanCurrent, &m_elevatorController) },
					 true);

	createTransition({ "MergePostponedToCurrent21" }, { "Ready", "ProcessedLists", "GoingDown" });

	createTransition({ "MergePostponedToCurrent22" }, { "Ready", "ProcessedLists", "GoingUp" });
}

void ElevatorPetriNet::createButtonPressingTransitions()
{
	createTransition({ "DestinationButton", "Ready" }, { "D3" }, vector<string>{ "GoingUp_", "GoingDown_" },
					 { bind(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) }, true);

	createTransition({ "DestinationButton", "GoingUp_", "Ready" }, { "D1", "GoingUp_" },
					 { bind(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) }, true);

	createTransition({ "DestinationButton", "GoingDown_", "Ready" }, { "D2", "GoingDown_" },
					 { bind(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) }, true);

	createTransition({ "D1" }, { "AddToTravel" },
					 { bind(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "D1" }, { "AddToNextTravel" },
					 { bind(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "D2" }, { "AddToNextTravel" },
					 { bind(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "D2" }, { "AddToTravel" },
					 { bind(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "D3" }, { "GoingUp", "AddToTravel" },
					 { bind(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "D3" }, { "GoingDown", "AddToTravel" },
					 { bind(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "AddToNextTravel" }, { "Ready" });

	createTransition({ "AddToTravel" }, { "Ready" });
}

void ElevatorPetriNet::createCallingButtonTransitions()
{
	// Calling the elevator to go up.

	createTransition({ "CallButtonUp", "Ready" }, { "D3" }, vector<string>{ "GoingUp_", "GoingDown_" },
					 { bind(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) }, true);

	createTransition({ "CallButtonUp", "GoingUp_", "Ready" }, { "D4", "GoingUp_" },
					 { bind(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) }, true);

	createTransition({ "CallButtonUp", "GoingDown_", "Ready" }, { "WaitToGoUp", "GoingDown_" });

	createTransition({ "D4" }, { "WaitToGoUp" },
					 { bind(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "D4" }, { "AddToTravel" },
					 { bind(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "WaitToGoUp" }, { "Ready" });


	// Calling the elevator to go down.

	createTransition({ "CallButtonDown", "Ready" }, { "D3" }, vector<string>{ "GoingUp_", "GoingDown_" },
					 { bind(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) }, true);

	createTransition({ "CallButtonDown", "GoingDown_", "Ready" }, { "D5", "GoingDown_" },
					 { bind(&ElevatorController::isMarkedFloorNotCurrentFloor, &m_elevatorController) }, true);

	createTransition({ "CallButtonDown", "GoingUp_", "Ready" }, { "WaitToGoDown", "GoingUp_" });

	createTransition({ "D5" }, { "AddToTravel" },
					 { bind(&ElevatorController::isMarkedFloorSmallerThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "D5" }, { "WaitToGoDown" },
					 { bind(&ElevatorController::isMarkedFloorGreaterThanCurrentFloor, &m_elevatorController) },
					 true);

	createTransition({ "WaitToGoDown" }, { "Ready" });
}
