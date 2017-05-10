# StrawBerry 语言规范

[TOC]

## 简介

StrawBerry是一个编译到字节码并由虚拟机执行的脚本语言, 虚拟机有垃圾回收功能(`Garbage Collection`), 语言使用者不必手动管理内存. 由于语言实现了闭包(`Closure`)功能, 所以对函数式编程风格有一定的支持. 并且由于内置了对象类型(`Object`), 所以再一定程度上也支持了面向对象编程风格.

StrawBerry语言的设计和语言特性主要参考了ECMAScript(Javascript), 所以ECMAScript的用户几乎可以无缝切换到StrawBerry. 尽管很多特性都参考了ECMAScript, 但是实现细节并不相同, 比如对值类型的闭包变量的处理就完全不同, 这些不同表明StrawBerry并不是ECMAScript的翻版, 而是一门独立的语言.

## 文法描述

```
Program
    : ( StatementList )?

PrimaryExpression
    : ObjectLiteral
    | "(" Expression ")"
    | Identifier
    | ArrayLiteral
    | Literal

Literal
    : NumberLiteral
    | STRING_LITERAL
    | BOOL_LITERAL
    | NULL_LITERAL

NumberLiteral
    : IntegerLiteral
    | FLOAT_LITERAL

IntegerLiteral
    : BIN_INTEGER_LITERAL
    | OCT_INTEGER_LITERAL
    | DEC_INTEGER_LITERAL
    | HEX_INTEGER_LITERAL

Identifier
    : IDENTIFIER

ArrayLiteral
    : "[" ( Elision )? ( ElementList )? ( Elision )? "]"

ElementList
    : ( Elision )? AssignmentExpression ( Elision AssignmentExpression )*

Elision
    : ( "," )+

ObjectLiteral
    : "{" ( PropertyNameAndValueList )? "}"

PropertyNameAndValueList
    : PropertyNameAndValue ( "," PropertyNameAndValue )* ( "," )?

PropertyNameAndValue
    : Identifier ":" AssignmentExpression

MemberExpression
    : PrimaryExpression ( MemberExpressionPart )*
    | FunctionExpression ( MemberExpressionPart )*

MemberExpressionPart
    : "[" Expression "]"
    | "." Identifer

CallExpression
    : MemberExpression Arguments ( CallExpressionPart )*

CallExpressionPart
    : Arguments
    | "[" Expression "]"
    | "." Identifer

Arguments
    : "(" (ArgumentList )? " )"

ArgumentList
    : AssignmentExpression ("," AssignmentExpression )*

LeftHandSideExpression
    : CallExpression
    | MemberExpression

PostfixExpression
    : LeftHandSideExpression ( PostfixOperator )?

PostfixOperator
    : "++"
    | "--"

PrefixExpression
    : PostfixExpression
    | ( UnaryOperator PrefixExpression )+

UnaryOperator
    : "typeof"
    | "++"
    | "--"
    | "+"
    | "-"
    | "~"
    | "!"

MultiplicativeExpression
    : PrefixExpression ( MultiplicativeOperator PrefixExpression )*

MultiplicativeOperator
    : "*"
    | "/"
    | "%"

AdditiveExpression
    : MultiplicativeExpression ( AdditiveOperator MultiplicativeExpression )*

AdditiveOperator
    : "+"
    | "-"

ShiftExpression
    : AdditiveExpression ( ShiftOperator AdditiveExpression )*

ShiftOperator
    : "<<"
    | ">>"

RelationalExpression
    : ShiftExpression ( RelationalOperator ShiftExpression )*

RelationalOperator
    : "<"
    | ">"
    | "<="
    | ">="

EqualityExpression
    : RelationalExpression ( EqualityOperator RelationalExpression )*

EqualityOperator
    : "=="
    | "!="

BitwiseANDExpression
    : EqualityExpression ( "&" EqualityExpression )*

BitwiseXORExpression
    : BitwiseANDExpression ( "^" BitwiseANDExpression )*

BitwiseORExpression
    : BitwiseXORExpression ( "|" BitwiseXORExpression )*

LogicalANDExpression
    : BitwiseORExpression ( "&&" BitwiseORExpression )*

LogicalORExpression
    : LogicalANDExpression ( "||" LogicalANDExpression )*

ConditionalExpression
    : LogicalORExpression ( "?" AssignmentExpression ":" AssignmentExpression )?

AssignmentExpression
    : LeftHandSideExpression AssignmentOperator AssignmentExpression
    | ConditionalExpression

AssignmentOperator
    : "="
    | "*="
    | "/="
    | "%="
    | "+="
    | "-="
    | "<<="
    | ">>="
    | "&="
    | "^="
    | "|="

Expression
    : AssignmentExpression ( "," AssignmentExpression )*

Statement
    : Block
    | VariableStatement
    | EmptyStatement
    | ExpressionStatement
    | IfStatement
    | ForStatement
    | DoStatement
    | WhileStatement
    | ContinueStatement
    | BreakStatement
    | ReturnStatement
    | SwitchStatement

EmptyStatement
    : ";"

Block
    : "{" ( StatementList )? "}"

StatementList
    : ( Statement )+

VariableStatement
    : "var" VariableDeclarationList ";"

VariableDeclarationList
    : VariableDeclaration ( "," VariableDeclaration )*

VariableDeclaration
    : Identifier ( Initialiser )?

Initialiser
    : "=" AssignmentExpression

ExpressionStatement
    : Expression ";"

IfStatement
    : "if" "(" Expression ")" Statement ( "else" Statement )?

ForStatement
    + "for" "(" ForStatementInitial ( Expression )? ";" ( Expression )? ")" Statement

ForStatementInitial
    : VariableStatement
    | ( Expression )? ";"

DoStatement
    : "do" Statement "while" "(" Expression ")" ";"

WhileStatement
    : "while" "(" Expression ")" Statement

ContinueStatement
    : "continue" ";"

BreakStatement
    : "break" ";"

ReturnStatement
    : "return" (Expression)? ";"

SwitchStatement
    : "switch" "(" Expression ")" CaseBlock

CaseBlock
    : "{" ( CaseClause )* ( DefaultClause )? ( CaseClause )* "}"

CaseClause
    : "case" Expression ":" ( StatementList )?

DefaultClause
    : "default" ":" ( StatementList )?

FunctionExpression
    : "function" "(" ( FormalParameterList )? ")" Block

FormalParameterList
    : Identifier ( "," Identifier )*
```

