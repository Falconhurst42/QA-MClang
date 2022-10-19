#include "Datapack.h"
#include "utils.h"
#include <string>
#include <vector>
#include <stdexcept>

/***************************************|
|                                       |
|              Sub-Types                |
|                                       |
|***************************************/
    std::string Datapack::Variable::getDeclaration() const {
        return type + " " + name;
    }

    std::string Datapack::Function::getText() const {
        std::string out("");
        out += rtype + " " + name + "(";
        for(const Variable& a : args)
            out += a.getDeclaration() + ", ";
        if(!args.empty()) {
            out.pop_back(); out.pop_back();
        }
        out += ") {\n" + body + "\n}";
        return out;
    }
    
/***************************************|
|                                       |
|         Parsing & Generation          |
|                                       |
|***************************************/
    std::string Datapack::generateText() const {
        std::string out("");
        if(space.space != NO_NAMESPACE) {
            out += space.getNamespacer() + "\n\n";
        }
        for(const Variable& v : vars) {
            out += v.getDeclaration() + ";\n";
        }
        for(const Function& f : foos) {
            out += f.getText() + "\n\n";
        }
        return out;
    }

    // determines the effective namespace. Accounts for in-file namespace and dash_n args of last build
    std::string Datapack::getPrimaryNamespace() const {
        // in-file namespace trumps
        if(space.space != NO_NAMESPACE)
            return space.space;
        // check for dash_n arg from last build
        if(rez.code != -1 && rez.dash_n != NO_NAMESPACE)
            return rez.dash_n;
        // default
        return DEF_NAMESPACE;
    }

/***************************************|
|                                       |
|               Building                |
|                                       |
|***************************************/
    // builds the Datapack and returns whether the build returned a successful code
    //  saves command result to Rez
    //  places file at `makeSourcePath(name)`
    //  outputs datapack to `makeCompiledPath(name)`
    //  optional -n argument for namespace
    bool Datapack::build(std::string n) {
        // make file
        std::ofstream f(getSourcePath());
        if(!f.is_open())
            throw std::runtime_error("Could not create file: " + getSourcePath());
        
        // pass text
        f << generateText();
        f.close();

        // cleanup previous build
        cleanupBuild();

        // run build
        rez = BuildResult(buildFile(getSourcePath(), getCompiledPath(), n), n);

        // return success
        return rez.code == 0;
    }

    // cleans up a given build
    void Datapack::cleanupBuild() {
        if(rez.code != -1) {
            // clean up files
            cleanupBuildFiles(getSourcePath(), getCompiledPath());
            
            // mark as cleaned
            rez.code = -1;
        }
    }