#include "assembler.h"

int main()
{
    if(assemble("test/data/test.asm","test/data/test.bin"))return 0;
    return 1;
}