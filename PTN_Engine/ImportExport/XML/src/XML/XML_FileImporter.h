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
//! \brief The XML_FileImporter class implements the import of a PTN_Engine object from an xml file.
//!
class XML_FileImporter : public IFileImporter //, public IFileImporter
{
public:
	~XML_FileImporter() override;
	XML_FileImporter() = default;
	XML_FileImporter(const XML_FileImporter &) = delete;
	XML_FileImporter(XML_FileImporter &&) = delete;
	XML_FileImporter &operator=(const XML_FileImporter &) = delete;
	XML_FileImporter &operator=(XML_FileImporter &&) = delete;

	//!
	//! \brief _import Imports a PTN_Engine object from an xml file.
	//! \param filePath - file path to the xml file with the PTN_Engine object.
	//! \param ptnEngine - PTN_Engine object to be populated.
	//!
	void _import(const std::string &filePath, PTN_Engine &ptnEngine) override;

private:
	std::string importActionsThreadOption() const override;

	std::vector<ArcProperties> importArcs() const override;

	std::vector<PlaceProperties> importPlaces() const override;

	std::vector<TransitionProperties> importTransitions() const override;

	pugi::xml_document m_document;
};

} // namespace ptne
