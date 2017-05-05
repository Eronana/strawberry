addsp 33
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
new_array
create_function label_1
store_global 6
pop
; arugment 0
push "小明"
load_global 6
call 1
store_global 7
pop
; arugment 0
push "小红"
load_global 6
call 1
store_global 8
pop
; arugment 0
push "I am a SB"
load_global 7
set_this
push "say"
object_get
call 1
pop
; arugment 0
push "I am a SB too"
load_global 8
set_this
push "say"
object_get
call 1
pop
; arugment 1
push '我'
; arugment 0
push '小明'
load_global 8
set_this
push "love"
object_get
call 2
pop
load_global 0
set_this
push "exit"
object_get
call 0
pop
new_array
create_function label_4
store_global 9
pop
; arugment 0
push "fib 0 to 19"
load_global 2
call 1
pop
; for statement
; init
push 0
store 10
pop
label_6:
; check expression
load 10
push 20
less
istrue
jmp label_7
; statement
; arugment 1
push ""
; arugment 0
; arugment 0
load 10
load_global 9
call 1
load_global 3
call 2
pop
label_8:
; loop expression
load 10
inc
store 10
pop
jmp label_6
label_7:
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
create_function label_9
store_global 10
pop
; arugment 0
push 100
load_global 10
call 1
store_global 11
pop
; arugment 0
push 0
load_global 10
call 1
store_global 12
pop
; for statement
; init
push 0
store 13
pop
label_11:
; check expression
load 13
push 20
less
istrue
jmp label_12
; statement
; arugment 1
push ""
; arugment 0
load_global 11
call 0
load_global 3
call 2
pop
; arugment 1
push ", "
; arugment 0
load_global 12
call 0
load_global 3
call 2
pop
label_13:
; loop expression
load 13
inc
store 13
pop
jmp label_11
label_12:
; arugment 0
push "\n------"
load_global 2
call 1
pop
new_array
create_function label_14
store_global 13
pop
new_object
push 0
push "counter"
object_set
pop
store_global 14
pop
; arugment 0
load_global 14
load_global 13
call 1
store_global 15
pop
; arugment 0
load_global 14
load_global 13
call 1
store_global 16
pop
; for statement
; init
push 0
store 17
pop
label_16:
; check expression
load 17
push 20
less
istrue
jmp label_17
; statement
; arugment 1
push ""
; arugment 0
load_global 15
call 0
load_global 3
call 2
pop
; arugment 1
push ", "
; arugment 0
load_global 16
call 0
load_global 3
call 2
pop
label_18:
; loop expression
load 17
inc
store 17
pop
jmp label_16
label_17:
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
store 17
pop
label_19:
; check expression
load 17
push 1000000
less
istrue
jmp label_20
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
store 18
pop
new_array
new_array
reset_this
array_push
store 18
pop
label_21:
; loop expression
load 17
inc
store 17
pop
jmp label_19
label_20:
push true
store_global 17
pop
push false
store_global 18
pop
load_global 17
load_global 18
land
store_global 19
pop
load_global 17
load_global 18
lor
store_global 20
pop
load_global 17
lnot
store_global 21
pop
load_global 18
lnot
store_global 22
pop
; arugment 5
load_global 22
; arugment 4
load_global 21
; arugment 3
load_global 20
; arugment 2
load_global 19
; arugment 1
load_global 18
; arugment 0
load_global 17
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
store 23
pop
label_22:
; check expression
load 23
load_global 1
less
istrue
jmp label_23
; statement
; arugment 0
load 23
load 23
mul
load_global 0
set_this
push "println"
object_get
call 1
pop
label_24:
; loop expression
load 23
inc
store 23
pop
jmp label_22
label_23:
; arugment 0
push "来打印一个9*9乘法表"
load_global 2
call 1
pop
; for statement
; init
push 1
store 23
pop
label_25:
; check expression
load 23
push 9
le
istrue
jmp label_26
; statement
; for statement
; init
push 1
store 24
pop
label_28:
; check expression
load 24
load 23
le
istrue
jmp label_29
; statement
; arugment 0
load 24
load_global 3
call 1
pop
; arugment 0
push "*"
load_global 3
call 1
pop
; arugment 0
load 23
load_global 3
call 1
pop
; arugment 0
push "="
load_global 3
call 1
pop
; arugment 0
load 23
load 24
mul
load_global 3
call 1
pop
; arugment 0
push "  "
load_global 3
call 1
pop
label_30:
; loop expression
load 24
inc
store 24
pop
jmp label_28
label_29:
; arugment 0
push ""
load_global 2
call 1
pop
label_27:
; loop expression
load 23
inc
store 23
pop
jmp label_25
label_26:
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
store_global 23
pop
push "sss"
load_global 23
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
store_global 24
pop
load_global 0
set_this
push "len"
object_get
store_global 25
pop
; arugment 0
push "----------------"
load_global 2
call 1
pop
; for statement
; init
push 0
store 26
pop
label_31:
; check expression
load 26
; arugment 0
load_global 23
load_global 25
call 1
less
istrue
jmp label_32
; statement
; arugment 0
load_global 23
set_this
load 26
object_get
load_global 2
call 1
pop
label_33:
; loop expression
load 26
inc
store 26
pop
jmp label_31
label_32:
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
load_global 23
load_global 24
call 1
load_global 24
call 5
pop
; if statement
; arugment 1
push "666"
; arugment 0
load_global 23
load_global 24
call 2
istrue
jmp label_34
; true branch
; for statement
; init
push 0
store 26
pop
label_35:
; check expression
load 26
; arugment 0
load_global 23
load_global 0
set_this
push "len"
object_get
call 1
less
istrue
jmp label_36
; statement
; arugment 0
load_global 23
set_this
load 26
object_get
load_global 2
call 1
pop
label_37:
; loop expression
load 26
inc
store 26
pop
jmp label_35
label_36:
label_34:
; if statement
; arugment 0
push 1
load_global 24
call 1
lnot
istrue
jmp label_38
; true branch
; arugment 0
push "不能push非树煮"
load_global 2
call 1
pop
label_38:
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
store_global 26
pop
load_global 0
set_this
push "get_keys"
object_get
store_global 27
pop
; arugment 0
load_global 26
load_global 27
call 1
store_global 28
pop
; for statement
; init
push 0
store 29
pop
label_39:
; check expression
load 29
; arugment 0
load_global 28
load_global 25
call 1
less
istrue
jmp label_40
; statement
; arugment 0
load_global 28
set_this
load 29
object_get
load_global 2
call 1
pop
label_41:
; loop expression
load 29
inc
store 29
pop
jmp label_39
label_40:
load_global 26
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
load_global 26
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
store_global 29
pop
; arugment 3
push "hahaha"
; arugment 2
push 2222
; arugment 1
push "1"
; arugment 0
load_global 29
load_global 24
call 4
pop
; for statement
; init
push 0
store 30
pop
label_42:
; check expression
load 30
; arugment 0
load_global 29
load_global 25
call 1
less
istrue
jmp label_43
; statement
; arugment 0
load_global 29
set_this
load 30
object_get
load_global 2
call 1
pop
label_44:
; loop expression
load 30
inc
store 30
pop
jmp label_42
label_43:
; arugment 0
push "asadgasd"
load_global 2
call 1
pop
; switch statement
load_global 4
call 0
jmp label_46
; default clause
label_47:
; arugment 0
push "默认..."
load_global 2
call 1
pop
jmp label_49
label_46:
dup
push 1
eq
istrue
jmp label_48
label_49:
; arugment 0
push "case 1"
load_global 2
call 1
pop
jmp label_51
label_48:
dup
push 3
eq
istrue
jmp label_50
label_51:
; arugment 0
push "case 3"
load_global 2
call 1
pop
jmp label_45
jmp label_53
label_50:
jmp label_47
label_45:
pop
; arugment 0
push "请输入一个在[10,15]之间的整数\n"
load_global 3
call 1
pop
; while statement
label_52:
; check expression
load_global 4
call 0
store_global 30
push 10
ge
load_global 30
push 15
le
land
lnot
istrue
jmp label_53
; statement
; arugment 0
push "请输入一个在[10,15]之间的整数"
load_global 2
call 1
pop
jmp label_52
label_53:
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
label_54:
; statement
; arugment 0
push "请输入一个在[10,15]之间的整数"
load_global 2
call 1
pop
label_56:
; check expression
load_global 4
call 0
store_global 30
push 10
ge
load_global 30
push 15
le
land
lnot
isfalse
jmp label_54
label_55:
; if statement
push 1
push 1
add
push 2
gt
istrue
jmp label_58
; true branch
; arugment 0
push "1+1>2"
load_global 2
call 1
pop
jmp label_57
label_58:
; else branch
; arugment 0
push "1+1不大于2"
load_global 2
call 1
pop
label_57:
; arugment 0
push '来打个素数表试试\'\n'
load_global 3
call 1
pop
; for statement
; init
push 2
store 31
pop
label_59:
; check expression
load 31
push 1000
less
istrue
jmp label_60
; statement
; for statement
; init
push 2
store 32
pop
label_62:
; check expression
load 32
load 32
mul
load 31
le
istrue
jmp label_63
; statement
; if statement
load 31
load 32
mod
push 0
eq
istrue
jmp label_65
; true branch
jmp label_63
label_65:
label_64:
; loop expression
load 32
inc
store 32
pop
jmp label_62
label_63:
; if statement
load 32
load 32
mul
load 31
gt
istrue
jmp label_66
; true branch
; arugment 0
load 31
load_global 3
call 1
pop
; arugment 0
push " "
load_global 3
call 1
pop
label_66:
label_61:
; loop expression
load 31
inc
store 31
pop
jmp label_59
label_60:
; arugment 0
push "\nend"
load_global 2
call 1
pop
subsp 33
halt


