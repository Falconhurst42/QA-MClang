
# Beginner's guide for MCLang

Thank you for trying out MCLang in your Minecraft project. Let's walk you through the first steps into making a masterpiece!

## Creating a new Minecraft world

First, create a new Minecraft world for this guide. Then locate the Minecraft world's save folder, which on Windows is usually located in `%appdata%\.minecraft\saves`.

## Installation

The MCLang compiler can be installed either by downloading it directly (recommended) or by building the project yourself. Here I will only explain how to use a direct download, as it is a lot easier.

### Direct download

The MCLang compiler can be downloaded directly by going to the [Releases](https://github.com/PurpleStripedUnicorn/MCLang/releases) page.

After downloading, place the program in your Minecraft world's folder and rename it to `mclang.exe` if you're on Windows or just `mclang` on Linux, this isn't necessary but makes things a lot easier.

## Writing your first program

Create another file in your Minecraft world's folder named `tutorial.mcl`. Open it and put the following code in it:
```
void hello() {
    /say Hello World!
}
```
For now just copy and paste the code, I'll explain what it does later.

## Compiling the program

Now start up a console and navigate to you Minecraft world's folder. Then run the following command:
```
On Windows:   .\mclang.exe tutorial.mcl -o datapacks\mydatapack
On Linux:     ./mclang tutorial.mcl -o datapacks/mydatapack
```
This will compile your code and create a datapack. Load up your Minecraft world and run the command `/function dp:hello`. If everything went right, you should see `Hello World!` in chat.

The first part of the command is the executable name, the file we renamed to `mclang` earlier. The second part is the input filename, the file with your code to compile. The third part is a command line argument telling the compiler where to put the datapack.

**Warning: If there is already a datapack present at the location where you tell the compiler to put the datapack, this other datapack will be overwritten! Be very careful where you put your datapack!**

## Updating the code

Every time you update your program, you can run the compiler to update the datapack. Try to change the `Hello World!` to `Hello Steve!` and then run the command again. Now inside Minecraft first run `/reload` to reload all datapacks, then run `/function dp:hello` again and you should see the updated message show up.

## The example program explained

Now that you know how to compile a program let's go back to the program we just ran:
```
void hello() {
    /say Hello World!
}
```
In each program you write, you will write functions, the above code is an example of a function. The `void` means that the function does not return or "give back" anything, most functions start with this word.

The `hello` is the name of the function, you use this name to run the function inside minecraft using `/function dp:hello`. The `dp` in this command is the namespace of the datapack, `dp` is just a default value and can be changed using the `-n` command line argument.

After the name come the function arguments, this function has no arguments and therefore any the `()` is left. Then follows the actual code inside the function to run, enclosed by `{` and `}`.

If you want to put a Minecraft command inside your function, make sure to start it with `/` and don't put anything before it on the same line (except spaces or tabs).

We can easily add more commands by listing then below each other, for example:
```
void hello() {
    /say Hello World!
    /say Hello Steve!
    /say Hello Alex!
}
```
This will display `Hello World!`, `Hello Steve!` and then `Hello Alex!` in chat when you run `/function dp:hello`.

## Let's add some statements!

Now plain functions like this aren't very useful. Often you'll want to use if-statements, loops, variables, etc. Let's look at if-statements first and let's see a new example:
```
void hello() {
    /say Hello World!
    if ("entity @e[type=pig]") {
        /say There is a pig!
    }
}
```
This program still displays `Hello World!` in the chat when running `/function dp:hello`, but it will also check if there is a pig and display a message `There is a pig!` if there is one.

To understand what's going on here, it's useful to know about the [execute command]() in Minecraft. The compiler just uses the command `/execute if entity @e[type=pig] run [your code]` to execute this program. Likewise you can also use other `execute`-subcommands, like `as` or `positioned`:
```
void hello() {
    as ("@p") {
        /say Hello!
    }
    positioned ("0 64 0") {
        /summon pig ~ ~ ~
        /summon villager ~ ~ ~
    }
}
```

## If-statements

If-statements are special, because you can also use `else`, which will execute if the earlier `if`-statements did not execute:
```
void hello() {
    if ("entity @a[name=PurpleStriped]") {
        /say Hello PurpleStriped!
    } else if ("entity @a[name=PurpurStriped]") {
        /say Hello PurpurStriped!
    } else {
        as ("@a") {
            /say Hello!
        }
    }
}
```

## While-loops

If you want to keep executing a certain piece of code in a loop you can use a `while`-loop:
```
void hello() {
    while ("entity @e[type=pig]") {
        /kill @e[type=pig,limit=1,sort=closest]
    }
}
```
This will kill the closest pig until there are no more pigs left. Be careful when using `while`-loops because the program can easily get stuck in an infinite loop and then your game will crash when running the function!

## The `tick` and `load` functions

You can give your functions any name you want, and you can create as many functions as you want (put them below each other). Two function names however are special! When you call a function `tick`, it will be automatically executed every game tick. When naming a function `load` it will be executed when your datapack loads. For example:
```
void tick() {
    /say Spam!
}

void load() {
    /say Datapack has loaded!
}
```

## Comments

Sometimes you might want to add a comment in your code. This can be done by either using `//` for a single-line comment or `/*` and `*/` for multiple lines:
```
void func() {
    // This is a comment and does nothing
    /say Hello World!
    /* This is a multi-line
    comment */
    /say Woop woop!
}
```

## Variables

You can also use variables in your code, in a very similar way to [C](https://en.wikipedia.org/wiki/C_(programming_language)). You can use both `int`s (whole numbers) and `bool`s (true or false). For example:
```
void func() {
    // Will make x equal to 15
    int x = 7 + 8;
    // Add 1 to x to make it 16
    x++;
    // This will run the code inside the if-statement, because x is larger than 4
    if (x > 4) {
        // Boolean true
        bool b = true;
        // This loop will run once, because b is set to false after one loop
        while (b) {
            x++;
            b = false;
        }
        // Say hello to everyone
        /say Hello!
    }
}
```
Also note how all of these lines except for the commands end in a semicolon `;`!

## Strings

In most programming languages you can also use strings along with `bool` and `int`. In MCLang however you can only use `const str`, meaning strings that you cannot change. This is shown in the following example:
```
void func() {
    const str CLOSEST_PLAYER = "@p";
    as (CLOSEST_PLAYER) {
        /say I'm the closest player!
    }
}
```

## Code on!

Hopefully this beginner's guide gave you a quick introduction to MCLang. I hope you'll find the language very useful! There's still much more to learn, so also look at the [Syntax documentation](/docs/mclang/syntax.md) for more features!

\- PurpleStripedUnicorn
