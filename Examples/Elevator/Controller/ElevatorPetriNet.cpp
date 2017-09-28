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

ElevatorPetriNet::ElevatorPetriNet(std::shared_ptr<ElevatorController> ptrController):
	PTN_Engine{}
{
	//Create places

	//inputs
	createPlace("CloseDoors", 0, true);
	createPlace("OpenDoors", 0, true);
	createPlace("CallButtonUp", 0, true);
	createPlace("CallButtonDown", 0, true);
	createPlace("DestinationButton", 0, true);

	//controller places
	createPlace("Stopped", 1, make_shared<ControllerAction>(ptrController, &ElevatorController::elevatorStopped));
	createPlace("Moving", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::elevatorMoving));
	createPlace("DoorsOpened", 1, make_shared<ControllerAction>(ptrController, &ElevatorController::doorsAreOpen));
	createPlace("DoorsClosed", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::doorsAreClosed));
	createPlace("ArrivedFloor", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::arrivedFloor));
	createPlace("HasDestination", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::hasDestination));

	createPlace("GoingUp", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::goingUp));
	createPlace("GoingDown", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::goingDown));
	createPlace("AddToTravel", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::addDestination1)); 
	createPlace("AddToNextTravel", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::addDestination2));
	createPlace("WaitToGoUp", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::addWaitingToGoUp));
	createPlace("WaitToGoDown", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::addWaitingToGoDown));
	createPlace("ArrivedDestination", 0);
	createPlace("RemoveFromList", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::removeDestination));
	createPlace("SwapLists", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::rotateLists));
	createPlace("ProcessGoUp", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::processWaitingToGoUp));
	createPlace("ProcessGoDown", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::processWaitingToGoDown));
	createPlace("Ready", 1);
	createPlace("Aux1", 0);
	createPlace("Aux2", 0);
	createPlace("Aux3", 0);
	createPlace("Aux4", 0);
	createPlace("P1", 0);
	createPlace("P2", 0);
	
	createPlace("SetMinimum", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::setMinimum));
	createPlace("SetMaximum", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::setMaximum));
	createPlace("ResetMinimum", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::resetMinimum));
	createPlace("ResetMaximum", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::resetMaximum));

	//simulation
	createPlace("IncreaseFloor", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::increaseFloor)); 
	createPlace("DecreaseFloor", 0, make_shared<ControllerAction>(ptrController, &ElevatorController::decreaseFloor)); 

	
	//////////////////
	// Cabine

	createTransition({"CloseDoors", "Stopped", "DoorsOpened"}, {"Stopped", "DoorsClosed"});

	createTransition({"OpenDoors", "Stopped", "DoorsClosed"}, {"Stopped", "DoorsOpened"});

	createTransition({ "HasDestination", "Stopped", "DoorsClosed" }, { "HasDestination", "DoorsClosed", "Moving" }, 
		vector<string>({ "OpenDoors" }));

	createTransition({"ArrivedDestination", "Moving"}, {"Stopped", "OpenDoors", "Ready"});	
	//////////////////

	/////////////////
	// Simulation
	createTransition({"Moving", "GoingUp", "Ready", "HasDestination"}, {"Moving", "GoingUp", "IncreaseFloor", "HasDestination" },
		vector<string>({ "ArrivedFloor", "ArrivedDestination" }) );
	
	createTransition({"IncreaseFloor"}, {"ArrivedFloor", "Ready" });


	createTransition({"Moving", "GoingDown", "Ready", "HasDestination" }, {"Moving", "GoingDown", "DecreaseFloor", "HasDestination" },
		vector<string>({ "ArrivedFloor", "ArrivedDestination" }) );

	createTransition({"DecreaseFloor"}, {"ArrivedFloor", "Ready" });
	//////////////////

	//////////////////
	// Arriving a floor.	

	//arrived floor not in list
	createTransition({"Ready", "ArrivedFloor"}, { "Ready" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isFloorNotInList) }); 	

	//arrived floor in list
	createTransition({"Ready", "ArrivedFloor", "HasDestination"}, { "RemoveFromList" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isFloorInList) }); 	

	//arrived floor in list
	createTransition({"RemoveFromList"}, {"ArrivedDestination", "HasDestination"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListNotEmpty) }); 
	
	//arrived floor in list
	createTransition({"RemoveFromList"}, {"SwapLists"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) } ); 
	
	createTransition({"SwapLists", "GoingUp"}, { "ResetMaximum"}); 

	createTransition({ "ResetMaximum" }, { "ProcessGoDown" });

	createTransition({"SwapLists", "GoingDown"}, { "ResetMinimum"});

	createTransition({ "ResetMinimum" }, { "ProcessGoUp" });

	createTransition({"ProcessGoDown"},	{"ArrivedDestination"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) } );

	createTransition({"ProcessGoUp"}, {"ArrivedDestination"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isDestinationListEmpty) });



	createTransition({"ProcessGoDown"},	{"ArrivedDestination", "HasDestination", "GoingDown"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::hasDestinationSmallerThanCurrent) });

	createTransition({ "ProcessGoDown" }, { "ArrivedDestination", "HasDestination", "GoingUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::hasDestinationGreaterThanCurrent) });



	createTransition({"ProcessGoUp"}, {"ArrivedDestination", "HasDestination", "GoingUp"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::hasDestinationGreaterThanCurrent) });

	createTransition({ "ProcessGoUp" }, { "ArrivedDestination", "HasDestination", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::hasDestinationSmallerThanCurrent) });

	//////////////////	

	//////////////////
	// Pressing a floor button inside the elevator.	

	createTransition({"DestinationButton", "Ready"}, {"P1"}, { "GoingUp", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({"DestinationButton", "Ready", "GoingUp"}, {"Aux1", "GoingUp"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({"DestinationButton", "Ready", "GoingDown"}, {"Aux2", "GoingDown"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });
	
	//has no destination

	createTransition({ "P1" }, {"P2", "GoingUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorGreaterThanCurrentFloor) });

	createTransition({ "P1" }, { "P2", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorSmallerThanCurrentFloor) });

	createTransition({ "P2" }, { "AddToTravel" });

	//is moving up

	createTransition({"Aux1"}, {"AddToTravel"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::conditionAux1_T1) });

	createTransition({"Aux1"}, { "AddToNextTravel"},
		{ make_shared<FireCondition>(ptrController, &ElevatorController::conditionAux1_T2) });

	//is moving down

	createTransition({ "Aux2" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::conditionAux2_T1) });

	createTransition({ "Aux2" }, { "AddToNextTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::conditionAux2_T2) });


	createTransition({ "AddToNextTravel" }, { "Ready" });

	createTransition({ "AddToTravel", "HasDestination" }, { "Ready", "HasDestination" });

	createTransition({ "AddToTravel" }, { "Ready", "HasDestination" }, { "HasDestination" });
	//////////////////

	//////////////////
	// Calling the elevator to go up.	

	createTransition({ "CallButtonUp", "Ready", "GoingUp" }, { "Aux3", "GoingUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "CallButtonUp", "Ready", "GoingDown" }, { "WaitToGoUp", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "CallButtonUp", "Ready" }, { "SetMinimum" }, { "GoingUp", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) });

	createTransition({ "SetMinimum" }, { "P1" });

	createTransition({ "Aux3" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::conditionAux3_T1) });

	createTransition({ "Aux3" }, { "WaitToGoUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::conditionAux3_T2) });

	createTransition({ "WaitToGoUp" }, { "Ready" });
	//////////////////

	//////////////////
	// Calling the elevator to go down.	

	createTransition({ "CallButtonDown", "Ready", "GoingDown" }, { "Aux4", "GoingDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) }
	);

	createTransition({ "CallButtonDown", "Ready", "GoingUp" }, { "WaitToGoDown", "GoingUp" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) } );

	createTransition({ "CallButtonDown", "Ready" }, { "SetMaximum" }, { "GoingUp", "GoingDown" }, 
		{ make_shared<FireCondition>(ptrController, &ElevatorController::isMarkedFloorNotCurrentFloor) } );

	createTransition({ "SetMaximum" }, { "P1" });

	createTransition({ "Aux4" }, { "AddToTravel" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::conditionAux4_T1) } );

	createTransition({ "Aux4" }, { "WaitToGoDown" },
		{ make_shared<FireCondition>(ptrController, &ElevatorController::conditionAux4_T2) });

	createTransition({ "WaitToGoDown" }, { "Ready" });
	//////////////////

}

void ElevatorPetriNet::closeDoors()
{
	incrementInputPlace("CloseDoors");
	//execute(true);
	execute();
}

void ElevatorPetriNet::openDoors()
{
	incrementInputPlace("OpenDoors");
	//execute(true);
	execute();
}

void ElevatorPetriNet::callButtonUp()
{
	incrementInputPlace("CallButtonUp");
	//execute(true);
	execute();
}

void ElevatorPetriNet::callButtonDown()
{
	incrementInputPlace("CallButtonDown");
	//execute(true);
	execute();
}

void ElevatorPetriNet::destinationButton()
{
	incrementInputPlace("DestinationButton");
	//execute(true);
	execute();
}

//void Controller::MenuStateMachine::pressA()
//{
//	incrementInputPlace("InputA");
//	execute();
//}






