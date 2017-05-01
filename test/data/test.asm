load 15,3
load 16,4
load 17,5
add 16,17
mul 15,16
load 16,2
shl 15,16
set_global 1,15
get_global 15,0
set_this 15
load 15,"println"
object_get 15,15
set_global 2,15
get_global 15,0
set_this 15
load 15,"print"
object_get 15,15
set_global 3,15
get_global 15,0
set_this 15
load 15,"scani"
object_get 15,15
set_global 4,15
get_global 15,2
; arugment 0
load 16,"haha: "
get_global 17,1
add 16,17
call 17,1
grv 15
load 15,7
get_global 16,1
swap 15,16
mod 15,16
set_global 1,15
get_global 15,2
; arugment 0
load 16,"haha mo 7: "
get_global 17,1
add 16,17
call 17,1
grv 15
; for statement
; init
load 15,0
mov 5,15
label_0:
; check expression
mov 15,5
get_global 16,1
less 15,16
istrue 15
jmp label_1
; statement
get_global 15,0
set_this 15
load 15,"println"
object_get 15,15
; arugment 0
mov 16,5
mov 17,5
mul 16,17
call 17,1
grv 15
label_2:
; loop expression
mov 15,5
mov 16,15
inc 16
mov 5,16
jmp label_0
label_1:
get_global 15,2
; arugment 0
load 16,"来打印一个9*9乘法表"
call 17,1
grv 15
; for statement
; init
load 15,1
mov 5,15
label_3:
; check expression
mov 15,5
load 16,9
le 15,16
istrue 15
jmp label_4
; statement
; for statement
; init
load 15,1
mov 6,15
label_6:
; check expression
mov 15,6
mov 16,5
le 15,16
istrue 15
jmp label_7
; statement
get_global 15,3
; arugment 0
mov 16,6
call 17,1
grv 15
get_global 15,3
; arugment 0
load 16,"*"
call 17,1
grv 15
get_global 15,3
; arugment 0
mov 16,5
call 17,1
grv 15
get_global 15,3
; arugment 0
load 16,"="
call 17,1
grv 15
get_global 15,3
; arugment 0
mov 16,5
mov 17,6
mul 16,17
call 17,1
grv 15
get_global 15,3
; arugment 0
load 16,"  "
call 17,1
grv 15
label_8:
; loop expression
mov 15,6
mov 16,15
inc 16
mov 6,16
jmp label_6
label_7:
get_global 15,2
; arugment 0
load 16,""
call 17,1
grv 15
label_5:
; loop expression
mov 15,5
mov 16,15
inc 16
mov 5,16
jmp label_3
label_4:
get_global 15,2
; arugment 0
load 16,"现在开始测试..树煮"
call 17,1
grv 15
new_array 15
load 16,1
array_push 16
load 16,"222"
array_push 16
load 16,3.1315926
array_push 16
load 16,4
array_push 16
set_global 5,15
load 15,"sss"
get_global 16,5
set_this 16
load 16,1
object_get 16,16
swap 15,16
add 15,16
object_reset 15
get_global 15,0
set_this 15
load 15,"push"
object_get 15,15
set_global 6,15
get_global 15,0
set_this 15
load 15,"len"
object_get 15,15
set_global 7,15
get_global 15,2
; arugment 0
load 16,"----------------"
call 17,1
grv 15
; for statement
; init
load 15,0
mov 8,15
label_9:
; check expression
mov 15,8
get_global 16,7
; arugment 0
get_global 17,5
call 18,1
grv 16
less 15,16
istrue 15
jmp label_10
; statement
get_global 15,2
; arugment 0
get_global 16,5
set_this 16
mov 16,8
object_get 16,16
call 17,1
grv 15
label_11:
; loop expression
mov 15,8
mov 16,15
inc 16
mov 8,16
jmp label_9
label_10:
get_global 15,6
; arugment 4
load 16,"66666"
; arugment 3
load 17,"魔法师"
; arugment 2
load 18,"膜蛤"
; arugment 1
load 19,"fuck the world"
; arugment 0
get_global 20,6
; arugment 0
get_global 21,5
call 22,1
grv 20
call 21,5
grv 15
; if statement
get_global 15,6
; arugment 1
load 16,"666"
; arugment 0
get_global 17,5
call 18,2
grv 15
istrue 15
jmp label_12
; true branch
; for statement
; init
load 15,0
mov 8,15
label_13:
; check expression
mov 15,8
get_global 16,0
set_this 16
load 16,"len"
object_get 16,16
; arugment 0
get_global 17,5
call 18,1
grv 16
less 15,16
istrue 15
jmp label_14
; statement
get_global 15,2
; arugment 0
get_global 16,5
set_this 16
mov 16,8
object_get 16,16
call 17,1
grv 15
label_15:
; loop expression
mov 15,8
mov 16,15
inc 16
mov 8,16
jmp label_13
label_14:
label_12:
; if statement
get_global 15,6
; arugment 0
load 16,1
call 17,1
grv 15
lnot 15
istrue 15
jmp label_16
; true branch
get_global 15,2
; arugment 0
load 16,"不能push非树煮"
call 17,1
grv 15
label_16:
new_object 15
load 16,"world"
load 17,"string"
object_set 17,16
load 16,1234f
load 17,"int"
object_set 17,16
load 16,222.123123
load 17,"float"
object_set 17,16
new_array 16
load 17,1
array_push 17
load 17,2
array_push 17
load 17,3
array_push 17
load 17,"xxx"
array_push 17
new_object 17
new_array 18
load 19,1234
array_push 19
new_array 19
load 20,6666
array_push 20
load 20,77777
array_push 20
set_this 18
array_push 19
set_this 17
load 19,"wtf"
object_set 19,18
set_this 16
array_push 17
set_this 15
load 17,"array"
object_set 17,16
new_object 16
load 17,"maki"
load 18,"nico"
object_set 18,17
load 17,2.3333
load 18,"kotiri"
object_set 18,17
set_this 15
load 17,"object"
object_set 17,16
set_global 8,15
get_global 15,0
set_this 15
load 15,"get_keys"
object_get 15,15
set_global 9,15
get_global 15,9
; arugment 0
get_global 16,8
call 17,1
grv 15
set_global 10,15
; for statement
; init
load 15,0
mov 11,15
label_17:
; check expression
mov 15,11
get_global 16,7
; arugment 0
get_global 17,10
call 18,1
grv 16
less 15,16
istrue 15
jmp label_18
; statement
get_global 15,2
; arugment 0
get_global 16,10
set_this 16
mov 16,11
object_get 16,16
call 17,1
grv 15
label_19:
; loop expression
mov 15,11
mov 16,15
inc 16
mov 11,16
jmp label_17
label_18:
get_global 15,8
set_this 15
load 15,"array"
object_get 15,15
set_this 15
load 15,4
object_get 15,15
set_this 15
load 15,"wtf"
object_get 15,15
set_this 15
load 15,1
object_get 15,15
set_this 15
load 15,1
object_get 15,15
mov 16,15
inc 16
object_reset 16
get_global 15,8
set_this 15
load 15,"array"
object_get 15,15
set_this 15
load 15,4
object_get 15,15
set_this 15
load 15,"wtf"
object_get 15,15
set_this 15
load 15,1
object_get 15,15
set_global 11,15
get_global 15,6
; arugment 3
load 16,"hahaha"
; arugment 2
load 17,2222
; arugment 1
load 18,"1"
; arugment 0
get_global 19,11
call 20,4
grv 15
; for statement
; init
load 15,0
mov 12,15
label_20:
; check expression
mov 15,12
get_global 16,7
; arugment 0
get_global 17,11
call 18,1
grv 16
less 15,16
istrue 15
jmp label_21
; statement
get_global 15,2
; arugment 0
get_global 16,11
set_this 16
mov 16,12
object_get 16,16
call 17,1
grv 15
label_22:
; loop expression
mov 15,12
mov 16,15
inc 16
mov 12,16
jmp label_20
label_21:
get_global 15,2
; arugment 0
load 16,"asadgasd"
call 17,1
grv 15
get_global 15,3
; arugment 0
load 16,"请输入一个在[10,15]之间的整数\n"
call 17,1
grv 15
; while statement
label_23:
; check expression
get_global 15,4
call 16,0
grv 15
set_global 12,15
load 16,10
ge 15,16
get_global 16,12
load 17,15
le 16,17
land 15,16
lnot 15
istrue 15
jmp label_24
; statement
get_global 15,2
; arugment 0
load 16,"请输入一个在[10,15]之间的整数"
call 17,1
grv 15
jmp label_23
label_24:
get_global 15,2
; arugment 0
load 16,"我知道..用do写会更优雅一些...上面我只是为了测试while..."
call 17,1
grv 15
get_global 15,2
; arugment 0
load 16,"于是我用do再来一次"
call 17,1
grv 15
; do statement
label_25:
; statement
get_global 15,2
; arugment 0
load 16,"请输入一个在[10,15]之间的整数"
call 17,1
grv 15
label_27:
; check expression
get_global 15,4
call 16,0
grv 15
set_global 12,15
load 16,10
ge 15,16
get_global 16,12
load 17,15
le 16,17
land 15,16
lnot 15
isfalse 15
jmp label_25
label_26:
; if statement
load 15,1
load 16,1
add 15,16
load 16,2
gt 15,16
istrue 15
jmp label_29
; true branch
get_global 15,2
; arugment 0
load 16,"1+1>2"
call 17,1
grv 15
jmp label_28
label_29:
; else branch
get_global 15,2
; arugment 0
load 16,"1+1不大于2"
call 17,1
grv 15
label_28:
; switch statement
get_global 15,4
call 16,0
grv 15
; default clause
label_32:
get_global 16,2
; arugment 0
load 17,"默认..."
call 18,1
grv 16
jmp label_34
label_31:
load 16,1
eq 16,15
istrue 16
jmp label_33
label_34:
get_global 16,2
; arugment 0
load 17,"case 1"
call 18,1
grv 16
jmp label_30
label_33:
load 16,3
eq 16,15
istrue 16
jmp label_35
get_global 16,2
; arugment 0
load 17,"case 3"
call 18,1
grv 16
label_35:
jmp label_32
label_30:
get_global 15,3
; arugment 0
load 16,'来打个素数表试试\'\n'
call 17,1
grv 15
; for statement
; init
load 15,2
mov 13,15
label_36:
; check expression
mov 15,13
load 16,1000
less 15,16
istrue 15
jmp label_37
; statement
; for statement
; init
load 15,2
mov 14,15
label_39:
; check expression
mov 15,14
mov 16,14
mul 15,16
mov 16,13
le 15,16
istrue 15
jmp label_40
; statement
; if statement
mov 15,13
mov 16,14
mod 15,16
load 16,0
eq 15,16
istrue 15
jmp label_42
; true branch
jmp label_40
label_42:
label_41:
; loop expression
mov 15,14
mov 16,15
inc 16
mov 14,16
jmp label_39
label_40:
; if statement
mov 15,14
mov 16,14
mul 15,16
mov 16,13
gt 15,16
istrue 15
jmp label_43
; true branch
get_global 15,3
; arugment 0
mov 16,13
call 17,1
grv 15
get_global 15,3
; arugment 0
load 16," "
call 17,1
grv 15
label_43:
label_38:
; loop expression
mov 15,13
mov 16,15
inc 16
mov 13,16
jmp label_36
label_37:
get_global 15,2
; arugment 0
load 16,""
call 17,1
grv 15
