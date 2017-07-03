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

FixturePetriNet::FixturePetriNet():
	m_dispatcher(std::make_shared<Dispatcher>())
{
	m_dispatcher->initialize();
}

void FixturePetriNet::testState(const size_t expectedTokens[s_numberOfPlaces])
{
	if(!m_dispatcher)
	{
		throw std::runtime_error("No dispatcher available");
	}

	size_t tokens[7];

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

	for(size_t i = 0; i < s_numberOfPlaces; ++i )
	{
		size_t a = expectedTokens[i];
		EXPECT_EQ(a, tokens[i]);
	}

}
