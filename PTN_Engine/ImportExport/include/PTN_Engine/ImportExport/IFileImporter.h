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
#include "PTN_Engine/PTN_Engine.h"
#include <vector>

namespace ptne
{
class PTN_Engine;

//!
//! \brief The IImporter class
//!
class DLL_PUBLIC IFileImporter
{
public:
	virtual ~IFileImporter() = default;

	//!
	//! \brief import
	//! \param ptnEngine
	//!
	virtual void _import(const std::string &filePath, PTN_Engine &ptnEngine) = 0;

protected:

	void _import(PTN_Engine &ptnEngine) const;

private:
	//!
	//! \brief importActionsThreadOption
	//! \return
	//!
	virtual std::string importActionsThreadOption() const = 0;

	//!
	//! \brief importPlaces
	//! \return
	//!
	virtual std::vector<PlaceProperties> importPlaces() const = 0;

	//!
	//! \brief importTransitions
	//! \return
	//!
	virtual std::vector<TransitionProperties> importTransitions() const = 0;

	//!
	//! \brief importArcs
	//! \return
	//!
	virtual std::vector<ArcProperties> importArcs() const = 0;
};

} // namespace ptne
