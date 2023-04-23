/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2023 Eduardo Valgôde
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

#include "Fixtures/FixturePetriNet.h"
#include "Mocks/RegisteredFunctions/Controller.h"
#include "Mocks/Simple/SimpleController.h"
#include "PTN_Engine/PTN_Exception.h"
#include "gtest/gtest.h"
#include <chrono>
#include <thread>
#include <latch>


using namespace std::chrono;

TEST_F(FixturePetriNet, RoundRobin_1)
{
	m_dispatcher.setRoundRobinMode(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
	EXPECT_EQ(true, m_dispatcher.m_isWaitingPackage && m_dispatcher.m_isChannelASelected &&
					(!m_dispatcher.m_isChannelBSelected));

	// m_plInputWaitPackage;
	// m_plWaitPackage;
	// m_plChannelA;
	// m_plChannelB;
	// m_plSelectA;
	// m_plSelectB;
	// m_plPackageCounter;
	size_t expectedState[7] = { 0, 1, 0, 0, 1, 0, 0 };

	testRoundRobinState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);
	EXPECT_TRUE(m_dispatcher.m_isWaitingPackage && m_dispatcher.m_isChannelBSelected &&
				(!m_dispatcher.m_isChannelASelected));

	expectedState[4] = 0; // m_plSelectA;
	expectedState[5] = 1; // m_plSelectB;
	expectedState[6] = 1; // m_plPackageCounter;
	testRoundRobinState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);
	EXPECT_TRUE(m_dispatcher.m_isWaitingPackage && m_dispatcher.m_isChannelASelected &&
				(!m_dispatcher.m_isChannelBSelected));

	expectedState[4] = 1; // m_plSelectA;
	expectedState[5] = 0; // m_plSelectB;
	expectedState[6] = 2; // m_plPackageCounter;
	testRoundRobinState(expectedState);

	m_dispatcher.setResetCounter(true);
	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);
	expectedState[4] = 0; // m_plSelectA;
	expectedState[5] = 1; // m_plSelectB;
	expectedState[6] = 0; // m_plPackageCounter;
	testRoundRobinState(expectedState);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, RoundRobin_1_detached)
{
	m_dispatcher.setRoundRobinMode(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED);
	EXPECT_EQ(true, m_dispatcher.m_isWaitingPackage && m_dispatcher.m_isChannelASelected &&
					(!m_dispatcher.m_isChannelBSelected));

	// m_plInputWaitPackage;
	// m_plWaitPackage;
	// m_plChannelA;
	// m_plChannelB;
	// m_plSelectA;
	// m_plSelectB;
	// m_plPackageCounter;
	size_t expectedState[7] = { 0, 1, 0, 0, 1, 0, 0 };

	testRoundRobinState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);
	EXPECT_TRUE(m_dispatcher.m_isWaitingPackage && m_dispatcher.m_isChannelBSelected &&
				(!m_dispatcher.m_isChannelASelected));

	expectedState[4] = 0; // m_plSelectA;
	expectedState[5] = 1; // m_plSelectB;
	expectedState[6] = 1; // m_plPackageCounter;
	testRoundRobinState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);
	EXPECT_TRUE(m_dispatcher.m_isWaitingPackage && m_dispatcher.m_isChannelASelected &&
				(!m_dispatcher.m_isChannelBSelected));

	expectedState[4] = 1; // m_plSelectA;
	expectedState[5] = 0; // m_plSelectB;
	expectedState[6] = 2; // m_plPackageCounter;
	testRoundRobinState(expectedState);

	m_dispatcher.setResetCounter(true);
	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);
	expectedState[4] = 0; // m_plSelectA;
	expectedState[5] = 1; // m_plSelectB;
	expectedState[6] = 0; // m_plPackageCounter;
	testRoundRobinState(expectedState);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, RoundRobin_1_continuous2)
{
	m_dispatcher.setRoundRobinMode(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
	EXPECT_EQ(true, m_dispatcher.m_isWaitingPackage && m_dispatcher.m_isChannelASelected &&
					(!m_dispatcher.m_isChannelBSelected));

	// m_plInputWaitPackage;
	// m_plWaitPackage;
	// m_plChannelA;
	// m_plChannelB;
	// m_plSelectA;
	// m_plSelectB;
	// m_plPackageCounter;
	size_t expectedState[7] = { 0, 1, 0, 0, 1, 0, 0 };

	testRoundRobinState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);
	EXPECT_TRUE(m_dispatcher.m_isWaitingPackage && m_dispatcher.m_isChannelBSelected &&
				(!m_dispatcher.m_isChannelASelected));

	expectedState[4] = 0; // m_plSelectA;
	expectedState[5] = 1; // m_plSelectB;
	expectedState[6] = 1; // m_plPackageCounter;
	testRoundRobinState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);
	EXPECT_TRUE(m_dispatcher.m_isWaitingPackage && m_dispatcher.m_isChannelASelected &&
				(!m_dispatcher.m_isChannelBSelected));

	expectedState[4] = 1; // m_plSelectA;
	expectedState[5] = 0; // m_plSelectB;
	expectedState[6] = 2; // m_plPackageCounter;
	testRoundRobinState(expectedState);

	m_dispatcher.setResetCounter(true);
	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);
	expectedState[4] = 0; // m_plSelectA;
	expectedState[5] = 1; // m_plSelectB;
	expectedState[6] = 0; // m_plPackageCounter;
	testRoundRobinState(expectedState);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, FreeChoice_1)
{
	size_t numberOfIterations = 1100;
	size_t expectedState[7] = { 0, 1, 0, 0, 0, 0, numberOfIterations };
	m_dispatcher.setResetCounter(false);
	m_dispatcher.setFreeChoiceMode(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
	for (size_t i = 0; i < numberOfIterations; ++i)
	{
		m_dispatcher.dispatch();
		std::this_thread::sleep_for(10ms);
	}
	testFreeChoiceState(expectedState);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, FreeChoice_1_detached)
{
	size_t numberOfIterations = 1100;
	size_t expectedState[7] = { 0, 1, 0, 0, 0, 0, numberOfIterations };
	m_dispatcher.setResetCounter(false);
	m_dispatcher.setFreeChoiceMode(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED);
	for (size_t i = 0; i < numberOfIterations; ++i)
	{
		m_dispatcher.dispatch();
		std::this_thread::sleep_for(10ms);
	}
	testFreeChoiceState(expectedState);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, Weights_1)
{
	m_dispatcher.setResetCounter(true);
	m_dispatcher.setWeightedPN(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	size_t expectedState[4] = { 0, 1, 0, 0 };
	testWeightedState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	expectedState[1] = 2;
	testWeightedState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	expectedState[1] = 0;
	expectedState[2] = 4;
	expectedState[3] = 10;
	testWeightedState(expectedState);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, Weights_1_detached)
{
	m_dispatcher.setResetCounter(true);
	m_dispatcher.setWeightedPN(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	size_t expectedState[4] = { 0, 1, 0, 0 };
	testWeightedState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	expectedState[1] = 2;
	testWeightedState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	expectedState[1] = 0;
	expectedState[2] = 4;
	expectedState[3] = 10;
	testWeightedState(expectedState);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, Inhibited_SingleThread)
{
	m_dispatcher.setResetCounter(true);
	m_dispatcher.setInhibitedPN(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);

	m_dispatcher.dispatch();
	m_dispatcher.execute();
	size_t expectedState[6] = { 0, 0, 0, 0, 1, 1 };
	testInhibitedState(expectedState);

	m_dispatcher.dispatch();
	m_dispatcher.execute();
	size_t expectedState_[6] = { 0, 1, 1, 1, 0, 0 };
	testInhibitedState(expectedState_);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, Inhibited_1)
{
	m_dispatcher.setResetCounter(true);
	m_dispatcher.setInhibitedPN(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	size_t expectedState[6] = { 0, 0, 0, 0, 1, 1 };
	testInhibitedState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	size_t expectedState_[6] = { 0, 1, 1, 1, 0, 0 };
	testInhibitedState(expectedState_);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, Inhibited_1_detached)
{
	m_dispatcher.setResetCounter(true);
	m_dispatcher.setInhibitedPN(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	size_t expectedState[6] = { 0, 0, 0, 0, 1, 1 };
	testInhibitedState(expectedState);

	m_dispatcher.dispatch();
	std::this_thread::sleep_for(10ms);

	size_t expectedState_[6] = { 0, 1, 1, 1, 0, 0 };
	testInhibitedState(expectedState_);
	m_dispatcher.stop();
}

TEST_F(FixturePetriNet, ThreadSafety)
{
	SimpleController simpleController;

	const size_t numberOfThreads = 8;
	simpleController.doSomethingConcurrently(numberOfThreads);
	std::this_thread::sleep_for(10ms);

	EXPECT_EQ(numberOfThreads, simpleController.getNumberOfDifferentThreads());

	// TODO get a better way to test
	EXPECT_EQ(0, simpleController.getNumberOfTokens("P2"));
	simpleController.stop();
}

TEST(Registered, RegisteredMethods)
{
	Controller controller;
	controller.initialize();

	controller.startExecution();
	std::this_thread::sleep_for(10ms);

	// TODO get a better way to test
	EXPECT_EQ(1, controller.getNumberOfTokens("P1"));
	EXPECT_EQ(0, controller.getNumberOfTokens("P2"));

	EXPECT_EQ("actionPlace1", controller.getSomeString());
	controller.stop();
}

TEST(Registered, AlreadyRegisteredMethods)
{
	Controller controller;
	controller.initialize();

	ASSERT_THROW(controller.initialize(), ptne::PTN_Exception);
}

TEST(Conditions, AdditionalActivationConditions)
{
	using std::vector;

	size_t m_callCount = 0;
	bool m_hasConnectionToServer = true;

	ptne::PTN_Engine pn;
	pn.createPlace("P1", 0, true);
	pn.createPlace("P2", 0);

	pn.createTransition({ "P1" }, { "P2" },
						vector<ptne::ConditionFunction>{ [&m_callCount, &m_hasConnectionToServer]()
														 {
															 ++m_callCount;
															 return m_hasConnectionToServer;
														 } });


	EXPECT_EQ(0, m_callCount);
	pn.execute();
	pn.incrementInputPlace("P1");
	std::this_thread::sleep_for(10ms);
	pn.stop();
	EXPECT_EQ(1, m_callCount);
	vector<size_t> numberOfTokens;
	numberOfTokens.emplace_back(pn.getNumberOfTokens("P1"));
	numberOfTokens.emplace_back(pn.getNumberOfTokens("P2"));
	EXPECT_EQ(vector<size_t>({ 0, 1 }), numberOfTokens);
}

TEST(ActionsThreadOption, Ordering)
{
	using std::vector;
	using namespace ptne;

	std::string str;
	std::mutex mut;

	ActionFunction action1 = [&str, &mut]()
	{
		std::lock_guard<std::mutex> lg(mut);
		str.append("a");
	};
	ActionFunction action2 = [&str, &mut]()
	{
		std::lock_guard<std::mutex> lg(mut);
		str.append("b");
	};

	PTN_Engine pn_singleThread(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);
	pn_singleThread.createPlace("P1", 0, action1, true);
	pn_singleThread.createPlace("Counter", 100000);
	pn_singleThread.createPlace("P2", 0, action2);
	pn_singleThread.createTransition({ "P1", "Counter" }, { "P2" });
	pn_singleThread.createTransition({ "P2" }, { "P1" });
	pn_singleThread.incrementInputPlace("P1");
	pn_singleThread.execute();
	EXPECT_EQ(0, pn_singleThread.getNumberOfTokens("P2"));
	EXPECT_FALSE(str.find("aa") != std::string::npos);
	EXPECT_FALSE(str.find("bb") != std::string::npos);
	str.clear();

	std::atomic<bool> finished = false;
	ActionFunction action3 = [&finished]() { 
		finished = true; 
	};

	PTN_Engine pn_jobQueue(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);
	pn_jobQueue.createPlace("P1", 0, action1, true);
	pn_jobQueue.createPlace("Counter", 100000);
	pn_jobQueue.createPlace("P2", 0, action2);
	pn_jobQueue.createPlace("P3", 0, action3);
	pn_jobQueue.createTransition({ "P1", "Counter" }, { "P2" });
	pn_jobQueue.createTransition({ "P2" }, { "P1" });
	pn_jobQueue.createTransition({ "P1" }, { "P3" }, vector<std::string>{ "Counter" });
	pn_jobQueue.execute();
	pn_jobQueue.incrementInputPlace("P1");
	while (!finished)
	{
		std::this_thread::sleep_for(100ms);
	}
	pn_jobQueue.stop();
	EXPECT_EQ(0, pn_jobQueue.getNumberOfTokens("P2"));
	{
		std::lock_guard<std::mutex> lg(mut);
		EXPECT_FALSE(str.find("aa") != std::string::npos);
		EXPECT_FALSE(str.find("bb") != std::string::npos);
		str.clear();
	}

	finished = false;
	PTN_Engine pn_eventloop(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
	pn_eventloop.createPlace("P1", 0, action1, true);
	pn_eventloop.createPlace("Counter", 100000);
	pn_eventloop.createPlace("P2", 0, action2);
	pn_eventloop.createPlace("P3", 0, action3);
	pn_eventloop.createTransition({ "P1", "Counter" }, { "P2" });
	pn_eventloop.createTransition({ "P2" }, { "P1" });
	pn_eventloop.createTransition({ "P1" }, { "P3" }, vector<std::string>{ "Counter" });
	pn_eventloop.execute();
	pn_eventloop.incrementInputPlace("P1");
	while (!finished)
	{
		std::this_thread::sleep_for(100ms);
	}
	pn_eventloop.stop();
	EXPECT_EQ(0, pn_eventloop.getNumberOfTokens("P2"));
	{
		std::lock_guard<std::mutex> lg(mut);
		EXPECT_FALSE(str.find("aa") != std::string::npos);
		EXPECT_FALSE(str.find("bb") != std::string::npos);
		str.clear();
	}

	finished = false;
	PTN_Engine pn_detached(PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED);
	pn_detached.createPlace("P1", 0, action1, true);
	pn_detached.createPlace("Counter", 100000);
	pn_detached.createPlace("P2", 0, action2);
	pn_detached.createPlace("P3", 0, action3);
	pn_detached.createTransition({ "P1", "Counter" }, { "P2" });
	pn_detached.createTransition({ "P2" }, { "P1" });
	pn_detached.createTransition({ "P1" }, { "P3" }, vector<std::string>{ "Counter" });
	pn_detached.execute();
	pn_detached.incrementInputPlace("P1");
	while (!finished)
	{
		std::this_thread::sleep_for(100ms);
	}
	pn_detached.stop();
	EXPECT_EQ(0, pn_detached.getNumberOfTokens("P2"));
	{
		std::lock_guard<std::mutex> lg(mut);
		EXPECT_TRUE(str.find("aa") != std::string::npos);
		EXPECT_TRUE(str.find("bb") != std::string::npos);
	}
}

TEST(Concurrency, ThreadSafety_1)
{
	using namespace ptne;
	PTN_Engine pn;

	pn.createPlace("P1", 0, true);
	pn.createPlace("Counter", 5);
	pn.createPlace("P2", 0);
	std::atomic<bool> finished = false;
	ActionFunction flagFinish = [&finished]() { finished = true; };
	pn.createPlace("P3", 0, flagFinish);
	pn.createTransition({ "P1", "Counter" }, { "P2" });
	pn.createTransition({}, { "P3" }, std::vector<std::string>{ "Counter", "P3" });
	pn.execute();

	const size_t totalThreads = 16;
	std::latch _latch(totalThreads);
	auto f = [&pn, &_latch, &finished]()
	{
		_latch.wait();
		for (size_t i = 0; i < 10; ++i)
		{
			pn.incrementInputPlace("P1");
		}

		while (!finished)
		{
			std::this_thread::sleep_for(10ms);
		}
		pn.stop();
	};

	std::vector<std::thread> threads;

	for (size_t i = 0; i < totalThreads; ++i)
	{
		threads.emplace_back(std::thread(f));
		_latch.count_down();
	}

	for (auto& t : threads)
	{
		t.join();
	}
	
	EXPECT_EQ(0, pn.getNumberOfTokens("Counter"));
	EXPECT_EQ(155, pn.getNumberOfTokens("P1"));
	EXPECT_EQ(5, pn.getNumberOfTokens("P2"));
	EXPECT_EQ(1, pn.getNumberOfTokens("P3"));	
}
