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

#include "PTN_Engine/PTN_Engine.h"
#include "XML/XML_FileExporter.h"
#include <pugixml.hpp>
#include <vector>

namespace ptne
{
using namespace pugi;
using namespace std;

void XML_FileExporter::_export(const PTN_Engine &ptnEngine, const string& filePath)
{
	m_filePath = filePath;
	m_rootNode = m_document.append_child("PTN-Engine");
	m_rootNode.append_attribute("version").set_value("3.0");
	m_rootNode.append_attribute("format").set_value("1");
	m_rootNode.append_attribute("name").set_value("blah");
	m_placesNode = m_rootNode.append_child("Places");
	m_transitionsNode = m_rootNode.append_child("Transitions");
	m_arcsNode = m_rootNode.append_child("Arcs");
	IFileExporter::_export(ptnEngine);
	saveFile();
}

void XML_FileExporter::exportActionsThreadOption(const string &actionsThreadOption)
{
	m_rootNode.append_attribute("actionsThreadOption").set_value(actionsThreadOption.c_str());
}

void XML_FileExporter::exportPlace(const PlaceProperties &placeProperties)
{
	xml_node placeNode = m_placesNode.append_child("Place");
	placeNode.append_attribute("name").set_value(placeProperties.name.c_str());
	placeNode.append_attribute("tokens").set_value(placeProperties.initialNumberOfTokens);
	placeNode.append_attribute("input").set_value(placeProperties.input ? true : false);
	placeNode.append_attribute("onEnterAction").set_value(placeProperties.onEnterActionFunctionName.c_str());
	placeNode.append_attribute("onExitAction").set_value(placeProperties.onExitActionFunctionName.c_str());
}

void XML_FileExporter::exportTransition(const TransitionProperties &transitionProperties)
{
	xml_node transitionNode = m_transitionsNode.append_child("Transition");

	xml_node name = transitionNode.append_child("Name");
	name.append_attribute("value").set_value(transitionProperties.name.c_str());

	xml_node activationConditionsNode = transitionNode.append_child("ActivationConditions");
	for (const auto &activationCondition : transitionProperties.additionalConditionsNames)
	{
		xml_node activationConditionNode = activationConditionsNode.append_child("ActivationCondition");
		activationConditionNode.append_attribute("name").set_value(activationCondition.c_str());
	}

	xml_node requireNoActionsInExecution = transitionNode.append_child("RequireNoActionsInExecution");
	requireNoActionsInExecution.append_attribute("value").set_value(transitionProperties.requireNoActionsInExecution? "true" : "false");

	auto exportArcs = [this](const vector<ArcProperties> arcsProperties, const string &typeStr)
	{
		for (const auto &arcProperties : arcsProperties)
		{
			xml_node arcNode = m_arcsNode.append_child("Arc");
			xml_node place = arcNode.append_child("Place");
			place.append_attribute("value").set_value(arcProperties.placeName.c_str());

			xml_node transition = arcNode.append_child("Transition");
			transition.append_attribute("value").set_value(arcProperties.transitionName.c_str());

			xml_node weight = arcNode.append_child("Weight");
			weight.append_attribute("value").set_value(to_string(arcProperties.weight).c_str());

			xml_node type = arcNode.append_child("Type");
			type.append_attribute("value").set_value(typeStr.c_str());
		}
	};
	exportArcs(transitionProperties.activationArcs, "Activation");
	exportArcs(transitionProperties.destinationArcs, "Destination");
	exportArcs(transitionProperties.inhibitorArcs, "Inhibitor");
}

void XML_FileExporter::saveFile() const
{
	m_document.save_file(m_filePath.c_str());
}

} // namespace ptne
