![PTN Engine](<../Logo/PTN%20Engine%20Logo.svg> "PTN Engine")

# ***PTN Engine* Specification**

This document specifies the *PTN Engine* library version x.x.x .

**THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.**

## The **PTN Engine** Concept

The *PTN Engine* is a C++ library that implements the rules of Petri nets. 

Its purpose is to be integrated in other programs and to enable the 
integrating program to have its logic defined by a Petri net.
Third party code can be invoked as tokens enter and leave each place in the net.

The *PTN Engine* aims to:
 * control the software that integrates it so that its business logic conforms
   to a specified Petri net;
 * be easy to integrate with third party code.

Quality is a concern in the development of the *PTN Engine*. For this reason
unit tests and documentation are also part of this project. There is still much to
be improved concerning this topic, which hopefully will be addressed in each new 
release.

### Implemented Petri net features

The basics: place, transition, arc (implicit) and token.

Implemented Petri net extensions:
- inhibitor arc;
- reset arc;
- arc weights;

### Control features
These features are what allows the Petri net to communicate with the controller. 
- external (to the net) boolean functions can be added to a transition. This is 
a way to implement events influencing the firing of a transition.
- external (to the net) methods can be executed when a token enters and when a 
token leaves a place. In other words: control or simulation actions can be 
triggered by tokens entering and leaving a place.

### Runtime options

The PTN-Engine offers 4 different modes of operation, by selecting a
ACTIONS_THREAD_OPTION on construction.

These modes are:

SINGLE_THREAD
In single thread mode, as the name says, the PTN-Engine will only use one thread.
This means all the business logic of the net executes in the same thread. If an
action is triggered by a token reaching a place, this action will be executed 
synchronously, and the net will be on hold until the execution of the action is 
concluded.

EVENT_LOOP
In this mode, the Petri net is executed in a different thread. This allows for
events to be processed asynchronously. However, running actions triggered by a
token entering or leaving a place will be done in the same thread that is running
the Petri net.

DETACHED
This mode is similar to the EVENT_LOOP mode, but any actions triggered by the
tokens entering or leaving a place, will be executed in their own separate thread.
There is no guarantee of order of execution of actions that are triggered almost
at the same time in this mode. The order will depend on the system's scheduling.

JOB_QUEUE
This mode is again similar to the EVENT_LOOP mode. As hinted by the name, a Job
Queue thread will be created. Actions will be added to the Job Queue as a job to be
executed. This mode of operation guarantees that the order of execution of the
actions is the same as the order in which they were triggered.


### Licenses and Related 3rd Party Software

The *PTN Engine* is licensed under the Apache License, Version 2.0 .

Its tests link statically and dynamically against 
[Google Test](https://github.com/google/googletest).

Build instructions are scripted to be used with CMake.

### Activity Diagrams

TODO

## Remarks about *Version 1.x.x*

This version supports only a subset of Petri nets.

It can nonetheless be used to implement finite-state machines and marked graphs.

## Alternatives
If the *PTN Engine* does not meet your requirements, here are some alternative
suggestions:

[Boost MSM](http://www.boost.org/doc/libs/1_64_0/libs/msm/doc/HTML/index.html)

Others alternatives using Petri nets can be looked for here:
https://www.informatik.uni-hamburg.de/TGI/PetriNets/tools/quick.html

## **PTN Engine** Implementation
### General Structure

The *PTN Engine* library can be both statically and dynamically linked to its
clients.

### Dependencies

This library is implemented in C++ 20.

C++11 specific:
 - standard smart pointers
 - override
 - final
 - auto
 - range-based for loops
 - using (replacing typedef)
 - brace-init
 - tuple
 - to_string

The *PTN Engine* relies on: 
-[Google Test](https://github.com/google/googletest) for unit testing.
-[pugixml](https://pugixml.org/) to import and export Petri nets.

### Structure

#### PTN Engine

The *PTN Engine* is implemented within the namespace ptne.

TODO - include Doxygen documentation

#### ImportExport

Implements the import and export of Petri nets.

#### White Box Tests
Collection of tests that access the internals of the *PTN Engine*.

#### Black Box Tests
Collection of tests that can only access the public interface of the
*PTN Engine*.

#### Examples
Collection of examples using the *PTN Engine*.

## Performance
In this version performance was not yet evaluated. This point should be
considered in future releases.

### Optimization
No optimization techniques were used.

### Real Time
Not tested in a real time operating systems. Probably not supported due to the
use of exceptions.

### Run-time
#### Time complexity
No analysis or metrics at this point.
TODO

#### Spatial Complexity and Memory Footprint
No analysis or metrics at this point.
TODO

### Compile-time
No analysis or compilation metrics at this point.
TODO

## Safety
### Compiler
TODO - compiler settings and impact on safety

### Memory Allocation
Memory is allocated during runtime using the standard memory allocators.

Behaviour of the program in an environment with not enough memory available is
not yet specified nor tested. This is a point for future developments. (TODO)  

### Thread Safety
The *PTN Engine* is thread safe.

### Code Coverage
No analysis or compilation metrics at this point.
TODO

### Exception Safety
No analysis or compilation metrics at this point.
TODO

### Static Analysis
No analysis or compilation metrics at this point.
TODO
(...in a separate document, when done put a reference here to it)

### Known Problems
No analysis or compilation metrics at this point.
TODO
(...in a separate document, when done put a reference here to it)

## Documentation
The *PTN Engine* documentation can be found in the "Doc" directory and consists
of:
 * Specification (this document)
 * Code Documentation
 * User Guide
 * Test Reports - TODO
 * Benchmarks - TODO

Additionally the examples also provide valuable insight in how to use the *PTN
Engine*.

### Specification
The current document is the *PTN Engine* specification. It describes what the
*PTN Engine* is, what it attempts to achieve and in what way.

### Code Documentation
The code is commented according to the Doxygen format and doxyfiles are also
provided at Doc/Doxygen/. There are two configurations for the documentation
generation. One including everything (*PTN Engine*, tests and examples) and
another only for the *PTN Engine* itself.

Doxygen can be used to generate this part of the documentation.

### User Guide
Explains how the user can integrate the *PTN Engine* in his own program and
how to use all features of the engine.
The user guide is available at ```/Doc/User Guide```.

### Test Reports
TODO ...in a separate document, when done put a reference here to it

### Benchmarks
TODO ...in a separate document, when done put a reference here to it
