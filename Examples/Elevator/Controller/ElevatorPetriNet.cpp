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

#include "Controller/ElevatorPetriNet.h"

using namespace ptne;
using namespace std;

ElevatorPetriNet::ElevatorPetriNet(shared_ptr<ElevatorController> ptrController):
	PTN_Engine{}
{
	//Create places
	createInputPlaces();
	createCabinePlaces(ptrController);
	createArrivalPlaces(ptrController);
	createButtonPressPlaces(ptrController);
	createSimulationPlaces(ptrController);	

	//CreateTransitions
	createCabineTransitions(ptrController);
	createSimulationTransitions(ptrController);
	createArrivingFloorTransitions(ptrController);
	createButtonPressingTransitions(ptrController);
	createCallingButtonTransitions(ptrController);
}

void ElevatorPetriNet::closeDoors()
{
	incrementAndExecute("CloseDoors");
}

void ElevatorPetriNet::openDoors()
{
	incrementAndExecute("OpenDoors");
}

void ElevatorPetriNet::callButtonUp()
{
	incrementAndExecute("CallButtonUp");
}

void ElevatorPetriNet::callButtonDown()
{
	incrementAndExecute("CallButtonDown");
}

void ElevatorPetriNet::destinationButton()
{
	incrementAndExecute("DestinationButton");
}

void ElevatorPetriNet::incrementAndExecute(const string& inputPlace, const bool debug)
{
	incrementInputPlace(inputPlace);
	execute(debug);
}

void ElevatorPetriNet::createInputPlaces()
{
	createPlace("CloseDoors", 0, true);
	createPlace("OpenDoors", 0, true);
	createPlace("CallButtonUp", 0, true);
	createPlace("CallButtonDown", 0, true);
	createPlace("DestinationButton", 0, true);
}

void ElevatorPetriNet::createCabinePlaces(shared_ptr<ElevatorController> ptrController)
{
	createPlace("Stopped", 0, bind( &ElevatorController::elevatorStopped, ptrController));
	createPlace("Stopped_", 1);
	createPlace("Moving", 0, bind( &ElevatorController::elevatorMoving, ptrController));
	createPlace("Moving_", 0);
	createPlace("DoorsOpened", 1, bind( &ElevatorController::doorsAreOpen, ptrController));
	createPlace("DoorsClosed", 0, bind( &ElevatorController::doorsAreClosed, ptrController));
	createPlace("DoorsClosed_", 0);
	
	createPlace("RemoveFromListGU", 0, bind( &ElevatorController::removeDestinationGU, ptrController));
	createPlace("RemoveFromListGD", 0, bind( &ElevatorController::removeDestinationGD, ptrController));
	createPlace("ArrivedDestination", 0);

	createPlace("ProcessLists", 0);
	createPlace("ProcessedLists", 1, bind( &ElevatorController::processedLists, ptrController));
	
	createPlace("GoingUp", 0, bind( &ElevatorController::goingUp, ptrController));
	createPlace("GoingUp_", 0);
	createPlace("GoingDown", 0, bind( &ElevatorController::goingDown, ptrController));
	createPlace("GoingDown_", 0);
}

void ElevatorPetriNet::createArrivalPlaces(shared_ptr<ElevatorController> ptrController)
{
	createPlace("Ready", 1);

	createPlace("SwapGD", 0, bind( &ElevatorController::rotateLists, ptrController));
	createPlace("SwapGU", 0, bind( &ElevatorController::rotateLists, ptrController));

	createPlace("MergeGoingUpGTCurrent", 0, bind( &ElevatorController::mergeGoingUpGTCurrent, ptrController));
	createPlace("MergeMinGoingUp1", 0, bind( &ElevatorController::mergeMinGoingUp, ptrController));
	createPlace("MergeMaxGoingDown1", 0, bind( &ElevatorController::mergeMaxGoingDown, ptrController));
	createPlace("MergePostponedToCurrent11", 0, bind( &ElevatorController::mergePostponedToCurrent, ptrController));
	createPlace("MergePostponedToCurrent12", 0, bind( &ElevatorController::mergePostponedToCurrent, ptrController));

	createPlace("MergeGoingDownSTCurrent", 0, bind( &ElevatorController::mergeGoingDownSTCurrent, ptrController));
	createPlace("MergeMaxGoingDown2", 0, bind( &ElevatorController::mergeMaxGoingDown, ptrController));
	createPlace("MergeMinGoingUp2", 0, bind( &ElevatorController::mergeMinGoingUp, ptrController));
	createPlace("MergePostponedToCurrent21", 0, bind( &ElevatorController::mergePostponedToCurrent, ptrController));
	createPlace("MergePostponedToCurrent22", 0, bind( &ElevatorController::mergePostponedToCurrent, ptrController));
}

