#ifndef _CODEDATA_H_
#define _CODEDATA_H_

#include <cstdlib>
#include <memory>
#include "vm_data.h"
using namespace std;
#define HEADER_MAGIC 0x4D414552

struct CodeConfig
{
    uint32_t magic;
    size_t int_table_size,
        float_table_size,
        string_table_size,
        string_raw_size,
        code_raw_size;
    size_t stack_size;
    size_t entry_point;
};

struct CodeData
{
    CodeConfig config;
    unique_ptr<NULL_TYPE[]> int_table;
    unique_ptr<FLOAT_TYPE[]> float_table;
    unique_ptr<char*[]> string_table;
    unique_ptr<char[]> string_raw;
    unique_ptr<CODE[]> code_raw;
    bool load(const char *filename);
    bool load(FILE *fp);
};

bool write_code_data(const char *filename,int entry_point,vector<int> int_table_list,vector<FLOAT_TYPE> float_table_list,vector<string> string_table_list,vector<CODE> code);
#endif

