// Commands
// Tests MCLangs recognition of valid/invalid minecraft commands
// Ethan (created), 12/7/2022, 0.3.4-alpha

#include <gtest/gtest.h>
#include "universal/test_utils.h"
#include "universal/utils.h"
#include "TestSetup.h"
#include <vector>
#include <string>

/***************************************|
|                                       |
|             All Commands              |
|                                       |
|***************************************/

// per https://minecraft.fandom.com/wiki/Commands#List_and_summary_of_commands
std::vector<std::string> ALL_CMDS = {
    "advancement", "attribute", "bossbar", "clear", "clone", "data", "datapack", 
    "defaultgamemode", "difficulty", "effect", "enchant", "execute", "experience", "fill" , 
    "forceload", "gamemode", "gamerule", "give" , "help", "item", "kill", 
    "locate", "loot", "msg", "particle", "place", "playsound", "recipe", "reload", "say", 
    "schedule", "scoreboard", "seed", "setblock", "setworldspawn", "spawnpoint", "spectate", 
    "spreadplayers", "stopsound", "summon", "tag", "team", "teleport", "tell", "tellraw", 
    "time", "title", "tm", "tp", "w", "weather", "worldborder", "xp",
// these cmds should be recognized but aren't
//    most (all?) only apply to mutliplayer servers
    "ban", "banlist", "debug", "deop", "jfr", "kick", "list", "me", "op", "pardon", "perf", 
    "publish", "save", "setidletimeout", "stop", "teammsg", "trigger", "whitelist"
}, 
// commands that have never been legal in Java
ILLEGAL_CMDS = {
    "ability", "alwaysday", "camerashake", "changesetting", "clearspawnpoint", "connect", 
    "damage", "daylock", "dedicatedwsserver", "dialogue", "event", "fog", "gametest", 
    "immutableworld", "mobevent", "music", "ops", "permission", "playanimation", "remove",
    "ride", "save", "setmaxplayers", "structure", "tickingarea", "titleraw", "toggledownfall", 
    "volumearea", "warden_spawn_tracker", "wb", "worldbuilder", "wsserve"
}, 
// commands that were legal, but then were removed
REMOVED_CMDS = {
    "?", "achievement", "banip", "blockdata", "broadcast", "chunk", "clearfixedinv", "detect",
    "entitydata", "executeasself", "home", "mixer", "placefeature", "position",
    "replaceitem", "resupply", "setfixedinvslot", "setfixedinvslots", "setspawn", "solid",
    "stats", "testfor", "testforblock", "testforblocks", "toggledownfall", "transferserver",
    "unban", "videostream", "videostreamaction", "warden_spawn_tracker",
// these cmds shouldn't be recognized but are
    "locatebiome", "replaceitem"
};

