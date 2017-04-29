#include "vm.h"
#include "memory.h"
#include <memory>
#include <cstdio>

#define SIZEOF(a) (sizeof(a)/sizeof(a[0]))
#define GET_NAME_LIST(NAME) #NAME ,
#define EXTENT_INT(a,N) ((a)|(~((((a)&(1<<(N-1)))<<1)-1)))
#define DEF_FUNC(NAME) void VirtualMachine::GET_FUNC_NAME(NAME)()
#define CUR_INS code_raw[ip]
#define GET_A() int a=get_A(CUR_INS);V_VALUE &A=l_stack[a];
#define GET_AB() int a=get_a(CUR_INS),b=get_b(CUR_INS);V_VALUE &A=l_stack[a],&B=l_stack[b];
const char *op_name[]={OP_CODE(GET_NAME_LIST)};

const char *type_name[]={
    "null",
    "bool",
    "int",
    "float",
    "string",
    "array",
    "object",
    "native function",
    "function"
};

OPCODE get_ins(CODE code)
{
    return (OPCODE)(code>>26);
}

int get_A(CODE code)
{
    return EXTENT_INT(code&0x3ffffff,26);
}

int get_a(CODE code)
{
    return EXTENT_INT((code&0x3ffffff)>>13,13);
}

int get_b(CODE code)
{
    return EXTENT_INT(code&0x1fff,13);
}

DEF_FUNC(NEW_ARRAY)
{
    GET_A();
    A.type=T_ARRAY;
    A.v_array=NEW(ARRAY_OTYPE);
}

DEF_FUNC(NEW_OBJECT)
{
    GET_A();
    A.type=T_OBJECT;
    A.v_object=NEW(OBJECT_OTYPE);
}

DEF_FUNC(SET_THIS)
{
    GET_A();
    reg_this=A;
}

DEF_FUNC(OBJECT_GET)
{
    GET_AB();
    if(reg_this.type==T_OBJECT)
    {
        array_last_index=-1;
        B=(*reg_this.v_object)[object_last_key=A.toString()];
    }
    else if(reg_this.type==T_ARRAY)
    {
        B=(*reg_this.v_array)[array_last_index=A.toInt()];
    }
}

DEF_FUNC(OBJECT_SET)
{
    GET_AB();
    if(reg_this.type==T_OBJECT)(*reg_this.v_object)[A.toString()]=B;
    else if(reg_this.type==T_ARRAY)(*reg_this.v_array)[A.toInt()]=B;
}

DEF_FUNC(OBJECT_RESET)
{
    GET_A();
    if(array_last_index==-1)(*reg_this.v_array)[array_last_index]=A;
    else(*reg_this.v_object)[object_last_key]=A;
}

DEF_FUNC(ARRAY_PUSH)
{
    GET_A();
    reg_this.v_array->push_back(A);
}

DEF_FUNC(GET_GLOBAL)
{
    GET_AB();
    A=v_stack[b];
}

DEF_FUNC(SET_GLOBAL)
{
    GET_AB();
    v_stack[b]=A;
}

DEF_FUNC(SWAP)
{
    GET_AB();
    swap(A,B);
}

DEF_FUNC(NLOAD)
{
    GET_A();
    A.setNull();
}

DEF_FUNC(BLOAD)
{
    GET_AB();
    A.type=T_BOOL;
    A.v_bool=int_table[b];
}

DEF_FUNC(ILOAD)
{
    GET_AB();
    A.type=T_INT;
    A.v_int=int_table[b];
}

DEF_FUNC(FLOAD)
{
    GET_AB();
    A.type=T_FLOAT;
    A.v_float=float_table[b];
}

DEF_FUNC(SLOAD)
{
    GET_AB();
    A.type=T_STRING;
    A.v_string=NEW(STRING_OTYPE);
    *A.v_string=string_table[b];
}

DEF_FUNC(MOV)
{
    GET_AB();
    A=B;
}

DEF_FUNC(CALL)
{
    GET_AB();
    V_VALUE &func=l_stack[a-b-1];
    if(func.type==T_FUNCTION)
    {
        stack_frame.push({a,ip});
        l_stack+=a;
        ip=func.v_function-1;
    }
    else if(func.type==T_NATIVE_FUNCTION)
    {
        func.v_native_function(b,l_stack+a,reg_ret);
    }
}

DEF_FUNC(RET)
{
    GET_A();
    reg_ret=A;
    l_stack-=stack_frame.top().sp;
    ip=stack_frame.top().ip-1;
    stack_frame.pop();
}

