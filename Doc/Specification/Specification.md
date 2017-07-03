# ***PTN Engine* Specification**

This document specifies the *PTN Engine* library version 1.0.0 .

**Responsibility for the usage or integration of the *PTN Engine* belongs to its
users alone.**

## The **PTN Engine** Concept

The *PTN Engine* is a C++ 11 library that implements (some of the types of)
Petri nets, also known as place transition nets, and calls third party code
according to its state.

Its purpose is to allow C++ developers to easily implement a program whose
behaviour is defined by a Petri net.

The engine can be integrated by deriving from its classes. It provides a PTN
base class and Petri nets can be defined in the constructor of its children.
Third party code can be invoked as tokens enter and leave each place in the net,
 thus being controlled by the net.

The *PTN Engine* aims to:
 * control the software that integrates it so that its business logic conforms
   to a specified Petri net;
 * make the business logic of client software more flexible;
 * be easy to integrate with third party code.

Quality is a concern in the development of the *PTN Engine*. For this reason
unit tests and documentation accompany its delivery. There is much to be
improved concerning this topic, which hopefully will be addressed in future
releases.

### Licenses and Related 3rd Party Software

The *PTN Engine* is licensed under the Apache License, Version 2.0 .

Its tests link statically and dynamically against Google Test.

Build instructions are scripted to be used with CMake.

### Activity Diagrams

TODO

## Remarks about *Version 1.0.0*

This version supports only small subset of Petri nets. Hence free
choice, weights and priority are not (yet) supported.

It can nonetheless be used to implement finite-state machines and marked graphs.

The CMake build scripts support the compilers GCC and MSVC.


## Alternatives
If the *PTN Engine* does not meet your requirements, here are some alternative
suggestions:

Boost MSM: http://www.boost.org/doc/libs/1_64_0/libs/msm/doc/HTML/index.html

Others alternatives using Petri nets can be looked for here:
https://www.informatik.uni-hamburg.de/TGI/PetriNets/tools/quick.html

## **PTN Engine** Implementation
### General Structure

The *PTN Engine* library can be both statically and dynamically linked to its
clients.

### Dependencies

This library is implemented in C++ 11.

C++11 specific:
 - standard smart pointers
 - override
 - final
 - auto
 - range-based for loops
 - using (replacing typedef)
 - brace-init

The *PTN Engine* relies on Google Test (https://github.com/google/googletest)
for unit testing.

### Structure

#### PTN Engine

The *PTN Engine* is implemented within the namespace ptne.

##### Class Diagram
The class structure is as follows:

TODO - For the time being generate the Doxygen documentation.

##### PTN_Engine
This is the base class that implements the Petri net logic. The integrator
should create a class that inherits from this one. Inside its constructor Places
should be created as shared pointers and passed to the engine (addPlace).

Transitions should be created also in the constructor of the class inheriting
from PTN_Engine.

For more information on how to do this please read the code of the samples.

##### Place
This class defines a place in a Petri net. Additionally function pointers can be
passed a Place so that they are called when a token enters or leaves the place.

Places can be defined as entry places, which are places that simulate the
happening of an event and which can be manipulated by the outside code.

##### IActionFunctor
Base class for methods to be called by the Place when a token enters or leaves.

##### IConditionFunctor
Base class for additional conditions that can disable or enable a transition in
the net.

##### Action
Template class that facilitates the creation of function pointers to a
particular class, to be used in the net's places.

##### ActivationCondition
Template class that facilitates the creation of function pointers to a
particular class, to be used in the net's places.

#### White Box Tests
Collection of tests that access the internals of the *PTN Engine*.

#### Black Box Tests
Collection of tests that can only access the public interface of the
*PTN Engine*.

#### Samples
Collection of samples using the *PTN Engine*.

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

#### Space Complexity and Memory Footprint
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
This library was in its latest release only tested and considered for single
threaded operation. TODO

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

Additionally the samples also provide valuable insight in how to use the *PTN
Engine*.

### Specification
The current document is the *PTN Engine* specification. It describes what the
*PTN Engine* is, what it attempts to achieve and in what way.

### Code Documentation
The code is commented according to the Doxygen format and doxyfiles are also
provided at Doc/Doxygen/. There are two configurations for the documentation
generation. One including everything (*PTN Engine*, tests and samples) and
another only for the *PTN Engine* itself.

Doxygen can be used to generate this part of the documentation.

### User Guide
This document, or collection of documents, explains how the user can integrate
the *PTN Engine* in his own program and how to use all features of the engine.
The user guide is available at ```/Doc/User Guide```.

### Test Reports
TODO ...in a separate document, when done put a reference here to it

### Benchmarks
TODO ...in a separate document, when done put a reference here to it
