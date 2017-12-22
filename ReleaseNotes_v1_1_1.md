# *PTN Engine* Release Notes

## version 1.1.1 - 2017-12-22

### Documentation

### Bug fixes
 - Corrected version numbers. 


## version 1.1.0 - 2017-10-01

### Features
 - Implemented free choice. When more than one transition is active at the same 
 time, the transition that will be fired first is randomly chosen. (Note also
 that at the moment there is no parallelism available). 
 - Weighted arcs: arcs can now have weights different than 1. The weight can be 
 specified in the transition constructors.
 - Inhibitor arc. It is possible to define inhibitor arcs in the transition 
 constructors.
 - Reset arc. It is possible to define a reset arc in the transition 
 constructors.
 - Added the Elevator example. It exemplifies a more elaborate usage of the 
 *PTN Engine*.
 
### Non-Functional Features
 - Created types for exceptions, which translate into strong typed error 
 messages.
 - Overloaded the place and transition contructors, which allows cleaner user 
 code.
 - Added the option to print to an output stream the marking of the net while
 executing (mostly for debugging purposes).
 - Added checking for token overflow.
 - Extended the unit test coverage.

### Documentation
 - General improvements and corrections to the documentation.

### Bug fixes
- Corrected failing test. An exception must be thrown if there is an attempt
 to exit a place with not enough tokens to do it.
- Corrected bug in CMake script that prevented the tests makefiles from being
generated.
