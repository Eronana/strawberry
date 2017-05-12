#include "vm.h"
#include "memory.h"
#include "disassembler.h"
#include <memory>
#include <cstdio>

#define EXTENT_INT(a,N) ((a)|(~((((a)&(1<<(N-1)))<<1)-1)))
#define DEF_FUNC(NAME) void VirtualMachine::GET_FUNC_NAME(NAME)()
#define CUR_INS code_data.code_raw[ip]
#define NEXT_INS code_data.code_raw[next_ip]
#define CUR_INS_LEN op_length[CUR_INS]
#define SKIP_INS next_ip+=op_length[NEXT_INS]
#define KEEP_INS next_ip=ip
const char *type_name[]={
    "Null",
    "Bool",
    "Int",
    "Float",
    "String",
    "Array",
    "Object",
    "Native Function",
    "Function"
};
int8_t VirtualMachine::get_int8()
{
    return (int8_t)code_data.code_raw[ip+1];
}
int16_t VirtualMachine::get_int16()
{
    return *(int16_t*)&code_data.code_raw[ip+1];
}
int32_t VirtualMachine::get_int32()
{
    return *(int32_t*)&code_data.code_raw[ip+1];
}

#define STOP v_stack.top()
#define SPOP v_stack.pop()


DEF_FUNC(LOAD_GLOBAL)
{
    v_stack.push(v_stack[get_int32()]);
}


DEF_FUNC(STORE_GLOBAL)
{
    v_stack[get_int32()]=STOP;
}

DEF_FUNC(LOAD)
{
    v_stack.push(v_stack[get_int32()+l_stack]);
}
    
DEF_FUNC(STORE)
{
    v_stack[get_int32()+l_stack]=STOP;
}

DEF_FUNC(LOAD_EXTERNAL)
{
    v_stack.push((*v_external.v_array)[get_int32()]);
}

DEF_FUNC(STORE_EXTERNAL)
{
    (*v_external.v_array)[get_int32()]=STOP;
}

DEF_FUNC(NPUSH)
{
    push();
}

DEF_FUNC(BPUSH)
{
    push(bool(get_int8()));
}

DEF_FUNC(IPUSH)
{
    push(code_data.int_table[get_int32()]);
}

DEF_FUNC(FPUSH)
{
    push(code_data.float_table[get_int32()]);
}

DEF_FUNC(SPUSH)
{
    push(code_data.string_table[get_int32()]);
}

DEF_FUNC(POP)
{
    v_stack.sub();
}


DEF_FUNC(NEW_ARRAY)
{
    v_stack.add();
    STOP.type=T_ARRAY;
    STOP.v_array=newArray();
    reg_this=STOP;
}

DEF_FUNC(NEW_OBJECT)
{
    v_stack.add();
    STOP.type=T_OBJECT;
    STOP.v_object=newObject();
    reg_this=STOP;
}

DEF_FUNC(CREATE_FUNCTION)
{
    ARRAY_TYPE external=STOP.v_array;
    STOP.type=T_FUNCTION;
    STOP.v_function={get_int32(),external};
}

DEF_FUNC(SET_THIS)
{
    reg_this=SPOP;
}

DEF_FUNC(RESET_THIS)
{
    reg_this=v_stack.top(1);
}

DEF_FUNC(ARRAY_PUSH)
{
    reg_this.v_array->push_back(SPOP);
}

DEF_FUNC(OBJECT_GET)
{
    switch(reg_this.type)
    {
        case T_STRING:
            array_last_index=STOP.toInt();
            STOP.type=T_STRING;
            STOP.v_string=newString();
            *STOP.v_string=(*reg_this.v_string)[array_last_index];
            break;
        case T_ARRAY:
            array_last_index=STOP.toInt();
            STOP=(*reg_this.v_array)[array_last_index];
            break;
        case T_OBJECT:
            array_last_index=-1;
            object_last_key=STOP.toString();
            STOP=(*reg_this.v_object)[object_last_key];
            break;
        default:
            STOP.setNull();
    }
}

