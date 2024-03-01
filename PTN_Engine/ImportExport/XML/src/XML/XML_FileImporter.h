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

#include "PTN_Engine/ImportExport/IFileImporter.h"
#include <pugixml.hpp>
#include <vector>

namespace ptne
{
class PTN_Engine;

//!
//! \brief The XML_Importer class
//!
class XML_FileImporter : public IFileImporter//, public IFileImporter
{
public:
	~XML_FileImporter() override = default;

	void _import(const std::string& filePath, PTN_Engine &ptnEngine) override;

private:

	//!
	//! \brief importActionsThreadOption
	//! \return
	//!
	std::string importActionsThreadOption() const override;

	//!
	//! \brief importPlaces
	//! \return
	//!
	std::vector<PlaceProperties> importPlaces() const override;

	//!
	//! \brief importTransitions
	//! \return
	//!
	std::vector<TransitionProperties> importTransitions() const override;

	//!
	//! \brief importArcs
	//! \return
	//!
	std::vector<ArcProperties> importArcs() const override;

	//!
	//! \brief m_document
	//!
	pugi::xml_document m_document;
};
} // namespace ptne