// Tests MCLangs recognition of valid minecraft commands
// Ethan (created), 12/7/2022, Currently Fails
//    these cmds should be recognized but aren't
//    most (all?) only apply to mutliplayer servers
//      "ban", "banlist", "debug", "deop", "jfr", "kick", "list", "me", "op", "pardon", "perf", 
//      "publish", "save", "setidletimeout", "stop", "teammsg", "trigger", "whitelist"
TEST(Commands, AllCommands) {
    // make DP
    std::string contents("");
    for(std::string str : ALL_CMDS)
        contents += std::string("    /") + str + "\n";
    Datapack src((std::vector<Datapack::Function>){ 
        {"void",
        "all_cmds",
        std::vector<Datapack::Variable>(),
        contents}
    });
    src.name = "Commands_AllCommands";

    // build
    src.build();

    // verify functions are there
    for(std::string str : ALL_CMDS) {
        EXPECT_TRUE(inStr(getMCFunction(src.name, DEF_NAMESPACE, "all_cmds"), str));
    }

    // verify good build
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// Tests MCLangs (non)recognition of invalid minecraft commands
// Ethan (created), 12/7/2022, Should Pass
TEST(Commands, InvalidCommands) {
    // make DP
    std::string contents("");
    for(std::string str : ILLEGAL_CMDS)
        contents += std::string("    /") + str + "\n";
    Datapack src((std::vector<Datapack::Function>){ 
        {"void",
        "invalid_cmds",
        std::vector<Datapack::Variable>(),
        contents}
    });
    src.name = "Commands_InvalidCommands";

    // build
    src.build();

    // verify functions are there
    // verify warnings
    for(std::string str : ILLEGAL_CMDS) {
        EXPECT_TRUE(inStr(getMCFunction(src.name, DEF_NAMESPACE, "invalid_cmds"), str));
        EXPECT_TRUE(inStr(src.rez.output, std::string("The given command \"") + str + "\" is not known to be a valid command."));
    }
    
    // verify good build (mark warnings)
    VERIFY_GOOD_BUILD(src, true);

    // built files cleaned up by destructor when src falls out of scope
}

// Tests MCLangs (non)recognition of removed minecraft commands
// Ethan (created), 12/7/2022, Currently Fails
//   these cmds shouldn't be recognized but are
//     "locatebiome", "replaceitem"
TEST(Commands, RemovedCommands) {
    // make DP
    std::string contents("");
    for(std::string str : REMOVED_CMDS)
        contents += std::string("    /") + str + "\n";
    Datapack src((std::vector<Datapack::Function>){ 
        {"void",
        "removed_cmds",
        std::vector<Datapack::Variable>(),
        contents}
    });
    src.name = "Commands_RemovedCommands";

    // build
    src.build();

    // verify functions are there
    // verify warnings
    for(std::string str : REMOVED_CMDS) {
        EXPECT_TRUE(inStr(getMCFunction(src.name, DEF_NAMESPACE, "removed_cmds"), str));
        EXPECT_TRUE(inStr(src.rez.output, std::string("The given command \"") + str + "\" is not known to be a valid command."));
    }
    
    // verify good build (mark warnings)
    VERIFY_GOOD_BUILD(src, true);

    // built files cleaned up by destructor when src falls out of scope
}


// Tests MCLangs handling of alternative casing
//   verifies that warning is sent but files are still created
// Ethan (created), 12/7/2022, Should Pass
TEST(Commands, CaseChanges) {
    // make DP
    Datapack src((std::vector<Datapack::Function>){ 
        {"void",
        "main",
        std::vector<Datapack::Variable>(),
        "/Say \"Hello World!\""}
    });
    src.name = "Commands_CaseChanges";

    // build
    src.build();

    // verify functions are there
    EXPECT_TRUE(inStr(getMCFunction(src.name, DEF_NAMESPACE, "main"), "Say \"Hello World!\""));
    // verify warnings
    EXPECT_TRUE(inStr(src.rez.output, std::string("The given command \"Say\" is not known to be a valid command.")));
    
    // verify good build (mark warnings)
    VERIFY_GOOD_BUILD(src, true);

    // built files cleaned up by destructor when src falls out of scope
}

// Tests MCLangs handling of "empty command
//   verifies that warning is sent but files are still created
// Ethan (created), 12/7/2022, Should Pass
TEST(Commands, EmptyCommand) {
    // make DP
    Datapack src((std::vector<Datapack::Function>){ 
        {"void",
        "main",
        std::vector<Datapack::Variable>(),
        "/ say \"Hello World!\""}
    });
    src.name = "Commands_CaseChanges";

    // build
    src.build();

    // verify functions are there
    EXPECT_TRUE(inStr(getMCFunction(src.name, DEF_NAMESPACE, "main"), " say \"Hello World!\""));
    // verify warnings
    EXPECT_TRUE(inStr(src.rez.output, std::string("The given command \"\" is not known to be a valid command.")));
    
    // verify good build (mark warnings)
    VERIFY_GOOD_BUILD(src, true);

    // built files cleaned up by destructor when src falls out of scope
}