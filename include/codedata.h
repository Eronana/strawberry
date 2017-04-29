#ifndef _CODEDATA_H_
#define _CODEDATA_H_

#include <cstdlib>
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
#endif
