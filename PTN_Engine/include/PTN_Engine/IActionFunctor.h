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

#include "PTN_Engine/Utilities/Explicit.h"

namespace ptne
{
	//! Interface class for functors called by the net.
	/*!
	 * Interface for the functors to be called on enter and on exit
	 * of tokens in places of the net.
	 */
	class DLL_PUBLIC IActionFunctor
	{
	public:

		virtual ~IActionFunctor(){}

		//! Will be called on enter or on exit of tokens in a given place.
		virtual void operator()() = 0;

	};

}
