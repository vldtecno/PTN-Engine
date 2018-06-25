/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
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
#include "Mocks/Simple/SimpleController.h"
#include "Mocks/RegisteredFunctions/Controller.h"

using namespace std;

FixturePetriNet::FixturePetriNet():
	m_dispatcher(make_shared<Dispatcher>())
{
	m_dispatcher->initialize();
}

void FixturePetriNet::testRoundRobinState(const size_t expectedTokens[s_numberOfRoundRobinPlaces])
{
	if(!m_dispatcher)
	{
		throw runtime_error("No dispatcher available");
	}

	size_t tokens[s_numberOfRoundRobinPlaces];

	//TODO
	//Dangerous(ugly) cast necessary only for testing. This does not need to exist within a normal use case.
	//Nonetheless it would be nice to fix it.
	Dispatcher::IDispatcherPetriNet *dispatcherPetriNet = m_dispatcher->m_pPetriNet.get();

	if(Dispatcher::RoundRobinPetriNet* roundRobinPetriNet = dynamic_cast<Dispatcher::RoundRobinPetriNet*>(dispatcherPetriNet))
	{
		tokens[0] = roundRobinPetriNet->getNumberOfTokens("InputWaitPackage");
		tokens[1] = roundRobinPetriNet->getNumberOfTokens("WaitPackage");
		tokens[2] = roundRobinPetriNet->getNumberOfTokens("ChannelA");
		tokens[3] = roundRobinPetriNet->getNumberOfTokens("ChannelB");
		tokens[4] = roundRobinPetriNet->getNumberOfTokens("SelectA");
		tokens[5] = roundRobinPetriNet->getNumberOfTokens("SelectB");
		tokens[6] = roundRobinPetriNet->getNumberOfTokens("PackageCounter");
	}

	for(size_t i = 0; i < s_numberOfRoundRobinPlaces; ++i )
	{
		size_t a = expectedTokens[i];
		EXPECT_EQ(a, tokens[i]);
	}

}


void FixturePetriNet::testFreeChoiceState(const size_t expectedTokens[s_numberOfFreeChoicePlaces])
{
	if(!m_dispatcher)
	{
		throw runtime_error("No dispatcher available");
	}

	size_t tokens[s_numberOfFreeChoicePlaces];

	//TODO
	//Dangerous(ugly) cast necessary only for testing. This does not need to exist within a normal use case.
	//Nonetheless it would be nice to fix it.
	Dispatcher::IDispatcherPetriNet *dispatcherPetriNet = m_dispatcher->m_pPetriNet.get();

	if(Dispatcher::FreeChoicePetriNet* freeChoicePetriNet = dynamic_cast<Dispatcher::FreeChoicePetriNet*>(dispatcherPetriNet))
	{
		tokens[0] = freeChoicePetriNet->getNumberOfTokens("InputWaitPackage");
		tokens[1] = freeChoicePetriNet->getNumberOfTokens("WaitPackage");
		tokens[2] = freeChoicePetriNet->getNumberOfTokens("ChannelA");
		tokens[3] = freeChoicePetriNet->getNumberOfTokens("CounterA");
		tokens[4] = freeChoicePetriNet->getNumberOfTokens("ChannelB");
		tokens[5] = freeChoicePetriNet->getNumberOfTokens("CounterB");
		tokens[6] = freeChoicePetriNet->getNumberOfTokens("PackageCounter");
	}
	else
	{
		throw runtime_error("Could not cast to FreeChoicePetriNet");
	}

	for(size_t i = 0; i < 2; ++i )
	{
		size_t a = expectedTokens[i];
		EXPECT_EQ(a, tokens[i]);
	}

	EXPECT_EQ(expectedTokens[6], tokens[6]);

	EXPECT_TRUE(tokens[3] > 0 || tokens[5] > 0) << tokens[3] << " " << tokens[5];

	float metric =
		abs(static_cast<float>(tokens[3])- static_cast<float>(tokens[5]))
		/
		(tokens[3]+tokens[5]);

	EXPECT_TRUE(metric < 0.04);

}

