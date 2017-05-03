#ifndef _DISASSEMBLER_H_
#define _DISASSEMBLER_H_

#include <cstdio>
#include "vm_data.h"
#include "codedata.h"
bool disassemble(const char *binfile,const char *outfile);
bool disassemble(FILE *in,FILE *out);
void discode(CODE &op,CodeData &code_data,FILE *out);
#endif
