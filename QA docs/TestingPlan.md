# Testing Plan
## MCLang 0.3.4-alpha
### Created 10/26/2022 by Adam Callahan, Brendan van Diest, Ethan Worth, 
### Updated 11/16/2022 by Ethan Worth
 - Specifiy pywinauto integration methodology under "How we are combining our tests"
 - Added "Code Review" Section

# To Add?
 - ## Product View (static analysis, code coverage)
   - dicussion of potential methods (need to alter the MCLang build)
 - ## User View
   - -ilities
     - Reliability (stress test)

<br/>

# Table of Contents
 - ## Purpose of Document
 - ## System Overview
 - ## Coding Practices
   - Datatypes
   - Variables
   - Functions
   - Tests
 - ## Relevant Terms
 - ## What we are testing
 - ## What we are not testing
 - ## Exploratory Testing
 - ## How we are combining our tests
 - ## Code Review
 - ## Bug Handling

<br/>

# Purpose of Document
This plan documents our methodology for testing the open-source programming language [MCLang](https://github.com/PurpleStripedUnicorn/MCLang), while compiles C-like syntax to a Minecraft datapack. Our testing is aimed at ensuring that the language features implemented so far by the developer behave reasonably and consistently regardless of the source code provided. This document is intended to formalize and direct the efforts of the current testing team, as well as providing onboarding documentation for any other developers or testers.

<br/>

# System Overview
The MCLang project targets Ubuntu 20.04.3. As such, all testers should use a Ubuntu machine or the Windows Subsystem for Linux with an Ubuntu distribution. All testing files are hosted and source-controlled through a [public GitHub repository](https://github.com/Falconhurst42/QA-MClang), dubbed "QA-MCLang" which is a superset of the [original "MCLang" repository](https://github.com/PurpleStripedUnicorn/MCLang). All tests target a local compilation of MCLang, built using `make` and `g++` 9.3.0 per the original repository’s `README`. Low-level testing is automated using a modern distribution of GoogleTest. Centralized testing libraries for C++/GTest are developed in `QA-MCLang/tests/universal`, but for source-control reasons, testers will develop tests in individual subfolders under `QA-MCLang/tests/<their name>`.

<br/>

# Coding Practices

## Language
Automated tests for C++ and GTest, are to be compiled with g++ 9.3.0. As such, modern C++ syntax and features are encouraged up to C++17, but later versions are not supported. Use of `using namespace std` is strongly discouraged, especially in header files. Testing headers make use of Unix header such as `sys/stat.h` and `dirent.h` for interacting with the file structure. 

## Header files
All generic functions and classes (meant to be used in more than one test) should be placed in an appropriate header file in the `QA-MCLang/tests/universal`. Configuration constants and parameters should also be placed in these headers. However, instatiations of global *__objects__* for use in testing should be placed in a `.h` file in the *__tester's subfolder__*. 

Header files should each begin with a descriptive header comment in the following format
```
// Filename.ext
// Description
//  cont...
//  cont...
// Created: Tester, Date
// [Updated: Tester, date]*
```

Header files should also be divide into sections using block comments of the following form
```
/***************************************|
|                                       |
|             File Checkers             |
|                                       |
|***************************************/
```

## Datatypes
Datatypes, such as classes, structs and typdefs, should have short (preferrably 1 or 2 word) names which clearly and concisely describe the data object they represent. PascalCase is preferred, i.e. `Datapack`, `CommandResult`, `Datapack::Function`. Aggressive typing and abstraction is encouraged to group related types and provide aliasing for the many different types of `string` which appear throughout testing. Furthermore, classes within classes are useful for describing types-within-types and providing a form of namespacing distrinction (when necessary). Related datatypes and functions should be combined into a singular header. A division of `.h` and `.cpp` files is preferred for larger (~50+ lines) header files.
 - ## TODO: commenting standards for datatypes


## Variables
Variables names should be short, but still representative, written in snake_case. Single-character variable names should be avoided, except as loop variables. Constants should always be used liberally in place of so-called "magic numbers", and should be denoted with ALL_CAPS. If an explicit value is used more than once, there's a good chance it should be a constant. Examples: `out`, `filename`, `BASE_PATH`
 - ## TODO: commenting standards for variables

## Functions
Functions should be created and leveraged to provide standardization for generic tasks, reduce duplicated code, and allow for centralized changes to testing strategies. Pass-by-reference should not be used as a means of returning data. If multiple values must be returned, a container such as `std::vector`, `std::pair`, or a custom `struct` should be used. Raw pointers should be avoided. Teh `const` qualifier should be used on functions (`foo() const`) and referenced arguments (`const var&`) wherever possible. Functions should have concise but possibly lengthy names which clearly describe their purpose. Formats such as `actionDescriptorObject` or `objectExists` are preferred, i.e. `getFileContents`, `directoryExists`, or `cleanupBuildFiles`. Functions which are intended to combine multiple associated GTest calls should use the naming conventions of GTest functions, i.e. `VERIFY_GOOD_BUILD`.
 - ## TODO: commenting standards for functions

## Tests
Test suites should focus on a single feature of MCLang. Individual tests should focus on a single dimension or edge-case of that feature. If it is necessary to enumerate many nearly-equivalent tests to cover several combinations of testing variables, it might be advisable to develope a generalize function to allow iterating over these cases with a single test. That said, overfunctionalization can be wasteful and dividing behaviors into individual tests is useful for pinpointing failures. Test suites should be named with PascalCase. Individual tests are named with Snaked_PascalCase. It may be necessary to express multiple objects in a single name (i.e. `TickNI_NoVars`), which may be divided using an underscore for clarity. However, test names should remain brief. Avoid unnecessary information such as `VerifyBuild_HelloWorld_BuildSucceeds`.

Test suites should be divided into individual header files, which begin with a header of the form
```cpp
// Test Suite
// Description
// Tester Created, Date, MCLang Version
// [Tester updated, date, version]
```
Likewise, individual tests should be commented with a header with the form
```cpp
// Test Suite
// Description
// Tester Created, Date, Status (should pass, should fail)
// [Tester updated, date, status]
```
Furthermore, testers should ensure that every test cleans up all objects and files it created, and justify this fact with a comment at the end of the function (i.e. "// built files cleaned up by destructor when src falls out of scope").

<br/>

# Relevant Terms
 - How minecraft commands work
 - execute substatements
 - C-like conditionals

<br/>

# What we are testing
We are seeking to thoroughly MCLang's capacity to parse its C-like language into corresponding Minecraft function files. We will accomplish this using C++ to programatically generate input files, system command to compile via MCLang, and GTest to verify that the command results and compiled files align with expectations. Additionaly, we are exploring the possibility of using pywinauto, screenshots, and OCR to verify that compiled packs are available to use in Minecraft. However, that testing plan is still foggy, as verifying behavior in minecraft is difficult using conventional testing tools. 

The following is a rough enumerations of those features we aim to test with the C++-to-compiler-to-GTest pipeline.
 - File system
   - consisently handle invalid paths
   - paths with spaces
 - Comments
   - `//`
   - `/**/`
 - Individual functions()
   - `.mcfunction` for each named function
   - tag `.json` files for specialty functions `tick` and `load`
   - duplicate functions
 - `/execute` sub-clauses
   - Nested clauses
     - duplicate clauses
   - Model this in datapack? or just parse manually?
 - C-like statements
   - if
   - else if
   - else
   - for
   - Do inaccessible branches get compiled?
 - `const str` parsing
   - Verify that strings are properly substituted through execute subclauses
   - Is whitespace being parsed correctly?
   - Escape characters in explicit strings
   - Containing comment characters, clock comments
 - Compile-time arithmetic
   - DBZ errors 
   - Verify each individual operation (+, -, /, *, %)
   - Boolean comparison
 - Variable declaration and manipulation
   - declaration in absence of a `load` function
 - Preprocessor
   - `#define`
     - special characters: `\`, `/`, etc
   - `#include`
     - invalid paths
     - duplicate function names
     - cyclic includes
   - Namespacing
     - `-n` compiler args
     - conflicting `namespace` delcarations
     - Place the namespacer at different places throughout the file
     - interaction with `#include`
 - MCLang’s recognition of valid minecraft commands
   - `/?`
   - platform specific
   - initeraction with preprocessor `#define`

<br/>

# What we are not testing
Verifying that datapacks generate the desired effects when run in Minecraft is beyond the scope of this plan. The furthest exploration of Minecraft functionality currently considered is simply that functions are accessible through the `/function` command's auto-fill.

<br/>

# Exploratory Testing
Each group member individually performed exploratory testing of the MCLang project, seeking to better understand its behavior. Specifically, we sought to explore features of the parser, understand the logic of generated packs, and experiment with compiled behavior in Minecraft. The particular activities and conclusions of each group member are as follows:
 - ## Adam
   I experimented with inaccesible if-statements and returns. MCLang recognizes that anything within an if (False) block should not be compiled, however it still makes the MCLang dummy scoreboard .mcfunction file, just without the code inside the if statement. It does not recognize non boolean comparisons, for example if (2 == 3) behaves differently than the false block and still is compiled normally. Everything after a return statement is not compiled and seems to be completely ignored by the program, however if you put a return inside of an if-statement that always gets reached, for example if (true), then it still compiles the code after that return. I will try to write a few tests cases to check and verify these different behaviors.
 - ## Brendan
   I explored more minecraft commands to see how each compiles/behaves. Specifically, I looked into `time`, `weather`, `spawn`, `kill`, `teleport`, and `gamemode`. Each minecraft command worked in-game (as expected). The main issue is having commands run back to back. If I used 'gamemode creative' then 'gamemode survival' the second command would not execute. I believe that is caused from Minecraft not being able to work as fast. 
 - ## Ethan
   I specfically explored the consequences of the preprocessor and a couple quirks of the parser in general. This is in part building off the code review where we hit the `#define` code. I'm concluding that the parser handles omitted whitespace well (in a very C-like way), however the trend of naive mincraft parsing continues. You can put returns in strings and they will be copied blingly into the commands, which breaks the datapack (but doesn't through an error). In my mind, this could be considered a bug.

   I also noticed that define statements let you make oneliners which include mincraft commands (which you couldn't do without them since normaly MC commands are only recognized at the beginning of a line). Furthermore, the syntax follows C convetions with brackless if statements and using `\` to do mutli-line defines. Finally, nested mutli-target `execute` clauses can exponentially grow the number of commands executed by a pack.  that's not really a bug, just a great was to crash and corrupt your world by summoning 100,00 bats.

<br/>

# How we are combining our tests
Currently GTest tests are being developed in separate folders for convenience to avoid Git conflicts. However, a system must be developed to combine these disparate tests into a fully automated, unified test suite. This requires standardization of testing methods, as well as education into the use of git branches.

Furthermore, we plan on verifying the datapacks behavior in Minecraft using the Python library pywinauto. This functionality will be integrated with C++ by launching a Python script at the beginning of the GTest C++ files using a system command. The two programs will communicate data using files placed in a designated "Python" folder. Specifically, an individual GTests will move a compiled datapack into the python folder for pywinauto to process asynchronously. Pywinauto will run a standard test and output its results in a corresponding text file. The final GTest suite will consist of tests corresponding to each of the tests which called pywinauto. Each test will wait for pywinauto to create the corresponding output file and perform verification on the results. When all GTests are complete, C++ will create a final file which informs Python that it can terminate. As a rule, if C++ creates a file/folder in the python subfolder, Python will be the one to delete it once it has read and received the message (and vice versa).

There were definite road blocks that prevented us from creating a robust Python script. The most important road block is not being able to properly find elements in the Minecraft menus. We found a solution using loops of `keyboard.send_keys("{TAB}")` to navigate the menus, however, if we did not have our MCLang world as our last accessed world, the script would not open the correct world. We deemed that this solution was not good enough. There were too many things we had to do for the script to work making it more of a chore to run, especially across our different clients. 

The function `print_control_identifiers()` prints all the dialogs from the current application, which is what we used to find the title of Minecraft, but we were unable to find any information about the menus. That output gave us: 
```
  Dialog - 'Minecraft 1.19.3'    (L0, T0, R1920, B1080)
  ['Minecraft 1.19.3Dialog', 'Minecraft 1.19.3', 'Dialog']
  child_window(title="Minecraft 1.19.3", control_type="Window")
```
When using `print_control_identifiers()` there are pieces of information that pywinauto can use to find what you are looking for. It uses an algorithm called "best match." This essentially finds the closest element to what you set it as. To properly use this algorithm and `print_control_identifiers()` for our case would look like this:

   `app.Minecraft1193Dialog.print_control_identifiers()`

This should have returned the identifiers of the Minecraft dialog, but it would only output that same text as above. To navigate "deeper" into the Minecraft application, we tried various forms of that same command. For instance:

`app.Minecraft1193Dialog.Menu.print_control_identifiers()`

Each variation would throw this error:
```
pywinauto.findwindows.ElementNotFoundError: {'best_match': 'Menu', 'top_level_only': False, 'parent': <uia_element_info.UIAElementInfo - 'Minecraft 1.19.3', GLFW30, 853526>, 'backend': 'uia'}
```
The `{'best_match': 'Menu'` is the best match algorithm not finding 'Menu' when searching through the Minecraft dialog. Upon further research, we found that pywinauto works best with default Windows applications like Paint, File Explorer, and apps like that. It is also reasonable to say that the Minecraft devs didn't want anyone to be able to create a script to run their game, so making elements in the menus hard to access makes sense. The best way  we could find the titles of the menu elements is by looking at source code, which, if accessible, would be a miracle. 

# Code Reviews
A single unified, but cursory code review of the MCLang source code was completed on 11/11, which yielded some additional insight into the workings of the product. In this meeting, each group member pre-selected a nd presented interesting functions from the code base, and the group discussed the details and vulernabilities of each section. Further code review is encouraged as group members build out additional tests.

Test case reviews will be completed on 12/4 to allow group members to review and comment on the test cases created by other group members. The goal of these reviews is to ensure a unified vision and quality across test suites and move towards a cohesive final product. Each review will consist of a candid report from each group member on the state and coverage of their tests, followed by a peer review of specific tests, and finally a discussion of future steps.

<br/>

# Bug Handling
It is necessary to draw a distinction between bugs and limitations of the software. For example, while MCLang checks that a given command name is a valid Minecraft command, it does not verify more complicate syntax, such as argument types or JSON parsing. It would not be helpful to identify this behavior as a bug, as it is simply beyond the (current) scope of the software.

However, when MCLang behave inconsistently or unexpectedly when confronted with a problem within it's domain, that is a bug. For example, if it fails to reasonably compile a file but also does not throw an error message or terminate compilation, that can be marked as clearly erroneous behavior. If a test detects such behavior, the bug will be noted and submitted to the developer as a bug report. The test will remain in the suite, and a note will be made in it's test name and header comment that it is known to fail. No test should ever craash the execution, even if it fails.
