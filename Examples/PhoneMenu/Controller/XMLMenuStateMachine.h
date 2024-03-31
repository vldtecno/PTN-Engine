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

#pragma once

#include "Controller/IMenuStateMachine.h"

class Controller;

//!
//! \brief Implements PTN Engine net defining the phone menu behaviour.
//!
class XMLMenuStateMachine : public IMenuStateMachine
{
public:
	~XMLMenuStateMachine() override = default;
	XMLMenuStateMachine() = default;
	XMLMenuStateMachine(const XMLMenuStateMachine &) = delete;
	XMLMenuStateMachine(XMLMenuStateMachine &&) = delete;
	XMLMenuStateMachine &operator=(XMLMenuStateMachine &) = delete;
	XMLMenuStateMachine &operator=(XMLMenuStateMachine &&) = delete;

	void pressA() override;
	void pressB() override;
	void pressC() override;
	void setControllerFunctions(Controller &menuController) override;

	void exportStateMachine(const std::string &filePath) const;
	void importStateMachine(const std::string &filePath);
};
