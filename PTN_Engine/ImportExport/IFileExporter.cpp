/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2024 Eduardo Valgôde
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

#include "PTN_Engine/ImportExport/IFileExporter.h"
#include "PTN_Engine/ImportExport/ActionsThreadOptionConversions.h"
#include "PTN_Engine/PTN_Exception.h"

namespace ptne
{
using namespace std;

IFileExporter::~IFileExporter() = default;

void IFileExporter::_exportInt(const PTN_Engine &ptnEngine)
{
	if (ptnEngine.isEventLoopRunning())
	{
		throw PTN_Exception("Cannot change actions thread option while the event loop is running.");
	}

	exportActionsThreadOption(ActionsThreadOptionConversions::toString(ptnEngine.getActionsThreadOption()));

	for (const auto &placeProperties : ptnEngine.getPlacesProperties())
	{
		exportPlace(placeProperties);
	}

	for (const auto &transitionProperties : ptnEngine.getTransitionsProperties())
	{
		exportTransition(transitionProperties);
	}
}

} // namespace ptne
