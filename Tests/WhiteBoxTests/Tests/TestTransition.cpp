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

#include "PTN_Engine/Executor/ActionsExecutorFactory.h"
#include "PTN_Engine/PTN_EngineImp.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include <gtest/gtest.h>


using namespace std;
using namespace ptne;

class Transition_ExecutorObj : public testing::Test
{
public:
	shared_ptr<IActionsExecutor> executor =
	ActionsExecutorFactory::createExecutor(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);
};

class Transition_PTNEngineAndPlace : public Transition_ExecutorObj
{
public:
	void SetUp() override
	{
		Transition_ExecutorObj::SetUp();
		p1 = make_shared<Place>(PlaceProperties{}, executor);
	}
	SharedPtrPlace p1 = nullptr;
};

TEST_F(Transition_PTNEngineAndPlace, constructor_with_invalid_pointer_to_place_throws)
{
	WeakPtrPlace wP1(p1);
	p1.reset();
	ASSERT_THROW(Transition t("", {}, {}, { { wP1, 0 } }, {}, false), PTN_Exception);
}

TEST_F(Transition_PTNEngineAndPlace, constructor_with_repeated_input_arcs_throws)
{
	WeakPtrPlace wP1(p1);
	p1.reset();
	ASSERT_THROW(Transition t("", { { wP1, 0 }, { wP1, 0 } }, {}, {}, {}, false),
				 ActivationPlaceRepetitionException);
}

TEST_F(Transition_PTNEngineAndPlace, constructor_with_repeated_destination_arcs_throws)
{
	WeakPtrPlace wP1(p1);
	p1.reset();
	ASSERT_THROW(Transition t("", {}, { { wP1, 0 }, { wP1, 0 } }, {}, {}, false),
				 DestinationPlaceRepetitionException);
}

TEST_F(Transition_PTNEngineAndPlace, constructor_with_repeated_inhibitor_arcs_throws)
{
	WeakPtrPlace wP1(p1);
	p1.reset();
	ASSERT_THROW(Transition t("", {}, {}, { { wP1, 0 }, { wP1, 0 } }, {}, false),
				 InhibitorPlaceRepetitionException);
}

TEST(Transition_, getTransitionProperties_empty_transition_properties_is_exported)
{
	Transition t("", {}, {}, {}, {}, false);
	auto transitionProperties = t.getTransitionProperties();
	EXPECT_TRUE(transitionProperties.activationArcs.empty());
	EXPECT_TRUE(transitionProperties.destinationArcs.empty());
	EXPECT_TRUE(transitionProperties.inhibitorArcs.empty());
	EXPECT_EQ("", transitionProperties.name);
	EXPECT_TRUE(transitionProperties.additionalConditions.empty());
	EXPECT_TRUE(transitionProperties.additionalConditionsNames.empty());
	EXPECT_FALSE(transitionProperties.requireNoActionsInExecution);
}

TEST(Transition_, getTransitionProperties_requiresNoActionsInExecution_property_is_exported)
{
	Transition t("", {}, {}, {}, {}, true);
	auto transitionProperties = t.getTransitionProperties();
	EXPECT_TRUE(transitionProperties.activationArcs.empty());
	EXPECT_TRUE(transitionProperties.destinationArcs.empty());
	EXPECT_TRUE(transitionProperties.inhibitorArcs.empty());
	EXPECT_EQ("", transitionProperties.name);
	EXPECT_TRUE(transitionProperties.additionalConditions.empty());
	EXPECT_TRUE(transitionProperties.additionalConditionsNames.empty());
	EXPECT_TRUE(transitionProperties.requireNoActionsInExecution);
}

TEST(Transition_, getTransitionProperties_name_property_is_exported)
{
	Transition t("Abc asb", {}, {}, {}, {}, true);
	auto transitionProperties = t.getTransitionProperties();
	EXPECT_TRUE(transitionProperties.activationArcs.empty());
	EXPECT_TRUE(transitionProperties.destinationArcs.empty());
	EXPECT_TRUE(transitionProperties.inhibitorArcs.empty());
	EXPECT_EQ("Abc asb", transitionProperties.name);
	EXPECT_TRUE(transitionProperties.additionalConditions.empty());
	EXPECT_TRUE(transitionProperties.additionalConditionsNames.empty());
	EXPECT_TRUE(transitionProperties.requireNoActionsInExecution);
}