void ElevatorPetriNet::createButtonPressPlaces(shared_ptr<ElevatorController> ptrController)
{
	createPlace("AddToTravel", 0, bind( &ElevatorController::addDestination1, ptrController));
	createPlace("AddToNextTravel", 0, bind( &ElevatorController::addDestination2, ptrController));
	createPlace("WaitToGoUp", 0, bind( &ElevatorController::addWaitingToGoUp, ptrController));
	createPlace("WaitToGoDown", 0, bind( &ElevatorController::addWaitingToGoDown, ptrController));
	createPlace("D1", 0);
	createPlace("D2", 0);
	createPlace("D3", 0);
	createPlace("D4", 0);
	createPlace("D5", 0);
}

void ElevatorPetriNet::createSimulationPlaces(shared_ptr<ElevatorController> ptrController)
{
	createPlace("IncreaseFloor", 0, bind(&ElevatorController::increaseFloor, ptrController));
	createPlace("DecreaseFloor", 0, bind(&ElevatorController::decreaseFloor, ptrController));
}

void ElevatorPetriNet::createCabineTransitions(shared_ptr<ElevatorController> ptrController)
{
	createTransition({ "CloseDoors", "Stopped_", "DoorsOpened" }, { "Stopped_", "DoorsClosed" });

	createTransition({ "OpenDoors", "Stopped_", "DoorsClosed_" }, { "Stopped_", "DoorsOpened" });

	createTransition({ "Stopped_", "DoorsClosed_", "ProcessedLists" }, { "DoorsClosed_", "Moving" },
		{ "OpenDoors" },
		{ bind(&ElevatorController::isDestinationListNotEmpty, ptrController) });

	createTransition({ "ArrivedDestination", "Moving_" }, { "Stopped", "OpenDoors" });

	// Trick to prevent read arcs from executing multiple times one place
	createTransition({ "DoorsClosed" }, { "DoorsClosed_" });
	createTransition({ "Moving" }, { "Moving_" });
	createTransition({ "Stopped" }, { "Stopped_" });
	createTransition({ "GoingUp" }, { "GoingUp_" });
	createTransition({ "GoingDown" }, { "GoingDown_" });
}

void ElevatorPetriNet::createSimulationTransitions(shared_ptr<ElevatorController> ptrController)
{
	createTransition({ "GoingUp_", "Ready", "Moving_" }, { "Moving_", "GoingUp_", "IncreaseFloor" },
		vector<string>{ "ArrivedDestination" });

	createTransition({ "GoingDown_", "Ready", "Moving_" }, { "Moving_", "GoingDown_", "DecreaseFloor" },
		vector<string>{ "ArrivedDestination" });

	createTransition({ "IncreaseFloor" }, { "Ready" },
		{ bind(&ElevatorController::isFloorNotInList, ptrController) });

	createTransition({ "DecreaseFloor" }, { "Ready" },
		{ bind( &ElevatorController::isFloorNotInList, ptrController) });

	createTransition({ "IncreaseFloor" }, { "RemoveFromListGU" },
		{ bind( &ElevatorController::isFloorInList, ptrController) });

	createTransition({ "RemoveFromListGU" }, { "ProcessLists", "ArrivedDestination", "Ready" });

	createTransition({ "DecreaseFloor" }, { "RemoveFromListGD" },
		{ bind( &ElevatorController::isFloorInList, ptrController) });

	createTransition({ "RemoveFromListGD" }, { "ProcessLists", "ArrivedDestination", "Ready" });

}

