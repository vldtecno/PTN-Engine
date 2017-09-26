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

ElevatorPetriNet::ElevatorPetriNet(std::shared_ptr<ElevatorController> ptrController):
	PTN_Engine{}
{
	using namespace std;

	//Create places

	//inputs
	addPlace("CloseDoors", 0, nullptr, nullptr, true);
	addPlace("OpenDoors", 0, nullptr, nullptr, true);
	addPlace("CallButtonUp", 0, nullptr, nullptr, true);
	addPlace("CallButtonDown", 0, nullptr, nullptr, true);
	addPlace("DestinationButton", 0, nullptr, nullptr, true);

	//controller places
	addPlace("Stopped", 1, make_shared<ControllerAction>(ptrController, &ElevatorController::elevatorStopped), nullptr);
	addPlace("Moving", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::elevatorMoving), nullptr);
	addPlace("DoorsOpened", 1, make_shared<ControllerAction>(ptrController, &ElevatorController::doorsAreOpen), nullptr);
	addPlace("DoorsClosed", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::doorsAreClosed), nullptr);
	addPlace("ArrivedFloor", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::arrivedFloor), nullptr);
	addPlace("HasDestination", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::hasDestination), nullptr);

	addPlace("GoingUp", 0, nullptr, nullptr);
	addPlace("GoingDown", 0, nullptr, nullptr);
	addPlace("AddToTravel", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::addDestination1), nullptr); 
	addPlace("AddToNextTravel", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::addDestination2), nullptr); //needs function
	addPlace("WaitToGoUp", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::addWaitingToGoUp), nullptr);
	addPlace("WaitToGoDown", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::addWaitingToGoDown), nullptr);
	addPlace("ArrivedDestination", 0, nullptr, nullptr);
	addPlace("RemoveFromList", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::removeDestination), nullptr); //needs function
	addPlace("SwapLists", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::rotateLists), nullptr); //needs function
	addPlace("ProcessGoUp", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::processWaitingToGoUp), nullptr); //needs function
	addPlace("ProcessGoDown", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::processWaitingToGoDown), nullptr); //needs function
	addPlace("Ready", 1, nullptr, nullptr);
	addPlace("Aux1", 0, nullptr, nullptr);
	addPlace("Aux2", 0, nullptr, nullptr);
	addPlace("Aux3", 0, nullptr, nullptr);
	addPlace("P1", 0, nullptr, nullptr);
	addPlace("P2", 0, nullptr, nullptr);

	//simulation
	addPlace("IncreaseFloor", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::increaseFloor), nullptr); //needs function
	addPlace("DecreaseFloor", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::decreaseFloor), nullptr); //needs function

	
	//Create transitions

	createTransition({"CloseDoors", "Stopped", "DoorsOpened"}, {"Stopped", "DoorsClosed"},
		{}
	);

	createTransition({"OpenDoors", "Stopped", "DoorsClosed"}, {"Stopped", "DoorsOpened"},
		{}
	);

	createTransition({"HasDestination", "Stopped", "DoorsClosed" }, {"HasDestination", "DoorsClosed", "Moving"},
		{}
	);

	createTransition({"ArrivedDestination", "Moving"}, {"Stopped", "OpenDoors", "Ready"},
		{}
	);

	/////////////////
	//simulation
	createTransition({"Moving", "GoingUp", "Ready", "HasDestination"}, {"Moving", "GoingUp", "IncreaseFloor", "HasDestination" },
		{},
		{ "ArrivedFloor", "ArrivedDestination" }
	);
	
	createTransition({"IncreaseFloor"}, {"ArrivedFloor", "Ready" },
		{} 
	);


	createTransition({"Moving", "GoingDown", "Ready", "HasDestination" }, {"Moving", "GoingDown", "DecreaseFloor", "HasDestination" },
		{},
		{ "ArrivedFloor", "ArrivedDestination" }
	);

	createTransition({"DecreaseFloor"}, {"ArrivedFloor", "Ready" },
		{}
	);

	////////////

	//////////////////
	//Arriving a floor.	

	createTransition({"Ready", "ArrivedFloor"}, { "Ready" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isFloorNotInList) }
	); //arrived floor not in list

	createTransition({"Ready", "ArrivedFloor", "HasDestination"}, { "RemoveFromList" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isFloorInList) }
	); //arrived floor in list

	createTransition({"RemoveFromList"}, {"ArrivedDestination", "HasDestination"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty) }
	); //arrived floor not in list

	createTransition({"RemoveFromList"}, {"SwapLists"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) } 
	); //arrived floor in list

	createTransition({"SwapLists", "GoingUp"}, {"ProcessGoDown"},
		{}
	); 

	createTransition({"SwapLists", "GoingDown"}, {"ProcessGoUp"},
		{}
	);

	createTransition({"ProcessGoDown"},	{"ArrivedDestination"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) } 
	);

	createTransition({"ProcessGoUp"}, {"ArrivedDestination"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) } 
	);

	createTransition({"ProcessGoDown"},	{"ArrivedDestination", "HasDestination", "GoingDown"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty) } 
	);

	createTransition({"ProcessGoUp"}, {"ArrivedDestination", "HasDestination", "GoingUp"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty) } 
	);

	//////////////////
	

	//////////////////
	// Pressing a floor button inside the elevator.	

	createTransition({"DestinationButton", "Ready"}, {"P1"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) },
		{"GoingUp", "GoingDown"}
	);

	createTransition({"DestinationButton", "Ready", "GoingUp"}, {"Aux1", "GoingUp"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) } 
	);

	createTransition({"DestinationButton", "Ready", "GoingDown"}, {"Aux2", "GoingDown"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) } 
	);
	
	//has no destination

	createTransition({ "P1" }, {"P2", "GoingUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) } 
	);

	createTransition({ "P1" }, { "P2", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) } 
	);

	createTransition({ "P2" }, { "AddToTravel" },
		{} 
	);

	//is moving up

	createTransition({"Aux1"}, {"AddToTravel"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) } 
	);

	createTransition({"Aux1"}, { "AddToNextTravel"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) } 
	);

	//is moving down

	createTransition({ "Aux2" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) } 
	);

	createTransition({ "Aux2" }, { "AddToNextTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) } 
	);


	createTransition({ "AddToNextTravel" }, { "Ready" },
		{} 
	);

	createTransition({ "AddToTravel", "HasDestination" }, { "Ready", "HasDestination" },
		{}
	);

	createTransition({ "AddToTravel" }, { "Ready", "HasDestination" },
		{},
		{ "HasDestination" }
	);

	//////////////////


	//////////////////
	//Calling the elevator to go up.	

	createTransition({ "CallButtonUp", "Ready", "GoingUp" }, { "Aux2", "GoingUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) } 
	);

	createTransition({ "CallButtonUp", "Ready", "GoingDown" }, { "AddToNextTravel", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) } 
	);

	createTransition({ "CallButtonUp", "Ready" }, { "P1" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) }, 
		{ "GoingUp", "GoingDown" }
	);

	createTransition({ "Aux2" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) } 
	);

	createTransition({ "Aux2" }, { "WaitToGoUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) } 
	);

	createTransition({ "WaitToGoUp" }, { "Ready" },
		{} 
	);

	//////////////////

	//////////////////
	//Calling the elevator to go down.	

	createTransition({ "CallButtonDown", "Ready", "GoingDown" }, { "Aux3", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) }
	);

	createTransition({ "CallButtonDown", "Ready", "GoingUp" }, { "AddToNextTravel", "GoingUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) } 
	);

	createTransition({ "CallButtonDown", "Ready" }, { "P1" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) }, 
		{ "GoingUp", "GoingDown" }
	);

	createTransition({ "Aux3" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) }
	);

	createTransition({ "Aux3" }, { "WaitToGoDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) }
	);

	createTransition({ "WaitToGoDown" }, { "Ready" },
		{}
	);
	//////////////////

}

void ElevatorPetriNet::closeDoors()
{
	incrementInputPlace("CloseDoors");
	execute();
}

void ElevatorPetriNet::openDoors()
{
	incrementInputPlace("OpenDoors");
	execute();
}

void ElevatorPetriNet::callButtonUp()
{
	incrementInputPlace("CallButtonUp");
	execute();
}

void ElevatorPetriNet::callButtonDown()
{
	incrementInputPlace("CallButtonDown");
	execute();
}

void ElevatorPetriNet::destinationButton()
{
	incrementInputPlace("DestinationButton");
	execute();
}

//void Controller::MenuStateMachine::pressA()
//{
//	incrementInputPlace("InputA");
//	execute();
//}