TEST_F(Transition_PTNEngineAndPlace, getTransitionProperties_activations_arc_are_exported)
{
	WeakPtrPlace wP1(p1);

	Transition t("Abc asb", { Arc{ .place = wP1 } }, {}, {}, {}, true);
	auto transitionProperties = t.getTransitionProperties();

	EXPECT_EQ(1, transitionProperties.activationArcs.size());
	EXPECT_TRUE(transitionProperties.destinationArcs.empty());
	EXPECT_TRUE(transitionProperties.inhibitorArcs.empty());
	EXPECT_EQ("Abc asb", transitionProperties.name);
	EXPECT_TRUE(transitionProperties.additionalConditions.empty());
	EXPECT_TRUE(transitionProperties.additionalConditionsNames.empty());
	EXPECT_TRUE(transitionProperties.requireNoActionsInExecution);
}

TEST_F(Transition_PTNEngineAndPlace, getTransitionProperties_all_arcs_are_exported)
{
	WeakPtrPlace wP1(p1);

	ConditionFunction cf = []() { return true; };

	Transition t("some string", { { .place = wP1 } }, { { .place = wP1 } }, { { .place = wP1 } },
				 { { "asd", cf } }, true);
	auto transitionProperties = t.getTransitionProperties();

	EXPECT_EQ(1, transitionProperties.activationArcs.size());
	EXPECT_EQ(1, transitionProperties.destinationArcs.size());
	EXPECT_EQ(1, transitionProperties.inhibitorArcs.size());
	EXPECT_EQ("some string", transitionProperties.name);
	EXPECT_EQ(1, transitionProperties.additionalConditions.size());
	EXPECT_EQ(1, transitionProperties.additionalConditionsNames.size());
	EXPECT_TRUE(transitionProperties.requireNoActionsInExecution);
}

TEST_F(Transition_PTNEngineAndPlace, addArc_add_activation_arc_to_empty_transition_does_not_throw)
{
	Transition t("", {}, {}, {}, {}, false);
	EXPECT_NO_THROW(t.addArc(p1, ArcProperties::Type::ACTIVATION, 1));
	EXPECT_EQ(1, t.getActivationArcs().size());
}

TEST_F(Transition_PTNEngineAndPlace, addArc_add_destination_arc_to_empty_transition_does_not_throw)
{
	Transition t("", {}, {}, {}, {}, false);
	EXPECT_NO_THROW(t.addArc(p1, ArcProperties::Type::DESTINATION, 1));
	EXPECT_EQ(1, t.getDestinationArcs().size());
}

TEST_F(Transition_PTNEngineAndPlace, addArc_add_inhibitor_arc_to_empty_transition_does_not_throw)
{
	Transition t("", {}, {}, {}, {}, false);
	EXPECT_NO_THROW(t.addArc(p1, ArcProperties::Type::INHIBITOR, 1));
	EXPECT_EQ(1, t.getInhibitorArcs().size());
}

TEST_F(Transition_PTNEngineAndPlace, addArc_add_repeated_activation_arc_to_transition_does_not_duplicate_arcs)
{
	Transition t("", {}, {}, {}, {}, false);
	EXPECT_NO_THROW(t.addArc(p1, ArcProperties::Type::ACTIVATION, 1));
	EXPECT_THROW(t.addArc(p1, ArcProperties::Type::ACTIVATION, 1), PTN_Exception);
}

TEST_F(Transition_PTNEngineAndPlace, addArc_add_bidirectional_arc_to_transition_adds_two_arcs)
{
	Transition t("", {}, {}, {}, {}, false);
	EXPECT_NO_THROW(t.addArc(p1, ArcProperties::Type::BIDIRECTIONAL, 1));

	EXPECT_EQ(1, t.getActivationArcs().size());
	EXPECT_EQ(1, t.getDestinationArcs().size());
}

