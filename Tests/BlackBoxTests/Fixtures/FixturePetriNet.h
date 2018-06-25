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
#include "Mocks/Dispatcher/Dispatcher.h"

//! Fixture for testing a complete PTN net.
/*!
 * To test a complete PTN Engine net from the users point of view.
 * It only uses the public interface.
 */
class FixturePetriNet: public ::testing::Test
{
public:

	//! Total number of places in the net.
	const static size_t s_numberOfRoundRobinPlaces = 7;

	//! Total number of places in the net.
	const static size_t s_numberOfFreeChoicePlaces = 7;

	//! Total number of places in the net.
	const static size_t s_numberOfWeightedPlaces = 4;

	//! Total number of places in the net.
	const static size_t s_numberOfInhibitedNetPlaces = 6;

	//! Constructor.
	FixturePetriNet();

	/*!
	 * Tests the number of tokens in all places of the net.
	 * \param expectedTokens Expected number of tokens in each place.
	 */
	void testRoundRobinState(const size_t expectedTokens[s_numberOfRoundRobinPlaces]);

	/*!
	 * Tests the number of tokens in all places of the net.
	 * \param expectedTokens Expected number of tokens in each place.
	 */
	void testFreeChoiceState(const size_t expectedTokens[s_numberOfFreeChoicePlaces]);

	/*!
	 * Tests the number of tokens in all places of the net.
	 * \param expectedTokens Expected number of tokens in each place.
	 */
	void testWeightedState(const size_t expectedTokens[s_numberOfWeightedPlaces]);

	/*!
	 * Tests the number of tokens in all places of the net.
	 * \param expectedTokens Expected number of tokens in each place.
	 */
	void testInhibitedState(const size_t expectedTokens[s_numberOfInhibitedNetPlaces]);

	/*!
	 * 
	 * 
	 */
	void testThreadSafety();

	/*!
	 *
	 *
	 */
	void testRegisteredMethods();

	//! Controller containing the PTN Engine net.
	std::shared_ptr<Dispatcher> m_dispatcher;

};
