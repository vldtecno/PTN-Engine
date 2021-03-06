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

#include <future>
#include <memory>
#include <ostream>
#include <set>

//! A very simple controller with a very simple petri net.
class SimpleController : public std::enable_shared_from_this<SimpleController>
{
	class SimplePetriNet;
	friend class SimplePetriNet;
	friend class FixturePetriNet;

public:
	SimpleController();

	SimpleController(const SimpleController &) = delete;

	SimpleController &operator=(const SimpleController &) = delete;

	~SimpleController() = default;

	void initialize();

	void doSomethingConcurrently(const size_t numberOfThreads);

	size_t getNumberOfDifferentThreads() const;

private:
	void collectThreadId();

	std::unique_ptr<SimplePetriNet> m_petriNet;

	std::set<std::thread::id> m_collectedThreadIds;
};

#include "Mocks/Simple/SimplePetriNet.h"
