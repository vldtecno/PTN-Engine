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
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty) });

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
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isFloorNotInList) });

	createTransition({ "DecreaseFloor" }, { "Ready" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isFloorNotInList) });

	createTransition({ "IncreaseFloor" }, { "RemoveFromListGU" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isFloorInList) });

	createTransition({ "RemoveFromListGU" }, { "ProcessLists", "ArrivedDestination", "Ready" });

	createTransition({ "DecreaseFloor" }, { "RemoveFromListGD" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isFloorInList) });

	createTransition({ "RemoveFromListGD" }, { "ProcessLists", "ArrivedDestination", "Ready" });

}

void ElevatorPetriNet::createArrivingFloorTransitions(shared_ptr<ElevatorController> ptrController)
{
	// Arriving a floor going down.	

	createTransition({ "Ready", "ProcessLists", "GoingDown_", "DoorsClosed_" },
		{ "Ready", "GoingDown_", "DoorsClosed_", "ProcessedLists" },
		{ "OpenDoors", "ArrivedDestination" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty) });

	createTransition({ "Ready", "ProcessLists", "GoingDown_", "DoorsClosed_" }, { "DoorsClosed_", "SwapGD" },
		{ "OpenDoors", "ArrivedDestination" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) });

	createTransition({ "SwapGD" }, { "MergeGoingUpGTCurrent" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty) });

	createTransition({ "SwapGD" }, { "MergeMinGoingUp1" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) });

	createTransition({ "MergeGoingUpGTCurrent" }, { "Ready", "ProcessedLists", "GoingUp" });

	createTransition({ "MergeMinGoingUp1" }, { "MergeMaxGoingDown1" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) });

	createTransition({ "MergeMinGoingUp1" }, { "GoingDown", "Ready", "ProcessedLists" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMinSmallerThanCurrent) });

	createTransition({ "MergeMinGoingUp1" }, { "MergePostponedToCurrent11" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMinGreaterThanCurrent) });

	createTransition({ "MergeMaxGoingDown1" }, { "Ready", "ProcessedLists" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) });

	createTransition({ "MergeMaxGoingDown1" }, { "MergePostponedToCurrent12" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty),
		  make_shared<FireCondition>(ptrController, &ElevatorController::isMaxSmallerThanCurrent) });

	createTransition({ "MergeMaxGoingDown1" }, { "Ready", "ProcessedLists", "GoingUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty),
		  make_shared<FireCondition>(ptrController, &ElevatorController::isMaxGreaterThanCurrent) });

	createTransition({ "MergePostponedToCurrent11" }, { "Ready", "ProcessedLists", "GoingUp" });

	createTransition({ "MergePostponedToCurrent12" }, { "Ready", "ProcessedLists", "GoingDown" });


	// Arriving a floor going up.	

	createTransition({ "Ready", "ProcessLists", "GoingUp_", "DoorsClosed_" },
		{ "Ready", "GoingUp_", "DoorsClosed_", "ProcessedLists" },
		{ "OpenDoors", "ArrivedDestination" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty) });

	createTransition({ "Ready", "ProcessLists", "GoingUp_", "DoorsClosed_" }, { "DoorsClosed_", "SwapGU" },
		{ "OpenDoors", "ArrivedDestination" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) });

	createTransition({ "SwapGU" }, { "MergeGoingDownSTCurrent" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty) });

	createTransition({ "SwapGU" }, { "MergeMaxGoingDown2" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) });

	createTransition({ "MergeGoingDownSTCurrent" }, { "Ready", "ProcessedLists", "GoingDown" });

	createTransition({ "MergeMaxGoingDown2" }, { "MergeMinGoingUp2" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) });

	createTransition({ "MergeMaxGoingDown2" }, { "GoingUp", "Ready", "ProcessedLists" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMaxGreaterThanCurrent) });

	createTransition({ "MergeMaxGoingDown2" }, { "MergePostponedToCurrent21" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMaxSmallerThanCurrent) });

	createTransition({ "MergeMinGoingUp2" }, { "Ready", "ProcessedLists" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) });

	createTransition({ "MergeMinGoingUp2" }, { "MergePostponedToCurrent22" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty),
		  make_shared<FireCondition>(ptrController, &ElevatorController::isMinGreaterThanCurrent) });

	createTransition({ "MergeMinGoingUp2" }, { "Ready", "ProcessedLists", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty),
		  make_shared<FireCondition>(ptrController, &ElevatorController::isMinSmallerThanCurrent) });

	createTransition({ "MergePostponedToCurrent21" }, { "Ready", "ProcessedLists", "GoingDown" });

	createTransition({ "MergePostponedToCurrent22" }, { "Ready", "ProcessedLists", "GoingUp" });

}

void ElevatorPetriNet::createButtonPressingTransitions(shared_ptr<ElevatorController> ptrController)
{
	createTransition({ "DestinationButton", "Ready" }, { "D3" },
		vector<string>{ "GoingUp_", "GoingDown_" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "DestinationButton", "GoingUp_", "Ready" }, { "D1", "GoingUp_" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "DestinationButton", "GoingDown_", "Ready" }, { "D2", "GoingDown_" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "D1" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) });

	createTransition({ "D1" }, { "AddToNextTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) });

	createTransition({ "D2" }, { "AddToNextTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) });

	createTransition({ "D2" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) });

	createTransition({ "D3" }, { "GoingUp",  "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) });

	createTransition({ "D3" }, { "GoingDown",  "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) });

	createTransition({ "AddToNextTravel" }, { "Ready" });

	createTransition({ "AddToTravel" }, { "Ready" });
}

void ElevatorPetriNet::createCallingButtonTransitions(shared_ptr<ElevatorController> ptrController)
{
	// Calling the elevator to go up.

	createTransition({ "CallButtonUp", "Ready" }, { "D3" },
		vector<string>{ "GoingUp_", "GoingDown_" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "CallButtonUp", "GoingUp_", "Ready" }, { "D4", "GoingUp_" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "CallButtonUp", "GoingDown_", "Ready" }, { "WaitToGoUp", "GoingDown_" });

	createTransition({ "D4" }, { "WaitToGoUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) });

	createTransition({ "D4" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) });

	createTransition({ "WaitToGoUp" }, { "Ready" });


	// Calling the elevator to go down.	

	createTransition({ "CallButtonDown", "Ready" }, { "D3" },
		vector<string>{ "GoingUp_", "GoingDown_" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "CallButtonDown", "GoingDown_", "Ready" }, { "D5", "GoingDown_" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "CallButtonDown", "GoingUp_", "Ready" }, { "WaitToGoDown", "GoingUp_" });

	createTransition({ "D5" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) });

	createTransition({ "D5" }, { "WaitToGoDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) });

	createTransition({ "WaitToGoDown" }, { "Ready" });
}

