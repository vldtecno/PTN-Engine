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

#include "Controller/MenuStateMachine.h"

#include "PTN_Engine/Place.h"

using namespace ptne;

Controller::MenuStateMachine::MenuStateMachine(std::shared_ptr<Controller> ptrMenuController):
	PTN_Engine{}
{
	using namespace std;

	//Places
	addPlace("InputA",0, nullptr, nullptr, true);
	addPlace("InputB",0, nullptr, nullptr, true);
	addPlace("InputC", 0, nullptr, nullptr, true);
	addPlace("CallsMenuSelected", 1,
			make_shared<ControllerAction>(ptrMenuController, &Controller::showMainMenu),
			nullptr);
	addPlace("MessagesMenuSelected", 0,
			make_shared<ControllerAction>(ptrMenuController, &Controller::showMainMenu),
			nullptr);
	addPlace("SelectMessagesOption", 0,
			make_shared<ControllerAction>(ptrMenuController, &Controller::selectMessagesOption),
			nullptr);
	addPlace("SelectCallsOption", 0,
			make_shared<ControllerAction>(ptrMenuController, &Controller::selectCallsOption),
			nullptr);
	addPlace("CallsLog", 0,
			make_shared<ControllerAction>(ptrMenuController, &Controller::showCallsMenu),
			nullptr);
	addPlace("MessagesMenu", 0,
			make_shared<ControllerAction>(ptrMenuController, &Controller::showMessageMenu),
			nullptr);
	addPlace("NextMessage", 0,
			make_shared<ControllerAction>(ptrMenuController, &Controller::selectNextMessage),
			nullptr);
	addPlace("ShowMessage", 0,
			make_shared<ControllerAction>(ptrMenuController, &Controller::showMessage),
			nullptr);


	//Transitions

	//Switch menu
	createTransition(
			{"InputA", "CallsMenuSelected"},
			{"SelectMessagesOption"});

	createTransition(
			{"SelectMessagesOption"},
			{"MessagesMenuSelected"});

	//Switch menu
	createTransition(
			{"InputA", "MessagesMenuSelected"},
			{"SelectCallsOption"});

	createTransition(
			{"SelectCallsOption"},
			{"CallsMenuSelected"});

	//Select calls log
	createTransition(
			{"InputB", "CallsMenuSelected"},
			{"CallsLog"});

	//Leave calls log
	createTransition(
			{"InputC", "CallsLog"},
			{"CallsMenuSelected"});

	//Select messages menu
	createTransition(
			{"InputB", "MessagesMenuSelected"},
			{"MessagesMenu"});

	//Leave messages menu
	createTransition(
			{"InputC", "MessagesMenu"},
			{"MessagesMenuSelected"});

	//Select next message
	createTransition(
			{"InputA", "MessagesMenu"},
			{"NextMessage"});

	createTransition(
			{"NextMessage"},
			{"MessagesMenu"});

	//Show message
	createTransition(
			{"InputB", "MessagesMenu"},
			{"ShowMessage"});

	//Leave show message
	createTransition(
			{"InputC", "ShowMessage"},
			{"MessagesMenu"});
}

void Controller::MenuStateMachine::pressA()
{
	incrementInputPlace("InputA");
	execute();
}

void Controller::MenuStateMachine::pressB()
{
	incrementInputPlace("InputB");
	execute();
}

void Controller::MenuStateMachine::pressC()
{
	incrementInputPlace("InputC");
	execute();
}




