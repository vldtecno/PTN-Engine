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

#include "Mocks/Simple/SimpleController.h"
#include <thread>

using namespace std;

SimpleController::SimpleController()
: m_petriNet(*this)
{
}

SimpleController::~SimpleController() = default;

void SimpleController::doSomethingConcurrently(const size_t numberOfThreads)
{
	m_collectedThreadIds.clear();
	auto f = [this]()
	{
		this_thread::sleep_for(chrono::milliseconds(5));
		for (int i = 0; i < 10; ++i)
		{
			m_petriNet.addExecuteP1();
		}
	};

	vector<jthread> threads;
	for (size_t i = 0; i < numberOfThreads; ++i)
	{
		threads.emplace_back(f);
	}
}

size_t SimpleController::getNumberOfDifferentThreads() const
{
	return m_collectedThreadIds.size();
}

void SimpleController::collectThreadId()
{
	m_collectedThreadIds.insert(this_thread::get_id());
}

size_t SimpleController::getNumberOfTokens(const string &placeName) const
{
	return m_petriNet.getNumberOfTokens(placeName);
}

void SimpleController::stop()
{
	m_petriNet.stop();
}
