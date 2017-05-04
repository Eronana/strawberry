addsp 13
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
store 5
pop
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
pop
label_2:
; loop expression
load 5
inc
store 5
pop
jmp label_0
label_1:
; arugment 0
push "来打印一个9*9乘法表"
load_global 2
call 1
pop
; for statement
; init
push 1
store 5
pop
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
pop
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
pop
; arugment 0
push "*"
load_global 3
call 1
pop
; arugment 0
load 5
load_global 3
call 1
pop
; arugment 0
push "="
load_global 3
call 1
pop
; arugment 0
load 5
load 6
mul
load_global 3
call 1
pop
; arugment 0
push "  "
load_global 3
call 1
pop
label_8:
; loop expression
load 6
inc
store 6
pop
jmp label_6
label_7:
; arugment 0
push ""
load_global 2
call 1
pop
label_5:
; loop expression
load 5
inc
store 5
pop
jmp label_3
label_4:
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
store_global 5
pop
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
pop
load_global 0
set_this
push "len"
object_get
store_global 7
pop
; arugment 0
push "----------------"
load_global 2
call 1
pop
; for statement
; init
push 0
store 8
pop
label_9:
; check expression
load 8
; arugment 0
load_global 5
load_global 7
call 1
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
pop
label_11:
; loop expression
load 8
inc
store 8
pop
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
load_global 6
call 5
pop
; if statement
; arugment 1
push "666"
; arugment 0
load_global 5
load_global 6
call 2
istrue
jmp label_12
; true branch
; for statement
; init
push 0
store 8
pop
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
pop
label_15:
; loop expression
load 8
inc
store 8
pop
jmp label_13
label_14:
label_12:
; if statement
; arugment 0
push 1
load_global 6
call 1
lnot
istrue
jmp label_16
; true branch
; arugment 0
push "不能push非树煮"
load_global 2
call 1
pop
label_16:
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
store_global 8
pop
load_global 0
set_this
push "get_keys"
object_get
store_global 9
pop
; arugment 0
load_global 8
load_global 9
call 1
store_global 10
pop
; for statement
; init
push 0
store 11
pop
label_17:
; check expression
load 11
; arugment 0
load_global 10
load_global 7
call 1
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
pop
label_19:
; loop expression
load 11
inc
store 11
pop
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
pop
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
pop
; for statement
; init
push 0
store 12
pop
label_20:
; check expression
load 12
; arugment 0
load_global 11
load_global 7
call 1
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
pop
label_22:
; loop expression
load 12
inc
store 12
pop
jmp label_20
label_21:
; arugment 0
push "asadgasd"
load_global 2
call 1
pop
; switch statement
load_global 4
call 0
jmp label_24
; default clause
label_25:
; arugment 0
push "默认..."
load_global 2
call 1
pop
jmp label_27
label_24:
dup
push 1
eq
istrue
jmp label_26
label_27:
; arugment 0
push "case 1"
load_global 2
call 1
pop
jmp label_29
label_26:
dup
push 3
eq
istrue
jmp label_28
label_29:
; arugment 0
push "case 3"
load_global 2
call 1
pop
jmp label_23
jmp label_31
label_28:
jmp label_25
label_23:
pop
subsp 13