## 语言概述

### 数据类型

- 空类型

  空类型(`Null`)的值只有一个, 也就是`null`. 

  与ECMAScript不同, StrawBerry 只有`null`,没有`undefined`.

- 布尔类型

  布尔类型(`Bool`)的值有两个: `true`表示真, `false`表示假.

- 整数类型

  整数类型(`Int`),即有符号正数类型, 其表示范围由编译器与虚拟机编译到的平台决定. 

  通常来说, 如果编译到32位, 整数范围为$[-214783648, 2147483647]$, 也就是$[-2^{31}, 2^{31}-1]$.

  如果编译到64位, 整数范围为$[9223372036854775808, 9223372036854775807]$,也就是$[-2^{63}, 2^{63}-1]$.

  整数的字面值可以是二进制, 八进制, 十进制和十六进制.

  二进制整数的前缀为`0b`, 如`0b0101101010`

  八进制整数的前缀为`0`, 如`01234567`

  十进制整数没有前缀, 如`123456789`

  十六进制整数的前缀为`0x`, 如`0x1234defg

- 实数类型

  实数类型(`Float`)也叫浮点数类型, 其表示范围由`double`类型决定, 通常情况下为$[2.2^{-308},1.8^{308}]$ .

  字面值位带小数点(`.`)的实数, 或者是添加浮点数后缀`f`或`F`. 如`1.234`, `123f`和`456F`. 

- 字符串类型

  字符串类型(`String`)用于存储字符串.

  用单引号`'`或双引号`"`引起来的字符为字符串字面值.

  支持使用下标存取字符, 如 `str[1]="s"`.

- 数组类型

  数组类型(`Array`)可以存储多个连续的任意类型的数据, 包括数组类型.

  数字字面值为用`[]`括起来的, 并且用`,`分隔的数据, 如`[1,2,"str"]`.

  访问对象元素可以使用下标操作符`[]`, 操作符内的值将被当做整数处理.

