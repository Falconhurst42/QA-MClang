
#include "compiler/argread.h"
#include "compiler/compiler.h"

int main(int argc, char *argv[]) {
    Compiler comp;
    ArgReader argReader(&comp);
    argReader.read(argc, argv);
    comp.compile();
    return 0;
}