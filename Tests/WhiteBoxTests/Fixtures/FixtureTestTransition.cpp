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

#include "Fixtures/FixtureTestTransition.h"

using namespace ptne;
using namespace std;

FixtureTestTransition::FixtureTestTransition():
	m_controller{make_shared<Controller>(Controller{})}
{

}

void FixtureTestTransition::createTransition(const vector<size_t> inputTokens,
		const vector<size_t> outputTokens,
		const vector<size_t> expectedInputTokens,
		const vector<size_t> expectedOutputTokens,
		VectorOfConditions conditions,
		const bool expectedFireResult)
{
	vector<shared_ptr<Place>> inputPlaces = createPlaces(inputTokens);
	vector<weak_ptr<Place>> wInputPlaces = createPlaceWPtrs(inputPlaces);

	vector<shared_ptr<Place>> outputPlaces = createPlaces(outputTokens);
	vector<weak_ptr<Place>> wOutputPlaces = createPlaceWPtrs(outputPlaces);

	for(size_t i : outputTokens)
	{
		shared_ptr<Place> newPlace (
				new Place(i, nullptr, nullptr));
		outputPlaces.push_back(newPlace);
		wOutputPlaces.push_back(newPlace);
	}

	Transition transition(wInputPlaces, wOutputPlaces, conditions);

	EXPECT_EQ(expectedFireResult, transition.execute());

	size_t i = 0;
	for(size_t expected : expectedInputTokens)
	{
		EXPECT_EQ(expected, inputPlaces[i]->getNumberOfTokens());
		++i;
	}

	i = 0;
	for(size_t expected : expectedOutputTokens)
	{
		EXPECT_EQ(expected, outputPlaces[i]->getNumberOfTokens());
		++i;
	}
}


void FixtureTestTransition::createTransitionWithWeights(
		const vector<size_t> inputTokens,
		const vector<size_t> inputWeights,
		const vector<size_t> outputTokens,
		const vector<size_t> ouptutWeights,
		const vector<size_t> expectedInputTokens,
		const vector<size_t> expectedOutputTokens,
		VectorOfConditions conditions,
		const bool expectedFireResult)
{
	EXPECT_EQ(inputTokens.size(), inputWeights.size());
	EXPECT_EQ(outputTokens.size(), ouptutWeights.size());
	EXPECT_EQ(outputTokens.size(), expectedOutputTokens.size());


	//Create input places
	vector<shared_ptr<Place>> inputPlaces = createPlaces(inputTokens);
	vector<weak_ptr<Place>> wPtrInputPlaces = createPlaceWPtrs(inputPlaces);

	//Create output places
	vector<shared_ptr<Place>> outputPlaces = createPlaces(outputTokens);
	vector<weak_ptr<Place>> wOutputPlaces = createPlaceWPtrs(outputPlaces);

	//Create transition
	Transition transition(wPtrInputPlaces, inputWeights, wOutputPlaces, ouptutWeights, conditions);

	//Test transition
	EXPECT_EQ(expectedFireResult, transition.execute());

	size_t i = 0;
	for(size_t expected : expectedInputTokens)
	{
		EXPECT_EQ(expected, inputPlaces[i]->getNumberOfTokens());
		++i;
	}

	i = 0;
	for(size_t expected : expectedOutputTokens)
	{
		EXPECT_EQ(expected, outputPlaces[i]->getNumberOfTokens());
		++i;
	}
}

vector<shared_ptr<Place>> FixtureTestTransition::createPlaces(const vector<size_t> inputTokens)
{
	//Create input places
	vector<shared_ptr<Place>> inputPlaces;
	for(size_t i : inputTokens)
	{
		shared_ptr<Place> newPlace (
				new Place(i, nullptr, nullptr));
		inputPlaces.push_back(newPlace);
	}
	return inputPlaces;
}

vector<weak_ptr<Place>> FixtureTestTransition::createPlaceWPtrs(const vector<shared_ptr<Place>> places)
{
	//Create input places
	vector<weak_ptr<Place>> wPtrPlaces;
	for(shared_ptr<Place> spPlace : places)
	{
		wPtrPlaces.push_back(weak_ptr<Place>(spPlace));
	}
	return wPtrPlaces;
}


