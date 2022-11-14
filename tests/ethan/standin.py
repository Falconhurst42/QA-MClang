import os

PYTHON_DIR = "../files/python"

conts = os.listdir(PYTHON_DIR)
dirs = [PYTHON_DIR + "/" + c for c in conts if os.path.isdir(PYTHON_DIR + "/" + c)]
files = [PYTHON_DIR + "/" + c for c in conts if os.path.isfile(PYTHON_DIR + "/" + c)]

print(f"Dirs: [{', '.join(dirs)}]")
print(f"Files: [{', '.join(files)}]")

"""
    system((std::string("cp -r ") + src._makeCompiledPath() + " " + src._makePythonPath()).c_str());
    system("python3 standin.py");

    #include "PythonCleanup.cpp"

    u.h
,
                  PYTHON_PATH = "../files/python/"

    u.c
// get location of python
inline std::string makePythonPath(std::string packname) {
    return PYTHON_PATH + packname;
}

    dp.h
    // get location of python
    std::string _makePythonPath() const {
        return makePythonPath(name);
    }
"""