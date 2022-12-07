/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2019-2023 Eduardo Valgôde
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

#include "XML/XML_Exporter.h"
#include <PTN_Engine/ImportExport/ExportFactory.h>

namespace ptne
{
using namespace std;

unique_ptr<IExporter> ExportFactory::createXMLExporter()
{
    return make_unique<XML_Exporter>();
}

} // namespace ptne
