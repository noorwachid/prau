# Prau
C++ build system

[UNDER DEVELOPMENT]

TLDR; If you are an "expert" C++ should use CMake instead.

But if you are dumb to understand what CMake actually does,
Welcome to my dead simple build system.

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

By default the mode is release and enabling all warning
~~~ sh
prau build --mode release # -O2
prau build --mode debug   # -O0 -g

prau build --warning all  # -Wall -Wextra
prau build --warning none #
~~~

To pass command line arguments to your executable:
~~~ sh
prau run -- a b c
~~~