TEST_F(Transition_PTNEngineAndPlace, execute_empty_transition_does_nothing)
{
	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 1);
	ASSERT_EQ(0, p1->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(0, p1->getNumberOfTokens());
}

TEST_F(Transition_PTNEngineAndPlace, execute_with_invalid_pointer_to_place_throws)
{
	WeakPtrPlace wP1(p1);
	Transition t("", {}, {}, { { wP1, 1 } }, {}, false);
	p1.reset();
	ASSERT_THROW(t.execute(), ExpiredSharedPtrException);
}

/*
 *  ___         ||
 * | 1 |___1___\||
 * |___|       /||
 *              ||
 */
TEST_F(Transition_PTNEngineAndPlace, execute_simple_transit_no_destination)
{
	p1->setNumberOfTokens(1);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 1);
	ASSERT_EQ(1, p1->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(0, p1->getNumberOfTokens());
}

/*
 *  ___         ||         ___
 * | 1 |___1___\||___1___\| 0 |
 * |___|       /||       /|___|
 *              ||
 */
TEST_F(Transition_ExecutorObj, execute_simple_transit_one_destination_place)
{
	SharedPtrPlace p1 = make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 1 }, executor);
	SharedPtrPlace p2 = make_shared<Place>(PlaceProperties{ .name = "P2" }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 1);
	t.addArc(p2, ArcProperties::Type::DESTINATION, 1);
	ASSERT_EQ(1, p1->getNumberOfTokens());
	ASSERT_EQ(0, p2->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(0, p1->getNumberOfTokens());
	EXPECT_EQ(1, p2->getNumberOfTokens());
}

/*
 *  ___         ||
 * | 1 |___1___\||
 * |___|       /||
 *              ||
 *  ___         ||
 * | 1 |___1___\||
 * |___|       /||
 *              ||
 */
TEST_F(Transition_ExecutorObj, execute_two_activation_places_no_destination)
{
	SharedPtrPlace p1 = make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 1 }, executor);
	SharedPtrPlace p2 = make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 1 }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 1);
	t.addArc(p2, ArcProperties::Type::ACTIVATION, 1);
	ASSERT_EQ(1, p1->getNumberOfTokens());
	ASSERT_EQ(1, p2->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(0, p1->getNumberOfTokens());
	EXPECT_EQ(0, p2->getNumberOfTokens());
}

/*
 *  ___         ||
 * | 1 |___1___\||
 * |___|       /||         ___
 *              ||___1___\| 0 |
 *  ___         ||       /|___|
 * | 1 |___1___\||
 * |___|       /||
 *              ||
 */
TEST_F(Transition_ExecutorObj, execute_two_activation_places_one_destination_place)
{
	SharedPtrPlace p1 = make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 1 }, executor);
	SharedPtrPlace p2 = make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 1 }, executor);
	SharedPtrPlace p3 = make_shared<Place>(PlaceProperties{ .name = "P3" }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 1);
	t.addArc(p2, ArcProperties::Type::ACTIVATION, 1);
	t.addArc(p3, ArcProperties::Type::DESTINATION, 1);
	ASSERT_EQ(1, p1->getNumberOfTokens());
	ASSERT_EQ(1, p2->getNumberOfTokens());
	ASSERT_EQ(0, p3->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(0, p1->getNumberOfTokens());
	EXPECT_EQ(0, p2->getNumberOfTokens());
	EXPECT_EQ(1, p3->getNumberOfTokens());
}

/*
 *  ___         ||         ___
 * | 1 |___1___\||___1___\| 0 |
 * |___|       /||       /|___|
 *              ||
 *  ___         ||         ___
 * | 1 |___1___\||___1___\| 0 |
 * |___|       /||       /|___|
 *              ||
 */
