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

#include "WhiteBoxTests/Fixtures/FixtureTestTransition.h"

using namespace ptne;

FixtureTestTransition::FixtureTestTransition():
	m_controller{std::make_shared<Controller>(Controller{})}
{

}

void FixtureTestTransition::createTransition(const std::vector<size_t> inputTokens,
		const std::vector<size_t> outputTokens,
		const std::vector<size_t> expectedOutputTokens,
		VectorOfConditions conditions,
		const bool expectedFireResult)
{
	std::vector<std::shared_ptr<Place>> inputPlaces;
	std::vector<std::weak_ptr<Place>> wInputPlaces;
	for(size_t i : inputTokens)
	{
		std::shared_ptr<Place> newPlace (
				new Place(i, nullptr, nullptr));
		inputPlaces.push_back(newPlace);
		wInputPlaces.push_back(newPlace);
	}



	std::vector<std::shared_ptr<Place>> outputPlaces;
	std::vector<std::weak_ptr<Place>> wOutputPlaces;
	for(size_t i : outputTokens)
	{
		std::shared_ptr<Place> newPlace (
				new Place(i, nullptr, nullptr));
		outputPlaces.push_back(newPlace);
		wOutputPlaces.push_back(newPlace);
	}

	Transition transition(wInputPlaces, wOutputPlaces, conditions);

	EXPECT_EQ(expectedFireResult, transition.execute());

	size_t i = 0;
	for(size_t expected : expectedOutputTokens)
	{
		std::shared_ptr<Place> newPlace(
				new Place(i, nullptr, nullptr));
		outputPlaces.push_back(newPlace);
		wOutputPlaces.push_back(newPlace);
		EXPECT_EQ(expected, outputPlaces[i]->getNumberOfTokens());
		++i;
	}
}



