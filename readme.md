# Prau
C++ build system

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

To pass command line arguments to your executable:
~~~ sh
prau run -- a b c
~~~
