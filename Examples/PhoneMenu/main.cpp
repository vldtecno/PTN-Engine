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

#include "Controller/Controller.h"
#include "Controller/MenuStateMachine.h"
#include "Controller/XMLMenuStateMachine.h"
#include "Model/Call.h"
#include "Model/List.h"
#include "Model/Message.h"
#include "View/CallLogView.h"
#include "View/MainMenuView.h"
#include "View/MessagesMenuView.h"
#include <iostream>
#include <memory>

using namespace std;

static void programLoop(Controller &controller)
{
	bool exit = false;
	do
	{
		cout << "a - switch option, b - Select, c - escape, q - quit" << endl;

		string userInput;
		cin >> userInput;

		if (userInput == "a" || userInput == "A")
		{
			controller.pressA();
		}
		else if (userInput == "b" || userInput == "B")
		{
			controller.pressB();
		}
		else if (userInput == "c" || userInput == "C")
		{
			controller.pressC();
		}
		else if (userInput == "q" || userInput == "Q")
		{
			cout << "Quitting..." << endl;
			controller.stop();
			exit = true;
		}
		else
		{
			cout << "Invalid input" << endl;
		}
	} while (!exit);
}

void populateMessageList(List<Message> &messageList)
{
	messageList.addItem(Message("Hi...", "Hi John, don't forget to water the plants."));
	messageList.addItem(Message("Is...", "Is this thing working?"));
	messageList.addItem(Message("Please...", "Please top up your phone."));
}

void populateCallLog(List<Call> &callLog)
{
	callLog.addItem(Call("10:05:03 29-JUN-2018", "John Doe"));
	callLog.addItem(Call("09:35:50 29-JUN-2018", "Sally Smith"));
	callLog.addItem(Call("09:12:01 29-JUN-2018", "Dick Jones"));
	callLog.addItem(Call("08:59:47 29-JUN-2018", "Harry Brown"));
}

void runLoop(Controller &controller)
{
	controller.execute();
	controller.pressA();
	programLoop(controller);
	controller.stop();
}

int main(int, char **)
{
	// Create data
	List<Message> messageList;
	populateMessageList(messageList);
	List<Call> callLog;
	populateCallLog(callLog);

	// Create views
	MainMenuView mainMenuView;
	MessagesMenuView messagesMenuView(messageList);
	CallLogView callLogView(callLog);

	// Create the controller
	Controller controller(mainMenuView, messagesMenuView, callLogView, messageList, callLog);

	// Set a state machine in the controller
	controller.setStateMachine(make_shared<MenuStateMachine>());

	runLoop(controller);

	cout << "Switching state machine" << endl;

	auto xmlMenuStateMachine = make_shared<XMLMenuStateMachine>();
	controller.setStateMachine(xmlMenuStateMachine);
#ifdef WIN32
	xmlMenuStateMachine->importStateMachine("../PhoneMenu.xml");
#else
	xmlMenuStateMachine->importStateMachine("./PhoneMenu.xml");
#endif
	// export the state machine just as an example
	xmlMenuStateMachine->exportStateMachine("./PhoneMenuExport.xml");

	runLoop(controller);

	cout << "Successfully terminated" << endl;
	return 0;
}
