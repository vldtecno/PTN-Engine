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

#include <PTN_Engine/IImporter.h>
#include <memory>
#include <pugixml.hpp>
#include <vector>

namespace ptne
{
class PTN_Engine;

/*!
 * \brief The XML_Importer class
 */
class XML_Importer : public IImporter
{
public:
	/*!
	 * \brief XML_Importer
	 * \param filePath
	 */
	explicit XML_Importer(const std::string &filePath);

	~XML_Importer() override;

	/*!
	 * \brief getActionsThreadOption
	 * \return
	 */
	std::string getActionsThreadOption() const override;

	/*!
	 * \brief getPlaces
	 * \return
	 */
	std::vector<PlaceInfo> getPlaces() const override;

	/*!
	 * \brief getTransitions
	 * \return
	 */
	std::vector<TransitionInfo> getTransitions() const override;

private:
	// void collectDocumentInformation(const pugi::xml_document &doc) const;

	/*!
	 * \brief getAttributeValue
	 * \param node
	 * \param attrName
	 * \return
	 */
	std::string getAttributeValue(const pugi::xml_node &node, const std::string &attrName) const;

	/*!
	 * \brief collectTransitionAttributes
	 * \param transition
	 * \param attribute
	 * \return
	 */
	std::vector<std::string>
	collectTransitionAttributes(const pugi::xml_node &transition, const std::string &attribute) const;

	/*!
	 * \brief collectArcAttributes
	 * \param transition
	 * \param attribute
	 * \return
	 */
	std::pair<std::vector<std::string>, std::vector<size_t>>
	collectArcAttributes(const pugi::xml_node &transition, const std::string &attribute) const;

	bool getRequireNoActionsInExecution(const pugi::xml_node &transition) const;

	/*!
	 * \brief m_document
	 */
	pugi::xml_document m_document;
};
} // namespace ptne
