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

#include "View/MessagesMenuView.h"
#include <iostream>

using namespace std;

void MessagesMenuView::showMessagesList(const MessageList& messageList, const int selectedMessage)
{
	for(size_t i = 0 ; i < messageList.size(); ++i)
	{
		if(i == selectedMessage)
		{
			cout << "=>";
		}
		else
		{
			cout << "  ";
		}
		cout << i << " - " << messageList.getItem(i).m_title << endl;
	}
}

void MessagesMenuView::displayMessage(const Message& message)
{
	cout << "TITLE:" << endl;
	cout << message.m_title << endl<<endl;

	cout << "MESSAGE:" << endl;
	cout << message.m_body << endl<<endl;
}