DEF_FUNC(OBJECT_SET)
{
    if(reg_this.type==T_OBJECT)
    {
        string key=SPOP.toString();
        (*reg_this.v_object)[key]=STOP;
    }
    
    else if(reg_this.type==T_ARRAY)
    {
        int key=SPOP.toInt();
        (*reg_this.v_array)[key]=STOP;
    }
}

DEF_FUNC(OBJECT_RESET)
{
    if(array_last_index!=-1)(*reg_this.v_array)[array_last_index]=STOP;
    else(*reg_this.v_object)[object_last_key]=STOP;
}


DEF_FUNC(DUP)
{
    v_stack.push(STOP);
}

DEF_FUNC(SWAP)
{
    swap(STOP,v_stack.top(1));
}

DEF_FUNC(INC)
{
    ++STOP;
}

DEF_FUNC(DEC)
{
    --STOP;
}

DEF_FUNC(POS)
{
    STOP.positive();
}

DEF_FUNC(NEG)
{
    STOP.negative();
}

DEF_FUNC(BNOT)
{
    STOP.bnot();
}

DEF_FUNC(LNOT)
{
    STOP.lnot();
}

DEF_FUNC(TYPEOF)
{
    STOP.v_string=newString();
    *STOP.v_string=type_name[STOP.type];
    STOP.type=T_STRING;
}

DEF_FUNC(MUL)
{
    auto a=SPOP;
    STOP*=a;
}
DEF_FUNC(DIV)
{
    auto a=SPOP;
    STOP/=a;
}
DEF_FUNC(MOD)
{
    auto a=SPOP;
    STOP%=a;
}
DEF_FUNC(ADD)
{
    auto a=SPOP;
    STOP+=a;
}
DEF_FUNC(SUB)
{
    auto a=SPOP;
    STOP-=a;
}
DEF_FUNC(BAND)
{
    auto a=SPOP;
    STOP&=a;
}
DEF_FUNC(XOR)
{
    auto a=SPOP;
    STOP^=a;
}
DEF_FUNC(BOR)
{
    auto a=SPOP;
    STOP|=a;
}
DEF_FUNC(LAND)
{
    auto a=SPOP;
    STOP.land(a);
}
DEF_FUNC(LOR)
{
    auto a=SPOP;
    STOP.lor(a);
}
DEF_FUNC(SHL)
{
    auto a=SPOP;
    STOP<<=a;
}
DEF_FUNC(SHR)
{
    auto a=SPOP;
    STOP>>=a;
}

DEF_FUNC(LESS)
{
    auto a=SPOP;
    STOP.setBool(STOP<a);
}
DEF_FUNC(GT)
{
    auto a=SPOP;
    STOP.setBool(STOP>a);
}
DEF_FUNC(LE)
{
    auto a=SPOP;
    STOP.setBool(STOP<=a);
}
DEF_FUNC(GE)
{
    auto a=SPOP;
    STOP.setBool(STOP>=a);
}
DEF_FUNC(EQ)
{
    auto a=SPOP;
    STOP.setBool(STOP==a);
}
DEF_FUNC(IEQ)
{
    auto a=SPOP;
    STOP.setBool(STOP!=a);
}
DEF_FUNC(ISTRUE)
{
    if(SPOP)SKIP_INS;
}
DEF_FUNC(ISFALSE)
{
    if(!SPOP)SKIP_INS;
}

DEF_FUNC(ADDSP)
{
    v_stack.add(get_int32());
}

DEF_FUNC(SUBSP)
{
    v_stack.sub(get_int32());
}

DEF_FUNC(JMP)
{
    ip=get_int32();
    KEEP_INS;
}

DEF_FUNC(CALL)
{
    call(get_int32());
}

DEF_FUNC(RET)
{
    l_stack=stack_frame.top().sp;
    ip=stack_frame.top().ip;
    KEEP_INS;
    reg_ret=STOP;
    v_stack.resize(stack_frame.top().size);
    STOP=reg_ret;
    stack_frame.pop();
}