void ElevatorPetriNet::createArrivingFloorTransitions(shared_ptr<ElevatorController> ptrController)
{
	// Arriving a floor going down.	

	createTransition({ "Ready", "ProcessLists", "GoingDown_", "DoorsClosed_" },
		{ "Ready", "GoingDown_", "DoorsClosed_", "ProcessedLists" },
		{ "OpenDoors", "ArrivedDestination" },
		{ bind( &ElevatorController::isDestinationListNotEmpty, ptrController) });

	createTransition({ "Ready", "ProcessLists", "GoingDown_", "DoorsClosed_" }, { "DoorsClosed_", "SwapGD" },
		{ "OpenDoors", "ArrivedDestination" },
		{ bind( &ElevatorController::isDestinationListEmpty, ptrController) });

	createTransition({ "SwapGD" }, { "MergeGoingUpGTCurrent" },
		{ bind( &ElevatorController::isDestinationListNotEmpty, ptrController) });

	createTransition({ "SwapGD" }, { "MergeMinGoingUp1" },
		{ bind( &ElevatorController::isDestinationListEmpty, ptrController) });

	createTransition({ "MergeGoingUpGTCurrent" }, { "Ready", "ProcessedLists", "GoingUp" });

	createTransition({ "MergeMinGoingUp1" }, { "MergeMaxGoingDown1" },
		{ bind( &ElevatorController::isDestinationListEmpty, ptrController) });

	createTransition({ "MergeMinGoingUp1" }, { "GoingDown", "Ready", "ProcessedLists" },
		{ bind( &ElevatorController::isMinSmallerThanCurrent, ptrController) });

	createTransition({ "MergeMinGoingUp1" }, { "MergePostponedToCurrent11" },
		{ bind( &ElevatorController::isMinGreaterThanCurrent, ptrController) });

	createTransition({ "MergeMaxGoingDown1" }, { "Ready", "ProcessedLists" },
		{ bind( &ElevatorController::isDestinationListEmpty, ptrController) });

	createTransition({ "MergeMaxGoingDown1" }, { "MergePostponedToCurrent12" },
		{ bind( &ElevatorController::isDestinationListNotEmpty, ptrController),
		  bind( &ElevatorController::isMaxSmallerThanCurrent, ptrController) });

	createTransition({ "MergeMaxGoingDown1" }, { "Ready", "ProcessedLists", "GoingUp" },
		{ bind( &ElevatorController::isDestinationListNotEmpty, ptrController),
		  bind( &ElevatorController::isMaxGreaterThanCurrent, ptrController) });

	createTransition({ "MergePostponedToCurrent11" }, { "Ready", "ProcessedLists", "GoingUp" });

	createTransition({ "MergePostponedToCurrent12" }, { "Ready", "ProcessedLists", "GoingDown" });


	// Arriving a floor going up.	

	createTransition({ "Ready", "ProcessLists", "GoingUp_", "DoorsClosed_" },
		{ "Ready", "GoingUp_", "DoorsClosed_", "ProcessedLists" },
		{ "OpenDoors", "ArrivedDestination" },
		{ bind( &ElevatorController::isDestinationListNotEmpty, ptrController) });

	createTransition({ "Ready", "ProcessLists", "GoingUp_", "DoorsClosed_" }, { "DoorsClosed_", "SwapGU" },
		{ "OpenDoors", "ArrivedDestination" },
		{ bind( &ElevatorController::isDestinationListEmpty, ptrController) });

	createTransition({ "SwapGU" }, { "MergeGoingDownSTCurrent" },
		{ bind( &ElevatorController::isDestinationListNotEmpty, ptrController) });

	createTransition({ "SwapGU" }, { "MergeMaxGoingDown2" },
		{ bind( &ElevatorController::isDestinationListEmpty, ptrController) });

	createTransition({ "MergeGoingDownSTCurrent" }, { "Ready", "ProcessedLists", "GoingDown" });

	createTransition({ "MergeMaxGoingDown2" }, { "MergeMinGoingUp2" },
		{ bind( &ElevatorController::isDestinationListEmpty, ptrController) });

	createTransition({ "MergeMaxGoingDown2" }, { "GoingUp", "Ready", "ProcessedLists" },
		{ bind( &ElevatorController::isMaxGreaterThanCurrent, ptrController) });

	createTransition({ "MergeMaxGoingDown2" }, { "MergePostponedToCurrent21" },
		{ bind( &ElevatorController::isMaxSmallerThanCurrent, ptrController) });

	createTransition({ "MergeMinGoingUp2" }, { "Ready", "ProcessedLists" },
		{ bind( &ElevatorController::isDestinationListEmpty, ptrController) });

	createTransition({ "MergeMinGoingUp2" }, { "MergePostponedToCurrent22" },
		{ bind( &ElevatorController::isDestinationListNotEmpty, ptrController),
		  bind( &ElevatorController::isMinGreaterThanCurrent, ptrController) });

	createTransition({ "MergeMinGoingUp2" }, { "Ready", "ProcessedLists", "GoingDown" },
		{ bind( &ElevatorController::isDestinationListNotEmpty, ptrController),
		  bind( &ElevatorController::isMinSmallerThanCurrent, ptrController) });

	createTransition({ "MergePostponedToCurrent21" }, { "Ready", "ProcessedLists", "GoingDown" });

	createTransition({ "MergePostponedToCurrent22" }, { "Ready", "ProcessedLists", "GoingUp" });

}

