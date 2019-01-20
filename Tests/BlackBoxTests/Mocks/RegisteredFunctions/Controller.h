/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2018 Eduardo Valgôde
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

#include <memory>
#include <string>

#include "PTN_Engine/ActivationCondition.h"
#include "PTN_Engine/PTN_Engine.h"

class RegisteredFunctionsPN;

class Controller : public std::enable_shared_from_this<Controller>
{
	friend class F2;
	friend class RegisteredFunctionsPN;

public:
	Controller();

	Controller(const Controller &) = delete;

	Controller &operator=(const Controller &) = delete;

	~Controller() = default;

	void initialize();

	void startExecution();

	std::string getSomeString() const;

private:
	std::unique_ptr<RegisteredFunctionsPN> m_petriNet;

	void actionPlace1();

	void actionPlace2();

	bool externalCondition1() const;

	bool externalCondition2() const;

	bool externalCondition3() const;

	std::string m_someString;
};

template class ptne::ActivationCondition<Controller>;
using ControllerFireCondition = ptne::ActivationCondition<Controller>;