DEF_FUNC(JMP)
{
    GET_A();
    ip+=a;
}

DEF_FUNC(POS)
{
    GET_A();
    A.positive();
}

DEF_FUNC(NEG)
{
    GET_A();
    A.negative();
}

DEF_FUNC(BNOT)
{
    GET_A();
    A.bnot();
}

DEF_FUNC(LNOT)
{
    GET_A();
    A.lnot();
}

DEF_FUNC(TYPEOF)
{
    GET_A();
    A.v_string=NEW(STRING_OTYPE);
    *A.v_string=type_name[A.type];
    A.type=T_STRING;
}

DEF_FUNC(INC)
{
    GET_A();
    ++A;
}

DEF_FUNC(DEC)
{
    GET_A();
    --A;
}

DEF_FUNC(MUL)
{
    GET_AB();
    A*=B;
}
DEF_FUNC(DIV)
{
    GET_AB();
    A/=B;
}
DEF_FUNC(MOD)
{
    GET_AB();
    A%=B;
}
DEF_FUNC(ADD)
{
    GET_AB();
    A+=B;
}
DEF_FUNC(SUB)
{
    GET_AB();
    A-=B;
}
DEF_FUNC(BAND)
{
    GET_AB();
    A&=B;
}
DEF_FUNC(XOR)
{
    GET_AB();
    A^=B;
}
DEF_FUNC(BOR)
{
    GET_AB();
    A|=B;
}
DEF_FUNC(LAND)
{
    GET_AB();
    A.land(B);
}
DEF_FUNC(LOR)
{
    GET_AB();
    A.lor(B);
}
DEF_FUNC(SHL)
{
    GET_AB();
    A<<=B;
}
DEF_FUNC(SHR)
{
    GET_AB();
    A>>=B;
}

DEF_FUNC(LESS)
{
    GET_AB();
    if(A<B)ip++;
}
DEF_FUNC(GT)
{
    GET_AB();
    if(A>B)ip++;
}
DEF_FUNC(LE)
{
    GET_AB();
    if(A<=B)ip++;
}
DEF_FUNC(GE)
{
    GET_AB();
    if(A>=B)ip++;
}
DEF_FUNC(EQ)
{
    GET_AB();
    if(A==B)ip++;
}
DEF_FUNC(IEQ)
{
    GET_AB();
    if(A!=B)ip++;
}
DEF_FUNC(ISTRUE)
{
    GET_A();
    if(A)ip++;
}
DEF_FUNC(ISFALSE)
{
    GET_A();
    if(!A)ip++;
}
DEF_FUNC(HALT)
{
    // nothing to do
}

bool VirtualMachine::parse_data(char *filename)
{
    FILE *fp=fopen(filename,"rb");

    uint32_t magic;
    fread(&magic,1,4,fp);
    if(magic!=HEADER_MAGIC)
    {
        fclose(fp);
        return false;
    }
    fread(&config,sizeof(config),1,fp);
    int_table=unique_ptr<int[]>(new int[config.int_table_size]);
    float_table=unique_ptr<double[]>(new double[config.float_table_size]);
    string_table=unique_ptr<char*[]>(new char*[config.string_table_size]);
    string_raw=unique_ptr<char[]>(new char[config.string_raw_size]);
    code_raw=unique_ptr<CODE[]>(new CODE[config.code_raw_size]);
    v_stack=unique_ptr<V_VALUE[]>(l_stack=new V_VALUE[config.stack_size]);

    fread(int_table.get(),sizeof(int),config.int_table_size,fp);
    fread(float_table.get(),sizeof(double),config.float_table_size,fp);
    size_t tmp;
    fread(&tmp,sizeof(tmp),1,fp); // hack for '\0' of string
    fread(string_raw.get(),1,config.string_raw_size,fp);
    fread(code_raw.get(),sizeof(CODE),config.code_raw_size,fp);
    fclose(fp);
    char *buffer=string_raw.get();
    string_table[0]=buffer;
    for(int i=1;i<config.string_table_size;i++)
    {
        while(*buffer++);
        string_table[i]=buffer;
    }
    ip=config.entry_point;
    return true;
}

void VirtualMachine::run()
{
    unsigned int t=clock();
    for(;;ip++)
    {
        OPCODE ins=get_ins(CUR_INS);
        if(ins==OP_HALT)break;
        if(ins>=0&&ins<=OP_HALT)(this->*op_func[ins])();
        else printf("Unknow instructor: %d\n",ins);
    }
    printf("halt in %lu clocks\n",clock()-t);
}
