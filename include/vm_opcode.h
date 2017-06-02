#ifndef _OPCODE_H_
#define _OPCODE_H_

#include <cstdlib>
/*
load_global a
    st.push(global[a])

store_global a
    global[a]=st.pop()

load a
    st.push(local[a])

store a
    local[a]=st.pop()

load_external a
    st.push(external[a])

store_external a
    external[a]=st.pop()

push a
    st.push(a)

pop
    st.pop()

new_array
    st.push(new Array)

new_object
    st.push(new Object)

create_function a
    create function

set_this
    reg_this=st.pop()

get_this
    st.push(reg_this)

reset_this
    swap
    dup
    set_this
    swap

array_push
    reg_this.push_back(st.pop())

object_get
    st.push(reg_this[st.pop()])

object_set
    key=st.pop()
    val=st.pop()
    reg_this[key]=val

object_reset
    this[last_key]=st.pop()

dup
    st.push(st.pop())

swap
    swap(st.top(),st.stop())

inc
    st.top()++

dec
    st.top()--

pos
    st.top()=+st.top()

neg
    st.top()=-st.top()

bnot
    st.top()=~st.top()

lnot
    st.top()=!st.top()

typeof
    st.top()=typeof st.top()

mul
div
mod
add
sub
band
xor
bor
land
lor
shl
shr
less
gt
le
ge
eq
ieq
    b=st.pop()
    a=st.pop()
    st.push(a X b)

istrue
    if (st.pop()) skip(next_instruction);

isfalse
    if (st.pop()) skip(next_instruction);

addsp a
    sp+=a

subsp a
    sp-=a

call argc
    st.pop()(...)

jmp8 a
    ip+=a

jmp16 a
    ip+=a

jmp32 a
    ip=a

ret
    reg_rv=st.pop()
    return

halt
    halt
*/

#define OP_CODE(T) \
T(LOAD_GLOBAL,5)\
T(STORE_GLOBAL,5)\
T(LOAD,5)\
T(STORE,5)\
T(LOAD_EXTERNAL,5)\
T(STORE_EXTERNAL,5)\
T(NPUSH,1)\
T(BPUSH,2)\
T(IPUSH,5)\
T(FPUSH,5)\
T(SPUSH,5)\
T(POP,1)\
T(NEW_ARRAY,1)\
T(NEW_OBJECT,1)\
T(CREATE_FUNCTION,5)\
T(SET_THIS,1)\
T(GET_THIS,1)\
T(RESET_THIS,1)\
T(ARRAY_PUSH,1)\
T(OBJECT_GET,1)\
T(OBJECT_SET,1)\
T(OBJECT_RESET,1)\
T(DUP,1)\
T(SWAP,1)\
T(INC,1)\
T(DEC,1)\
T(POS,1)\
T(NEG,1)\
T(BNOT,1)\
T(LNOT,1)\
T(TYPEOF,1)\
T(MUL,1)\
T(DIV,1)\
T(MOD,1)\
T(ADD,1)\
T(SUB,1)\
T(BAND,1)\
T(XOR,1)\
T(BOR,1)\
T(LAND,1)\
T(LOR,1)\
T(SHL,1)\
T(SHR,1)\
T(LESS,1)\
T(GT,1)\
T(LE,1)\
T(GE,1)\
T(EQ,1)\
T(IEQ,1)\
T(ISTRUE,1)\
T(ISFALSE,1)\
T(ADDSP,5)\
T(SUBSP,5)\
T(JMP,5)\
T(CALL,5)\
T(RET,1)\
T(HALT,1)\


#define GET_OP_NAME(NAME,LEN) OP_##NAME,
#define GET_OP_LENGTH(NAME,LEN) LEN, 
enum OPCODE {OP_CODE(GET_OP_NAME)};
extern const size_t op_length[];


#endif
