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

TEST_F(FixtureTestTransition, T1)
{
	using namespace ptne;

	vector<size_t> inputTokens {1,1,1};
	vector<size_t> expectedInputTokens {0,0,0};
	vector<size_t> outputTokens {0,0,0};
	vector<size_t> expectedOutputTokens {1,1,1};
	VectorOfConditions conditions {make_shared<ControllerFireCondition>(ControllerFireCondition(m_controller, &Controller::activationCondition))};
	bool expectedFireResult = true;

	createTransition(inputTokens, outputTokens, expectedInputTokens, expectedOutputTokens, conditions, expectedFireResult);
}

TEST_F(FixtureTestTransition, T2)
{
	using namespace ptne;

	vector<size_t> inputTokens {1,0,1};
	vector<size_t> expectedInputTokens {1,0,1};
	vector<size_t> outputTokens {0,0,0};
	vector<size_t> expectedOutputTokens {0,0,0};
	VectorOfConditions conditions {make_shared<ControllerFireCondition>(ControllerFireCondition(m_controller, &Controller::activationCondition))};
	bool expectedFireResult = false;

	createTransition(inputTokens, outputTokens, expectedInputTokens, expectedOutputTokens, conditions, expectedFireResult);
}

TEST_F(FixtureTestTransition, T3)
{
	using namespace ptne;

	vector<size_t> inputTokens {1,1,1};
	vector<size_t> expectedInputTokens {1,1,1};
	vector<size_t> outputTokens {0,0,0};
	vector<size_t> expectedOutputTokens {0,0,0};
	VectorOfConditions conditions {make_shared<ControllerFireCondition>(ControllerFireCondition(m_controller, &Controller::activationCondition))};
	bool expectedFireResult = false;
	m_controller->setFireCondition(false);

	createTransition(inputTokens, outputTokens, expectedInputTokens, expectedOutputTokens, conditions, expectedFireResult);
}

TEST_F(FixtureTestTransition, T4_Weights)
{
	using namespace ptne;

	vector<size_t> inputTokens {2,2};
	vector<size_t> inputWeights {2,1};
	vector<size_t> outputTokens {0};
	vector<size_t> outputWeighs {2};
	vector<size_t> expectedInputTokens{0,1};
	vector<size_t> expectedOutputTokens {2};
	VectorOfConditions conditions {};
	bool expectedFireResult = true;

	createTransitionWithWeights(inputTokens, inputWeights, outputTokens, outputWeighs, expectedInputTokens, expectedOutputTokens, conditions, expectedFireResult);
}