TEST_F(Transition_ExecutorObj, execute_two_activation_places_two_destination_place)
{
	SharedPtrPlace p1 = make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 1 }, executor);
	SharedPtrPlace p2 = make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 1 }, executor);
	SharedPtrPlace p3 = make_shared<Place>(PlaceProperties{ .name = "P3" }, executor);
	SharedPtrPlace p4 = make_shared<Place>(PlaceProperties{ .name = "P4" }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 1);
	t.addArc(p2, ArcProperties::Type::ACTIVATION, 1);
	t.addArc(p3, ArcProperties::Type::DESTINATION, 1);
	t.addArc(p4, ArcProperties::Type::DESTINATION, 1);
	ASSERT_EQ(1, p1->getNumberOfTokens());
	ASSERT_EQ(1, p2->getNumberOfTokens());
	ASSERT_EQ(0, p3->getNumberOfTokens());
	ASSERT_EQ(0, p4->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(0, p1->getNumberOfTokens());
	EXPECT_EQ(0, p2->getNumberOfTokens());
	EXPECT_EQ(1, p3->getNumberOfTokens());
	EXPECT_EQ(1, p4->getNumberOfTokens());
}

/*
 *  ___         ||         ___
 * | 1 |___1___\||___1___\| 0 |
 * |___|       /||       /|___|
 *              ||
 *              ||         ___
 *              ||___1___\| 0 |
 *              ||       /|___|
 *              ||
 */
TEST_F(Transition_ExecutorObj, execute_one_activation_places_two_destination_place)
{
	SharedPtrPlace p1 = make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 1 }, executor);
	SharedPtrPlace p2 = make_shared<Place>(PlaceProperties{ .name = "P3" }, executor);
	SharedPtrPlace p3 = make_shared<Place>(PlaceProperties{ .name = "P4" }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 1);
	t.addArc(p2, ArcProperties::Type::DESTINATION, 1);
	t.addArc(p3, ArcProperties::Type::DESTINATION, 1);
	ASSERT_EQ(1, p1->getNumberOfTokens());
	ASSERT_EQ(0, p2->getNumberOfTokens());
	ASSERT_EQ(0, p3->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(0, p1->getNumberOfTokens());
	EXPECT_EQ(1, p2->getNumberOfTokens());
	EXPECT_EQ(1, p3->getNumberOfTokens());
}

/*
 *  ___         ||         ___
 * | 1 |___2___\||___1___\| 0 |
 * |___|       /||       /|___|
 *              ||
 *  ___         ||         ___
 * | 1 |___1___\||___3___\| 0 |
 * |___|       /||       /|___|
 *              ||
 */
TEST_F(Transition_ExecutorObj, execute_two_activation_places_two_destination_place_arcs_with_custom_weights)
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 1, .input = true }, executor);
	SharedPtrPlace p2 = make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 1 }, executor);
	SharedPtrPlace p3 = make_shared<Place>(PlaceProperties{ .name = "P3" }, executor);
	SharedPtrPlace p4 = make_shared<Place>(PlaceProperties{ .name = "P4" }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 2);
	t.addArc(p2, ArcProperties::Type::ACTIVATION, 1);
	t.addArc(p3, ArcProperties::Type::DESTINATION, 1);
	t.addArc(p4, ArcProperties::Type::DESTINATION, 3);
	ASSERT_EQ(1, p1->getNumberOfTokens());
	ASSERT_EQ(1, p2->getNumberOfTokens());
	ASSERT_EQ(0, p3->getNumberOfTokens());
	ASSERT_EQ(0, p4->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(1, p1->getNumberOfTokens());
	EXPECT_EQ(1, p2->getNumberOfTokens());
	EXPECT_EQ(0, p3->getNumberOfTokens());
	EXPECT_EQ(0, p4->getNumberOfTokens());
	p1->setNumberOfTokens(2);
	t.execute();
	EXPECT_EQ(0, p1->getNumberOfTokens());
	EXPECT_EQ(0, p2->getNumberOfTokens());
	EXPECT_EQ(1, p3->getNumberOfTokens());
	EXPECT_EQ(3, p4->getNumberOfTokens());
}

/*
 *  ___          ||
 * | 1 |___1____\||
 * |___|        /||         ___
 *               ||___1___\| 0 |
 *  ___          ||       /|___|
 * | 1 |___1___ o||
 * |___|         ||
 *               ||
 */
