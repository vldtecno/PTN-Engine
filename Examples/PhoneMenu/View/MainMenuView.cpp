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

#include "../../../Examples/PhoneMenu/View/MainMenuView.h"

#include <iostream>
#include <stdexcept>

using namespace std;

MainMenuView::MainMenuView():
	m_selection(0)
{

}

MainMenuView::~MainMenuView()
{

}

void MainMenuView::showMenu() const
{
	cout << "OPTIONS:" << endl;
	cout << "\tCall Log" << endl;
	cout << "\tMessages" << endl << endl;
	cout << "SELECTED: ";
	switch(m_selection)
	{
	default:
		throw runtime_error("Undefined");
	case 0:
		cout << "Call Log" << endl;
		break;
	case 1:
		cout << "Messages" << endl;
		break;
	}
}

void MainMenuView::select(const int option)
{
	switch(option)
	{
	default:
		throw runtime_error("Invalid option");
	case 0:
		m_selection = 0; break;
	case 1:
		m_selection = 1; break;
	}

}
