
# MCLang - Programming Minecraft Datapacks 
#

[![GitHub license](https://badgen.net/github/license/PurpleStripedUnicorn/MCLang)](https://github.com/PurpleStripedUnicorn/MCLang/blob/master/LICENSE)
[![Twitter](https://badgen.net/badge/icon/twitter/cyan?icon=twitter&label)](https://twitter.com/striped_purple)
[![GitHub latest commit](https://badgen.net/github/last-commit/PurpleStripedUnicorn/MCLang/main)](https://GitHub.com/PurpleStripedUnicorn/MCLang/commit/)

MCLang is programming language which can be converted to minecraft commands, using the MCLang compiler.

*Disclaimer: This project is not affiliated with Mojang or Minecraft in any way.*

## Building the project

You can also choose to build the project for yourself to make modifications or to use a later (unstable) version of MCLang. This can be done by downloading the source code here in GitHub, then in Linux run `make`* to build the MCLang compiler. The executable will be named `build/main`. You can also use `make os=WINDOWS`** (still in Linux) to build the project for Windows.

_* Compilation has only been tested on Ubuntu 20.04.3 LTS, g++ version 9.3.0._

_** Compilation has only been tested on Ubuntu 20.04.3 LTS, gcc version 9.3-win32._

## Testing Setup
 - ## Building MCLang
   - Instructions for building MCLang can be found above, perform them on your machine before trying to set up testing
   - For Windows users, WSL [Ubuntu](https://ubuntu.com/wsl) environment is recommended
 - ## Configuring Tests
   - To setup GTest on WSL Ubuntu, follow [these instructions](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/)
 - ## Run Master Suite
   - Navigate CLI to `QA-MCLang/tests/`
   - Run `cmake CMakeLists.txt` to configure gtest for the folder on your machine
   - To build tests, run `make`
   - To run tests, run `./runTests`