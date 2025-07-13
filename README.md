# UCLA CS 31 – Fall 2024 Projects

This repository contains my solutions to the programming assignments for **CS 31: Introduction to Computer Science I** at UCLA, completed during Fall 2024. The course covers fundamental C++ programming concepts, software engineering practices, and algorithmic thinking.

## Overview

| Project | Topics Covered | Key Files |
|--------:|----------------|-----------|
| **Project 1** | Basic I/O, logic errors, debugging | `logic_error.cpp`, `original.cpp` |
| **Project 2** | Conditional statements, input validation | `rental.cpp`, `report.txt` |
| **Project 3** | Loops, file output formatting | `poll.cpp`, `report.txt` |
| **Project 4** | Arrays, functions | `array.cpp`, `report.txt` |
| **Project 5** | Strings, modular design, function reuse | `stars.cpp`, `utilities.cpp`, `utilities.h` |
| **Project 6** | Structuring multi-file programs | `hw.txt` |
| **Project 7** | Simulations, OOP | `bees.cpp` |
| **Project 7.1** | Final exam warmup| `warmup.cpp` |

Each project was written in standard C++ and compiled/tested on macOS using `clang++` or Xcode.

## How to build and run

You can compile most programs using:
```bash
clang++ -std=c++11 -Wall -Werror -o main <filename>.cpp
./main
```

For multi-file projects:
```bash
clang++ -std=c++11 -Wall -Werror -o main *.cpp
./main
```