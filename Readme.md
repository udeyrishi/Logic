# Logic
[![Build Status](https://travis-ci.com/udeyrishi/Logic.svg?token=ez7psV6XvuSyQ3hU3b5M&branch=master)](https://travis-ci.com/udeyrishi/Logic)

Logic is a simple language(-ish)/tool for doing Boolean Function manipulations. It is also a C++ library for doing these manipulations in your own application (the `core` module inside).

This has been tested to work on macOS and Linux. _Should_ work on Windows as well, but hasn't been tested yet. The following building and running guides are written with macOS and Linux in mind, but it should be easy to adapt these to Windows.

##Building

Requirements:

* One of (might work with other compilers and/or OSes, but not tested):
	* macOS with LLVM/clang++ 7.3.0+
	* Linux with g++ 4.9+
	* Linux with clang++ 3.6+ 
* cmake
* make
* [Optional] Python

```sh
# This will put the binaries in ./bin
cmake . && make logic

# To compile the logic_tests executable
cmake . && make logic_tests

# Or to do everything, use this handy script.
# Use flag -d for the debug version
# This will output to the release/bin (or debug/bin) directory
./build.py
```

##Running
Assuming the `logic` binary is in your path:

```sh
# To launch the shell for an interactive session, go:
logic

# You can also use logic for interpreting a source file:
logic <path to file>

# To run the tests:
path/to/binary/logic_tests
```

##Language basics

Let's look at some simple sample code:

```
let x = a & b;
let y = b | c;
let z = $x ^ $y;
print !$z;
```

The above will define two Boolean functions `x` and `y` which are functions of `a` and `b`, and `b` and `c` respectively (through the command `let`). Then it'll create another Boolean function `z`, which is a function of the earlier defined functions `x` and `y` (notice the `$` sign). Then, it'll print the truth table corresponding to the Boolean function expression passed to the command `print`; in this case, the earlier defined Boolean function `z`, notted. So the output would be:

```
| c | b | a |
---------------
| 0 | 0 | 0 | 1
| 0 | 0 | 1 | 1
| 0 | 1 | 0 | 0
| 0 | 1 | 1 | 1
| 1 | 0 | 0 | 0
| 1 | 0 | 1 | 0
| 1 | 1 | 0 | 0
| 1 | 1 | 1 | 1
```

Besides variables, you can use the constants `1` and `0` in the expressions as well, such as:

```
let foo = 1;
p !$z | $foo;
```

This will output:

```
| c | b | a |
---------------
| 0 | 0 | 0 | 1
| 0 | 0 | 1 | 1
| 0 | 1 | 0 | 1
| 0 | 1 | 1 | 1
| 1 | 0 | 0 | 1
| 1 | 0 | 1 | 1
| 1 | 1 | 0 | 1
| 1 | 1 | 1 | 1
```

`$foo` here is a special constant value Boolean function.

###Commands
The first word in every line of Logic code is a command. Everything that follows is passed to that command as arguments. Different commands expect different number of arguments.

The list of currently defined commands, their aliases in the parenthesis, and what they do:

* `let` (`l`): Defines a new Boolean function in the current workspace, or overwrites an existing one with the same name. Expects 1 argument (everything that follows `let`).
* `print` (`p`): Prints the truth table representing the Boolean function passed to it. It can be the name of a function defined earlier (like `$z`), or a more complex expression that is parsed, evaluated, printed, and destroyed (e.g.: `print $z & a`, or `print !$z` above). Expects 1 argument (everything that follows `print`).
*  `delete` (`d`): Deletes the Boolean function with the specified name from the current workspace. Expects 1 argument (everything that follows `delete`).
*  `quit` (`q`): In the interactive mode, quits the shell. If used in a script, will stop execution. Expects no arguments.

###Operators
* `$`: Binary operator 'and'
* `|`: Binary operator 'or'
* `^`: Binary opeartor 'xor'
* `!`: Unary operator 'not'
* `(`, `)`: Parenthesis for specifying operator precedence.

Note: You should always use parenthesis to specify the operator precedence, because there is no agreed upon natural precedence between the different binary operators (unary operators have a natural precedence--they apply to the immediately following operand). For instance: `a | (b & d | !c) & c` can be interpreted as `(a | ((b & d) | !c)) & c` if using _greedy_ precedence, or as `a | ((b & (d | !c)) & c)` if using the _lazy_ precedence.
The convention here is to use the latter _lazy_ one.

###Special tokens
There are some special tokens that you can use in a Boolean function:

#### `$` (the dollar sign)
When used immediately before a symbol, that symbol is used as the name of Boolean function defined before in your program (vs. an independent variable in your function). E.g.:

```
let x = a & b;
let y = x & b;
let z = $x & b;
let f = $cat & a;
```

Here, `x` is a Boolean function of two independent variables `a` and `b`. Similarly, `y` is a function of two independent variables `x` and `b` (the `x` here does not have the `$` prefix, so it's not interpreted as a function name in the current workspace). `z`, on the other hand, is a function of `x`, a Boolean function defined earlier, and `b`, an independent variable. So, it is equivalent to writing `let z = a & b & b;`. The last function definition for `f` will fail, because the interpreter will try to lookup a function named `cat` in the current workspace, and it doesn't exist.

Also note that when using `$`, the value of the following function is applied immediately (vs. keeping a reference). So:

```
let x = a & b;
let y = $x ^ c;
let x = a | b;
p $y;
```

Here, even though the value of the Boolean function named `x` is changed later to `a | b`, it was `a & b` when y was defined. So the value of `y` is `(a & b) ^ c`. So the output is:

```
| c | b | a |
---------------
| 0 | 0 | 0 | 0
| 0 | 0 | 1 | 0
| 0 | 1 | 0 | 0
| 0 | 1 | 1 | 1
| 1 | 0 | 0 | 1
| 1 | 0 | 1 | 1
| 1 | 1 | 0 | 1
| 1 | 1 | 1 | 0
```

Also note that the `$` has the highest precedence. For instance, in the above example, the value of `x` is first computed, and then xor-ed with `c` to get the Boolean function for `y`. So, the equivalent definition would be `let y = (a & b) ^ c;` (and not `a & (b ^ c)`, which would've been the case if you went `let y = a & b ^ c`).

#### `:` (the colon sign):
You can use the colon sign towards the end of the Boolean function to specify a comma separated list of values of certain variables. This will then collapse the function by abiding to these conditions, and only return the result. e.g.:

```
let x = (a & b) | c : b = 1, a = 0;
p ($x | d) | e : d = 1;
```

Here, `x` is the function `(a & b) | c`, when `b` is true and `a` is false. That simplifies to `let x = c`. Then, the args to the `p` command are `($x | d) | e` (simplifies to `(c | d) | e`) when `d` is true. That simplifies to a function of c and e that is always true. So, the output is:

```
| e | c |
-----------
| 0 | 0 | 1
| 0 | 1 | 1
| 1 | 0 | 1
| 1 | 1 | 1
```

Note that after applying the conditions, you could be left with a constant value Boolean function. The output in that case will be `0` or `1`, as was shown in the introductary section above.

You can use the `:` token in a `let` command or in a `print` command. You can use it exactly once towards the end of the function definition.

#### `;` (the semi-colon sign):
Marks the end of line of code. The first whitespace separated word between two successive `;` will be interpreted as the command, and everything else will be passed as the arguments to that command.

#### `#` (the pound sign):
A comment line.

---
Congrats, you made it through! I'll be adding more features, and keeping this Readme updated. If you have a suggestion, leave an issue. If you have an improvement, leave a PR.
