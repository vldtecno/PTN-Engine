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

#pragma once

#include "Model/Call.h"
#include "Model/List.h"
#include "Model/Message.h"
#include "View/CallLogView.h"
#include "View/MainMenuView.h"
#include "View/MessagesMenuView.h"
#include "View/MainMenuView.h"
#include <memory>


//! Example of a controller class
/*!
 * Controls the display of a simplified hypothetical phone's menus,
 * using only three keys.
 */
class Controller: public std::enable_shared_from_this<Controller>
{
public:

	using MessageList = List<Message>;

	using CallList = List<Call>;

	//! Constructor.
	Controller();

	~Controller();

	//! Necessary to have an initialize method at this point.
	/*!
	 * Passes a shared pointer from "this" to the PTN Engine net.
	 * "shared_from_this" cannot be called in the constructor.
	 */
	void initialize();

	//Set pointers to data sources

	//! Set the message data (owned by the application).
	void setMessageList(std::shared_ptr<MessageList>);

	//! Set the call log data (owned by the application).
	void setCallLog(std::shared_ptr<CallList>);


	//Set pointers to view components

	//! Set the view of the main menu (owned by the application).
	void setMainMenuView(std::shared_ptr<MainMenuView>);

	//! Set the view of the messages (owned by the application).
	void setMessagesMenuView(std::shared_ptr<MessagesMenuView>);

	//! Set the view of the call logs (owned by the application).
	void setCallLogView(std::shared_ptr<CallLogView>);


	//Actions

	//! Triggers an event indicating key "A" was pressed.
	void pressA();

	//! Triggers an event indicating key "B" was pressed.
	void pressB();

	//! Triggers an event indicating key "C" was pressed.
	void pressC();

private:

	//! Base (nested) class for a Petri net based state machine.
	class IControllerPetriNet;

	//! One concrete (nested) class of a Petri net based state machine.
	class MenuStateMachine;


	using PtrPetriNet = std::unique_ptr<IControllerPetriNet>;

	//! The state machine of the controller.
	PtrPetriNet m_pPetriNet;

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


	//Views

	//! A view of the main menu (owned by the application).
	std::weak_ptr<MainMenuView> m_mainMenu;

	//! A view of the messages (owned by the application).
	std::weak_ptr<MessagesMenuView> m_messagesMenu;

	//! A view of the call logs (owned by the application).
	std::weak_ptr<CallLogView> m_callLogView;


	//Data sources

	//! The message data (owned by the application).
	std::weak_ptr<MessageList> m_messageList;

	//! The call log data (owned by the application).
	std::weak_ptr<CallList> m_callLog;


	//Internals

	//! Index of the selected message.
	size_t m_messageSelected;
};


#include "../../../Examples/PhoneMenu/Controller/MenuStateMachine.h"
