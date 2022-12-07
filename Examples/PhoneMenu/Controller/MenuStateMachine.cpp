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

#include "Controller/MenuStateMachine.h"
#include "Controller/Controller.h"

using namespace ptne;

MenuStateMachine::MenuStateMachine(Controller &menuController)
	: PTN_Engine()
{
	using namespace std;

	//Places
	createPlace("InputA", 0, true);
	createPlace("InputB", 0, true);
	createPlace("InputC", 0, true);
	createPlace("CallsMenuSelected", 1, bind(&Controller::showMainMenu, &menuController));
	createPlace("MessagesMenuSelected", 0, bind(&Controller::showMainMenu, &menuController));
	createPlace("SelectMessagesOption", 0, bind(&Controller::selectMessagesOption, &menuController));
	createPlace("SelectCallsOption", 0, bind(&Controller::selectCallsOption, &menuController));
	createPlace("CallsLog", 0, bind(&Controller::showCallsMenu, &menuController));
	createPlace("MessagesMenu", 0, bind(&Controller::showMessageMenu, &menuController));
	createPlace("NextMessage", 0, bind(&Controller::selectNextMessage, &menuController));
	createPlace("ShowMessage", 0, bind(&Controller::showMessage, &menuController));

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

void MenuStateMachine::pressA()
{
	incrementInputPlace("InputA");
}

void MenuStateMachine::pressB()
{
	incrementInputPlace("InputB");
}

void MenuStateMachine::pressC()
{
	incrementInputPlace("InputC");
}