TEST_F(Transition_ExecutorObj, execute_one_activation_place_one_inhibitor_arc_one_destination_place)
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 1, .input = true }, executor);
	SharedPtrPlace p2 = make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 1 }, executor);
	SharedPtrPlace p3 = make_shared<Place>(PlaceProperties{ .name = "P3" }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 1);
	t.addArc(p2, ArcProperties::Type::INHIBITOR, 1);
	t.addArc(p3, ArcProperties::Type::DESTINATION, 1);
	ASSERT_EQ(1, p1->getNumberOfTokens());
	ASSERT_EQ(1, p2->getNumberOfTokens());
	ASSERT_EQ(0, p3->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(1, p1->getNumberOfTokens());
	EXPECT_EQ(1, p2->getNumberOfTokens());
	EXPECT_EQ(0, p3->getNumberOfTokens());
}

/*
 *  ___          ||
 * | 5 |___5____\||
 * |___|        /||         ___
 *               ||___1___\| 0 |
 *  ___          ||       /|___|
 * | 1 |___2___ o||
 * |___|         ||
 *               ||
 */
TEST_F(Transition_ExecutorObj,
       execute_one_activation_place_one_inhibitor_arc_one_destination_place_with_custom_weights)
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 5, .input = true }, executor);
	SharedPtrPlace p2 =
	make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 1, .input = true }, executor);
	SharedPtrPlace p3 = make_shared<Place>(PlaceProperties{ .name = "P3" }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 5);
	t.addArc(p2, ArcProperties::Type::INHIBITOR, 2);
	t.addArc(p3, ArcProperties::Type::DESTINATION, 1);
	ASSERT_EQ(5, p1->getNumberOfTokens());
	ASSERT_EQ(1, p2->getNumberOfTokens());
	ASSERT_EQ(0, p3->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(5, p1->getNumberOfTokens());
	EXPECT_EQ(1, p2->getNumberOfTokens());
	EXPECT_EQ(0, p3->getNumberOfTokens());
	p2->setNumberOfTokens(0);
	EXPECT_EQ(5, p1->getNumberOfTokens());
	EXPECT_EQ(0, p2->getNumberOfTokens());
	EXPECT_EQ(0, p3->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(0, p1->getNumberOfTokens());
	EXPECT_EQ(0, p2->getNumberOfTokens());
	EXPECT_EQ(1, p3->getNumberOfTokens());
}

/*
 *  ___         ||
 * | 5 |___3___\||
 * |___|       /||         ___
 *              ||___1___\| 0 |
 *  ___         ||       /|___|
 * | 3 |___2___\||
 * |___|       /||
 *              ||
 * condition __\||
 * function1   /||
 *              ||
 * condition __\||
 * function2   /||
 */
TEST_F(Transition_ExecutorObj, execute_only_fires_if_all_condition_functions_return_true)
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 5, .input = true }, executor);
	SharedPtrPlace p2 =
	make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 3, .input = true }, executor);
	SharedPtrPlace p3 = make_shared<Place>(PlaceProperties{ .name = "P3" }, executor);

	bool conditionFunction1ReturnVal = false;
	ptne::ConditionFunction conditionFunction1 = [&conditionFunction1ReturnVal]()
	{ return conditionFunction1ReturnVal; };

	bool conditionFunction2ReturnVal = false;
	ptne::ConditionFunction conditionFunction2 = [&conditionFunction2ReturnVal]()
	{ return conditionFunction2ReturnVal; };

	Transition t("", { { p1, 3 }, { p2, 2 } }, { { p3, 1 } }, {},
				 { { "", conditionFunction1 }, { "", conditionFunction2 } }, false);
	ASSERT_EQ(5, p1->getNumberOfTokens());
	ASSERT_EQ(3, p2->getNumberOfTokens());
	ASSERT_EQ(0, p3->getNumberOfTokens());
	t.execute();
	EXPECT_EQ(5, p1->getNumberOfTokens());
	EXPECT_EQ(3, p2->getNumberOfTokens());
	EXPECT_EQ(0, p3->getNumberOfTokens());
	conditionFunction1ReturnVal = true;
	t.execute();
	EXPECT_EQ(5, p1->getNumberOfTokens());
	EXPECT_EQ(3, p2->getNumberOfTokens());
	EXPECT_EQ(0, p3->getNumberOfTokens());
	conditionFunction2ReturnVal = true;
	t.execute();
	EXPECT_EQ(2, p1->getNumberOfTokens());
	EXPECT_EQ(1, p2->getNumberOfTokens());
	EXPECT_EQ(1, p3->getNumberOfTokens());
}


