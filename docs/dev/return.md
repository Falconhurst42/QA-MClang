
# Return statements

Return statements are kept track of with the variables `0ret` and `0retv`. The `0ret` variable is either `0` or `1` and indicates wether a return statement has been executed yet. The `0retv` variable contains the value to be returned.

## Control flow checks

To ensure that the program does not execute any lines after a `return` statement, a control flow check can be inserted. This is a check in the form of an `execute if` minecraft command, that checks for a certain condition. If the condition is met, a function is executed that all of the code that comes after the control flow check.

## Write stack

An optimization that is implemented to prevent statements directly after a return statement, is a write stack, implemented in the bytecode manager. This is a stack containing boolean values. If there is at least one `false` on the stack, then the `write` function in the bytecode manager will not do anything.

In the following cases the write stack changes:
- At the start of each code block a `true` is added to the write stack.
- When a return statement is encountered, the top of the write stack is set to `false`.
- At the end of each code block the top entry of the write stack is removed.

## Control flow

There are three things that affect control flow related to `return` statements, these are execute statements, loops and if-statements.

### Execute statements

Execute statements can have return statements inside them, these can impact continuation of the program execution.

- Because execute statements can have the code block be executed multiple times, a check after the execute is added for `0ret` being 0.
- After an execute statement, a control flow check is added to check if `0ret` is 0.

An example is shown below:
```
as ("@a") {
    int x = 0;
    int y = 3;
}
```
This will then be converted to bytecode in the following way:
```
entry function:
    as @a if "0ret == 0" call 1
    if "0ret == 0" call entry function remainder

1:
    "x = 0"
    "y = 3"
```

### Loops

Loops work in a very similar way to execute statements, however the loop condition can be an expression, which can in turn modify the state of the program.

- The loop condition is checked in a separate function
- In the entry function, the loop condition function is called.
- At the end of the loop condition function, a conditional call to the loop code block is added.
- At the end of the loop code block a conditional call is added to the condition function, which executes if `0ret` is 0.
- After the loop a control flow check for `0ret` being 0 is added in the entry function.

Lets consider the following example and notice how the loop condition also modifies the state of the program, this needs to be accounted for when dealing with `return` statements!
```
while (x++) {
    x -= 2;
    /say hello!
}
```
This will then be converted to bytecode in the following way:
```
entry function:
    call 1
    if "0ret == 0" call entry function remainder

1: -- loop condition check
    if "x++" call 2

2: -- loop code block
    "x -= 2"
    "/say hello!"
    if "0ret == 0" call 1
```

*Note: The last line of function 2, `if "0ret == 0" call 1`, will not be ignored when there is a `return` statement in the loop code block. This is due to the way `return` statements are implemented. Hence this line cannot be changed to `call 1`.*

### If-statements

If-statements are more complex. All of the `else-if`s are added in a chain where at the end of end function in the chain the condition for a temporary variable being 0 is checked.

Consider the following example:
```
if (x == 3)
    x = 2;
else if (x > 4)
    x = 4;
else if (x++)
    x = 1
else
    x = 0;
```
Suppose the temporary variable is `0tmp0`, then this is converted to bytecode in the following way:
```
entry function:
    set 0tmp0 to 0
    if "x == 3" call 1
    if "0tmp0 == 0" call 2
    if "0ret == 0" call entry function remainder

1: -- 1st condition block
    "x = 2"
    set 0tmp0 to 1

2: -- 2nd condition check
    if "x > 4" call 3
    if "0tmp0 == 0" call 4

3: -- 2nd condition block
    "x = 4"
    set 0tmp0 to 1

4: -- 3rd condition check
    if "x++" call 5
    if "0tmp0 == 0" call 6

5: -- 3rd condition block
    "x = 1"
    set 0tmp0 to 1

6: -- else block
    "x = 0"
```
There do not need to be checks for `0ret` being 0 inside the condition blocks because the only way a return statement could be executed is if a code block inside the `if-statement` was executed, and hence `0tmp0` was set to 1.