; functions
; function expression
label_0:
load -1
load -2
add
ret
; function expression
label_2:
; arugment 2
load -1
; arugment 1
push "say:"
; arugment 0
load_external 0
load_global 2
call 3
pop
load null
ret
; function expression
label_3:
; arugment 4
load -2
; arugment 3
push 'love'
; arugment 2
load -1
; arugment 1
push "say:"
; arugment 0
load_external 0
load_global 2
call 5
pop
load null
ret
; function expression
label_1:
new_object
new_array
load -1
array_push
create_function label_2
reset_this
push "say"
object_set
pop
new_array
load -1
array_push
create_function label_3
reset_this
push "love"
object_set
pop
ret
; function expression
label_4:
; if statement
load -1
push 2
ge
istrue
jmp label_5
; true branch
; arugment 0
load -1
push 1
sub
load_global 9
call 1
; arugment 0
load -1
push 2
sub
load_global 9
call 1
add
ret
label_5:
load -1
ret
; function expression
label_10:
load_external 0
inc
store_external 0
ret
; function expression
label_9:
new_array
load -1
array_push
create_function label_10
ret
; function expression
label_15:
load_external 0
set_this
push "counter"
object_get
inc
object_reset
ret
; function expression
label_14:
new_array
load -1
array_push
create_function label_15
ret
