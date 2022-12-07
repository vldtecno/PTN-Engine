/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2023 Eduardo Valgôde
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

#include "Model/Call.h"
#include "Model/List.h"
#include "Model/Message.h"
#include "PTN_Engine/PTN_Engine.h"
#include "View/CallLogView.h"
#include "View/MainMenuView.h"
#include "View/MessagesMenuView.h"

//! Example of a controller class
/*!
 * Controls the display of a simplified hypothetical phone's menus,
 * using only three keys.
 */
class Controller
{
public:
	using MessageList = List<Message>;

	using CallList = List<Call>;

	Controller();

	~Controller();

	void execute();

	void stop();

	// Actions

	//! Triggers an event indicating key "A" was pressed.
	void pressA();

	//! Triggers an event indicating key "B" was pressed.
	void pressB();

	//! Triggers an event indicating key "C" was pressed.
	void pressC();

	MessageList &messageList();

	CallList &callLog();

	void exportStateMachine(const std::string &filePath) const;

	void importStateMachine(const std::string &filePath);

private:
	//! The state machine of the controller.
	ptne::PTN_Engine m_petriNet;

	//! Show the main menu on screen.
	void showMainMenu();

	//! Select the calls option.
	void selectCallsOption();

	//! Select the messages option.
	void selectMessagesOption();

	//! Show the calls menu or log.
	void showCallsMenu();

	//! Show the messages menu.
	void showMessageMenu();

	//! Select the next message.
	void selectNextMessage();

	//! Show the selected message.
	void showMessage();

	//! Dummy function for showing in import export from files
	bool justReturnTrue();

	// Views

	//! A view of the main menu.
	MainMenuView m_mainMenu;

	//! A view of the messages.
	MessagesMenuView m_messagesMenu;

	//! A view of the call logs.
	CallLogView m_callLogView;

	// Data sources

	//! The message data.
	MessageList m_messageList;

	//! The call log data.
	CallList m_callLog;

	// Internals

	//! Index of the selected message.
	size_t m_messageSelected;
};
