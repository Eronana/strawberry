addsp 30
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
push "test function"
load_global 2
call 1
pop
new_array
create_function label_0
store_global 5
pop
; arugment 0
push "1+2="
; arugment 1
push 2
; arugment 0
push 1
load_global 5
call 2
add
load_global 2
call 1
pop
load_global 0
set_this
push "exit"
object_get
call 0
pop
new_array
load 6
array_push
create_function label_1
store_global 6
pop
; arugment 0
push "fib 0 to 19"
load_global 2
call 1
pop
; for statement
; init
push 0
store 7
pop
label_3:
; check expression
load 7
push 20
less
istrue
jmp label_4
; statement
; arugment 1
push ""
; arugment 0
; arugment 0
load 7
load_global 6
call 1
load_global 3
call 2
pop
label_5:
; loop expression
load 7
inc
store 7
pop
jmp label_3
label_4:
; arugment 0
push ""
load_global 2
call 1
pop
; arugment 0
push "test closure"
load_global 2
call 1
pop
new_array
create_function label_6
store_global 7
pop
; arugment 0
push 100
load_global 7
call 1
store_global 8
pop
; arugment 0
push 0
load_global 7
call 1
store_global 9
pop
; for statement
; init
push 0
store 10
pop
label_8:
; check expression
load 10
push 20
less
istrue
jmp label_9
; statement
; arugment 1
push ""
; arugment 0
load_global 8
call 0
load_global 3
call 2
pop
; arugment 1
push ", "
; arugment 0
load_global 9
call 0
load_global 3
call 2
pop
label_10:
; loop expression
load 10
inc
store 10
pop
jmp label_8
label_9:
; arugment 0
push "\n------"
load_global 2
call 1
pop
new_array
create_function label_11
store_global 10
pop
new_object
push 0
push "counter"
object_set
pop
store_global 11
pop
; arugment 0
load_global 11
load_global 10
call 1
store_global 12
pop
; arugment 0
load_global 11
load_global 10
call 1
store_global 13
pop
; for statement
; init
push 0
store 14
pop
label_13:
; check expression
load 14
push 20
less
istrue
jmp label_14
; statement
; arugment 1
push ""
; arugment 0
load_global 12
call 0
load_global 3
call 2
pop
; arugment 1
push ", "
; arugment 0
load_global 13
call 0
load_global 3
call 2
pop
label_15:
; loop expression
load 14
inc
store 14
pop
jmp label_13
label_14:
; arugment 0
push "\n------"
load_global 2
call 1
pop
; arugment 0
push "test GC"
load_global 2
call 1
pop
; for statement
; init
push 0
store 14
pop
label_16:
; check expression
load 14
push 1000000
less
istrue
jmp label_17
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
store 15
pop
new_array
new_array
reset_this
array_push
store 15
pop
label_18:
; loop expression
load 14
inc
store 14
pop
jmp label_16
label_17:
push true
store_global 14
pop
push false
store_global 15
pop
load_global 14
load_global 15
land
store_global 16
pop
load_global 14
load_global 15
lor
store_global 17
pop
load_global 14
lnot
store_global 18
pop
load_global 15
lnot
store_global 19
pop
; arugment 5
load_global 19
; arugment 4
load_global 18
; arugment 3
load_global 17
; arugment 2
load_global 16
; arugment 1
load_global 15
; arugment 0
load_global 14
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
store 20
pop
label_19:
; check expression
load 20
load_global 1
less
istrue
jmp label_20
; statement
; arugment 0
load 20
load 20
mul
load_global 0
set_this
push "println"
object_get
call 1
pop
label_21:
; loop expression
load 20
inc
store 20
pop
jmp label_19
label_20:
; arugment 0
push "来打印一个9*9乘法表"
load_global 2
call 1
pop
; for statement
; init
push 1
store 20
pop
label_22:
; check expression
load 20
push 9
le
istrue
jmp label_23
; statement
; for statement
; init
push 1
store 21
pop
label_25:
; check expression
load 21
load 20
le
istrue
jmp label_26
; statement
; arugment 0
load 21
load_global 3
call 1
pop
; arugment 0
push "*"
load_global 3
call 1
pop
; arugment 0
load 20
load_global 3
call 1
pop
; arugment 0
push "="
load_global 3
call 1
pop
; arugment 0
load 20
load 21
mul
load_global 3
call 1
pop
; arugment 0
push "  "
load_global 3
call 1
pop
label_27:
; loop expression
load 21
inc
store 21
pop
jmp label_25
label_26:
; arugment 0
push ""
load_global 2
call 1
pop
label_24:
; loop expression
load 20
inc
store 20
pop
jmp label_22
label_23:
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
store_global 20
pop
push "sss"
load_global 20
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
store_global 21
pop
load_global 0
set_this
push "len"
object_get
store_global 22
pop
; arugment 0
push "----------------"
load_global 2
call 1
pop
; for statement
; init
push 0
store 23
pop
label_28:
; check expression
load 23
; arugment 0
load_global 20
load_global 22
call 1
less
istrue
jmp label_29
; statement
; arugment 0
load_global 20
set_this
load 23
object_get
load_global 2
call 1
pop
label_30:
; loop expression
load 23
inc
store 23
pop
jmp label_28
label_29:
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
load_global 20
load_global 21
call 1
load_global 21
call 5
pop
; if statement
; arugment 1
push "666"
; arugment 0
load_global 20
load_global 21
call 2
istrue
jmp label_31
; true branch
; for statement
; init
push 0
store 23
pop
label_32:
; check expression
load 23
; arugment 0
load_global 20
load_global 0
set_this
push "len"
object_get
call 1
less
istrue
jmp label_33
; statement
; arugment 0
load_global 20
set_this
load 23
object_get
load_global 2
call 1
pop
label_34:
; loop expression
load 23
inc
store 23
pop
jmp label_32
label_33:
label_31:
; if statement
; arugment 0
push 1
load_global 21
call 1
lnot
istrue
jmp label_35
; true branch
; arugment 0
push "不能push非树煮"
load_global 2
call 1
pop
label_35:
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
store_global 23
pop
load_global 0
set_this
push "get_keys"
object_get
store_global 24
pop
; arugment 0
load_global 23
load_global 24
call 1
store_global 25
pop
; for statement
; init
push 0
store 26
pop
label_36:
; check expression
load 26
; arugment 0
load_global 25
load_global 22
call 1
less
istrue
jmp label_37
; statement
; arugment 0
load_global 25
set_this
load 26
object_get
load_global 2
call 1
pop
label_38:
; loop expression
load 26
inc
store 26
pop
jmp label_36
label_37:
load_global 23
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
load_global 23
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
store_global 26
pop
; arugment 3
push "hahaha"
; arugment 2
push 2222
; arugment 1
push "1"
; arugment 0
load_global 26
load_global 21
call 4
pop
; for statement
; init
push 0
store 27
pop
label_39:
; check expression
load 27
; arugment 0
load_global 26
load_global 22
call 1
less
istrue
jmp label_40
; statement
; arugment 0
load_global 26
set_this
load 27
object_get
load_global 2
call 1
pop
label_41:
; loop expression
load 27
inc
store 27
pop
jmp label_39
label_40:
; arugment 0
push "asadgasd"
load_global 2
call 1
pop
; switch statement
load_global 4
call 0
jmp label_43
; default clause
label_44:
; arugment 0
push "默认..."
load_global 2
call 1
pop
jmp label_46
label_43:
dup
push 1
eq
istrue
jmp label_45
label_46:
; arugment 0
push "case 1"
load_global 2
call 1
pop
jmp label_48
label_45:
dup
push 3
eq
istrue
jmp label_47
label_48:
; arugment 0
push "case 3"
load_global 2
call 1
pop
jmp label_42
jmp label_50
label_47:
jmp label_44
label_42:
pop
; arugment 0
push "请输入一个在[10,15]之间的整数\n"
load_global 3
call 1
pop
; while statement
label_49:
; check expression
load_global 4
call 0
store_global 27
push 10
ge
load_global 27
push 15
le
land
lnot
istrue
jmp label_50
; statement
; arugment 0
push "请输入一个在[10,15]之间的整数"
load_global 2
call 1
pop
jmp label_49
label_50:
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
label_51:
; statement
; arugment 0
push "请输入一个在[10,15]之间的整数"
load_global 2
call 1
pop
label_53:
; check expression
load_global 4
call 0
store_global 27
push 10
ge
load_global 27
push 15
le
land
lnot
isfalse
jmp label_51
label_52:
; if statement
push 1
push 1
add
push 2
gt
istrue
jmp label_55
; true branch
; arugment 0
push "1+1>2"
load_global 2
call 1
pop
jmp label_54
label_55:
; else branch
; arugment 0
push "1+1不大于2"
load_global 2
call 1
pop
label_54:
; arugment 0
push '来打个素数表试试\'\n'
load_global 3
call 1
pop
; for statement
; init
push 2
store 28
pop
label_56:
; check expression
load 28
push 1000
less
istrue
jmp label_57
; statement
; for statement
; init
push 2
store 29
pop
label_59:
; check expression
load 29
load 29
mul
load 28
le
istrue
jmp label_60
; statement
; if statement
load 28
load 29
mod
push 0
eq
istrue
jmp label_62
; true branch
jmp label_60
label_62:
label_61:
; loop expression
load 29
inc
store 29
pop
jmp label_59
label_60:
; if statement
load 29
load 29
mul
load 28
gt
istrue
jmp label_63
; true branch
; arugment 0
load 28
load_global 3
call 1
pop
; arugment 0
push " "
load_global 3
call 1
pop
label_63:
label_58:
; loop expression
load 28
inc
store 28
pop
jmp label_56
label_57:
; arugment 0
push "\nend"
load_global 2
call 1
pop
subsp 30
halt


; functions
; function expression
label_0:
load -1
load -2
add
ret
; function expression
label_1:
; if statement
load -1
push 2
ge
istrue
jmp label_2
; true branch
; arugment 0
load -1
push 1
sub
load_external 0
call 1
; arugment 0
load -1
push 2
sub
load_external 0
call 1
add
ret
label_2:
load -1
ret
; function expression
label_7:
load_external 0
inc
store_external 0
ret
; function expression
label_6:
new_array
load -1
array_push
create_function label_7
ret
; function expression
label_12:
load_external 0
set_this
push "counter"
object_get
inc
object_reset
ret
; function expression
label_11:
new_array
load -1
array_push
create_function label_12
ret
