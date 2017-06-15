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
#define v_external v_stack[stack_frame.top().size].v_function.external
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
    v_stack.push((*v_external)[get_int32()]);
}

DEF_FUNC(STORE_EXTERNAL)
{
    (*v_external)[get_int32()]=STOP;
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
    auto prototype = newObject();
    (*prototype)["__proto__"]=reg_system["prototypes"][string("Function")];
    STOP.v_function={get_int32(),external,prototype};
}

DEF_FUNC(SET_THIS)
{
    reg_this=SPOP;
}

DEF_FUNC(GET_THIS)
{
    v_stack.push(reg_this);
}

DEF_FUNC(RESET_THIS)
{
    reg_this=v_stack.top(1);
}

DEF_FUNC(ARRAY_PUSH)
{
    reg_this.v_array->push_back(SPOP);
}
void object_get(const V_VALUE &obj, const string &key, V_VALUE &val);
void object_get(OBJECT_TYPE obj, const string &key, V_VALUE &val)
{
    if(obj->count(key))val=(*obj)[key];
    else if(obj->count("__proto__"))object_get((*obj)["__proto__"],key,val);
    else val.setNull();
}

void object_get(const V_VALUE &obj, const string &key, V_VALUE &val)
{
    if(obj.type!=T_OBJECT)val.setNull();
    else object_get(obj.v_object,key,val);
}

void object_set(const V_VALUE &obj, const string &key, V_VALUE &val)
{
    (*obj.v_object)[key]=val;
}

void VirtualMachine::object_proto_get(const string &proto,const string &key, V_VALUE &val)
{
    V_VALUE &prototypes=reg_system["prototypes"];
    if(prototypes.type!=T_OBJECT)val.setNull();
    else object_get(prototypes[proto],key,val);
}
bool isnumber(const string &s)
{
    for(int i=0;i<s.length();i++)if(s[i]<'0'||s[i]>'9')return false;
    return true;
}
DEF_FUNC(OBJECT_GET)
{
    switch(reg_this.type)
    {
        case T_STRING:
            if(STOP.type==T_STRING)
            {
                object_proto_get("String",STOP.toString(),STOP);
                if(STOP.type!=T_NULL)break;
            }
            if(isnumber(STOP.toString()))
            {
                array_last_index=STOP.toInt();
                if(array_last_index>=0&&array_last_index<reg_this.v_string->length())
                {
                    STOP.type=T_STRING;
                    STOP.v_string=newString();
                    *STOP.v_string=(*reg_this.v_string)[array_last_index];
                    break;
                }
            }
            STOP.setNull();
            break;
        case T_ARRAY:
            if(STOP.type==T_STRING)
            {
                object_proto_get("Array",STOP.toString(),STOP);
                if(STOP.type!=T_NULL)break;
            }
            if(isnumber(STOP.toString()))
            {
                array_last_index=STOP.toInt();
                if(array_last_index>=0&&array_last_index<reg_this.v_array->size())
                {
                    STOP=(*reg_this.v_array)[array_last_index];
                    break;
                }
            }
            STOP.setNull();
            break;
        case T_OBJECT:
            array_last_index=-1;
            object_last_key=STOP.toString();
            object_get(reg_this,object_last_key,STOP);
            break;
        case T_INT:
            object_proto_get("Int",STOP.toString(),STOP);
            break;
        case T_FLOAT:
            object_proto_get("Float",STOP.toString(),STOP);
            break;
        case T_BOOL:
            object_proto_get("Bool",STOP.toString(),STOP);
            break;
        case T_FUNCTION:
            if(reg_this.type==T_FUNCTION&&STOP.toString()=="prototype")
            {
                STOP.type=T_OBJECT;
                STOP.v_object=reg_this.v_function.prototype;
                break;
            }
        case T_NATIVE_FUNCTION:
            object_proto_get("Function",STOP.toString(),STOP);
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
        object_set(reg_this,key,STOP);
    }
    
    else if(reg_this.type==T_ARRAY)
    {
        int key=SPOP.toInt();
        (*reg_this.v_array)[key]=STOP;
    }
    else STOP.setNull();
}

DEF_FUNC(OBJECT_RESET)
{
    if(array_last_index!=-1)(*reg_this.v_array)[array_last_index]=STOP;
    else object_set(reg_this,object_last_key,STOP);
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

template<typename T>
V_VALUE &add_object(T &obj, const string &key)
{
    obj[key].type=T_OBJECT;
    obj[key].v_object=newObject();
    return obj[key];
}

VirtualMachine::VirtualMachine()
{
    BUILDIN_FUNC_LIST(REG_BUILDIN_FUNC)
    setRoot(&v_stack);
    addExtraRoot(&reg_this);
    addExtraRoot(&reg_ret);

    auto prototypes = add_object(reg_system,"prototypes");
    auto Object = add_object(prototypes,"Object");
    auto Array = add_object(prototypes,"Array");
    auto String = add_object(prototypes,"String");
    auto Number = add_object(prototypes,"Number");
    auto Float = add_object(prototypes,"Float");
    auto Int = add_object(prototypes,"Int");
    auto Bool = add_object(prototypes,"Bool");
    auto Function = add_object(prototypes,"Function");
    Function[string("new")] = reg_system["__this_new__"];
    object_set(Array,"__proto__",Object);
    object_set(String,"__proto__",Object);
    object_set(Number,"__proto__",Object);
    object_set(Bool,"__proto__",Object);
    object_set(Function,"__proto__",Object);
    object_set(Float,"__proto__",Number);
    object_set(Int,"__proto__",Number);
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
        ip=func.v_function.func;
        KEEP_INS;
    }
    else if(func.type==T_NATIVE_FUNCTION)
    {
        auto size=v_stack.size();
        reg_ret.setNull();
        func.v_native_function(argc,*this);
        v_stack.resize(size);
        v_stack.sub(argc);
        STOP=reg_ret;
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

V_VALUE VirtualMachine::callReturn(int argc)
{
    DATATYPE type=v_stack.top(argc).type;
    call(argc);
    if(type==T_FUNCTION)runReturn();
    return SPOP;
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
    gc_enable=false;
    next_ip=ip+CUR_INS_LEN;
    if(CUR_INS>=0&&CUR_INS<=OP_HALT)(this->*op_func[CUR_INS])();
    else printf("Unknow instructor: %d\n",CUR_INS);
    //printf("STACK SIZE: %d\n",v_stack.size());
    //puts("-----------------");
    ip=next_ip;
    gc_enable=true;
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


