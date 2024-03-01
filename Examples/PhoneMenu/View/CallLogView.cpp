/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
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

#include "View/CallLogView.h"
#include <iostream>

using namespace std;

CallLogView::CallLogView(List<Call> &callLog)
: m_callLog(callLog)
{
}

void CallLogView::viewCallLog() const
{
	for (size_t i = 0; i < m_callLog.size(); ++i)
	{
		const Call &call = m_callLog.getItem(i);
		cout << call.m_date << " - " << call.m_number << endl;
	}
}
