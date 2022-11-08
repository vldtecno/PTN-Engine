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

#include <PTN_Engine/IExporter.h>
#include <memory>
#include <pugixml.hpp>
#include <vector>

namespace ptne
{

/*!
 * \brief The XML_Exporter class
 */
class XML_Exporter : public IExporter
{
public:
	XML_Exporter();

	~XML_Exporter();

	/*!
	 * \brief exportPlace
	 * \param name
	 * \param tokens
	 * \param isInput
	 * \param onEnterAction
	 * \param onExitAction
	 */
	void exportPlace(const std::string &name,
					 const std::string &tokens,
					 const std::string &isInput,
					 const std::string &onEnterAction,
					 const std::string &onExitAction);

	/*!
	 * \brief exportTransition
	 * \param activationPlaces
	 * \param destinationPlaces
	 * \param activationConditions
	 * \param inhibitorPlaces
	 */
	void exportTransition(const std::vector<std::tuple<std::string, size_t>> &activationPlaces,
						  const std::vector<std::tuple<std::string, size_t>> &destinationPlaces,
						  const std::vector<std::string> &activationConditions,
						  const std::vector<std::string> &inhibitorPlaces);

	/*!
	 * \brief saveFile
	 * \param filePath
	 */
	void saveFile(const std::string &filePath) const;

private:
	pugi::xml_node m_rootNode;
	pugi::xml_document m_document;
	pugi::xml_node m_placesNode;
	pugi::xml_node m_transitionsNode;
};
} // namespace ptne
