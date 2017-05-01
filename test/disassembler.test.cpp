#include "disassembler.h"
#include <cstdio>

int main()
{
    FILE *fp=fopen("test/data/test.bin","rb");
    disassemble(fp,stdout);
    fclose(fp);
    return 0;
}