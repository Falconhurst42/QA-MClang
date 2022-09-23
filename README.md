
# MCLang - Programming Minecraft Datapacks

[![GitHub license](https://badgen.net/github/license/PurpleStripedUnicorn/MCLang)](https://github.com/PurpleStripedUnicorn/MCLang/blob/master/LICENSE)
[![Twitter](https://badgen.net/badge/icon/twitter/cyan?icon=twitter&label)](https://twitter.com/striped_purple)
[![GitHub latest commit](https://badgen.net/github/last-commit/PurpleStripedUnicorn/MCLang/main)](https://GitHub.com/PurpleStripedUnicorn/MCLang/commit/)

MCLang is programming language which can be converted to minecraft commands, using the MCLang compiler.

*Disclaimer: This project is not affiliated with Mojang or Minecraft in any way.*

## Installation

The MCLang compiler can be installed either by downloading it directly (recommended) or by building the project yourself.

### Direct download

The MCLang compiler can be downloaded directly by going to the [Releases](https://github.com/PurpleStripedUnicorn/MCLang/releases) page. The operating systems Windows and Linux are supported. The executable is downloaded directly and should be run in the command line.

### Building the project

You can also choose to build the project for yourself to make modifications or to use a later (unstable) version of MCLang. This can be done by downloading the source code here in GitHub, then in Linux run `make`* to build the MCLang compiler. The executable will be named `build/main`. You can also use `make os=WINDOWS`** (still in Linux) to build the project for Windows.

_* Compilation has only been tested on Ubuntu 20.04.3 LTS, g++ version 9.3.0._

_** Compilation has only been tested on Ubuntu 20.04.3 LTS, gcc version 9.3-win32._

## Usage

For a full explanation see the [Beginner's guide](/docs/mclang/beginnersguide.md).

First create an `.mcl` file with your code in it. It is recommended to place the compiler executable and the `.mcl` file in the same folder.

Your `.mcl` file can be compiled with following command format:
```
<executable-name> <input-filename> [<command-line-options>]
```
The datapack output will by default be put in a folder named `out_datapack` in the current working directory. This can be changed by using the `-o` option. The default namespace for the datapack is `dp`. This can be changed by using the `-n` option. For a full list of command line arguments, use `-h` or `--help`.

It is recommended to use the `-o` option to reference a location inside the datapacks folder in your Minecraft world, so you don't have to copy and paste the output folder every time you run the compiler.

**Warning: If there is already a datapack present in the output folder, this datapack will be overwritten. Be very careful when selecting an output folder and make regular backups!**

When the generated datapack is placed inside the datapacks folder, open the Minecraft world and you can use `dp:functionname` to run a function you defined in your `.mcl` file.

## Syntax

The syntax is very similar to that of C. Minecraft commands can be inserted by using `/` at the start of a line. An example program that says hello to a player named PurpleStriped 100 times is shown below:
```
void main() {
    for (int i = 0; i < 100; i++) {
        if ("entity @a[name=PurpleStriped]") {
            /tellraw @a[name=PurpleStriped] {"text":"Hello PurpleStriped!"}
        } else {
            /tellraw @a {"text":"There is no PurpleStriped :("}
        }
    }
}
```
After compilation and putting the generated datapack in the correct folder, this function can be run in Minecraft with `/function dp:main`. Refer to the [Syntax documentation](/docs/mclang/syntax.md) and the [Beginner's guide](/docs/mclang/beginnersguide.md) for more information.

## Reporting bugs

This project is still an early stage of development, so there can be many bugs. Please report any bugs you find by creating an issue on GitHub and adding the "bug" label. Describe the problem as detailed as possible and provide an example where the bugs occurs.
