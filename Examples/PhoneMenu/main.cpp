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

#include <iostream>

#include "Controller/Controller.h"
#include "Model/List.h"

using namespace std;

void initializeMessageList(shared_ptr<Controller::MessageList> messageList)
{
	messageList->addItem(Message("Hi...","Hi John, don't forget to water the plants."));
	messageList->addItem(Message("Is...","Is this thing working?"));
	messageList->addItem(Message("Please...","Please top up your phone."));
}


void initializeCallList(shared_ptr<Controller::CallList> callLog)
{
	callLog->addItem(Call("10:05:03 29-JUN-2018", "John Doe"));
	callLog->addItem(Call("09:35:50 29-JUN-2018", "Sally Smith"));
	callLog->addItem(Call("09:12:01 29-JUN-2018", "Dick Jones"));
	callLog->addItem(Call("08:59:47 29-JUN-2018", "Harry Brown"));
}

void programLoop(shared_ptr<Controller> controller)
{
	string userInput;
	bool exit = false;
	do
	{
		cout<<"a - switch option, b - Select, c - escape, q - quit" << endl;

		string userInput;
		cin >> userInput;

		if(userInput == "a" || userInput == "A")
		{
			controller->pressA();
		}
		else if(userInput == "b" || userInput == "B")
		{
			controller->pressB();
		}
		else if(userInput == "c" || userInput == "C")
		{
			controller->pressC();
		}
		else if(userInput == "q" || userInput == "Q")
		{
			cout << "Quitting..." << endl;
			exit = true;
		}
		else
		{
			cout << "Invalid input" << endl;
		}

	} while(!exit);
}

int main(int, char**)
{
	//Set up data
	shared_ptr<Controller::MessageList> messageList = make_shared<Controller::MessageList>();
	initializeMessageList(messageList);
	shared_ptr<Controller::CallList> callLog = make_shared<Controller::CallList>();
	initializeCallList(callLog);

	//Set up views
	shared_ptr<MainMenuView> mainMenu = make_shared<MainMenuView>();
	shared_ptr<MessagesMenuView> messagesMenu = make_shared<MessagesMenuView>();
	shared_ptr<CallLogView> callLogView = make_shared<CallLogView>();

	//Set up controller
	shared_ptr<Controller> controller = make_shared<Controller>();
	controller->initialize();
	controller->setMessageList(messageList);
	controller->setCallLog(callLog);
	controller->setMainMenuView(mainMenu);
	controller->setMessagesMenuView(messagesMenu);
	controller->setCallLogView(callLogView);

	controller->pressA();

	programLoop(controller);

	cout << "Successfully terminated" << endl;
	return 0;
}





