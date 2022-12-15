# Setup
 - The project is hosted in a public [GitHub repository](https://github.com/Falconhurst42/QA-MClang), which contains all code from the [original MCLang repository](https://github.com/PurpleStripedUnicorn/MCLang) in addition to the QA files
 - ## Building MCLang
   - Instructions for building MCLang can be found in the [original repository](https://github.com/PurpleStripedUnicorn/MCLang#readme)
   - For Windows users, WSL [Ubuntu](https://ubuntu.com/wsl) environment is recommended
 - ## Configuring Tests
   - To setup GTest on WSL Ubuntu, follow [these instructions](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/)
 - ## Run Master Suite
   - Navigate CLI to `QA-MCLang/tests/`
   - Run `cmake CMakeLists.txt` to configure gtest for the folder on your machine
   - To build tests, run `make`
   - To run tests, run `./runTests`

# makeDP
This is a makefile to build a datapack directly with MCLang
 - Paths
   - `SRC_BASE` - where it will look for the .mcl
     - Default: `datapacks/source`
   - `OUT_BASE` - where it will put the completed pack
     - Default: datapacks/compiled
   - `WORLD_BASE` - base location of minecraft worlds
     - Default: `/mnt/c/Users/{ethan}/AppData/Roaming/.minecraft/saves`
     - Update this yourself
 - Args
   - `pack` - the name of the `.mcl` file you wish to compile (without the `.mcl`)
     - the make looks for `{SRC_BASE}/{pack}.mcl`
   - `world` (optional) - the name of the minecraft world you want to copy the pack to
 - Example
   - `make -f makeDP world=MCLang pack=test`
 - In Minecraft
   - `/reload`
   - The pack should be loaded, but you can check with `/datapack list` and enable with `/datapack enable {pack}`