void ElevatorPetriNet::createButtonPressingTransitions(shared_ptr<ElevatorController> ptrController)
{
	createTransition({ "DestinationButton", "Ready" }, { "D3" },
		vector<string>{ "GoingUp_", "GoingDown_" },
		{ bind( &ElevatorController::isMarkedFloorNotCurrentFloor, ptrController) });

	createTransition({ "DestinationButton", "GoingUp_", "Ready" }, { "D1", "GoingUp_" },
		{ bind( &ElevatorController::isMarkedFloorNotCurrentFloor, ptrController) });

	createTransition({ "DestinationButton", "GoingDown_", "Ready" }, { "D2", "GoingDown_" },
		{ bind( &ElevatorController::isMarkedFloorNotCurrentFloor, ptrController) });

	createTransition({ "D1" }, { "AddToTravel" },
		{ bind( &ElevatorController::isMarkedFloorGreaterThanCurrentFloor, ptrController) });

	createTransition({ "D1" }, { "AddToNextTravel" },
		{ bind( &ElevatorController::isMarkedFloorSmallerThanCurrentFloor, ptrController) });

	createTransition({ "D2" }, { "AddToNextTravel" },
		{ bind( &ElevatorController::isMarkedFloorGreaterThanCurrentFloor, ptrController) });

	createTransition({ "D2" }, { "AddToTravel" },
		{ bind( &ElevatorController::isMarkedFloorSmallerThanCurrentFloor, ptrController) });

	createTransition({ "D3" }, { "GoingUp",  "AddToTravel" },
		{ bind( &ElevatorController::isMarkedFloorGreaterThanCurrentFloor, ptrController) });

	createTransition({ "D3" }, { "GoingDown",  "AddToTravel" },
		{ bind( &ElevatorController::isMarkedFloorSmallerThanCurrentFloor, ptrController) });

	createTransition({ "AddToNextTravel" }, { "Ready" });

	createTransition({ "AddToTravel" }, { "Ready" });
}

void ElevatorPetriNet::createCallingButtonTransitions(shared_ptr<ElevatorController> ptrController)
{
	// Calling the elevator to go up.

	createTransition({ "CallButtonUp", "Ready" }, { "D3" },
		vector<string>{ "GoingUp_", "GoingDown_" },
		{ bind( &ElevatorController::isMarkedFloorNotCurrentFloor, ptrController) });

	createTransition({ "CallButtonUp", "GoingUp_", "Ready" }, { "D4", "GoingUp_" },
		{ bind( &ElevatorController::isMarkedFloorNotCurrentFloor, ptrController) });

	createTransition({ "CallButtonUp", "GoingDown_", "Ready" }, { "WaitToGoUp", "GoingDown_" });

	createTransition({ "D4" }, { "WaitToGoUp" },
		{ bind( &ElevatorController::isMarkedFloorSmallerThanCurrentFloor, ptrController) });

	createTransition({ "D4" }, { "AddToTravel" },
		{ bind( &ElevatorController::isMarkedFloorGreaterThanCurrentFloor, ptrController) });

	createTransition({ "WaitToGoUp" }, { "Ready" });


	// Calling the elevator to go down.	

	createTransition({ "CallButtonDown", "Ready" }, { "D3" },
		vector<string>{ "GoingUp_", "GoingDown_" },
		{ bind( &ElevatorController::isMarkedFloorNotCurrentFloor, ptrController) });

	createTransition({ "CallButtonDown", "GoingDown_", "Ready" }, { "D5", "GoingDown_" },
		{ bind( &ElevatorController::isMarkedFloorNotCurrentFloor, ptrController) });

	createTransition({ "CallButtonDown", "GoingUp_", "Ready" }, { "WaitToGoDown", "GoingUp_" });

	createTransition({ "D5" }, { "AddToTravel" },
		{ bind( &ElevatorController::isMarkedFloorSmallerThanCurrentFloor, ptrController) });

	createTransition({ "D5" }, { "WaitToGoDown" },
		{ bind( &ElevatorController::isMarkedFloorGreaterThanCurrentFloor, ptrController) });

	createTransition({ "WaitToGoDown" }, { "Ready" });
}

