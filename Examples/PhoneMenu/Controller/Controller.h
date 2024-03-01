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

#include "Controller/IMenuStateMachine.h"
#include "Model/Call.h"
#include "Model/List.h"
#include "Model/Message.h"
#include "View/CallLogView.h"
#include "View/MainMenuView.h"
#include "View/MessagesMenuView.h"
#include <memory>

//!
//! \brief Example of a controller class
//!
//! \details Controls the display of a simplified hypothetical phone's menus, using only three keys.
//!
class Controller
{
public:
	using MessageList = List<Message>;
	using CallList = List<Call>;

	Controller(MainMenuView &mainMenuView,
			   MessagesMenuView &messagesMenu,
			   CallLogView &callLogView,
			   MessageList &m_messageList,
			   CallList &m_callLog);

	~Controller();

	void execute();

	void stop();

	void setStateMachine(std::shared_ptr<IMenuStateMachine> stateMachine);

	// Actions

	//! Triggers an event indicating key "A" was pressed.
	void pressA();

	//! Triggers an event indicating key "B" was pressed.
	void pressB();

	//! Triggers an event indicating key "C" was pressed.
	void pressC();

	//! Show the main menu on screen.
	void showMainMenu() const;

	//! Select the calls option.
	void selectCallsOption();

	//! Select the messages option.
	void selectMessagesOption();

	//! Show the calls menu or log.
	void showCallsMenu() const;

	//! Show the messages menu.
	void showMessageMenu() const;

	//! Select the next message.
	void selectNextMessage();

	//! Show the selected message.
	void showMessage() const;

	//! Dummy function for showing in import export from files
	bool justReturnTrue() const;

private:
	//! The state machine of the controller.
	std::shared_ptr<IMenuStateMachine> m_menuStateMachine = nullptr;

	// Views

	//! A view of the main menu.
	MainMenuView &m_mainMenuView;

	//! A view of the messages.
	MessagesMenuView &m_messagesMenuView;

	//! A view of the call logs.
	CallLogView &m_callLogView;

	// Data sources

	//! The message data.
	MessageList &m_messageList;

	//! The call log data.
	CallList &m_callLog;

	// Internals

	//! Index of the selected message.
	size_t m_messageSelected = 0;
};
