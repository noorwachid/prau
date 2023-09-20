# Prau
C++ build system

[UNDER DEVELOPMENT]

TLDR; If you are an "expert" C++, you should use CMake instead.

But if you are too dumb to understand what CMake actually does,
Well to put it simply CMake is actually not a build system.
It will generate Makefile which is configuration file of the native build system.
And then compile and link the library or executable.

Anyway, welcome to my dead simple build system.

## Installation
I've tested it only on MacOS, but in theory should be working on GNU/Linux.
Windows support is kind of hard to do, because I don't have a Windows machine.

Open your terminal:
~~~ sh
git clone git@github.com:noorwachid/prau
cd prau
sh build.sh
sh install.sh
~~~

## Getting Started
Open your terminal:
~~~ sh
mkdir helloworld && cd helloworld
prau init 
prau run
~~~

You should get:
~~~ sh
hello world!
~~~

## Commands
#### build
Compile and link project
~~~ sh
prau build
~~~

By default the mode is release and enabling all warning
~~~ sh
prau build --mode release # -O2
prau build --mode debug   # -O0 -g

prau build --warning all  # -Wall -Wextra
prau build --warning none #
~~~

#### clean
Remove all generated objects, libraries and executable.
~~~ sh
prau clean
~~~

#### run
Execute if the project type is executable
~~~ sh
prau run
~~~

To pass command line arguments to your executable:
~~~ sh
prau run -- a b c
~~~

#### init
Initialize empty folder with hello world project
~~~ sh
prau init
~~~

#### show
Show the project dependency graph
~~~ sh
prau show
~~~
