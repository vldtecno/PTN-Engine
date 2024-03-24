/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
 * Copyright (c) 2021 Kale Evans
 * Copyright (c) 2024 Eduardo Valgôde
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

void FixtureTestTransition::createTransition(const vector<size_t> &inputTokens,
                                             const vector<size_t> &outputTokens,
                                             const vector<size_t> &expectedInputTokens,
                                             const vector<size_t> &expectedOutputTokens,
                                             const VectorOfConditions &conditions,
                                             const bool expectedFireResult)
{
	vector<SharedPtrPlace> inputPlaces = createPlaces(inputTokens);
	vector<WeakPtrPlace> wInputPlaces = createPlaceWPtrs(inputPlaces);

	vector<SharedPtrPlace> outputPlaces = createPlaces(outputTokens);
	vector<WeakPtrPlace> wOutputPlaces = createPlaceWPtrs(outputPlaces);

	for (size_t i : outputTokens)
	{
		auto newPlace = make_shared<Place>(m_ptnEngine, PlaceProperties{ .initialNumberOfTokens = i});
		outputPlaces.push_back(newPlace);
		wOutputPlaces.push_back(newPlace);
	}

	auto createArcs = [] (const vector<SharedPtrPlace> &places)
	{
		vector<Arc> arcs;
		for (const auto &place : places)
		{
			arcs.emplace_back(place);
		}
		return arcs;
	};

	Transition transition("T1",
						  createArcs(inputPlaces),
						  createArcs(outputPlaces),
						  {},
						  conditions,
						  false);

	EXPECT_EQ(expectedFireResult, transition.execute());

	size_t i = 0;
	for (size_t expected : expectedInputTokens)
	{
		EXPECT_EQ(expected, inputPlaces[i]->getNumberOfTokens());
		++i;
	}

	i = 0;
	for (size_t expected : expectedOutputTokens)
	{
		EXPECT_EQ(expected, outputPlaces[i]->getNumberOfTokens());
		++i;
	}
}


void FixtureTestTransition::createTransitionWithWeights(const vector<size_t> &inputTokens,
                                                        const vector<size_t> &inputWeights,
                                                        const vector<size_t> &outputTokens,
                                                        const vector<size_t> &outputWeights,
                                                        const vector<size_t> &expectedInputTokens,
                                                        const vector<size_t> &expectedOutputTokens,
                                                        const VectorOfConditions &conditions,
                                                        const bool expectedFireResult)
{
	EXPECT_EQ(inputTokens.size(), inputWeights.size());
	EXPECT_EQ(outputTokens.size(), outputWeights.size());
	EXPECT_EQ(outputTokens.size(), expectedOutputTokens.size());


	// Create input places
	vector<SharedPtrPlace> inputPlaces = createPlaces(inputTokens);
	vector<WeakPtrPlace> wPtrInputPlaces = createPlaceWPtrs(inputPlaces);

	// Create output places
	vector<SharedPtrPlace> outputPlaces = createPlaces(outputTokens);
	vector<WeakPtrPlace> wOutputPlaces = createPlaceWPtrs(outputPlaces);

	auto createArcs = [] (const vector<SharedPtrPlace> &places, const vector<size_t> &weights)
	{
		vector<Arc> arcs;
		for (size_t i = 0; i < places.size(); ++i)
		{
			arcs.emplace_back(places[i], weights[i]);
		}
		return arcs;
	};

	// Create transition
	Transition transition("T1",
						  createArcs(inputPlaces, inputWeights),
						  createArcs(outputPlaces, outputWeights),
						  {},
						  conditions,
						  false);

	// Test transition
	EXPECT_EQ(expectedFireResult, transition.execute());

	size_t i = 0;
	for (size_t expected : expectedInputTokens)
	{
		EXPECT_EQ(expected, inputPlaces[i]->getNumberOfTokens());
		++i;
	}

	i = 0;
	for (size_t expected : expectedOutputTokens)
	{
		EXPECT_EQ(expected, outputPlaces[i]->getNumberOfTokens());
		++i;
	}
}

vector<SharedPtrPlace> FixtureTestTransition::createPlaces(const vector<size_t> &inputTokens)
{
	// Create input places
	vector<SharedPtrPlace> inputPlaces;
	for (size_t i : inputTokens)
	{
		auto newPlace = make_shared<Place>(m_ptnEngine, PlaceProperties{ .initialNumberOfTokens = i });
		inputPlaces.push_back(newPlace);
	}
	return inputPlaces;
}

vector<WeakPtrPlace> FixtureTestTransition::createPlaceWPtrs(const vector<SharedPtrPlace> &places) const
{
	// Create input places
	vector<WeakPtrPlace> wPtrPlaces;
	for (const SharedPtrPlace &spPlace : places)
	{
		wPtrPlaces.push_back(WeakPtrPlace(spPlace));
	}
	return wPtrPlaces;
}