/*
 *  ___          ||
 * | 5 |___5____\||
 * |___|        /||         ___
 *               ||___1___\| 1 |
 *  ___          ||       /|   |
 * | 1 |___2___ \||        |   |
 * |___|        /||o___2___|   |
 *               ||        |___|
 */
TEST_F(Transition_ExecutorObj, getActivationArcs_returns_activation_arcs)
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 5, .input = true }, executor);
	SharedPtrPlace p2 =
	make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 1, .input = true }, executor);
	SharedPtrPlace p3 = make_shared<Place>(PlaceProperties{ .name = "P3" }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 5);
	t.addArc(p2, ArcProperties::Type::ACTIVATION, 2);
	t.addArc(p3, ArcProperties::Type::DESTINATION, 1);
	t.addArc(p3, ArcProperties::Type::INHIBITOR, 2);
	ASSERT_EQ(5, p1->getNumberOfTokens());
	ASSERT_EQ(1, p2->getNumberOfTokens());
	ASSERT_EQ(0, p3->getNumberOfTokens());

	auto activationArcs = t.getActivationArcs();
	EXPECT_EQ(2, activationArcs.size());
	EXPECT_EQ(p1, activationArcs[0].place.lock());
	EXPECT_EQ(p2, activationArcs[1].place.lock());
	EXPECT_EQ(5, activationArcs[0].weight);
	EXPECT_EQ(2, activationArcs[1].weight);
}

TEST(Transition_, get_additional_act_conditions_returns_additional_act_conditions)
{
	bool conditionFunction1ReturnVal = false;
	ptne::ConditionFunction conditionFunction1 = [&conditionFunction1ReturnVal]()
	{ return conditionFunction1ReturnVal; };

	bool conditionFunction2ReturnVal = false;
	ptne::ConditionFunction conditionFunction2 = [&conditionFunction2ReturnVal]()
	{ return conditionFunction2ReturnVal; };

	Transition t("", {}, {}, {}, { { "AC1", conditionFunction1 }, { "", conditionFunction2 } }, false);

	auto additionalActivationConditions = t.getAdditionalActivationConditions();

	EXPECT_EQ(2, additionalActivationConditions.size());
	EXPECT_EQ("AC1", additionalActivationConditions.at(0).first);
	EXPECT_FALSE(additionalActivationConditions.at(0).second());
	conditionFunction1ReturnVal = true;
	EXPECT_TRUE(additionalActivationConditions.at(0).second());

	EXPECT_EQ("", additionalActivationConditions.at(1).first);
	EXPECT_FALSE(additionalActivationConditions.at(1).second());
	conditionFunction2ReturnVal = true;
	EXPECT_TRUE(additionalActivationConditions.at(1).second());
}

TEST_F(Transition_ExecutorObj, getDestinationArcs_returns_destination_arcs)
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "", .initialNumberOfTokens = 5, .input = true }, executor);

	SharedPtrPlace p2 =
	make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 5, .input = true }, executor);

	Transition t1("", {}, {}, {}, {}, false);
	auto destinationArcs = t1.getDestinationArcs();
	EXPECT_EQ(0, destinationArcs.size());

	Transition t2("", {}, { { p1, 2 }, { p2 } }, {}, {}, false);
	destinationArcs = t2.getDestinationArcs();
	EXPECT_EQ(2, destinationArcs.size());

	EXPECT_EQ(2, destinationArcs.at(0).weight);
	EXPECT_EQ(p1, destinationArcs.at(0).place.lock());

	EXPECT_EQ(1, destinationArcs.at(1).weight);
	EXPECT_EQ(p2, destinationArcs.at(1).place.lock());
}

