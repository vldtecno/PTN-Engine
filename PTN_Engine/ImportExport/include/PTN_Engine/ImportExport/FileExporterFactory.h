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

class IFileExporter;

//!
//! \brief The FileExporterFactory class provides different types of file exporters.
//!
class DLL_PUBLIC FileExporterFactory
{
public:
	FileExporterFactory() = default;
	FileExporterFactory(const FileExporterFactory &) = delete;
	FileExporterFactory(FileExporterFactory &&) = delete;
	FileExporterFactory &operator=(const FileExporterFactory &) = delete;
	FileExporterFactory &operator=(FileExporterFactory &&) = delete;

	//!
	//! \brief createXMLFileExporter - creates a xml file exporter
	//! \return IFileExporter containing a XMLFileExporter
	//!
	static std::unique_ptr<IFileExporter> createXMLFileExporter();
};

} // namespace ptne
