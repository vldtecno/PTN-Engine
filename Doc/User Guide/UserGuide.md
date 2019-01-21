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

### 2. Create a class you wish to control with a Petri net.

The intention behind the engine is to control some process with a Petri net.
The object to be controlled should have its own class.

For this example let's assume the class to be controlled is called Controller.

```c++

class Controller
{
  public:

    // Contains public methods that can be invoked by PTN Engine
    // These are of two types:
    //      void Action();
    //          and
    //      bool Condition() const;

    //Example of some action methods

    void onEnterAction2();
    void onExitAction2();

    //Example of some additional activation condition methods

    bool isCondition1() const;
    bool isCondition2() const;

        . . .
};

```

### 3. Create your own *PTN Engine* derived class.

Create a class that inherits privately from PTN_Engine and that accepts a shared
pointer to a class whose methods are to be invoked by the net.

```c++

#include "PTN_Engine/PTN_Engine.h"

class MyPetriNet: private ptne::PTN_Engine
{
public:
    MyPetriNet(std::shared_ptr<Controller> ptrController);

    . . .

```

### 4. Implement the Petri net

In the constructor of the class that inherits from PTN_Engine (MyPetriNet in
this example) is where the Petri net is defined.

#### 4.1 Create the Petri net places

The first step is to create *all* the places in the net.

```c++
MyPetriNet::MyPetriNet(std::shared_ptr<Controller> ptrMenuController):
    PTN_Engine()
{
	//Example of an input place without actions associated
	addPlace(
	    "Place1",   //Place name
	    0,          //Initial number of tokens
	    nullptr,    //No on enter actions
	    nullptr,    //No on exit actions
	    true        //Marked as input place
	    );

	//Example of a place with on enter and on exit actions
	addPlace(
	    "Place2",   //Place name
	    1,          //Initial number of tokens
	    std::bind(&Controller::onEnterAction2, ptrController), //on enter action
	    std::bind(&Controller::onExitAction2, ptrController),  //on exit action
	    );


```

#### 4.2 Create the Petri net transitions

The last step is to create *all* transitions in the net.

```c++

//Example of a transition without additional conditions
createTransition(
        {"Place1", "Place2"},   //Activation places
        {"Place3"},             //Destination places
        {}                    //Additional conditions
        );

//Example of a transition with additional conditions
createTransition(
        {"InputC", "ShowMessage"},     //Activation places          
        {"MessagesMenu"},              //Destination places   
        //Vector of addditional conditions      
        {
            std::bind(&Controller::isCondition1, ptrController),
            std::bind(&Controller::isCondition2, ptrController),           
        }
        );                                 

	. . .

```

### 5 - Create access to input places

To control the PTN Engine net, you need to provide access to it via public 
methods in your new class.


```c++

#include "PTN_Engine/PTN_Engine.h"

class MyPetriNet: private ptne::PTN_Engine
{
public:

	MyPetriNet(std::shared_ptr<Controller> ptrController);

		. . .

	void someTrigger()
	{
		incrementInputPlace("someInputPlace");
		execute();
	}

		. . .

```
