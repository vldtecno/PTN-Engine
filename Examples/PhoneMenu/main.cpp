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

#include <iostream>

#include "Controller/Controller.h"

using namespace std;

void programLoop(Controller& controller)
{
	bool exit = false;
	do
	{
		cout<<"a - switch option, b - Select, c - escape, q - quit" << endl;

		string userInput;
		cin >> userInput;

		if(userInput == "a" || userInput == "A")
		{
			controller.pressA();
		}
		else if(userInput == "b" || userInput == "B")
		{
			controller.pressB();
		}
		else if(userInput == "c" || userInput == "C")
		{
			controller.pressC();
		}
		else if(userInput == "q" || userInput == "Q")
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

int main(int, char**)
{
	//Set up controller
	Controller controller;

	//Set up data
	controller.messageList().addItem(Message("Hi...","Hi John, don't forget to water the plants."));
	controller.messageList().addItem(Message("Is...","Is this thing working?"));
	controller.messageList().addItem(Message("Please...","Please top up your phone."));

	controller.callLog().addItem(Call("10:05:03 29-JUN-2018", "John Doe"));
	controller.callLog().addItem(Call("09:35:50 29-JUN-2018", "Sally Smith"));
	controller.callLog().addItem(Call("09:12:01 29-JUN-2018", "Dick Jones"));
	controller.callLog().addItem(Call("08:59:47 29-JUN-2018", "Harry Brown"));

	controller.execute();

	controller.pressA();

	programLoop(controller);

	cout << "Successfully terminated" << endl;
	return 0;
}
