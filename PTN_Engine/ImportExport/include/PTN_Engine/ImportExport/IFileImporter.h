/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2019-2024 Eduardo Valgôde
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

struct ArcProperties;
class PTN_Engine;
struct PlaceProperties;
struct TransitionProperties;

//!
//! \brief The IFileImporter class is an interface class for all PTN_Engine file importers.
//!
class DLL_PUBLIC IFileImporter
{
public:
	virtual ~IFileImporter();

	//!
	//! \brief Import a PTN_Engine object from a file.
	//! \param filePath - the file path where the PTN_Engine should be imported from.
	//! \param ptnEngine - the PTN_Object where the contents from the file will be inserted.
	//!
	virtual void _import(const std::string &filePath, PTN_Engine &ptnEngine) = 0;

protected:
	void _importInt(PTN_Engine &ptnEngine) const;

private:
	virtual std::string importActionsThreadOption() const = 0;
	virtual std::vector<PlaceProperties> importPlaces() const = 0;
	virtual std::vector<TransitionProperties> importTransitions() const = 0;
	virtual std::vector<ArcProperties> importArcs() const = 0;
};

} // namespace ptne
