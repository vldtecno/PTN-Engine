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

#include "PTN_Engine/PTN_Engine.h"

class SimpleController;

class SimplePetriNet : public ptne::PTN_Engine
{
	friend class FixturePetriNet;

public:
	explicit SimplePetriNet(SimpleController &controller);

	void addExecuteP1();

	size_t getNumberOfTokens(const std::string &placeName) const;
};
