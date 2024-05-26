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

namespace ptne
{

struct ArcProperties;
class PTN_Engine;
struct PlaceProperties;
struct TransitionProperties;

//!
//! \brief The IFileExporter class is an interface for all PTN_Engine file exporters.
//!
class DLL_PUBLIC IFileExporter
{
public:
	virtual ~IFileExporter();

	//!
	//! \brief Export a PTN_Engine object to a file.
	//! \param ptnEngine - the PTN_Engine object that should be exported.
	//! \param filePath - the file path that will contain the exported PTN_Engine object.
	//!
	virtual void _export(const PTN_Engine &ptnEngine, const std::string &filePath) = 0;

protected:
	void _exportInt(const PTN_Engine &ptnEngine);

private:
	virtual void exportActionsThreadOption(const std::string &actionsThreadOption) = 0;

	virtual void exportPlace(const PlaceProperties &placeProperties) = 0;

	virtual void exportTransition(const TransitionProperties &transitionPropereties) = 0;
};

} // namespace ptne
