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

class DLL_PUBLIC IExporter
{
public:
	virtual ~IExporter() = default;

	virtual void exportActionsThreadOption(const std::string &actionsThreadOption) = 0;

	virtual void exportPlace(const std::string &name,
							 const std::string &tokens,
							 const std::string &isInput,
							 const std::string &onEnterAction,
							 const std::string &onExitAction) = 0;

	virtual void exportTransition(const std::vector<std::tuple<std::string, size_t>> &activationPlaces,
								  const std::vector<std::tuple<std::string, size_t>> &destinationPlaces,
								  const std::vector<std::string> &activationConditions,
								  const std::vector<std::string> &inhibitorPlaces,
								  const bool requireNoActionsInExecution) = 0;

	virtual void saveFile(const std::string &filePath) const = 0;
};

} // namespace ptne
