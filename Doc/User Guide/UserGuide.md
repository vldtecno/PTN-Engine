![PTN Engine](<../Logo/PTN%20Engine%20Logo.svg> "PTN Engine")

# ***PTN Engine* User Guide**

## Getting Started

This is a step by step guide of how to integrate the *PTN Engine* in your 
program.

For further details you can read the code of the examples included in this
distribution.

### 1. Build the *PTN Engine*

You can generate a build configuration for *PTN Engine* running the provided
CMake scripts. These support GCC and Visual C++. Other compilers are not yet 
supported, so if you want to use a different compiler you will have to change 
or write the build scripts yourself.

Building with the provided CMake scripts will provide you the necessary
*PTN Engine* static and dynamic libraries.

In your own project configure your build to link against these libraries and
include the directory "PTN_Engine/include".

**Note**: You should use the same compiler and compiler settings for the
*PTN Engine* and for your application to guarantee binary compatibility.

### Step by step guide

#### 2. Create your own *PTN Engine* instance.

Create a PTN_Engine and select on of the ACTIONS_THREAD_OPTION options available.
object. Example:

```c++
using namespace ptne;
PTN_Engine pn(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);

```
At this point pn object is basically empty and has no Petri net defined. Specifying
the Petri net is done in the following steps.

#### 3. Create the action functions 

These are the functions you want to invoke when a token enters or exits a given
place.
Example:
```c++
ActionFunction compute = [&x, &result](){
	result *= x;
	if (x > 0)
	{
		--x;
	}
};
```

These can be any kind of functions as long as they return void, take no arguments
and are accessible to the PTN_Engine object.

#### 4. Create the condition functions (optional)

This is optional. If you want to add some additional conditions to the
transition's activation, now is the moment to do it.
Example:
```c++
ConditionFunction finished = [&x](){return x <= 1;};
```
These can be any kind of functions as long as they return bool, take no arguments
and are accessible to the PTN_Engine object.

#### 5 Create the Petri net's places

Create *all* places in the Petri net.
Example:
```c++
pn.createPlace("Compute", 0, compute, true);
pn.createPlace("Finished", 0);
```

#### 7 Create the Petri net's transitions

The last step is to create *all* transitions in the net.
Example:
```c++
pn.createTransition({"Compute"}, {"Finished"}, {finished});
pn.createTransition({"Compute"}, {"Compute"}, {notFinished});
```

#### 8 Run the Petri net

This last step finally runs the Petri net specified in the previous steps.
Example:
```c++
pn.incrementInputPlace("Compute");
pn.execute();
```

### Example

Following the previous step by step guide, your program should look somewhat
like:
```c++
#include <iostream>

//Include for the PTN Engine library
#include "PTN_Engine/PTN_Engine.h"

int main(int, char **)
{
using namespace ptne;

// Create a new Petri net called "pn".
PTN_Engine pn(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);

size_t input = 10;
size_t x = input;
size_t result = 1;

// Create the action functions to be executed when a token arrives to some
// place.
ActionFunction compute = [&x, &result]()
{
	result *= x;
	if (x > 0)
	{
		--x;
	}
};

// Create additional condition functions used to activate transitions in
// the Petri net.
ConditionFunction finished = [&x]() { return x <= 1; };
ConditionFunction notFinished = [&finished]() { return !finished(); };

// Create the Petri net places.
pn.createPlace("Compute", 0, compute, true);
pn.createPlace("Finished", 0);

// Create the Petri net transitions.
pn.createTransition({ "Compute" }, { "Compute" }, { notFinished });
pn.createTransition({ "Compute" }, { "Finished" }, { finished });

// Increment on token to the input place "Compute".
pn.incrementInputPlace("Compute");

// Start the execution of the Petri net.
pn.execute();

// Because in this example the net is running in SINGLE_THREAD mode
// when there is no more fireable transitions the execute returns
// instead of continuing running waiting for further events.
cout << "The factorial of " << input << " is " << result << endl;

cout << "Successfully terminated" << endl;
return 0;
}
```
