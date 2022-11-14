# Setup
 - The project is hosted in a public [GitHub repository](https://github.com/Falconhurst42/QA-MClang), which contains all code from the [original MCLang repository](https://github.com/PurpleStripedUnicorn/MCLang) in addition to the QA files
 - ## Building MCLang
   - Instructions for building MCLang can be found in the [original repository](https://github.com/PurpleStripedUnicorn/MCLang#readme)
   - For Windows users, WSL [Ubuntu](https://ubuntu.com/wsl) environment is recommended
 - ## Configuring Tests
   - Each team member should perform their test in their own subfolder of the QA-MCLang repository under `tests/<their name>`
   - To setup GTest on WSL Ubuntu, follow [these instructions](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/), with the following deviations
   - At the end, place a copy of the `CMakeLists.txt` file into your testing subfolder
   - Navigate the Ubuntu (WSL) command line to the folder
   - Run `cmake CMakeLists.txt` to configure gtest for the folder
   - To build a project, run `make`
   - To run a project, run `./runTests`
 - ## Reference
   - For examples of testing using this system, check out existing subfolders from other users
 - NOTE: Git has been finicky with tracking files in testing subfolders, be careful when pushing to ensure all your files are being committed

`If you get an error running make for your gTests that references -lThreads::Threads, you can fix it by deleting " -lThreads::Threads" from end of CMakeFiles/runTests.dir/link.txt`

# Standard Practices
 - The `tests/universal/utils.h` defines certain default paths/names in the `Params` section
 - ### Filepaths
   - `.mcl` source files should be placed in `tests/files/sourcefiles`
   - compiled datapacks should be placed in `tests/files/datafiles/<DPName>`
   - All source and compiled files should be created for a sigle test and removed when the test is complete.
 - ### 

# Testing Infrastructure
 - The `tests/universal` folder contains helpful utilities for performing automated tests on MCLang
 - Contents
   - `utils.h` - defines general tools for manipulating files and executing system commands
     - These tools are low-level, useful for tests that use the software in specific and possibly unintended ways

        | Section | Summary |
        |-|-|
        | Params | Defines certain default paths/names |
        |Path Generators | standardized functions for generating relative paths to source/compiled files |
        | File Checkers | functions for determining if a files and directories exists as well as accessing their contents |
        | General Utils | generally useful functs
        | Execute Commands | Functionality to robustly execute system commands and return their results. Special functions for building particular source files using MCLang and for deleting the compiled files afterwards |
        | `CommandResult` | Struct to store the output (`str`) and return code (`int`) of a command execution
    - `Datapack.[h/cpp]` - defines a specialized class (really a struct) to standardize the representation and building of datapacks
      - The `Datapack` class standardizes and automates the functionality declared in `utils.h`. This makes it much more concise and convenient for most testing, but limits your ability to explore certain undefined behaviors, espcially related to file management and some syntax.
        | Section | Summary |
        |-|-|
        |Sub-Types| Declares several simple subtypes representing the components of a `Datapack` |
        | ` - Datapack::Variable` | Represents the type (`str`) and name (`str`) of a single (global) variable |
        | ` - Datapack::Function` | Represents the rtype (`str`), name (`str`), arguments (`vector<Variable>`), and body (`str`) of a single function |
        | ` - Datapack::Namespace` | Represents a single namespace (`str`) |
        | ` - Datapack::BuildResult` | Child of `CommandResult` which also stores the `-n` namespace argument used in the build command |
        | Member Variables | Pack name (`str`), namespace (`Namespace`) variables (`vector<Variable>`), functions (`vector<Function>`), build data (`BuildResult`) |
        | Constructor | Build a datapack given a set of functions, variables, and namespace |
        | Parsing & Generation | convert datapack contents to text |
        | Path Generators | mirrors of `utils.h` generators which automatically pass Datapack data |
        | Building | Manages the building and cleanup of the datapack |
        | Destructor | Datapack destructor automatically calls cleanup, so it is unnecessary to call it explicitly |
    - `test_utils.h` - defines standarized gtest functions to verify that a given datapack built as expected