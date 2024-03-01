/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2023-2024 Eduardo Valgôde
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

#include <iostream>
#include "PTN_Engine/PTN_Engine.h"

using namespace std;

int main(int, char **)
{
	using namespace ptne;
	PTN_Engine pn(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);

	size_t x = 0;
	size_t result = 1;

	ActionFunction compute = [&x, &result]()
	{
		result *= x;
		if (x > 0)
		{
			--x;
		}
	};

	ConditionFunction finished = [&x]() { return x <= 1; };
	ConditionFunction notFinished = [&finished]() { return !finished(); };

	pn.createPlace({ .name="Compute",
					 .onEnterAction=compute,
					 .input=true});
	pn.createPlace({.name="Finished"});

	pn.createTransition({ .name = "T1",
						  .activationArcs = { { .placeName = "Compute" } },
						  .destinationArcs = { { .placeName = "Compute" } },
						  .additionalConditions={ notFinished } });
	pn.createTransition({ .name = "T2",
						  .activationArcs= { { .placeName = "Compute" } },
						  .destinationArcs = { { .placeName = "Finished" } },
						  .additionalConditions={ finished } });

	for (size_t i : { 0, 1, 2, 3, 6, 10 })
	{
		x = i;
		result = 1;
		pn.incrementInputPlace("Compute");
		pn.execute();
		cout << "The factorial of " << i << " is " << result << endl;
	}
	cout << "Successfully terminated" << endl;
	return 0;
}
