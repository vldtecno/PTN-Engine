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

#include "gtest/gtest.h"

#include "Fixtures/FixtureTestTransition.h"


using namespace std;
using namespace ptne;

TEST_F(FixtureTestTransition, T1)
{
	vector<size_t> inputTokens{ 1, 1, 1 };
	vector<size_t> expectedInputTokens{ 0, 0, 0 };
	vector<size_t> outputTokens{ 0, 0, 0 };
	vector<size_t> expectedOutputTokens{ 1, 1, 1 };
	VectorOfConditions conditions{ { "", bind(&Controller::activationCondition, m_controller) } };
	bool expectedFireResult = true;

	createTransition(inputTokens, outputTokens, expectedInputTokens, expectedOutputTokens, conditions,
					 expectedFireResult);
	EXPECT_EQ(1, m_controller->activationConditionCallCounter());
}

 TEST_F(FixtureTestTransition, T2)
{
	vector<size_t> inputTokens{ 1, 0, 1 };
	vector<size_t> expectedInputTokens{ 1, 0, 1 };
	vector<size_t> outputTokens{ 0, 0, 0 };
	vector<size_t> expectedOutputTokens{ 0, 0, 0 };
	VectorOfConditions conditions{ { "", bind(&Controller::activationCondition, m_controller) } };
	bool expectedFireResult = false;

	createTransition(inputTokens, outputTokens, expectedInputTokens, expectedOutputTokens, conditions,
					 expectedFireResult);

	EXPECT_EQ(0, m_controller->activationConditionCallCounter());
}

 TEST_F(FixtureTestTransition, T3)
{
	vector<size_t> inputTokens{ 1, 1, 1 };
	vector<size_t> expectedInputTokens{ 1, 1, 1 };
	vector<size_t> outputTokens{ 0, 0, 0 };
	vector<size_t> expectedOutputTokens{ 0, 0, 0 };
	VectorOfConditions conditions{ { "", bind(&Controller::activationCondition, m_controller) } };
	bool expectedFireResult = false;
	m_controller->setFireCondition(false);

	createTransition(inputTokens, outputTokens, expectedInputTokens, expectedOutputTokens, conditions,
					 expectedFireResult);

	EXPECT_EQ(1, m_controller->activationConditionCallCounter());
}

TEST_F(FixtureTestTransition, T_Weights)
{
	vector<size_t> inputTokens{ 2, 2 };
	vector<size_t> inputWeights{ 2, 1 };
	vector<size_t> outputTokens{ 0 };
	vector<size_t> outputWeights{ 2 };
	vector<size_t> expectedInputTokens{ 0, 1 };
	vector<size_t> expectedOutputTokens{ 2 };
	VectorOfConditions conditions{};
	bool expectedFireResult = true;

	createTransitionWithWeights(inputTokens, inputWeights, outputTokens, outputWeights, expectedInputTokens,
								expectedOutputTokens, conditions, expectedFireResult);
}

TEST_F(FixtureTestTransition, T_ZeroValueWeightException)
{
	vector<size_t> inputTokens{ 2, 2 };
	vector<size_t> inputWeights{ 2, 1 };
	vector<size_t> outputTokens{ 0 };
	vector<size_t> outputWeights{ 0 };
	VectorOfConditions conditions{};

	vector<shared_ptr<Place>> inputPlaces = createPlaces(inputTokens);
	vector<weak_ptr<Place>> wInputPlaces = createPlaceWPtrs(inputPlaces);

	vector<shared_ptr<Place>> outputPlaces = createPlaces(outputTokens);
	vector<weak_ptr<Place>> wOutputPlaces = createPlaceWPtrs(outputPlaces);

	ASSERT_THROW(Transition(wInputPlaces, inputWeights, wOutputPlaces, outputWeights, {}, conditions),
				 Transition::ZeroValueWeightException);
}

TEST_F(FixtureTestTransition, T_ActivationWeightDimensionException)
{
	vector<size_t> inputTokens{ 2, 2 };
	vector<size_t> inputWeights{ 2 };
	vector<size_t> outputTokens{ 0 };
	vector<size_t> outputWeights{ 1 };
	VectorOfConditions conditions{};

	vector<shared_ptr<Place>> inputPlaces = createPlaces(inputTokens);
	vector<weak_ptr<Place>> wInputPlaces = createPlaceWPtrs(inputPlaces);

	vector<shared_ptr<Place>> outputPlaces = createPlaces(outputTokens);
	vector<weak_ptr<Place>> wOutputPlaces = createPlaceWPtrs(outputPlaces);

	ASSERT_THROW(Transition(wInputPlaces, inputWeights, wOutputPlaces, outputWeights, {}, conditions),
				 Transition::ActivationWeightDimensionException);
}

TEST_F(FixtureTestTransition, T_DestinationWeightDimensionException)
{
	vector<size_t> inputTokens{ 2, 2 };
	vector<size_t> inputWeights{ 2, 3 };
	vector<size_t> outputTokens{ 0 };
	vector<size_t> outputWeights{ 1, 5 };
	VectorOfConditions conditions{};

	vector<shared_ptr<Place>> inputPlaces = createPlaces(inputTokens);
	vector<weak_ptr<Place>> wInputPlaces = createPlaceWPtrs(inputPlaces);

	vector<shared_ptr<Place>> outputPlaces = createPlaces(outputTokens);
	vector<weak_ptr<Place>> wOutputPlaces = createPlaceWPtrs(outputPlaces);

	ASSERT_THROW(Transition(wInputPlaces, inputWeights, wOutputPlaces, outputWeights, {}, conditions),
				 Transition::DestinationWeightDimensionException);
}

TEST_F(FixtureTestTransition, T_ActivationPlaceRepetitionException)
{
	vector<size_t> inputTokens{ 2, 2 };
	vector<size_t> inputWeights{ 2, 3, 1 };
	vector<size_t> outputTokens{ 1, 2 };
	vector<size_t> outputWeights{ 1, 5 };
	VectorOfConditions conditions{};

	vector<shared_ptr<Place>> inputPlaces = createPlaces(inputTokens);
	vector<weak_ptr<Place>> wInputPlaces = createPlaceWPtrs(inputPlaces);
	wInputPlaces.push_back(inputPlaces.at(0));

	vector<shared_ptr<Place>> outputPlaces = createPlaces(outputTokens);
	vector<weak_ptr<Place>> wOutputPlaces = createPlaceWPtrs(outputPlaces);

	ASSERT_THROW(Transition(wInputPlaces, inputWeights, wOutputPlaces, outputWeights, {}, conditions),
				 Transition::ActivationPlaceRepetitionException);
}

TEST_F(FixtureTestTransition, T_DestinationPlaceRepetitionException)
{
	vector<size_t> inputTokens{ 2, 2 };
	vector<size_t> inputWeights{ 2, 3 };
	vector<size_t> outputTokens{ 1, 2 };
	vector<size_t> outputWeights{ 1, 5, 3 };
	VectorOfConditions conditions{};

	vector<shared_ptr<Place>> inputPlaces = createPlaces(inputTokens);
	vector<weak_ptr<Place>> wInputPlaces = createPlaceWPtrs(inputPlaces);

	vector<shared_ptr<Place>> outputPlaces = createPlaces(outputTokens);
	vector<weak_ptr<Place>> wOutputPlaces = createPlaceWPtrs(outputPlaces);
	wOutputPlaces.push_back(outputPlaces.at(0));

	ASSERT_THROW(Transition(wInputPlaces, inputWeights, wOutputPlaces, outputWeights, {}, conditions),
				 Transition::DestinationPlaceRepetitionException);
}