DEF_FUNC(HALT)
{
    // nothing to do
}

#define REG_BUILDIN_FUNC(NAME) registerNativeFunction(#NAME,&GET_BUILDIN_FUNC_NAME(NAME));
VirtualMachine::VirtualMachine()
{
    BUILDIN_FUNC_LIST(REG_BUILDIN_FUNC)
    setRoot(&v_stack);
    addExtraRoot(&reg_this);
    addExtraRoot(&reg_ret);
    addExtraRoot(&v_external);
}

void VirtualMachine::push()
{
    v_stack.add();
    STOP.setNull();
}
void VirtualMachine::push(const V_VALUE &a)
{
    v_stack.push(a);
}
void VirtualMachine::push(bool a)
{
    v_stack.add();
    STOP.type=T_BOOL;
    STOP.v_bool=a;
}
void VirtualMachine::push(int a)
{
    v_stack.add();
    STOP.type=T_INT;
    STOP.v_int=a;
}
void VirtualMachine::push(double a)
{
    v_stack.add();
    STOP.type=T_FLOAT;
    STOP.v_float=a;
}
void VirtualMachine::push(const char *s)
{
    push(string(s));
}
void VirtualMachine::push(const string &s)
{
    v_stack.add();
    STOP.type=T_STRING;
    STOP.v_string=newString();
    *STOP.v_string=s;
}

void VirtualMachine::call(int argc)
{
    V_VALUE &func=v_stack.top(argc);
    if(func.type==T_FUNCTION)
    {
        stack_frame.push({l_stack,next_ip,v_stack.size()-argc});
        l_stack=v_stack.size();
        v_external.type=T_ARRAY;
        v_external.v_array=func.v_function.external;
        ip=func.v_function.func;
        KEEP_INS;
    }
    else if(func.type==T_NATIVE_FUNCTION)
    {
        auto size=v_stack.size();
        func.v_native_function(argc,*this);
        v_stack.resize(size);
        v_stack.sub(argc);
        STOP=reg_ret;
        reg_ret.setNull();
    }
}

bool VirtualMachine::load(const char *filename)
{
    if(!code_data.load(filename))return false;
    l_stack=0;
    v_stack.add();
    STOP.type=T_OBJECT;
    STOP.v_object=&reg_system;
    return true;
}
extern map<void*,pair<int,bool>> memory_table;

void VirtualMachine::callReturn(int argc)
{
    DATATYPE type=v_stack.top(argc).type;
    call(argc);
    if(type==T_FUNCTION)runReturn();
}

void VirtualMachine::runReturn()
{
    while(CUR_INS!=OP_RET)runStep();
    runStep();
}
void VirtualMachine::runStep()
{
    //discode(CUR_INS,code_data,stdout);
    //continue;
    next_ip=ip+CUR_INS_LEN;
    if(CUR_INS>=0&&CUR_INS<=OP_HALT)(this->*op_func[CUR_INS])();
    else printf("Unknow instructor: %d\n",CUR_INS);
    //printf("STACK SIZE: %d\n",v_stack.size());
    //puts("-----------------");
    ip=next_ip;
}
void VirtualMachine::run()
{
    unsigned int t=clock();
    ip=code_data.config.entry_point;
    int code_raw_size=code_data.config.code_raw_size;
    while(ip>=0&&ip<code_raw_size&&CUR_INS!=OP_HALT)runStep();
    printf("halt in %lu clocks\n",clock()-t);
    //printf("memory_table: %u\n",memory_table.size());
    //gc();printf("memory_table: %u\n",memory_table.size());
    
    //printf("STACK SIZE: %d\n",v_stack.size());
}

void VirtualMachine::registerNativeFunction(const string &name,NATIVE_FUNCTION_TYPE func)
{
    reg_system[name].type=T_NATIVE_FUNCTION;
    reg_system[name].v_native_function=func;
}


