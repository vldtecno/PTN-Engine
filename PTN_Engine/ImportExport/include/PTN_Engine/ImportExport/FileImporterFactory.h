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
#include <memory>

namespace ptne
{

class IFileImporter;

//!
//! \brief The FileImporterFactory class provides different types of file importers.
//!
class DLL_PUBLIC FileImporterFactory
{
public:
	FileImporterFactory() = default;
	FileImporterFactory(const FileImporterFactory &) = delete;
	FileImporterFactory(FileImporterFactory &&) = delete;
	FileImporterFactory &operator=(const FileImporterFactory &) = delete;
	FileImporterFactory &operator=(FileImporterFactory &&) = delete;

	//!
	//! \brief createXMLFileImporter - creates a xml file importer
	//! \return IFileImporter containing a XMLFileImporter.
	//!
	static std::unique_ptr<IFileImporter> createXMLFileImporter();
};

} // namespace ptne
