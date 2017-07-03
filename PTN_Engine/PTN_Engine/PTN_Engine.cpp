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

#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/PTN_Engine/PTN_EngineImp.h"
#include "PTN_Engine/Place.h"

namespace ptne
{
	PTN_Engine::~PTN_Engine()
	{}

	PTN_Engine::PTN_Engine():
			m_implementation(new PTN_EngineImp())
	{
	}

	void PTN_Engine::createTransition(
			const std::vector<std::string>& activationPlaces,
			const std::vector<std::string>& destinationPlaces,
			const std::vector<ConditionFunctorPtr>& additionalConditions)
	{
		m_implementation->createTransition(activationPlaces, destinationPlaces, additionalConditions);
	}

	void PTN_Engine::execute()
	{
		m_implementation->execute();
	}

	void PTN_Engine::addPlace(const std::string& name,
			const size_t initialNumberOfTokens,
						ActionFunctorPtr onEnterAction,
						ActionFunctorPtr onExitAction,
						const bool input)
	{
		m_implementation->addPlace(name,
				initialNumberOfTokens,
				onEnterAction,
				onExitAction,
				input);
	}

	size_t PTN_Engine::getNumberOfTokens(const std::string& place) const
	{
		return m_implementation->getNumberOfTokens(place);
	}

	void PTN_Engine::incrementInputPlace(const std::string& place)
	{
		m_implementation->incrementInputPlace(place);
	}

}
