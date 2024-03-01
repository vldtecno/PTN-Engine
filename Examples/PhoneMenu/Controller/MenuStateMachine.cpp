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

#include "Controller/MenuStateMachine.h"
#include "Controller/Controller.h"

using namespace ptne;

void MenuStateMachine::setControllerFunctions(Controller &menuController)
{
	using namespace std;

	// Places
	createPlace({ .name="InputA",
				  .input=true });
	createPlace({ .name="InputB",
				  .input=true });
	createPlace({ .name="InputC",
				  .input=true });
	createPlace({ .name="CallsMenuSelected",
				  .initialNumberOfTokens=1,
				  .onEnterAction=bind_front(&Controller::showMainMenu, &menuController) });
	createPlace({ .name="MessagesMenuSelected",
				  .onEnterAction=bind_front(&Controller::showMainMenu, &menuController) });
	createPlace({ .name="SelectMessagesOption",
				  .onEnterAction=bind_front(&Controller::selectMessagesOption, &menuController) });
	createPlace({ .name="SelectCallsOption",
				  .onEnterAction=bind_front(&Controller::selectCallsOption, &menuController) });
	createPlace({ .name="CallsLog",
				  .onEnterAction=bind_front(&Controller::showCallsMenu, &menuController) });
	createPlace({ .name="MessagesMenu",
				  .onEnterAction=bind_front(&Controller::showMessageMenu, &menuController) });
	createPlace({ .name="NextMessage",
				  .onEnterAction=bind_front(&Controller::selectNextMessage, &menuController) });
	createPlace({ .name="ShowMessage",
				  .onEnterAction=bind_front(&Controller::showMessage, &menuController) });

	// Transitions

	// Switch menu
	createTransition({ .name = "T1",
					   .activationArcs = { {.placeName = "InputA" },
										   {.placeName = "CallsMenuSelected" } },
					   .destinationArcs = { {.placeName = "SelectMessagesOption" } } });

	createTransition({ .name = "T2",
					   .activationArcs = { { .placeName = "SelectMessagesOption" } },
					   .destinationArcs = { { .placeName = "MessagesMenuSelected" } } });

	// Switch menu
	createTransition({ .name = "T3",
					   .activationArcs = { { .placeName = "InputA" },
										   { .placeName = "MessagesMenuSelected" } },
					   .destinationArcs = { { .placeName = "SelectCallsOption" } } });

	createTransition({ .name = "T4",
					   .activationArcs = { { .placeName = "SelectCallsOption" } },
					   .destinationArcs = { { .placeName = "CallsMenuSelected" } } });

	// Select calls log
	createTransition({ .name = "T5",
					   .activationArcs = { { .placeName = "InputB" },
										   { .placeName = "CallsMenuSelected" } },
					   .destinationArcs = { { .placeName = "CallsLog" } } });

	// Leave calls log
	createTransition({ .name = "T6",
					   .activationArcs = { { .placeName = "InputC" },
										   { .placeName = "CallsLog" } },
					   .destinationArcs = { { .placeName = "CallsMenuSelected" } } });

	// Select messages menu
	createTransition({ .name = "T7",
					   .activationArcs = { { .placeName = "InputB" },
										   { .placeName = "MessagesMenuSelected" } },
					   .destinationArcs = { { .placeName = "MessagesMenu" } } });

	// Leave messages menu
	createTransition({ .name = "T8",
					   .activationArcs = { { .placeName = "InputC" },
										   { .placeName = "MessagesMenu" } },
					   .destinationArcs = { { .placeName = "MessagesMenuSelected" } } });

	// Select next message
	createTransition({ .name = "T9",
					   .activationArcs = { { .placeName = "InputA" },
										   { .placeName = "MessagesMenu" } },
					   .destinationArcs = { { .placeName = "NextMessage" } } });

	createTransition({ .name = "T10",
					   .activationArcs = { { .placeName = "NextMessage" } },
					   .destinationArcs = { { .placeName = "MessagesMenu" } } });

	// Show message
	createTransition({ .name = "T11",
					   .activationArcs = { { .placeName = "InputB" },
										   { .placeName = "MessagesMenu" } },
					   .destinationArcs = { { .placeName = "ShowMessage" } }});

	// Leave show message
	createTransition({ .name = "T12",
					   .activationArcs = { { .placeName = "InputC" },
										   { .placeName = "ShowMessage" } },
					   .destinationArcs = { { .placeName = "MessagesMenu" } } });
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
