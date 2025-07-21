![PTN Engine](<../Logo/PTN%20Engine%20Logo.svg> "PTN Engine")

# PTN Engine User Guide

## Getting Started

This is a step by step guide of how to integrate the *PTN Engine* in your program.

For further details you can read the code of the examples included in this distribution.

### 1 - Build the PTN Engine

You can generate a build configuration for *PTN Engine* running the provided
CMake scripts. These support GCC and Visual C++. Other compilers are not yet supported, so if you want to use a different compiler you will have to change or write the build scripts yourself.

Building with the provided CMake scripts will provide you the necessary *PTN Engine* static and dynamic libraries.

In your own project configure your build to link against these libraries and include the directory "PTN_Engine/include".

**Note**: You should use the same compiler and compiler settings for the *PTN Engine* and for your application to guarantee binary compatibility.

### 2 - Create your own PTN Engine instance.

Create a PTN_Engine and select on of the ACTIONS_THREAD_OPTION options available.
Example:

```cpp
using namespace ptne;
PTN_Engine pn(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);

```
At this point pn object is basically empty and has no Petri net defined. Specifying the Petri net is done in the following steps.

### 3 - Create the action functions 

These are the functions you want to invoke when a token enters or exits a given place.
Example:
```cpp
ActionFunction compute = [&x, &result](){
	result *= x;
	if (x > 0)
	{
		--x;
	}
};
```

These can be any kind of functions as long as they return void, take no arguments and are accessible to the PTN_Engine object.

### 4 - Create the condition functions (optional)

This is optional. If you want to add some additional conditions to the transition's activation, now is the moment to do it.
Example:
```cpp
ConditionFunction finished = [&x](){return x <= 1;};

ConditionFunction notFinished = [&finished]() { return !finished(); };
```
These can be any kind of functions as long as they return bool, take no arguments and are accessible to the PTN_Engine object.

### 5 - Create the Petri net's places

Create *all* places in the Petri net.
Example:
```cpp
pn.createPlace({ .name="Compute",
                 .onEnterAction=compute,
                 .input=true} );

pn.createPlace({ .name="Finished"} );
```

### 6 - Create the Petri net's transitions

The last step is to create *all* transitions in the net.
Example:
```cpp
pn.createTransition({ .name = "T1",
                      .activationArcs = { { .placeName = "Compute" } },
                      .destinationArcs = { { .placeName = "Compute" } },
                      .additionalConditions = { notFinished } });

pn.createTransition({ .name = "T2",
                      .activationArcs = { { .placeName = "Compute" } },
                      .destinationArcs = { { .placeName = "Finished" } },
                      .additionalConditions = { finished } });
```

### 7 - Run the Petri net

This last step finally runs the Petri net specified in the previous steps.
Example:
```cpp
pn.incrementInputPlace("Compute");
pn.execute();
```

### Example

Following the previous step by step guide, your program should look somewhat like:
```cpp
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
                     .input=true });

    pn.createPlace({ .name="Finished" });


    pn.createTransition({ .name = "T1",
                          .activationArcs = { { .placeName = "Compute" } },
                          .destinationArcs = { { .placeName = "Compute" } },
                          .additionalConditions = { notFinished } });

    pn.createTransition({ .name = "T2",
                          .activationArcs= { { .placeName = "Compute" } },
                          .destinationArcs = { { .placeName = "Finished" } },
                          .additionalConditions = { finished } });

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
```
