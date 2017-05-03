#ifndef _OPCODE_H_
#define _OPCODE_H_

/*
load_global a
    st.push(global[a])

store_global a
    global[a]=st.pop()

load a
    st.push(local[a])

store a
    local[a]=st.pop()

push a
    st.push(a)

pop
    st.pop()

new_array
    st.push(new Array)

new_object
    st.push(new Object)

set_this
    reg_this=st.pop()

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

istrue a
    if (st.pop()) skip(next_instruction);

isfalse a
    if (st.pop()) skip(next_instruction);

addsp
    sp+=st.pop()

subsp
    sp-=st.pop()

call argc
    st.pop()(...)

ret
    reg_rv=st.pop()
    return

halt
    halt
*/
#define OP_CODE(T) \
T(LOAD_GLOBAL)\
T(STORE_GLOBAL)\
T(LOAD)\
T(STORE)\
T(NPUSH)\
T(BPUSH)\
T(IPUSH)\
T(FPUSH)\
T(SPUSH)\
T(POP)\
T(NEW_ARRAY)\
T(NEW_OBJECT)\
T(SET_THIS)\
T(ARRAY_PUSH)\
T(OBJECT_GET)\
T(OBJECT_SET)\
T(OBJECT_RESET)\
T(DUP)\
T(SWAP)\
T(INC)\
T(DEC)\
T(POS)\
T(NEG)\
T(BNOT)\
T(LNOT)\
T(TYPEOF)\
T(MUL)\
T(DIV)\
T(MOD)\
T(ADD)\
T(SUB)\
T(BAND)\
T(XOR)\
T(BOR)\
T(LAND)\
T(LOR)\
T(SHL)\
T(SHR)\
T(LESS)\
T(GT)\
T(LE)\
T(GE)\
T(EQ)\
T(IEQ)\
T(ISTRUE)\
T(ISFALSE)\
T(ADDSP)\
T(SUBSP)\
T(CALL)\
T(RET)\
T(HALT)\


#define GET_OP_NAME(NAME,LENGTH) OP_##NAME,
enum OPCODE {OP_CODE(GET_OP_NAME)};


#endif