void FixturePetriNet::testWeightedState(const size_t expectedTokens[s_numberOfWeightedPlaces])
{
	if(!m_dispatcher)
	{
		throw runtime_error("No dispatcher available");
	}

	size_t tokens[s_numberOfWeightedPlaces];

	//TODO
	//Dangerous(ugly) cast necessary only for testing. This does not need to exist within a normal use case.
	//Nonetheless it would be nice to fix it.
	Dispatcher::IDispatcherPetriNet *dispatcherPetriNet = m_dispatcher->m_pPetriNet.get();

	if(Dispatcher::WeightedPetriNet* weightedPetriNet = dynamic_cast<Dispatcher::WeightedPetriNet*>(dispatcherPetriNet))
	{
		tokens[0] = weightedPetriNet->getNumberOfTokens("InputWaitPackage");
		tokens[1] = weightedPetriNet->getNumberOfTokens("WaitPackage");
		tokens[2] = weightedPetriNet->getNumberOfTokens("ChannelA");
		tokens[3] = weightedPetriNet->getNumberOfTokens("ChannelB");
	}

	for(size_t i = 0; i < s_numberOfWeightedPlaces; ++i )
	{
		size_t a = expectedTokens[i];
		EXPECT_EQ(a, tokens[i]);
	}

}


void FixturePetriNet::testInhibitedState(const size_t expectedTokens[s_numberOfInhibitedNetPlaces])
{
	if(!m_dispatcher)
	{
		throw runtime_error("No dispatcher available");
	}

	size_t tokens[s_numberOfInhibitedNetPlaces];

	//TODO
	//Dangerous(ugly) cast necessary only for testing. This does not need to exist within a normal use case.
	//Nonetheless it would be nice to fix it.
	Dispatcher::IDispatcherPetriNet *dispatcherPetriNet = m_dispatcher->m_pPetriNet.get();

	if(Dispatcher::InhibitedPetriNet* weightedPetriNet = dynamic_cast<Dispatcher::InhibitedPetriNet*>(dispatcherPetriNet))
	{
		tokens[0] = weightedPetriNet->getNumberOfTokens("InputWaitPackage");
		tokens[1] = weightedPetriNet->getNumberOfTokens("P1");
		tokens[2] = weightedPetriNet->getNumberOfTokens("P2");
		tokens[3] = weightedPetriNet->getNumberOfTokens("P3");
		tokens[4] = weightedPetriNet->getNumberOfTokens("P4");
		tokens[5] = weightedPetriNet->getNumberOfTokens("P5");
	}

	for(size_t i = 0; i < s_numberOfInhibitedNetPlaces; ++i )
	{
		size_t a = expectedTokens[i];
		EXPECT_EQ(a, tokens[i]);
	}

}


void FixturePetriNet::testThreadSafety()
{
	std::shared_ptr<SimpleController> simpleController = make_shared<SimpleController>();
	simpleController->initialize();

	const size_t numberOfThreads = 8;
	simpleController->doSomethingConcurrently(numberOfThreads);
	EXPECT_EQ(numberOfThreads, simpleController->getNumberOfDifferentThreads());

	//TODO get a better way to test
	EXPECT_EQ(0, simpleController->m_petriNet->getNumberOfTokens("P2"));
}

void FixturePetriNet::testRegisteredMethods()
{
	std::shared_ptr<SimpleController> simpleController = make_shared<SimpleController>();
	simpleController->initialize();

	const size_t numberOfThreads = 8;
	simpleController->doSomethingConcurrently(numberOfThreads);
	EXPECT_EQ(numberOfThreads, simpleController->getNumberOfDifferentThreads());

	//TODO get a better way to test
	EXPECT_EQ(0, simpleController->m_petriNet->getNumberOfTokens("P2"));
}

