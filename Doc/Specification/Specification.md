![PTN Engine](<../Logo/PTN%20Engine%20Logo.svg> "PTN Engine")

# PTN Engine Specification

This document specifies the *PTN Engine* library version: master branch.

**THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.**

## The PTN Engine Concept

The *PTN Engine* is a C++ library that implements the rules of Petri nets.

Its purpose is to be integrated in other programs and to enable the integrating program to have its logic defined by a Petri net.
Third party code can be invoked as tokens enter and leave each place in the net.

The *PTN Engine* aims to:
 * control the software that integrates it so that its business logic conforms to a specified Petri net;
 * be easy to integrate with third party code.

Quality is a concern in the development of the *PTN Engine*. For this reason unit tests and documentation are also part of this project. There is still much to be improved concerning this topic, which should be addressed in each new release.

### Implemented Petri net features

The basics: place, transition, arc (implicit) and token.

Implemented Petri net extensions:
- inhibitor arc;
- reset arc;
- arc weights;

### Control features
These features are what allows the Petri net to communicate with the controller.
- external boolean functions can be added to a transition. This is a way to implement events influencing the firing of a transition.
- external methods can be executed when a token enters and when a
token leaves a place. In other words: control or simulation actions can be triggered by tokens entering and leaving a place.

### Runtime options

The PTN-Engine offers 4 different modes of operation, by selecting a ACTIONS_THREAD_OPTION on construction.

These modes are:

SINGLE_THREAD
In single thread mode, as the name says, the PTN-Engine will only use one thread.
This means all the business logic of the net executes in the same thread. If an action is triggered by a token reaching a place, this action will be executed synchronously, and the net will be on hold until the execution of the action is concluded.

EVENT_LOOP
In this mode, the Petri net is executed in a different thread. This allows for events to be processed asynchronously. However, running actions triggered by a token entering or leaving a place will be done in the same thread that is running the Petri net.

DETACHED
This mode is similar to the EVENT_LOOP mode, but any actions triggered by the tokens entering or leaving a place, will be executed in their own separate thread.
There is no guarantee of order of execution of actions that are triggered almost at the same time in this mode. The order will depend on the system's scheduling.

JOB_QUEUE
This mode is again similar to the EVENT_LOOP mode. As hinted by the name, a Job Queue thread will be created. Actions will be added to the Job Queue as a job to be executed. This mode of operation guarantees that the order of execution of the actions is the same as the order in which they were triggered.

### Error Handling
The PTN Engine throws exceptions to signal runtime errors.

### Licenses and Related 3rd Party Software

The *PTN Engine* is licensed under the [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0) .

Its tests link statically and dynamically without modifications against [Google Test](https://github.com/google/googletest) licensed under the BSD-3-Clause license.

Importing and exporting petri nets to and from XML uses without modifications [pugixml](https://pugixml.org/) licensed under the MIT license.

Build instructions are scripted for CMake.

### Sequence/Activity Diagrams

TO DO

## Alternatives
If the *PTN Engine* does not meet your requirements, here are some alternative suggestions:

[Boost MSM](http://www.boost.org/doc/libs/1_64_0/libs/msm/doc/HTML/index.html)

Others alternatives using Petri nets can be looked for here:
https://www.informatik.uni-hamburg.de/TGI/PetriNets/tools/quick.html

## PTN Engine Implementation
### General Structure

The *PTN Engine* library can be both statically and dynamically linked to its clients.

### Dependencies

This library is implemented using C++ 20.

The *PTN Engine* depends on:
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
Collection of tests that can only access the public interface of the *PTN Engine*.

#### Examples
Collection of examples using the *PTN Engine*.

## Performance
In this version performance was not yet evaluated. This point should be
considered in future releases.

### Real Time
Not tested in a real time operating system.

### Run-time
TO DO: check complexity of algorithms, report key figures.

### Compile-time
TO DO: compilation metrics

## Safety
### Compiler
TO DO: compiler settings and impact on safety

### Memory Allocation
Memory is allocated during runtime using the standard memory allocators.

Behaviour of the program in an environment without enough memory available is not yet specified nor tested. This is a point for future developments. (TO DO)

### Thread Safety
The *PTN Engine* is thread safe.

### Code Coverage
It is possible to generate the code coverage using gcov and the provided tests.
TO DO: add instructions and report

### Exception Safety
TO DO: Add unit tests that address exception safety

### Known Problems
All problems are currently unknown :)

## Documentation
The *PTN Engine* documentation can be found in the "Doc" directory and consists
of:
 * Specification (this document)
 * Code Documentation
 * User Guide
 * Test Reports - TO DO
 * Benchmarks - TO DO

Additionally the examples also provide valuable insight on how to use the *PTN
Engine*.

### Specification
The current document is the *PTN Engine* specification. It describes what the
*PTN Engine* is, what it attempts to achieve and in what way.

### Code Documentation
The code is commented according to the Doxygen format and doxyfiles are also
provided at Doc/Doxygen/.

### User Guide
Explains how the user can integrate the *PTN Engine* in his own program and
how to use all features of the engine.
The user guide is available at ```/Doc/User Guide```.

### Test Reports
TO DO ...in a separate document, when done put a reference here to it

### Benchmarks
TO DO ...in a separate document, when done put a reference here to it
