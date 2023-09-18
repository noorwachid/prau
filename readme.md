# Prau
C++ toolkit

[UNDER DEVELOPMENT]

## Getting Started
Open your terminal:
~~~ sh
prau create hello && cd hello
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

## Task-List
- [ ] Build system
    - [x] Source parser
    - [ ] Multi-threaded source parser
    - [x] Source tree
    - [x] Source inverse tree
    - [x] Starter template
    - [x] Builder 
    - [ ] Multi-threaded builder
- [ ] Package manager
- [ ] Analizer -- `cppcheck` integration
- [ ] Leaker -- `valgrind` integration
- [ ] Profiler -- `valgrind` integration
- [ ] Tester -- `catch2` integration

## Command
- `compile`, compile a package
- `link`, link a package
- `build`, compile and link a package.
- `run`, run an executable package, will build first if no executable were found.
- `analyze`, staticly analize the source files
- `leak`, check for memory leak on runtime
- `profile`, check for performance on runtime
- `test`, test all defined cases
- `install`, install a package
- `uninstall`, uninstall a package
- `create`, create a package
