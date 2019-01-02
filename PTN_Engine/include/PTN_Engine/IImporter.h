/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2019 Eduardo Valgôde
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

namespace ptne
{
class PTN_Engine;

class DLL_PUBLIC IImporter
{
public:
	virtual ~IImporter() = default;

	virtual void createPlaces(PTN_Engine &petriNet) const = 0;
	virtual void createTransitions(PTN_Engine &petriNet) const = 0;
};

} // namespace ptne
