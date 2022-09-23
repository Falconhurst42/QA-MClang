
# Loop implementation

In this documentation we will look at a standard loop:
```
for (before; condition; after) {
    loop content
}
```
The while-loop is based on the for-loop by ignoring the "before" and "after" loop arguments.

## Bytecode function structure

To generate a `for`-loop, a extra function is generated that can call itself if the loop condition is satisfied. The bytecode output will look something like the following:
```
main:
    [before bytecode]
    [outputs condition result to 0tmp0]
    EXEC    unless score 0tmp0 matches 0, loopfunc

loopfunc:
    [loop content bytecode]
    [after bytecode]
    [outputs condition result to 0tmp0]
    EXEC    unless score 0tmp0 matches 0, loopfunc
```
Note that loops can also take strings as inputs, in which case the last steps of the `loopfunc` are different.