- 对象类型

  对象类型(`Object`)可以存储若干个由`键`和`值`组成的数据对.

  `键`必须为字符串类型, `值`可以为任何类型, 包括对象类型.

  其字面值由`{`开始, 由`}`结束. 在`{`和`}`之间是若干个(可以为0)数据对, 数据对之间用`,`分隔. 每个数据对的`键`和`值`之间由`:`分隔.

  下面是一个object字面值的例子:

  ```json
  {
    "hello": 'world',
    'int': 1234,
    "float": 123.456,
    "array": [1, 2, 3, "456", ['nested', 'array']],
    "object": {"nest": ['nest', "object"]}
  }
  ```

  访问对象元素可以使用下标操作符`[]`或点操作符`.`.

  使用下标操作符访问元素时, 操作符内的值将被当做字符串处理.

  使用点操作符访问元素时, 元素的`键`必须为合法标识符.

  下面是用两种方法访问元素的例子:

  ```javascript
  var obj = {
    "key": "value",
    "123": "456"
  };
  var a = obj["key"]; //ok
  var b = obj.key; // ok
  var c = obj["123"]; //ok
  var d = obj.123; //error
  ```

- 函数类型

  与ECMAScript一样, 函数(`function`)在StrawBerry中同样是一等公民(`first-class citizen`).

  这意味着函数可以同其他的数据类型一样使用, 如赋值, 作为参数传给函数.

  函数类型的定义会在稍后的章节提到.

- 本地函数类型

  同大多数脚本语言一样, 脚本需要都需要和本地语言进行交互, 于是使用本地语言编写的函数在ECMAScript中叫做本地函数(`native function`), 因为是宿主虚拟机内建的函数, 所以在StrawBerry中本地函数也叫内建函数(`build-in function`).

  在使用过程中函数和本地函数对于用户来说是透明的, 也就是在是用上这两种函数没有任何区别.

### 一元操作符

如果所操作的元素与预期不同(如对一个数组类型进行`++`操作), 则结果为`null`

- typeof

  使用`typeof a`得到a的类型, 其值一定为下列之一

  - "Null"
  - "Bool"
  - "Int"
  - "Float"
  - "String"
  - "Array"
  - "Object"
  - "Function"
  - "Native Function"

- ++

  `a++`的值为`a`本身, 但是取值之后将`a`增加1.

  `var b=a++;`相当于`var b=a;a=a+1;``

  ``++a`则先将`a`增加1, 然后返回增加后的值.

  `var b=++a;`相当于`a=a+1;var b=a;`

- --

  `a--`的值为`a`本身, 但是取值之后将`a`减少1.

  `var b=a--;`相当于`var b=a;a=a-1;`

  `--a`则先将`a`减少1, 然后返回减少后的值.

  `var b=--a;`相当于`a=a-1;var b=a;

- +

  如果操作数的类型是整数或是实数, 则返回a本身, 否则相当于把a转换为浮点数类型.

- -

  对操作数取负. 如果操作数既不是整数也不是实数, 那么则返回`null`.

- ~

  按位取反. 如果操作数不是整数, 则返回`null`.

- !

  逻辑取反操作符, 先讲操作数转为不尔类型, 然后取反.

### 二元操作符

如果所操作的元素与预期不同(如对一个数组类型进行`"abc"*"def"`操作), 则结果为`null`

// TODO

### 求值顺序

简单来说, 除了函数调用, 其他的表达式的求值顺序都是从左到右的.

- 逻辑或运算符

  若有表达式`a||b`, 则`a`先被求值.

  如果`a`为`true`, 则整个表达式的值为`a`的值, 并不会对`b`求值.

  否则会对`b`求值, 并且整个表达式的值为`b`的值

- 逻辑与运算符

  若有表达式`a&&b`, 则`a`先被求值.

  如果`a`为`true`, 则会对`b`求值, 并且整个表达式的值为`b`的值

  否则整个表达式的值为`a`的值, 并不会对`b`求值.

- 逗号运算符

  若有表达式`a,b,c,d`, 则依次对`a`, `b`, `c`, `d`进行求值, 并且将最后一个子表达式(`d`)的值作为整个表达式的值.

- 下标运算符

  若有表达式`a[b]`, 则先对`a`进行求值, 然后对`b`进行求值, 最后求的`a[b]`的值作为整个表达式的值.

- 三元运算符

  若有表达式`a?x:y`, 则先对`a`求值.

  若`a`为`true`, 则对`x`求值, 并将其作为整个表达式的值.

  否则对`y`求值, 并将其作为整个表达式的值.

- 二元运算符

  若`O`为一个二元操作符, 并且有表达式`a O b`, 则先对`a`求值, 再对`b`求值, 最后将`a O b`的值作为整个表达式的值.

- 函数调用

  若有表达式`f(a,b,c,d)`, 则先对`f`求值, 然后参数按照相反的顺序依次求值, 即先对`d`求值,然后是`c`, `b`, 最后是`a`.

  ​