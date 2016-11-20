# Logic
[![Build Status](https://travis-ci.com/udeyrishi/Logic.svg?token=ez7psV6XvuSyQ3hU3b5M&branch=master)](https://travis-ci.com/udeyrishi/Logic)

Logic is a domain specific scripting language/tool for doing Boolean Function manipulations. It is also a C++ library for doing these manipulations in your own application (the `core` module inside).

If you have any suggestions, feature requests, or improvements in mind, please leave an issue or submit a PR if you've already implemented it ;).

##Building and Installing

Requirements:

* One of (might work with other compilers and/or OSes, but not tested):
	* macOS with LLVM/clang++ 7.3.0+
	* Linux with g++ 4.9+
	* Linux with clang++ 3.6+
* cmake
* make

```sh
# To install logic in /usr/local/bin
./install.sh
```

Or, to just build:

```sh
# This will put the binaries in ./bin
cmake . && make logic

# To compile the logic_tests executable
cmake . && make logic_tests
```

##Running
Assuming the `logic` binary is in your path:

```sh
# Logic usage
$ logic -h
Usage:
  ./logic [options]
    options:
      <no option>         : starts in interactive mode if no option is provided
      [filepath]          : executes the code in the text file located at <filepath>
      -c, --code [code]   : executes the code string passed as the command line arg itself
      -h, --help          : print this usage info

# To run the tests
$ path/to/binary/logic_tests
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

* `let` (`l`): This comand is used for declaring new Boolean functions, overwriting existing Boolean functions, or updating certain values in a truthtable. e.g.:

```
>> let x = a & b; # Create new function
>> let x = a | c; # Overwrite x;
>> let $x[0] = 1; # Update the truthtable
>> p $x
| c | a |
-----------
| 0 | 0 | 1
| 0 | 1 | 1
| 1 | 0 | 1
| 1 | 1 | 1
```

* `print` (`p`): If the passed Boolean Function is a constant value function, then prints the constant value. Else, prints the truth table. The header of the truth table is the list of variables in little endian format (highest index variable is the leftmost, lowest is the rightmost).
*  `delete` (`d`): Deletes the Boolean function with the specified name from the current workspace.
*  `minterms` (`min`): Prints the minterms of the Boolean function expression passed.
*  `maxterms` (`max`): Prints the maxterms of the Boolean function expression passed.
*  `variables` (`v`): Prints the variables that the passed Boolean function is a function of in little endian format (highest index variable is the leftmost, lowest is the rightmost).
*  `quit` (`q`): In the interactive mode, quits the shell. If used in a script, will stop execution.
*  `if`/`else`/`else if` : Flow control commands. Work as you would expect them to. The condition to the `if` must be an expression that evaluates to a constant value Boolean function. e.g.:

```
if 0 { # You can use constants
    print a & b;
} else if !1 { # Or use operators
    print c & d;
} else if 0 {
    if ((p & h)[0]) { # Using parenthesis is optional
        print p & h;
    }
} else if (h | d)[3] { # As long as the result is a constant function
    if 1 {
    	 # The flow control will reach here and print (h | d)'s truth table
        print h | d;
    } else {
        print !1;
    }
}
```
*  `while` : Loop command. Works as you would expect it to. The condition argument abides by the same rules as the flow control commands above. e.g.:   

```
let x = a & b;
# Will iterate 3 times
while !$x[0] {
    if !$x[1] & !$x[2] & $x[3] {
        # First iteration here
        let x = $x | b;
    } else if !$x[1] & $x[2] & $x[3] {
        # Second iteration here
        let x = $x | a;
    } else if $x[1] & $x[2] & $x[3] {
        # Third iteration here. The loop will break after this
        let x = $x | 1;
    }
}

# The function was or-ed with 1 in the last iteration, 
# so the output will be a truthtable of a function of (a, b) that's always 1
p $x;
```

**Note**: The flow control and loop commands described above are valid logic commands as well, and abide by the same grammar: the first word is the command name, and the rest of the line consists of the arguments to that command. Now these arguments can be logic code wrapped in curly braces (as for `else`), be an evaluation condition + logic code in curly braces (as for `if` or `while`), or be a selectively allowed logic command + any commands that this command may require (as for `else`, when used in the `else if <condition> { <code> }` format). The only exception here is that a closing curly brace `}` does not need to be followed by a `;` explicitly; there is an implicit `;` after it. So even if you put `;` after all the closing braces in the examples above, the result will be the same. 

###Operators
* `&`: Binary operator 'and'
* `|`: Binary operator 'or'
* `^`: Binary opertor 'xor'
* `!`: Unary operator 'not'
* `(`, `)`: Parenthesis for specifying operator precedence
* `==`: Binary operator 'equals'. It takes in two Boolean functions as arguments, and returns a constant value Boolean function holding the value `0` or `1`. See this example for instance:

```
let x = (a & b) | (c | (d == 1));
p $x;
```
Since `d == 1` is the constant value Boolean function `0` (because `d` is _not_ the constant `1` always), and `c | 0 == c`, the result is the function `(a & b) | (c)`. Thus even though `d` was present in the original expression, the use of the `==` operator eliminated the function `x`'s dependency on `d`. Hence, the output:

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
| 1 | 1 | 1 | 1
```
* `[<unsigned int>]`: The index operator. If the passed function is a non-constant function, returns a constant value Boolean function having the value of the passed function's truth table at the specified index. Else, it is a no-op, and just returns the passed constant value function itself.

```
# Constant function case
>> let x = 1;
>> p $x[1000000];
1

# Truth table function case
>> let x = a & b;
>> p $x[3];
1
>> p $x[2];
0
>> p $x[4];
>> ERROR: index needs to be in range: [0, 3]
```

* `[k1=v1, k2=v2,...]`: The condition operator. This operator applies the specified conditions to the passed function, and returns the resulting function.

```
let x = ((a & b) | c)[b = 1, a = 0];
p (($x | d) | e)[d = 1];
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

Note: You should always use parenthesis to specify the operator precedence, because there is no agreed upon natural precedence between the different binary operators (unary operators have a natural precedence--they apply to the immediately following operand (or preceding, as is the case for the `[]` operator)). For instance: `a | (b & d | !c) & c` can be interpreted as `(a | ((b & d) | !c)) & c` if using _greedy_ precedence, or as `a | ((b & (d | !c)) & c)` if using the _lazy_ precedence.
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

#### `;` (the semi-colon sign):
Marks the end of line of code. The first whitespace (if any) separated word between two successive `;` will be interpreted as the command, and everything else will be passed as the arguments to that command.

#### `#` (the pound sign):
Everything that follows until a new line is a comment.

---
Congrats, you made it through! I'll be adding more features, and keeping this Readme updated.
