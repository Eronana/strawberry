addsp 15
push 3
push 4
push 5
add
mul
push 2
shl
store_global 1
load_global 0
set_this
push "println"
object_get
store_global 2
load_global 0
set_this
push "print"
object_get
store_global 3
load_global 0
set_this
push "scani"
object_get
store_global 4
; arugment 0
push "haha: "
load_global 1
add
load_global 2
call 1
grv
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
grv
pop
; for statement
; init
push 0
store 5
label_0:
; check expression
load 5
load_global 1
less
istrue
jmp label_1
; statement
; arugment 0
load 5
load 5
mul
load_global 0
set_this
push "println"
object_get
call 1
grv
pop
label_2:
; loop expression
load 5
dup
inc
store 5
jmp label_0
label_1:
; arugment 0
push "来打印一个9*9乘法表"
load_global 2
call 1
grv
pop
; for statement
; init
push 1
store 5
label_3:
; check expression
load 5
push 9
le
istrue
jmp label_4
; statement
; for statement
; init
push 1
store 6
label_6:
; check expression
load 6
load 5
le
istrue
jmp label_7
; statement
; arugment 0
load 6
load_global 3
call 1
grv
pop
; arugment 0
push "*"
load_global 3
call 1
grv
pop
; arugment 0
load 5
load_global 3
call 1
grv
pop
; arugment 0
push "="
load_global 3
call 1
grv
pop
; arugment 0
load 5
load 6
mul
load_global 3
call 1
grv
pop
; arugment 0
push "  "
load_global 3
call 1
grv
pop
label_8:
; loop expression
load 6
dup
inc
store 6
jmp label_6
label_7:
; arugment 0
push ""
load_global 2
call 1
grv
pop
label_5:
; loop expression
load 5
dup
inc
store 5
jmp label_3
label_4:
; arugment 0
push "现在开始测试..树煮"
load_global 2
call 1
grv
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
store_global 5
push "sss"
load_global 5
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
store_global 6
load_global 0
set_this
push "len"
object_get
store_global 7
; arugment 0
push "----------------"
load_global 2
call 1
grv
pop
; for statement
; init
push 0
store 8
label_9:
; check expression
load 8
; arugment 0
load_global 5
load_global 7
call 1
grv
less
istrue
jmp label_10
; statement
; arugment 0
load_global 5
set_this
load 8
object_get
load_global 2
call 1
grv
pop
label_11:
; loop expression
load 8
dup
inc
store 8
jmp label_9
label_10:
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
load_global 5
load_global 6
call 1
grv
load_global 6
call 5
grv
pop
; if statement
; arugment 1
push "666"
; arugment 0
load_global 5
load_global 6
call 2
grv
istrue
jmp label_12
; true branch
; for statement
; init
push 0
store 8
label_13:
; check expression
load 8
; arugment 0
load_global 5
load_global 0
set_this
push "len"
object_get
call 1
grv
less
istrue
jmp label_14
; statement
; arugment 0
load_global 5
set_this
load 8
object_get
load_global 2
call 1
grv
pop
label_15:
; loop expression
load 8
dup
inc
store 8
jmp label_13
label_14:
label_12:
; if statement
; arugment 0
push 1
load_global 6
call 1
grv
lnot
istrue
jmp label_16
; true branch
; arugment 0
push "不能push非树煮"
load_global 2
call 1
grv
pop
label_16:
new_object
push "world"
push "string"
object_set
push 1234f
push "int"
object_set
push 222.123123
push "float"
object_set
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
set_this
array_push
set_this
push "wtf"
object_set
set_this
array_push
set_this
push "array"
object_set
new_object
push "maki"
push "nico"
object_set
push 2.3333
push "kotiri"
object_set
set_this
push "object"
object_set
store_global 8
load_global 0
set_this
push "get_keys"
object_get
store_global 9
; arugment 0
load_global 8
load_global 9
call 1
grv
store_global 10
; for statement
; init
push 0
store 11
label_17:
; check expression
load 11
; arugment 0
load_global 10
load_global 7
call 1
grv
less
istrue
jmp label_18
; statement
; arugment 0
load_global 10
set_this
load 11
object_get
load_global 2
call 1
grv
pop
label_19:
; loop expression
load 11
dup
inc
store 11
jmp label_17
label_18:
load_global 8
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
dup
inc
object_reset
pop
load_global 8
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
store_global 11
; arugment 3
push "hahaha"
; arugment 2
push 2222
; arugment 1
push "1"
; arugment 0
load_global 11
load_global 6
call 4
grv
pop
; for statement
; init
push 0
store 12
label_20:
; check expression
load 12
; arugment 0
load_global 11
load_global 7
call 1
grv
less
istrue
jmp label_21
; statement
; arugment 0
load_global 11
set_this
load 12
object_get
load_global 2
call 1
grv
pop
label_22:
; loop expression
load 12
dup
inc
store 12
jmp label_20
label_21:
; arugment 0
push "asadgasd"
load_global 2
call 1
grv
pop
; arugment 0
push "请输入一个在[10,15]之间的整数\n"
load_global 3
call 1
grv
pop
; while statement
label_23:
; check expression
load_global 4
call 0
grv
store_global 12
push 10
ge
load_global 12
push 15
le
land
lnot
istrue
jmp label_24
; statement
; arugment 0
push "请输入一个在[10,15]之间的整数"
load_global 2
call 1
grv
pop
jmp label_23
label_24:
; arugment 0
push "我知道..用do写会更优雅一些...上面我只是为了测试while..."
load_global 2
call 1
grv
pop
; arugment 0
push "于是我用do再来一次"
load_global 2
call 1
grv
pop
; do statement
label_25:
; statement
; arugment 0
push "请输入一个在[10,15]之间的整数"
load_global 2
call 1
grv
pop
label_27:
; check expression
load_global 4
call 0
grv
store_global 12
push 10
ge
load_global 12
push 15
le
land
lnot
isfalse
jmp label_25
label_26:
; if statement
push 1
push 1
add
push 2
gt
istrue
jmp label_29
; true branch
; arugment 0
push "1+1>2"
load_global 2
call 1
grv
pop
jmp label_28
label_29:
; else branch
; arugment 0
push "1+1不大于2"
load_global 2
call 1
grv
pop
label_28:
; switch statement
load_global 4
call 0
grv
; default clause
label_32:
; arugment 0
push "默认..."
load_global 2
call 1
grv
pop
jmp label_34
label_31:
push 1
eq
istrue
jmp label_33
label_34:
; arugment 0
push "case 1"
load_global 2
call 1
grv
pop
jmp label_30
label_33:
push 3
eq
istrue
jmp label_35
; arugment 0
push "case 3"
load_global 2
call 1
grv
pop
label_35:
jmp label_32
label_30:
; arugment 0
push '来打个素数表试试\'\n'
load_global 3
call 1
grv
pop
; for statement
; init
push 2
store 13
label_36:
; check expression
load 13
push 1000
less
istrue
jmp label_37
; statement
; for statement
; init
push 2
store 14
pop
label_39:
; check expression
load 14
load 14
mul
load 13
le
istrue
jmp label_40
; statement
; if statement
load 13
load 14
mod
push 0
eq
istrue
jmp label_42
; true branch
jmp label_40
label_42:
label_41:
; loop expression
load 14
dup
inc
store 14
jmp label_39
label_40:
; if statement
load 14
load 14
mul
load 13
gt
istrue
jmp label_43
; true branch
; arugment 0
load 13
load_global 3
call 1
grv
; arugment 0
push " "
load_global 3
call 1
grv
pop
label_43:
label_38:
; loop expression
load 13
dup
inc
store 13
jmp label_36
label_37:
; arugment 0
push ""
load_global 2
call 1
grv
pop
subsp 15
