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

#pragma once

#include "gtest/gtest.h"
#include "PTN_Engine/Place.h"
#include "PTN_Engine/Transition.h"
#include <memory>
#include "Mocks/Controller.h"

//! Fixture to test transitions.
class FixtureTestTransition: public ::testing::Test
{
public:

	//! Constructor.
	FixtureTestTransition();

	using VectorOfPlaces = std::vector<std::weak_ptr<ptne::Place>>;

	using VectorOfConditions = std::vector<ptne::ConditionFunctorPtr>;

	/*!
	 * Test creating a transition and makes it available for further testing.
	 * \param inputTokens Tokens to put in each activation place.
	 * \param outputTokens Tokens to put in the destination places before firing.
	 * \param expectedInputTokens Expected number of tokens on the inputs after executing the transition.
	 * \param expectedOutputTokens Expected tokens after trying to fire the transition.
	 * \param conditions Additional conditions from the controller.
	 * \param expectedFireResult If it is expected that the condition fires.
	 */
	void createTransition(const std::vector<size_t> inputTokens,
			const std::vector<size_t> outputTokens,
			const std::vector<size_t> expectedInputTokens,
			const std::vector<size_t> expectedOutputTokens,
			VectorOfConditions conditions,
			const bool expectedFireResult);


	/*!
	 *
	 * \param inputTokens Tokens to put in each activation place.
	 * \param inputWeights Input weights.
	 * \param outputTokens Tokens to put in the destination places before firing.
	 * \param outputWeights Output weights.
	 * \param expectedInputTokens Expected number of tokens on the inputs after executing the transition.
	 * \param expectedOutputTokens Expected tokens after trying to fire the transition.
	 * \param conditions Additional conditions from the controller.
	 * \param expectedFireResult If it is expected that the condition fires.
	 */
	void createTransitionWithWeights(
			const std::vector<size_t> inputTokens,
			const std::vector<size_t> inputWeights,
			const std::vector<size_t> outputTokens,
			const std::vector<size_t> outputWeights,
			const std::vector<size_t> expectedInputTokens,
			const std::vector<size_t> expectedOutputTokens,
			VectorOfConditions conditions,
			const bool expectedFireResult);

protected:

	//!
	std::shared_ptr<Controller> m_controller;

};
