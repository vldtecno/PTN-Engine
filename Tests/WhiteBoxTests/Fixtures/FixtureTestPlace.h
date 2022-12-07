/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2023 Eduardo Valgôde
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

#include "Mocks/Controller.h"
#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/Place.h"


//! Fixture to test places.
class FixtureTestPlace : public ::testing::Test
{
public:
	//! Constructor.
	FixtureTestPlace();

	//! Test entering a place with default tokens.
	void enterPlace();

	//! Test exiting a place with default tokens.
	void exitPlace();

	//! Test input places specifics.
	void inputPlace();

	/*!
	 * Test entering a place with a specific number of tokens.
	 * \param tokens Number of tokens.
	 */
	void enterPlace(const size_t tokens);

	/*!
	 * Test exiting a place with a specific number of tokens.
	 * \param tokens Number of tokens.
	 */
	void exitPlace(const size_t tokens);

protected:
	//! Mock/fake controller
	std::shared_ptr<Controller> m_controller;

	ptne::PTN_Engine m_ptnEngine;

	//! A PTN Engine place.
	ptne::Place m_place;
};
