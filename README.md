# mazerandom

Implements an algorithm of **random maze generator** with no guarantee of having solution (i.e. you may find a situation with no path through the labirinth).

The main differences from [mazegenerator](https://github.com/optiklab/mazegenerator) are:
1. Here is NO Graph underneath of the generation algorithm (and thus, this is stupid way where you can't find a path...).
2. There is no particular fixed entry point or exit point.

It's very simple, but in the same time and stupid.

![picture](https://github.com/optiklab/mazerandom/blob/main/results/screenshot.jpg)

# How to run

The project is written in C++ with use of STL, SFML for graphics.
Project file is built using Visual Studio 2022 and Microsoft Windows.
So, you basically need to open the project using VS and press F5.

I didn't check the project on any other platforms except Windows (sorry).

# Author

Copyright (C) 2022 Anton "optiklab" Yarkov

https://github.com/optiklab/mazerandom

See LICENSE file in the repo.