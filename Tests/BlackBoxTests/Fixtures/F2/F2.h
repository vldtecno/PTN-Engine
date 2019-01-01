/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2018 Eduardo Valgôde
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

#include "Mocks/Dispatcher/Dispatcher.h"
#include "gtest/gtest.h"

//! Fixture for testing a complete PTN net.
/*!
 *
 */
class F2 : public ::testing::Test
{
public:
	//! Constructor.
	F2();

	/*!
	 * Test a Petri net whose methods are registered first.
	 */
	void testRegisteredMethods();

	/*!
	 * Test detection of already registered methods.
	 */
	void testRepeatedRegisteredMethods();

	//! Controller containing the PTN Engine net.
	std::shared_ptr<Dispatcher> m_dispatcher;
};
