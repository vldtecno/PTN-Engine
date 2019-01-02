/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2019 Eduardo Valgôde
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

#pragma once

#include "Controller/Controller.h"
#include "PTN_Engine/PTN_Engine.h"

//! Implements PTN Engine net defining the phone menu behaviour.
/*!
 * The behaviour is defined in the constructor.
 */
class Controller::MenuStateMachine:
        public ptne::PTN_Engine
{
public:

	/*!
	 * Constructor.
	 * \param ptrDispatcher Shared pointer to the controller.
	 */
	MenuStateMachine(std::shared_ptr<Controller> ptrMenuController);

	//! Press A key event.
	void pressA();

	//! Press B key event.
	void pressB();

	//! Press C key event.
	void pressC();
};
