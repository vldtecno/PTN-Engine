/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2019-2023 Eduardo Valgôde
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
#include <string>
#include <vector>

namespace ptne
{
class PTN_Engine;

class DLL_PUBLIC IImporter
{
public:
	virtual ~IImporter() = default;

	virtual std::string getActionsThreadOption() const = 0;

	using PlaceInfo = std::tuple<std::string, // name
								 size_t, // number of tokens
								 std::string, // on enter action
								 std::string, // on exit action
								 bool // is input place
								 >;

	/*!
	 * \brief getPlaces
	 * \return
	 */
	virtual std::vector<PlaceInfo> getPlaces() const = 0;


	using TransitionInfo = std::tuple<std::vector<std::string>, // activation places
									  std::vector<size_t>, // activation weights
									  std::vector<std::string>, // destination places
									  std::vector<size_t>, // destination weights
									  std::vector<std::string>, // inhibitor places
									  std::vector<std::string>, // activation conditions
									  bool // require no action in execution
									  >;

	/*!
	 * \brief getTransitions
	 * \return
	 */
	virtual std::vector<TransitionInfo> getTransitions() const = 0;
};

} // namespace ptne
