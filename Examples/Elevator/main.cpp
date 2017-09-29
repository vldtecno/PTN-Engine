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

#include <string>
#include <iostream>
#include "Controller/ElevatorController.h"

using namespace std;



void callElevatorUpMenu(shared_ptr<ElevatorController> controller)
{
	bool exit = false;
	do
	{
		cout << "Call elevator to go up. From which floor?" << endl;
		cout << "b - go back to previous menu" << endl;

		string userInput;
		cin >> userInput;

		if (userInput == "b" || userInput == "B")
		{
			cout << "Back" << endl;
			exit = true;
		}
		else
		{
			try
			{
				int callerFloor = stoi(userInput);

				if (controller->callElevatorUp(callerFloor))
				{
					exit = true;
				}
				else
				{
					cout << "Invalid floor" << endl;
				}
			}
			catch (const invalid_argument&)
			{
				cout << "Invalid input" << endl;
			}
		}

	} while (!exit);
}

void callElevatorDownMenu(shared_ptr<ElevatorController> controller)
{
	bool exit = false;
	do
	{
		cout << "Call elevator to go down. From which floor?" << endl;
		cout << "b - go back to previous menu" << endl;

		string userInput;
		cin >> userInput;

		if (userInput == "b" || userInput == "B")
		{
			cout << "Back" << endl;
			exit = true;
		}
		else
		{
			try
			{
				int callerFloor = stoi(userInput);

				if (controller->callElevatorDown(callerFloor))
				{
					exit = true;					
				}
				else
				{
					cout << "Invalid floor number" << endl;
				}
			}
			catch (const invalid_argument&)
			{
				cout << "Invalid input" << endl;				
			}

		}

	} while (!exit);
	
}

void setDestinationMenu(shared_ptr<ElevatorController> controller)
{
	bool exit = false;
	do
	{
		cout << "To which floor do you want to go?" << endl;
		cout << "b - go back to previous menu" << endl;

		string userInput;
		cin >> userInput;

		if (userInput == "b" || userInput == "B")
		{
			cout << "Back" << endl;
			exit = true;
		}
		else
		{
			try
			{
				int destination = stoi(userInput);

				if (controller->setDestinationFloor(destination))
				{
					exit = true;
				}
				else
				{
					cout << "Invalid floor number" << endl;
				}
			}
			catch (const invalid_argument&)
			{
				cout << "Invalid input" << endl;
			}
		}

	} while (!exit);
}




void programLoop(shared_ptr<ElevatorController> controller)
{	
	bool exit = false;
	do
	{
		cout << endl;
		cout << "Commands:" << endl;
		cout << "1 - Open elevator doors" << endl;
		cout << "2 - Close elevator doors" << endl;
		cout << "3 - Call elevator to go up" << endl;
		cout << "4 - Call elevator to go down" << endl;
		cout << "5 - Press floor button" << endl;
		cout << "q - Quit" << endl;

		string userInput;
		cin >> userInput;

		if (userInput == "1")
		{
			controller->openDoors();
		}
		else if (userInput == "2")
		{
			controller->closeDoors();
		}
		else if (userInput == "3")
		{
			callElevatorUpMenu(controller);
		}
		else if (userInput == "4")
		{
			callElevatorDownMenu(controller);
		}
		else if (userInput == "5")
		{
			setDestinationMenu(controller);
		}
		else if (userInput == "q" || userInput == "Q")
		{
			cout << "Quitting..." << endl;
			exit = true;
		}
		else
		{
			cout << "Invalid input" << endl;
		}

	} while (!exit);
}

int main(int argc, char** argv)
{
	//Set up controller
	shared_ptr<ElevatorController> controller = make_shared<ElevatorController>();
	controller->initialize();

	programLoop(controller);

	cout << "Successfully terminated" << endl;
	return 0;
}





