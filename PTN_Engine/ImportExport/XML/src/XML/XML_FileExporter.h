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

#include "PTN_Engine/ImportExport/IFileExporter.h"
#include <pugixml.hpp>

namespace ptne
{

//!
//! \brief The XML_FileExporter class implements the export of a PTN_Engine object to an xml file.
//!
class XML_FileExporter : public IFileExporter
{
public:
	~XML_FileExporter() override;
	XML_FileExporter() = default;
	XML_FileExporter(const XML_FileExporter &) = delete;
	XML_FileExporter(XML_FileExporter &&) = delete;
	XML_FileExporter &operator=(const XML_FileExporter &) = delete;
	XML_FileExporter &operator=(XML_FileExporter &&) = delete;

	//!
	//! \brief _export Exports a PTN_Engine object to a xml file.
	//! \param ptnEngine - the object to be exported.
	//! \param filePath - the file path of the new xml file.
	//!
	void _export(const PTN_Engine &ptnEngine, const std::string &filePath) override;

private:
	void exportActionsThreadOption(const std::string &actionsThreadOption) override;

	void exportPlace(const PlaceProperties &placeProperties) override;

	void exportTransition(const TransitionProperties &transitionProperties) override;

	void saveFile() const;

	pugi::xml_node m_arcsNode;

	pugi::xml_document m_document;

	std::string m_filePath;

	pugi::xml_node m_placesNode;

	pugi::xml_node m_rootNode;

	pugi::xml_node m_transitionsNode;
};

} // namespace ptne
