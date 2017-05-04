addsp 21
push 3
push 4
push 5
add
mul
push 2
shl
store_global 1
pop
load_global 0
set_this
push "println"
object_get
store_global 2
pop
load_global 0
set_this
push "print"
object_get
store_global 3
pop
load_global 0
set_this
push "scani"
object_get
store_global 4
pop
; arugment 0
push "test GC"
load_global 2
call 1
pop
; for statement
; init
push 0
store 5
pop
label_0:
; check expression
load 5
push 1000000
less
istrue
jmp label_1
; statement
push "hello"
push "hello"
add
push "hello"
add
push "hello"
add
push "hello"
add
store 6
pop
new_array
new_array
reset_this
array_push
store 6
pop
label_2:
; loop expression
load 5
inc
store 5
pop
jmp label_0
label_1:
push true
store_global 5
pop
push false
store_global 6
pop
load_global 5
load_global 6
land
store_global 7
pop
load_global 5
load_global 6
lor
store_global 8
pop
load_global 5
lnot
store_global 9
pop
load_global 6
lnot
store_global 10
pop
; arugment 5
load_global 10
; arugment 4
load_global 9
; arugment 3
load_global 8
; arugment 2
load_global 7
; arugment 1
load_global 6
; arugment 0
load_global 5
load_global 2
call 6
pop
; arugment 0
push "haha: "
load_global 1
add
load_global 2
call 1
pop
push 7
load_global 1
swap
mod
store_global 1
pop
; arugment 0
push "haha mo 7: "
load_global 1
add
load_global 2
call 1
pop
; for statement
; init
push 0
store 11
pop
label_3:
; check expression
load 11
load_global 1
less
istrue
jmp label_4
; statement
; arugment 0
load 11
load 11
mul
load_global 0
set_this
push "println"
object_get
call 1
pop
label_5:
; loop expression
load 11
inc
store 11
pop
jmp label_3
label_4:
; arugment 0
push "来打印一个9*9乘法表"
load_global 2
call 1
pop
; for statement
; init
push 1
store 11
pop
label_6:
; check expression
load 11
push 9
le
istrue
jmp label_7
; statement
; for statement
; init
push 1
store 12
pop
label_9:
; check expression
load 12
load 11
le
istrue
jmp label_10
; statement
; arugment 0
load 12
load_global 3
call 1
pop
; arugment 0
push "*"
load_global 3
call 1
pop
; arugment 0
load 11
load_global 3
call 1
pop
; arugment 0
push "="
load_global 3
call 1
pop
; arugment 0
load 11
load 12
mul
load_global 3
call 1
pop
; arugment 0
push "  "
load_global 3
call 1
pop
label_11:
; loop expression
load 12
inc
store 12
pop
jmp label_9
label_10:
; arugment 0
push ""
load_global 2
call 1
pop
label_8:
; loop expression
load 11
inc
store 11
pop
jmp label_6
label_7:
; arugment 0
push "现在开始测试..树煮"
load_global 2
call 1
pop
new_array
push 1
array_push
push "222"
array_push
push 3.1315926
array_push
push 4
array_push
store_global 11
pop
push "sss"
load_global 11
set_this
push 1
object_get
swap
add
object_reset
pop
load_global 0
set_this
push "push"
object_get
store_global 12
pop
load_global 0
set_this
push "len"
object_get
store_global 13
pop
; arugment 0
push "----------------"
load_global 2
call 1
pop
; for statement
; init
push 0
store 14
pop
label_12:
; check expression
load 14
; arugment 0
load_global 11
load_global 13
call 1
less
istrue
jmp label_13
; statement
; arugment 0
load_global 11
set_this
load 14
object_get
load_global 2
call 1
pop
label_14:
; loop expression
load 14
inc
store 14
pop
jmp label_12
label_13:
; arugment 4
push "66666"
; arugment 3
push "魔法师"
; arugment 2
push "膜蛤"
; arugment 1
push "fuck the world"
; arugment 0
; arugment 0
load_global 11
load_global 12
call 1
load_global 12
call 5
pop
; if statement
; arugment 1
push "666"
; arugment 0
load_global 11
load_global 12
call 2
istrue
jmp label_15
; true branch
; for statement
; init
push 0
store 14
pop
label_16:
; check expression
load 14
; arugment 0
load_global 11
load_global 0
set_this
push "len"
object_get
call 1
less
istrue
jmp label_17
; statement
; arugment 0
load_global 11
set_this
load 14
object_get
load_global 2
call 1
pop
label_18:
; loop expression
load 14
inc
store 14
pop
jmp label_16
label_17:
label_15:
; if statement
; arugment 0
push 1
load_global 12
call 1
lnot
istrue
jmp label_19
; true branch
; arugment 0
push "不能push非树煮"
load_global 2
call 1
pop
label_19:
new_object
push "world"
push "string"
object_set
pop
push 1234f
push "int"
object_set
pop
push 222.123123
push "float"
object_set
pop
new_array
push 1
array_push
push 2
array_push
push 3
array_push
push "xxx"
array_push
new_object
new_array
push 1234
array_push
new_array
push 6666
array_push
push 77777
array_push
reset_this
array_push
reset_this
push "wtf"
object_set
pop
reset_this
array_push
reset_this
push "array"
object_set
pop
new_object
push "maki"
push "nico"
object_set
pop
push 2.3333
push "kotiri"
object_set
pop
reset_this
push "object"
object_set
pop
store_global 14
pop
load_global 0
set_this
push "get_keys"
object_get
store_global 15
pop
; arugment 0
load_global 14
load_global 15
call 1
store_global 16
pop
; for statement
; init
push 0
store 17
pop
label_20:
; check expression
load 17
; arugment 0
load_global 16
load_global 13
call 1
less
istrue
jmp label_21
; statement
; arugment 0
load_global 16
set_this
load 17
object_get
load_global 2
call 1
pop
label_22:
; loop expression
load 17
inc
store 17
pop
jmp label_20
label_21:
load_global 14
set_this
push "array"
object_get
set_this
push 4
object_get
set_this
push "wtf"
object_get
set_this
push 1
object_get
set_this
push 1
object_get
inc
object_reset
pop
load_global 14
set_this
push "array"
object_get
set_this
push 4
object_get
set_this
push "wtf"
object_get
set_this
push 1
object_get
store_global 17
pop
; arugment 3
push "hahaha"
; arugment 2
push 2222
; arugment 1
push "1"
; arugment 0
load_global 17
load_global 12
call 4
pop
; for statement
; init
push 0
store 18
pop
label_23:
; check expression
load 18
; arugment 0
load_global 17
load_global 13
call 1
less
istrue
jmp label_24
; statement
; arugment 0
load_global 17
set_this
load 18
object_get
load_global 2
call 1
pop
label_25:
; loop expression
load 18
inc
store 18
pop
jmp label_23
label_24:
; arugment 0
push "asadgasd"
load_global 2
call 1
pop
; switch statement
load_global 4
call 0
jmp label_27
; default clause
label_28:
; arugment 0
push "默认..."
load_global 2
call 1
pop
jmp label_30
label_27:
dup
push 1
eq
istrue
jmp label_29
label_30:
; arugment 0
push "case 1"
load_global 2
call 1
pop
jmp label_32
label_29:
dup
push 3
eq
istrue
jmp label_31
label_32:
; arugment 0
push "case 3"
load_global 2
call 1
pop
jmp label_26
jmp label_34
label_31:
jmp label_28
label_26:
pop
; arugment 0
push "请输入一个在[10,15]之间的整数\n"
load_global 3
call 1
pop
; while statement
label_33:
; check expression
load_global 4
call 0
store_global 18
push 10
ge
load_global 18
push 15
le
land
lnot
istrue
jmp label_34
; statement
; arugment 0
push "请输入一个在[10,15]之间的整数"
load_global 2
call 1
pop
jmp label_33
label_34:
; arugment 0
push "我知道..用do写会更优雅一些...上面我只是为了测试while..."
load_global 2
call 1
pop
; arugment 0
push "于是我用do再来一次"
load_global 2
call 1
pop
; do statement
label_35:
; statement
; arugment 0
push "请输入一个在[10,15]之间的整数"
load_global 2
call 1
pop
label_37:
; check expression
load_global 4
call 0
store_global 18
push 10
ge
load_global 18
push 15
le
land
lnot
isfalse
jmp label_35
label_36:
; if statement
push 1
push 1
add
push 2
gt
istrue
jmp label_39
; true branch
; arugment 0
push "1+1>2"
load_global 2
call 1
pop
jmp label_38
label_39:
; else branch
; arugment 0
push "1+1不大于2"
load_global 2
call 1
pop
label_38:
; arugment 0
push '来打个素数表试试\'\n'
load_global 3
call 1
pop
; for statement
; init
push 2
store 19
pop
label_40:
; check expression
load 19
push 1000
less
istrue
jmp label_41
; statement
; for statement
; init
push 2
store 20
pop
label_43:
; check expression
load 20
load 20
mul
load 19
le
istrue
jmp label_44
; statement
; if statement
load 19
load 20
mod
push 0
eq
istrue
jmp label_46
; true branch
jmp label_44
label_46:
label_45:
; loop expression
load 20
inc
store 20
pop
jmp label_43
label_44:
; if statement
load 20
load 20
mul
load 19
gt
istrue
jmp label_47
; true branch
; arugment 0
load 19
load_global 3
call 1
pop
; arugment 0
push " "
load_global 3
call 1
pop
label_47:
label_42:
; loop expression
load 19
inc
store 19
pop
jmp label_40
label_41:
; arugment 0
push "\nend"
load_global 2
call 1
pop
subsp 21
