#include "PackFiles.h"

// builds the Datapack into PackFiles
//  optional -n argument for namespace
//  saves command result to Rez
//  places file at `makeSourcePath(name)`
//  outputs datapack to `makeCompiledPath(name)`
PackFiles::PackFiles(Datapack dp, std::string n) 
    : dp(dp), 
    name(dp.name) {
    // make source file
    std::ofstream f(dp._makeSourcePath());
    if(!f.is_open())
        throw std::runtime_error("Could not create file: " + dp._makeSourcePath());
    
    // write source file text
    f << dp.generateText();
    f.close();

    // run build
    rez = Datapack::BuildResult(buildFile(dp._makeSourcePath(), dp._makeCompiledPath(), n), n);

    // get default namespace
    // in-file namespace trumps
    if(dp.space.space != NO_NAMESPACE)
        namesp.space = dp.space;
    // check for dash_n arg from last build
    else if(rez.code != -1 && rez.dash_n != NO_NAMESPACE)
        namesp.space = rez.dash_n;
    // default
    else
        namesp.space = DEF_NAMESPACE;
}

/***************************************|
|                                       |
|                 GTests                |
|                                       |
|***************************************/
void PackFiles::VERIFY_GOOD_BUILD() const {
    // verify successful build
    VERIFY_BUILD_RESULT_SUCCESS();
    // verify file structure
    VERIFY_FILE_STRUCTURE();
    // verify function files are created
    VERIFY_FUNCTION_FILES();
}

void PackFiles::VERIFY_BUILD_RESULT_SUCCESS() const {
    // verify success code
    EXPECT_EQ(rez.code, 0);
    // verify no output
    EXPECT_EQ(rez.output, "");
}

// verify file structure
void PackFiles::VERIFY_FILE_STRUCTURE() const {
    const std::string BASE_PATH = _makeCompiledPath();
    EXPECT_TRUE(directoryExists(BASE_PATH));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + namesp.space));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + namesp.space + "/functions"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/minecraft"));
    EXPECT_TRUE(fileExists(BASE_PATH + "/pack.mcmeta"));
}

// verify function files are created
void PackFiles::VERIFY_FUNCTION_FILES() const {
    const std::string FUNCT_PATH = _makeFunctionsPath();
    const std::string TAG_PATH = _makeTagsPath();
    for(const Datapack::Function& f : dp.foos) {
        EXPECT_TRUE(_MCFunctionExists(f.getFormattedName()));
        // if f.name is in TAGGED_FUNCT_NAMES...
        if(std::find(TAGGED_FUNCT_NAMES.cbegin(), TAGGED_FUNCT_NAMES.cend(), f.getFormattedName()) != TAGGED_FUNCT_NAMES.cend()) {
            VERIFY_TAGGED_FUNCT(f, TAG_PATH);
        }
    }
}

// verify function files are created
void PackFiles::VERIFY_TAGGED_FUNCT(const Datapack::Function& f, std::string TAG_PATH) const {
    // check for file
    EXPECT_TRUE(fileExists(TAG_PATH + "/" + f.getFormattedName() + ".json"));
    // check file contents
    EXPECT_EQ(getFileContents(TAG_PATH + "/" + f.getFormattedName() + ".json"), "{\"values\":[\"" + namesp.space + ":" + f.getFormattedName() + "\"]}");
}

// verify build result indicates failure
//  - without successful return code
//  - with output containing `err`
//  - datapack folder not created
void PackFiles::VERIFY_BAD_BUILD(const Datapack& src, std::string err) const {
    // verify success code
    EXPECT_NE(src.rez.code, 0);
    // verify err in output
    if(err != "")
        EXPECT_TRUE(inStr(src.rez.output, err));

    // verify file structure
    const std::string BASE_PATH = src._makeCompiledPath();
    EXPECT_FALSE(directoryExists(BASE_PATH));
}

/***************************************|
|                                       |
|                Cleanup                |
|                                       |
|***************************************/
// cleans up the pack files
void PackFiles::_cleanupBuildFiles() {
    // (assuming) only files to clean up if build succeeded
    if(rez.code != -1) {
        // clean up files
        cleanupBuildFiles(_makeSourcePath(), _makeCompiledPath());
        
        // mark as cleaned
        rez.code = -1;
    }
}