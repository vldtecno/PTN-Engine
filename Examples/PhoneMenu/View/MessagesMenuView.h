/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2024 Eduardo Valgôde
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

#include "Model/List.h"
#include "Model/Message.h"

//! View of the messages menu.
class MessagesMenuView
{
public:
	using MessageList = List<Message>;

	MessagesMenuView(MessageList& messagesList);

	//!
	//! Show the message list on screen.
	//! \param messageList List of messages.
	//! \param selectedMessage Selected message from the list.
	//!
	void showMessagesList(const size_t selectedMessage) const;

	//!
	//! Print the selected message on screen.
	//! \param message Message to be displayed.
	//!
	void displayMessage(const Message &message) const;

private:
	MessageList &m_messageList;
};