TEST_F(Transition_ExecutorObj, getInhibitorArcs_returns_inhibitor_arcs)
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "", .initialNumberOfTokens = 5, .input = true }, executor);

	SharedPtrPlace p2 =
	make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 5, .input = true }, executor);

	Transition t1("", {}, {}, {}, {}, false);
	auto inhibitorArcs = t1.getInhibitorArcs();
	EXPECT_EQ(0, inhibitorArcs.size());

	Transition t2("", {}, {}, { { p1, 2 }, { p2 } }, {}, false);
	inhibitorArcs = t2.getInhibitorArcs();
	EXPECT_EQ(2, inhibitorArcs.size());

	EXPECT_EQ(2, inhibitorArcs.at(0).weight);
	EXPECT_EQ(p1, inhibitorArcs.at(0).place.lock());

	EXPECT_EQ(1, inhibitorArcs.at(1).weight);
	EXPECT_EQ(p2, inhibitorArcs.at(1).place.lock());
}

TEST(Transition_, getName_returns_name)
{
	Transition t1("", {}, {}, {}, {}, false);
	EXPECT_EQ("", t1.getName());

	Transition t2("AB", {}, {}, {}, {}, false);
	EXPECT_EQ("AB", t2.getName());
}

TEST_F(Transition_ExecutorObj,
	   isEnabled_conditions_depending_on_input_tokens) // TO DO - split up in different tests
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 0, .input = true }, executor);
	SharedPtrPlace p2 =
	make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 1, .input = true }, executor);
	SharedPtrPlace p3 =
	make_shared<Place>(PlaceProperties{ .name = "P3", .initialNumberOfTokens = 1, .input = true }, executor);

	Transition t("", {}, {}, {}, {}, false);
	t.addArc(p1, ArcProperties::Type::ACTIVATION, 5);
	ASSERT_EQ(0, p1->getNumberOfTokens());
	ASSERT_EQ(1, p2->getNumberOfTokens());
	EXPECT_FALSE(t.isEnabled());
	p1->setNumberOfTokens(2);
	EXPECT_FALSE(t.isEnabled());
	p1->setNumberOfTokens(5);
	EXPECT_TRUE(t.isEnabled());
	t.addArc(p2, ArcProperties::Type::INHIBITOR, 2);
	EXPECT_FALSE(t.isEnabled());
	p2->setNumberOfTokens(0);
	EXPECT_TRUE(t.isEnabled());
	t.addArc(p3, ArcProperties::Type::ACTIVATION, 2);
	EXPECT_FALSE(t.isEnabled());
	p3->setNumberOfTokens(3);
	EXPECT_TRUE(t.isEnabled());
}

/*
 *  ___         ||         ___
 * | 5 |___3___\||___1___\| 0 |
 * |___|       /||       /|___|
 *              ||
 *  ___         ||
 * | 3 |___2___\||
 * |___|       /||
 *              ||
 *  ___         ||
 * | 0 |___1___o||
 * |___|        ||
 *              ||
 * condition___\||
 * function1   /||
 *              ||
 *              ||
 * condition___\||
 * function2   /||
 *              ||
 */
TEST_F(Transition_ExecutorObj, isEnabled_is_true_but_only_fires_when_all_conditions_are_true)
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "P1", .initialNumberOfTokens = 5, .input = true }, executor);
	SharedPtrPlace p2 =
	make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 3, .input = true }, executor);
	SharedPtrPlace p3 =
	make_shared<Place>(PlaceProperties{ .name = "P3", .initialNumberOfTokens = 0, .input = true }, executor);
	SharedPtrPlace p4 =
	make_shared<Place>(PlaceProperties{ .name = "P4", .initialNumberOfTokens = 0, .input = true }, executor);


	bool conditionFunction1ReturnVal = false;
	ptne::ConditionFunction conditionFunction1 = [&conditionFunction1ReturnVal]()
	{ return conditionFunction1ReturnVal; };

	bool conditionFunction2ReturnVal = false;
	ptne::ConditionFunction conditionFunction2 = [&conditionFunction2ReturnVal]()
	{ return conditionFunction2ReturnVal; };

	Transition t("", { { p1, 3 }, { p2, 2 } }, { { p4, 1 } }, { { p3, 1 } },
				 { { "", conditionFunction1 }, { "", conditionFunction2 } }, false);

	EXPECT_TRUE(t.isEnabled());
	t.execute();
	EXPECT_EQ(5, p1->getNumberOfTokens());
	EXPECT_EQ(3, p2->getNumberOfTokens());
	EXPECT_EQ(0, p3->getNumberOfTokens());
	EXPECT_EQ(0, p4->getNumberOfTokens());

	conditionFunction1ReturnVal = true;
	EXPECT_TRUE(t.isEnabled());
	t.execute();
	EXPECT_EQ(5, p1->getNumberOfTokens());
	EXPECT_EQ(3, p2->getNumberOfTokens());
	EXPECT_EQ(0, p3->getNumberOfTokens());
	EXPECT_EQ(0, p4->getNumberOfTokens());

	conditionFunction2ReturnVal = true;
	EXPECT_TRUE(t.isEnabled());
	t.execute();
	EXPECT_EQ(2, p1->getNumberOfTokens());
	EXPECT_EQ(1, p2->getNumberOfTokens());
	EXPECT_EQ(0, p3->getNumberOfTokens());
	EXPECT_EQ(1, p4->getNumberOfTokens());
}

