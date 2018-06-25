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

#include "Mocks/RegisteredFunctions/Controller.h"
#include <thread>

using namespace std;

Controller::Controller():
	m_petriNet(nullptr)
{
}

void Controller::initialize()
{
	m_petriNet = make_unique<Controller::RegisteredFunctionsPN>(shared_from_this());
}

void Controller::doSomethingConcurrently(const size_t numberOfThreads)
{
	m_collectedThreadIds.clear();
	auto f = [&]()->void
	{
		this_thread::sleep_for(chrono::milliseconds(5));
		for (int i = 0; i < 10; ++i)
		{
			m_petriNet->addExecuteP1();
		}
	};

	vector<future<void>> tasks;
	for (size_t i = 0; i < numberOfThreads; ++i)
	{
		auto fut = async(launch::async, f);
		tasks.push_back(move(fut));
	}

	for (auto& task : tasks)
	{
		task.wait();
	}
}

size_t Controller::getNumberOfDifferentThreads() const
{
	return m_collectedThreadIds.size();
}

void Controller::collectThreadId()
{
	m_collectedThreadIds.insert(this_thread::get_id());
}
