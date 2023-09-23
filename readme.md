# Prau
C++ build system

[UNDER DEVELOPMENT]

## Author Note
I always recommend to use CMake instead. 

## Benchmark
On my machine with 2,3 GHz Intel Core i5 and 8 GB DDR3 memory.
Building without optimatimization:
| Command                   | Time |
|---------------------------|------|
| `prau build --mode debug` | 55.22s user 6.33s system 345% cpu 17.818 total |
| `ninja`                   | 52.74s user 5.57s system 349% cpu 16.703 total |
| `make`                    | 34.74s user 4.20s system 96% cpu 40.324 total  |

`ninja` is faster, 
but you can use `prau` to generate `CMakeList.txt`
and then use `cmake` to generate the `build.ninja`
because this benchmark do exactly that.

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

#### generate
Configuration generator
~~~ sh
prau generate cmake
~~~

Supported targets:
- sh (shell script)
- cmake
- clangd
