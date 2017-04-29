#ifndef _VM_H_
#define _VM_H_

#include "vm_opcode.h"
#include "vm_data.h"
#include "codedata.h"
#include <memory>
#include <stack>
using namespace std;

#define GET_FUNC_NAME(NAME) F_##NAME
#define GET_FUNC_LIST(NAME) &VirtualMachine::GET_FUNC_NAME(NAME),
#define DECL_FUNC(NAME) void GET_FUNC_NAME(NAME)();
#define GET_LIST_SIZE_F(NAME) 1+
#define GET_LIST_SIZE(LIST) (LIST(GET_LIST_SIZE_F)0)


class VirtualMachine
{
  private:
    CodeConfig config;

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
    OP_CODE(DECL_FUNC)
    void (VirtualMachine::*op_func[GET_LIST_SIZE(OP_CODE)])()={OP_CODE(GET_FUNC_LIST)};
    V_VALUE reg_this,reg_ret;
    int array_last_index;
    string object_last_key;
    int ip;
  public:

    bool load(const char *filename);
    void run();
    void registerNativeFunction(const string &name,NATIVE_FUNCTION_TYPE func);
};

#endif
