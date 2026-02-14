
# Contributing

Thank you for taking the time to help with this project. Reading this file will hopefully give you a better idea of how MWGH works.


## MWGH Documentation

Please note that most of the documentation for MWGH is in the source code. Every function and structure should be explained there.

Run "Make" to compile, then run "./MWGH" to start the game.


## Global variables

In MWGH, no global variables are allowed except for constants. This is so that the behaviour of every function only depends on it's parameters, this makes testing much easier.


## Cross platform

MWGH is built using drws-lib, it inherits the same ideas when it comes to cross platform programming. That is, use as few dependencies as possible and use optional dependencies only, even the C standard library is not used.

If any file includes a header using angle brackets ("<" and ">"), this is a bug. Unless it's in an optional dependency or if the code does not end up in the final executable (like assets/convert.c). Including files with double quote marks is okay, as long as those files are in the repository.


## Cross compiling

Remember to run "make clean" before compiling to a new platform.
