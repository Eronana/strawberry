#ifndef _VM_H_
#define _VM_H_

#include "vm_opcode.h"
#include "vm_data.h"
#include "buildin.h"
#include "codedata.h"
#include "stack.hpp"
#include <memory>
#include <stack>
using namespace std;

#define SIZEOF(a) (sizeof(a)/sizeof(a[0]))
#define GET_FUNC_NAME(NAME) F_##NAME
#define GET_FUNC_LIST(NAME,LEN) &VirtualMachine::GET_FUNC_NAME(NAME),
#define DECL_FUNC(NAME,LEN) void GET_FUNC_NAME(NAME)();
#define GET_LIST_SIZE_F(NAME,LEN) 1+
#define GET_LIST_SIZE() (OP_CODE(GET_LIST_SIZE_F)0)

#define DECL_FRIEND_BUILDIN_FUNC_SIGN(NAME) friend BUILD_FUNC_SIGN(NAME);


OPCODE get_ins(CODE code);
int get_A(CODE code);
int get_a(CODE code);
int get_b(CODE code);
class VirtualMachine
{
  private:
    struct StackFrame
    {
        int sp,argc,ip;
    };
    CodeData code_data;
    Stack<V_VALUE> v_stack;
    int l_stack;
    V_VALUE v_external;
    stack<StackFrame> stack_frame;
    OP_CODE(DECL_FUNC)
    void (VirtualMachine::*op_func[GET_LIST_SIZE()])()={OP_CODE(GET_FUNC_LIST)};
    V_VALUE reg_this,reg_ret;
    map<string,V_VALUE> reg_system;
    int array_last_index;
    string object_last_key;
    int ip,next_ip;
    int8_t get_int8();
    int16_t get_int16();
    int32_t get_int32();
    void push();
    void push(const V_VALUE &a);
    void push(bool a);
    void push(int a);
    void push(double a);
    void push(const char *);
    void push(const string &s);
    void call(int argc);
    void callReturn(int argc,const V_VALUE &func);
    void runReturn();
    void runStep();

  public:
    VirtualMachine();
    bool load(const char *filename);
    void run();
    void registerNativeFunction(const string &name,NATIVE_FUNCTION_TYPE func);
    BUILDIN_FUNC_LIST(DECL_FRIEND_BUILDIN_FUNC_SIGN)
};

#endif
