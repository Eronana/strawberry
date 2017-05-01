#include "vm.h"

#define file "test/data/test.bin"
int main()
{
    VirtualMachine vm;
    if(!vm.load(file))puts("cannot load \"" file "\"");
    else puts("loaded");
    vm.run();
    return 0;
}