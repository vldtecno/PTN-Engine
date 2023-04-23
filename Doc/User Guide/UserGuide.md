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

### 2. Create your own *PTN Engine* instance.

Create a PTN_Engine and select on of the ACTIONS_THREAD_OPTION options available.
object. Example:

```c++
using namespace ptne;
PTN_Engine pn(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);

```

### 3. Create the action functions 

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

### 4. Create the condition functions

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
