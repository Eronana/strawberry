#ifndef _VM_H_
#define _VM_H_

#include "vm_opcode.h"
#include "vm_data.h"
#include <memory>
#include <stack>
using namespace std;

#define HEADER_MAGIC 0x4D414552
#define GET_OP_LIST(NAME) OP_##NAME,

#define GET_FUNC_NAME(NAME) F_##NAME
#define GET_FUNC_LIST(NAME) &VirtualMachine::GET_FUNC_NAME(NAME),
#define DECL_FUNC(NAME) void GET_FUNC_NAME(NAME)();
#define GET_LIST_SIZE_F(NAME) 1+
#define GET_LIST_SIZE(LIST) (LIST(GET_LIST_SIZE_F)0)
enum OPCODE {OP_CODE(GET_OP_LIST)};

class VirtualMachine
{
  private:
    struct BINARY_CODE
    {
        size_t int_table_size,
            float_table_size,
            string_table_size,
            string_raw_size,
            code_raw_size;
        size_t stack_size;
        size_t entry_point;
    }config;

    unique_ptr<NULL_TYPE[]> int_table;
    unique_ptr<FLOAT_TYPE[]> float_table;
    unique_ptr<char*[]> string_table;
    unique_ptr<char[]> string_raw;
    unique_ptr<CODE[]> code_raw;

    struct StackFrame
    {
        int sp,ip;
    };

    unique_ptr<V_VALUE[]> v_stack;
    V_VALUE *l_stack;
    stack<StackFrame> stack_frame;
    bool parse_data(char *filename);
    OP_CODE(DECL_FUNC)
    void (VirtualMachine::*op_func[GET_LIST_SIZE(OP_CODE)])()={OP_CODE(GET_FUNC_LIST)};
    V_VALUE reg_this,reg_ret;
    int array_last_index;
    string object_last_key;
    int ip;
  public:

    bool load(const char *file);
    void run();
    void registerNativeFunction(const string &name,NATIVE_FUNCTION_TYPE func);
};

#endif