TEST_F(Transition_ExecutorObj, removeArc_removes_arc)
{
	SharedPtrPlace p1 =
	make_shared<Place>(PlaceProperties{ .name = "", .initialNumberOfTokens = 5, .input = true }, executor);

	SharedPtrPlace p2 =
	make_shared<Place>(PlaceProperties{ .name = "P2", .initialNumberOfTokens = 5, .input = true }, executor);

	Transition t1("", {}, {}, {}, {}, false);
	auto activationArcs = t1.getActivationArcs();
	auto destinationArcs = t1.getDestinationArcs();
	auto inhibitorArcs = t1.getInhibitorArcs();
	EXPECT_EQ(0, activationArcs.size());
	EXPECT_EQ(0, destinationArcs.size());
	EXPECT_EQ(0, inhibitorArcs.size());

	EXPECT_THROW(t1.removeArc(p1, ArcProperties::Type::ACTIVATION), PTN_Exception);
	EXPECT_THROW(t1.removeArc(p1, ArcProperties::Type::DESTINATION), PTN_Exception);
	EXPECT_THROW(t1.removeArc(p1, ArcProperties::Type::INHIBITOR), PTN_Exception);

	Transition t2("", { { p1 } }, { { p2 } }, { { p2 } }, {}, false);

	activationArcs = t2.getActivationArcs();
	destinationArcs = t2.getDestinationArcs();
	inhibitorArcs = t2.getInhibitorArcs();
	EXPECT_EQ(1, activationArcs.size());
	EXPECT_EQ(1, destinationArcs.size());
	EXPECT_EQ(1, inhibitorArcs.size());

	t2.removeArc(p1, ArcProperties::Type::ACTIVATION);
	t2.removeArc(p2, ArcProperties::Type::DESTINATION);
	t2.removeArc(p2, ArcProperties::Type::INHIBITOR);

	EXPECT_EQ(0, t2.getActivationArcs().size());
	EXPECT_EQ(0, t2.getDestinationArcs().size());
	EXPECT_EQ(0, t2.getInhibitorArcs().size());
}

TEST(Transition_, requireNoActionsInExecution_does_not_fire_if_onEnter_action_in_execution)
{
	atomic<bool> stop = true;
	auto onEnterActionFunction = [&stop]()
	{
		while (stop)
		{
			this_thread::sleep_for(20ms);
		}
		return;
	};

	PTN_EngineImp ptnEngine(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);

	ptnEngine.createPlace(
	{ .name = "P1", .initialNumberOfTokens = 0, .onEnterAction = onEnterActionFunction, .input = true });
	ptnEngine.createTransition(TransitionProperties{ "T1", { ArcProperties{ 1, "P1" } }, {}, {}, {}, {}, true });

	ptnEngine.execute();
	ASSERT_TRUE(ptnEngine.isEventLoopRunning());

	ptnEngine.incrementInputPlace("P1");
	this_thread::sleep_for(20ms);
	EXPECT_EQ(1, ptnEngine.getNumberOfTokens("P1"));

	stop = false;
	this_thread::sleep_for(100ms);
	EXPECT_EQ(0, ptnEngine.getNumberOfTokens("P1"));
}
