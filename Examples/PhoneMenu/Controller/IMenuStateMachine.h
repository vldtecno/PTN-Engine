/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2024 Eduardo Valgôde
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

#include "PTN_Engine/PTN_Engine.h"

class Controller;

class IMenuStateMachine : public ptne::PTN_Engine
{
public:
	virtual void setControllerFunctions(Controller &menuController) = 0;

	//! Press A key event.
	virtual void pressA() = 0;

	//! Press B key event.
	virtual void pressB() = 0;

	//! Press C key event.
	virtual void pressC() = 0;
};
