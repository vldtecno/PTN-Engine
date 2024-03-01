/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2018-2023 Eduardo Valgôde
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

#include "Mocks/RegisteredFunctions/RegisteredFunctionsPN.h"
#include <mutex>
#include <string>

class Controller
{
	friend class F2;
	friend class RegisteredFunctionsPN;

public:
	Controller();

	~Controller();

	Controller(const Controller &) = delete;

	Controller &operator=(const Controller &) = delete;

	void initialize();

	void startExecution();

	std::string getSomeString() const;

	size_t getNumberOfTokens(const std::string &placeName) const;

	void stop();

private:
	RegisteredFunctionsPN m_menuStateMachine;

	void actionPlace1();

	void actionPlace2();

	bool externalCondition1() const;

	bool externalCondition2() const;

	bool externalCondition3() const;

	std::string m_someString;

	static std::mutex s_mut;
};
