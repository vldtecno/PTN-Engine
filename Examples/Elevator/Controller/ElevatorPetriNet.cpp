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
	addPlace("Stopped", 1, nullptr, nullptr);
	addPlace("Moving", 0, nullptr, nullptr);
	addPlace("DoorsOpened", 1, nullptr, nullptr);
	addPlace("DoorsClosed", 0, nullptr, nullptr);
	addPlace("ArrivedFloor", 0, nullptr, nullptr);
	addPlace("HasDestination", 0, nullptr, nullptr);

	addPlace("GoingUp", 0, nullptr, nullptr);
	addPlace("GoingDown", 0, nullptr, nullptr);
	addPlace("AddToTravel", 0, nullptr, nullptr); //needs function
	addPlace("AddToNextTravel", 0, nullptr, nullptr); //needs function
	addPlace("WaitToGoUp", 0, nullptr, nullptr); 
	addPlace("WaitToGoDown", 0, nullptr, nullptr);
	addPlace("ArrivedDestination", 0, nullptr, nullptr);
	addPlace("RemoveFromList", 0, nullptr, nullptr); //needs function
	addPlace("SwapLists", 0, nullptr, nullptr); //needs function
	addPlace("ProcessGoUp", 0, nullptr, nullptr); //needs function
	addPlace("ProcessGoDown", 0, nullptr, nullptr); //needs function
	addPlace("Ready", 1, nullptr, nullptr);
	addPlace("Aux1", 0, nullptr, nullptr);
	addPlace("Aux2", 0, nullptr, nullptr);
	addPlace("Aux3", 0, nullptr, nullptr);
	addPlace("P1", 0, nullptr, nullptr);
	addPlace("P2", 0, nullptr, nullptr);

	//simulation
	addPlace("IncreaseFloor", 0, nullptr, nullptr); //needs function
	addPlace("DecreaseFloor", 0, nullptr, nullptr); //needs function

	
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
	createTransition({"Moving", "IncreaseFloor", "GoingUp"}, {"Moving", "OpenDoors"},
		{}
	);
	
	createTransition({"IncreaseFloor"}, {"IncreaseFloor"},
		{} //needs function
	);

	createTransition({"IncreaseFloor" }, {"ArrivedDestination"},
		{} //needs function
	);

	createTransition({"Moving", "DecreaseFloor", "GoingDown"}, {"Moving", "OpenDoors"},
		{}
	);

	createTransition({"DecreaseFloor"}, {"DecreaseFloor"},
		{} //needs function
	);

	createTransition({"DecreaseFloor"}, {"ArrivedDestination"},
		{} //needs function
	);
	////////////

	//////////////////
	//Arriving a floor.	

	createTransition({"Ready", "ArrivedFloor"}, { "Ready" },
		{} //needs function: floor not in list
	); //arrived floor not in list

	createTransition({"Ready", "ArrivedFloor", "HasDestination"}, { "RemoveFromList" },
		{} //needs function: floor in list
	); //arrived floor in list

	createTransition({"RemoveFromList"}, {"ArrivedDestination", "HasDestination"},
		{} //needs function: list not empty
	); //arrived floor not in list

	createTransition({"RemoveFromList"}, {"Swap"},
		{} //needs function: list not empty
	); //arrived floor in list

	createTransition({"Swap", "GoingUp"}, {"ProcessGoDown"},
		{}
	); 

	createTransition({"Swap", "GoingDown"}, {"ProcessGoUp"},
		{}
	);

	createTransition({"ProcessGoDown"},	{"ArrivedDestination"},
		{} //needs function: list is empty
	);

	createTransition({"ProcessGoUp"}, {"ArrivedDestination"},
		{} //needs function: list is empty
	);

	createTransition({"ProcessGoDown"},	{"ArrivedDestination", "HasDestination", "GoingDown"},
		{} //needs function: list is not empty
	);

	createTransition({"ProcessGoUp"}, {"ArrivedDestination", "HasDestination", "GoingUp"},
		{} //needs function: list is not empty
	);

	//////////////////
	

	//////////////////
	//Pressing a floor button inside the elevator.	

	createTransition({"DestinationButton", "Ready"}, {"P1"},
		{}, //needs function: marked floor != current floor
		{"GoingUp", "GoingDown"}
	);

	createTransition({"DestinationButton", "Ready", "GoingUp"}, {"Aux1", "GoingUp"},
		{} //needs function: marked floor != current floor
	);

	createTransition({"DestinationButton", "Ready", "GoingDown"}, {"Aux2", "GoingDown"},
		{} //needs function: marked floor != current floor
	);
	
	//has no destination

	createTransition({ "P1" }, {"P2", "GoingUp" },
		{} //needs function: pressed floor button > current floor
	);

	createTransition({ "P1" }, { "P2", "GoingDown" },
		{} //needs function: pressed floor button < current floor
	);

	createTransition({ "P2" }, { "AddToTravel" },
		{} 
	);

	//is moving up

	createTransition({"Aux1"}, {"AddToTravel"},
		{} //needs function: pressed floor button > current floor
	);

	createTransition({"Aux1"}, { "AddToNextTravel"},
		{} //needs function: pressed floor button < current floor
	);

	//is moving down

	createTransition({ "Aux2" }, { "AddToTravel" },
		{} //needs function: pressed floor button < current floor
	);

	createTransition({ "Aux2" }, { "AddToNextTravel" },
		{} //needs function: pressed floor button > current floor
	);


	createTransition({ "AddToNextTravel" }, { "Ready" },
		{} 
	);

	createTransition({ "AddToTravel", "HasDestination" }, { "Ready", "HasDestination" },
		{}
	);

	createTransition({ "AddToTravel" }, { "Ready" },
		{},
		{ "HasDestination" }
	);

	//////////////////


	//////////////////
	//Calling the elevator to go up.	

	createTransition({ "CallButtonUp", "Ready", "GoingUp" }, { "Aux2", "GoingUp" },
		{} //needs function: marked floor != current floor
	);

	createTransition({ "CallButtonUp", "Ready", "GoingDown" }, { "AddToNextTravel", "GoingDown" },
		{} //needs function: marked floor != current floor
	);

	createTransition({ "CallButtonUp", "Ready" }, { "P1" },
		{}, //needs function: marked floor != current floor
		{ "GoingUp", "GoingDown" }
	);

	createTransition({ "Aux2" }, { "AddToTravel" },
		{} //needs function: floor > current floor
	);

	createTransition({ "Aux2" }, { "WaitToGoUp" },
		{} //needs function: floor < current floor
	);

	createTransition({ "WaitToGoUp" }, { "Ready" },
		{} 
	);

	//////////////////

	//////////////////
	//Calling the elevator to go down.	

	createTransition({ "CallButtonDown", "Ready", "GoingDown" }, { "Aux3", "GoingDown" },
		{} //needs function: marked floor != current floor
	);

	createTransition({ "CallButtonDown", "Ready", "GoingUp" }, { "AddToNextTravel", "GoingUp" },
		{} //needs function: marked floor != current floor
	);

	createTransition({ "CallButtonDown", "Ready" }, { "P1" },
		{}, //needs function: marked floor != current floor
		{ "GoingUp", "GoingDown" }
	);

	createTransition({ "Aux3" }, { "AddToTravel" },
		{} //needs function: floor > current floor
	);

	createTransition({ "Aux3" }, { "WaitToGoDown" },
		{} //needs function: floor < current floor
	);

	createTransition({ "WaitToGoDown" }, { "Ready" },
		{}
	);
	//////////////////

}

void ElevatorPetriNet::closeDoors()
{
	incrementInputPlace("CloseDoors");
}

void ElevatorPetriNet::openDoors()
{
	incrementInputPlace("OpenDoors");
}

void ElevatorPetriNet::callButtonUp()
{
	incrementInputPlace("CallButtonUp");
}

void ElevatorPetriNet::callButtonDown()
{
	incrementInputPlace("CallButtonDown");
}

void ElevatorPetriNet::destinationButton()
{
	incrementInputPlace("DestinationButton");
}

//void Controller::MenuStateMachine::pressA()
//{
//	incrementInputPlace("InputA");
//	execute();
//}






