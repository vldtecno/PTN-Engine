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
//! \brief The XML_Exporter class
//!
class XML_FileExporter : public IFileExporter
{
public:
	~XML_FileExporter() override = default;

	void _export(const PTN_Engine &ptnEngine, const std::string &filePath) override;

private:

	//!
	//! \brief exportActionsThreadOption
	//! \param actionsThreadOption
	//!
	void exportActionsThreadOption(const std::string &actionsThreadOption) override;

	//!
	//! \brief exportPlace
	//! \param placeProperties
	//!
	void exportPlace(const PlaceProperties &placeProperties) override;

	//!
	//! \brief exportTransition
	//! \param transitionProperties
	//!
	void exportTransition(const TransitionProperties &transitionProperties) override;

	//!
	//! \brief exportArc
	//! \param arcPropereties
	//!
	void exportArc(const std::vector<ArcProperties> &arcPropereties) override;

	//!
	//! \brief saveFile
	//! \param filePath
	//!
	void saveFile() const;

	pugi::xml_node m_rootNode;
	pugi::xml_document m_document;
	pugi::xml_node m_placesNode;
	pugi::xml_node m_transitionsNode;
	pugi::xml_node m_arcsNode;
	std::string m_filePath;
};
} // namespace ptne
