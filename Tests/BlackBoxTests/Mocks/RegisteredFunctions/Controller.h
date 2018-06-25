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
#include <ostream>
#include <set>
#include <future>

#include "PTN_Engine/ActivationCondition.h"
#include "PTN_Engine/Action.h"
#include "PTN_Engine/PTN_Engine.h"

class Controller: public std::enable_shared_from_this<Controller>
{
	class RegisteredFunctionsPN;

	friend class FixturePetriNet;
	friend class RegisteredFunctionsPN;

public:

	Controller();

	Controller(const Controller&) = delete;

	Controller& operator=(const Controller&) = delete;

	~Controller() = default;

	void initialize();

	void doSomethingConcurrently(const size_t numberOfThreads);

	size_t getNumberOfDifferentThreads() const;

private:

	void collectThreadId();

	std::unique_ptr<RegisteredFunctionsPN> m_petriNet;

	std::set<std::thread::id> m_collectedThreadIds;

};

template class ptne::Action<Controller>;
using ControllerAction2 = ptne::Action<Controller>;

template class ptne::ActivationCondition<Controller>;
using ControllerFireCondition2 = ptne::ActivationCondition<Controller>;

#include "Mocks/RegisteredFunctions/RegisteredFunctionsPN.h"
