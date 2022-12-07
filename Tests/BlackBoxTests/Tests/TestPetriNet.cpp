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
	using std::make_unique;
	using std::unique_ptr;
	using std::vector;
	class Controller
	{
		class PetriNet : public ptne::PTN_Engine
		{
			friend class FixturePetriNet;

		public:
			explicit PetriNet(Controller &controller)
			: ptne::PTN_Engine(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP)
			{
				createPlace("P1", 0, true);
				createPlace("P2", 0);

				createTransition({ "P1" }, { "P2" },
								 vector<ptne::ConditionFunction>{ [&controller]()
																  {
																	  ++controller.m_callCount;
																	  return controller.m_hasConnectionToServer;
																  } });
			}
		};

	public:
		Controller()
		: m_petriNet(nullptr)
		, m_callCount(0)
		, m_hasConnectionToServer(true){};

		void initialize()
		{
			m_petriNet = make_unique<Controller::PetriNet>(*this);
		}

		void execute()
		{
			m_petriNet->incrementInputPlace("P1");
			m_petriNet->execute();
		}

		vector<size_t> getNumberOfTokens()
		{
			vector<size_t> numberOfTokens;
			numberOfTokens.emplace_back(m_petriNet->getNumberOfTokens("P1"));
			numberOfTokens.emplace_back(m_petriNet->getNumberOfTokens("P2"));
			return numberOfTokens;
		}

		unique_ptr<PetriNet> m_petriNet;
		size_t m_callCount = 0;
		bool m_hasConnectionToServer = false;
	};

	Controller controller;
	controller.initialize();
	EXPECT_EQ(0, controller.m_callCount);
	controller.execute();
	std::this_thread::sleep_for(10ms);
	EXPECT_EQ(1, controller.m_callCount);
	EXPECT_EQ(vector<size_t>({ 0, 1 }), controller.getNumberOfTokens());
	controller.m_petriNet->stop();
}
