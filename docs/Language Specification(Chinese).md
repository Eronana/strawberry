# Strawberry 语言规范

[TOC]

## 简介

Strawberry是一个编译到字节码并由虚拟机执行的脚本语言, 虚拟机有垃圾回收功能(`Garbage Collection`), 语言使用者不必手动管理内存. 由于语言实现了闭包(`Closure`)功能, 所以对函数式编程风格有一定的支持. 并且由于内置了对象类型(`Object`), 所以再一定程度上也支持了面向对象编程风格.

Strawberry语言的设计和语言特性主要参考了ECMAScript(Javascript), 所以ECMAScript的用户几乎可以无缝切换到Strawberry. 尽管很多特性都参考了ECMAScript, 但是实现细节并不相同, 比如对值类型的闭包变量的处理就完全不同, 这些不同表明Strawberry并不是ECMAScript的翻版, 而是一门独立的语言.

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

#### 空类型

空类型(`Null`)的值只有一个, 也就是`null`. 

与ECMAScript不同, Strawberry 只有`null`,没有`undefined`.

#### 布尔类型

布尔类型(`Bool`)的值有两个: `true`表示真, `false`表示假.

#### 整数类型

整数类型(`Int`),即有符号正数类型, 其表示范围由编译器与虚拟机编译到的平台决定. 

通常来说, 如果编译到32位, 整数范围为$[-214783648, 2147483647]$, 也就是$[-2^{31}, 2^{31}-1]$.

如果编译到64位, 整数范围为$[9223372036854775808, 9223372036854775807]$,也就是$[-2^{63}, 2^{63}-1]$.

整数的字面值可以是二进制, 八进制, 十进制和十六进制.

二进制整数的前缀为`0b`, 如`0b0101101010`

八进制整数的前缀为`0`, 如`01234567`

十进制整数没有前缀, 如`123456789`

十六进制整数的前缀为`0x`, 如`0x1234defg

#### 实数类型

实数类型(`Float`)也叫浮点数类型, 其表示范围由`double`类型决定, 通常情况下为$[2.2^{-308},1.8^{308}]$ .

字面值位带小数点(`.`)的实数, 或者是添加浮点数后缀`f`或`F`. 如`1.234`, `123f`和`456F`. 

#### 字符串类型

字符串类型(`String`)用于存储字符串.

用单引号`'`或双引号`"`引起来的字符为字符串字面值.

支持使用下标存取字符, 如 `str[1]="s"`.

#### 数组类型

数组类型(`Array`)可以存储多个连续的任意类型的数据, 包括数组类型.

数字字面值为用`[]`括起来的, 并且用`,`分隔的数据, 如`[1,2,"str"]`.

访问对象元素可以使用下标操作符`[]`, 操作符内的值将被当做整数处理.

#### 对象类型

对象类型(`Object`)可以存储若干个由`键`和`值`组成的数据对.

`键`必须为字符串类型, `值`可以为任何类型, 包括对象类型.

其字面值由`{`开始, 由`}`结束. 在`{`和`}`之间是若干个(可以为0)数据对, 数据对之间用`,`分隔. 每个数据对的`键`和`值`之间由`:`分隔.

下面是一个object字面值的例子:

```javascript
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
#### 函数类型

与ECMAScript一样, 函数(`function`)在Strawberry中同样是一等公民(`first-class citizen`).

这意味着函数可以同其他的数据类型一样使用, 如赋值, 作为参数传给函数.

函数类型的定义会在稍后的章节提到.

#### 本地函数类型

同大多数脚本语言一样, 脚本需要都需要和本地语言进行交互, 于是使用本地语言编写的函数在ECMAScript中叫做本地函数(`native function`), 因为是宿主虚拟机内建的函数, 所以在Strawberry中本地函数也叫内建函数(`build-in function`).

在使用过程中函数和本地函数对于用户来说是透明的, 也就是在是用上这两种函数没有任何区别.

### 表达式

表达式是由若干个字面值或标识符或表达的若干次的合法操作所组成的.

合法的操作包括函数调用, 一元, 二元, 多元操作符的运算.

#### 函数表达式

`function` `(`  `参数列表` `)` `语句块`

参数列表为若干个由`,`分隔的标识符.

函数通常会赋值给一个变量, 这就像是其他语言的函数声明一样.

一个经典的函数表达式如下:

```javascript
var f=function(a,b)
{
  return a+b;
};
```

参数的个数可以为0, 也就是这样的函数:

```javascript
var f=function()
{
  return 666;
};
```

`语句块`中可以使用非参数非函数内的变量, 如全局变量:

```javascript
var a=5;
var f=function(b)
{
  return a+b;
};
```

如果`语句快`中使用了既不是参数, 也不是函数内声明的变量, 还不是全局变量, 那么这种变量叫做`闭包变量`.

下面是使用了`闭包变量`的例子:

```javascript
var f=function(a) // 函数1
{
  return function(b) //函数2
  {
    return a+b;
  };
};
```

对于上面的例子中, `函数2`所使用的变量`a`, 很显然不是这个函数的参数, 也不是这个函数内声明的变量, 更不是全局变量, 因为`a`是`函数1`的参数.

于是`函数2`在创建时, 将会对`a`求值, 并将其和`函数2`绑定到一起.

#### 函数调用

`f(a,b,c,d)`

`f`为函数类型的变量, `a`, `b`, `c`, `d`为参数, 参数的个数可以是零个, 即`f()`. 也可以是任意多个.

函数表达式的值为所调用函数的返回值.

#### 一元操作符

如果所操作的元素与预期不同(如对一个数组类型进行`++`操作), 则结果为`null`

- 取类型

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

- 自增

  `a++`的值为`a`本身, 但是取值之后将`a`增加1.

  `var b=a++;`相当于`var b=a;a=a+1;`

  `++a`则先将`a`增加1, 然后返回增加后的值.

  `var b=++a;`相当于`a=a+1;var b=a;`

- 自减

  `a--`的值为`a`本身, 但是取值之后将`a`减少1.

  `var b=a--;`相当于`var b=a;a=a-1;`

  `--a`则先将`a`减少1, 然后返回减少后的值.

  `var b=--a;`相当于`a=a-1;var b=a;

- 取正

  `+a`

  如果`a`的类型是整数或是实数, 则返回`a`本身, 否则相当于把`a`转换为浮点数类型.

- 取负

  `-a`

  对操作数取负. 如果`a`既不是整数也不是实数, 那么则返回`null`.

- 按位取反

  `~a`

  按位取反. 如果`a`不是整数, 则返回`null`.

- 逻辑取反

  `!a`

  逻辑取反操作符, 先讲`a`转为布尔类型, 然后取反.

- 下标运算符

  `a[i]`

  `a`的类型必须是`Array`, `Object`, 和`String`三者只一, 否则表达式的值为`null`.

  若`a`的类型为`Array`, 则把`i`视为整数. `a[i]`的值为数组`a`的第`i`个元素.

  若`a`的类型为`Object`, 则把`i`视为字符串, `a[i]`的值为对象`a`对应的键为`i`的值.

  若`a`的类型为`String`, 则把`i`视为整数. `a[i]`的的值为字符串`a`的第`i`个字符.

- 成员运算符

  `a.b`

  `a`的类型必须是`Object`, 否则表达式的值为`null`.

  `b`必须为合法标识符, `a.b`完全等价于`a["b"]`.

#### 二元操作符

如果所操作的元素与预期不同(如对一个数组类型进行`"abc"/"def"`操作), 则结果为`null`.

若`O`为一个二元操作符, 有表达式是`a O b`, 进行运算时, 将尝试把`b`转为与`a`相同的类型并进行运算. 如果转换失败, 则表达式的值为`null`

- 相加

  有表达式`a+b`, `a`的类型为必须为`Int`, `Float`, 或`String`.

  若`a`的类型为`Int`或`Float`, 则执行加法操作.

  若`a`的类型为`String`, 则执行字符串连接操作.

- 相减

  有表达式`a-b`, `a`的类型为必须为`Int`, `Float`, 进行加法操作.

- 相乘

  有表达式`a*b`, `a`的类型为必须为`Int`, `Float`, 或`String`.

  若`a`的类型为`Int`或`Float`, 则执行乘法操作.

  若`a`的类型为`String`, 则将`b`视为整数, 并且表达式的值为`b`个`a`.

- 相除

  有表达式`a/b`, `a`的类型为必须为`Int`, `Float`, 进行除法操作.

- 取模

  有表达式`a%b`, `a`的类型为必须为`Int`, `Float`, 进行取模操作.

- 左移

  `a<<b`

  将`a`左移`b`位.

  `a`必须为整数, `b`必须可以转换为整数, 否则表达式的值为`null`.

- 右移

  `a>>b`

  将`a`右移`b`位.

  `a`必须为整数, `b`必须可以转换为整数, 否则表达式的值为`null`.

- 小于

  `a<b`

  `a`和`b`必须为数字类型, 即必须为`Int`或`Float`两者之一, 否则表达式的值为`null`.

  如果`a`小于`b`, 则为`true`, 否则为`false`.

- 小于等于

  `a<=b`

  `a`和`b`必须为数字类型, 即必须为`Int`或`Float`两者之一, 否则表达式的值为`null`.

  如果`a`小于等于`b`, 则为`true`, 否则为`false`.

- 大于

  `a>b`

  `a`和`b`必须为数字类型, 即必须为`Int`或`Float`两者之一, 否则表达式的值为`null`.

  如果`a`大于`b`, 则为`true`, 否则为`false`.

- 大于等于

  `a>=b`

  `a`和`b`必须为数字类型, 即必须为`Int`或`Float`两者之一, 否则表达式的值为`null`.

  如果`a`大于等于`b`, 则为`true`, 否则为`false`.

- 相等

  `a==b`

  判断`a`和`b`是否相等.

  若`a`和`b`的类型不等, 则为`false`.

  否则判断`a`和`b`的值是否相等.

  对于`Int`, `Float`和`Bool`来说, 值相等即为`true`, 否则为`false`.

  对于`String`, `Object`和`Array`来说, 只要内部的元素全部相等即为`true`, 否则为`false`. 注意, Strawberry并不是像其他语言一样只有引用相等的时候才相等.

  对于`Function`来说, 必须调用函数和闭包变量都相等时, 才为`true`, 否则为`false`.

  对于`Native Function`来说, 只要指向的是同一个内建函数即为`true`.

- 不等

  `a!=b`

  相当于`!(a==b)`

- 按位与

  `a&b`

  讲`a`的每一位和`b`的每一位进行与运算.

  `a`和`b`必须都为整数 否则表达式的值为`null`.

- 异或

  `a^b`

  讲`a`的每一位和`b`的每一位进行异或运算.

  `a`和`b`必须都为整数 否则表达式的值为`null`.

- 按位或

  `a|b`

  讲`a`的每一位和`b`的每一位进行或运算.

  `a`和`b`必须都为整数 否则表达式的值为`null`.

- 逻辑与

  若有表达式`a&&b`, 则`a`先被求值.

  如果`a`为`true`, 则会对`b`求值, 并且整个表达式的值为`b`的值

  否则整个表达式的值为`a`的值, 并不会对`b`求值.

- 逻辑或

  若有表达式`a||b`, 则`a`先被求值.

  如果`a`为`true`, 则整个表达式的值为`a`的值, 并不会对`b`求值.

  否则会对`b`求值, 并且整个表达式的值为`b`的值

- 赋值

  `a=b`

  若`a`为标识符, 则直接对`b`求值, 然后把`b`的值赋给`a`.

  若`a`为表达式, 则先对`a`求值, 然后把`b`的值赋给`a`.

- 操作并赋值

  若`O`为操作符`+`, `-`, `*`, `/`, `%`, `<<`, `>>`, `|`, `^`, `&`中的一个.

  则`a O= b`相当于`a=a O b`.

#### 其他操作符

- 三元操作符

  若有表达式`a?x:y`, 则先对`a`求值.

  若`a`为`true`, 则对`x`求值, 并将其作为整个表达式的值.

  否则对`y`求值, 并将其作为整个表达式的值.

- 逗号操作符

  被逗号分隔的多个表达式会被依次求值, 并将最后一个表达式的值作为整个表达式的值.

  例如, 若有表达式`a,b,c,d`, 则依次对`a`, `b`, `c`, `d`进行求值, 并且将最后一个子表达式(`d`)的值作为整个表达式的值.

#### 操作符优先级

| 优先级  | 操作符    | 描述              | 结合性  |
| ---- | ------ | --------------- | ---- |
| 1    | ++     | 后缀自增            | 从左到右 |
| 1    | --     | 后缀自减            | 从左到右 |
| 1    | ()     | (表达式), 函数调用(参数) | 从左到右 |
| 1    | []     | 下标运算符           | 从左到右 |
| 1    | .      | 成员运算符           | 从左到右 |
| 2    | ++     | 前缀自增            | 从右到左 |
| 2    | --     | 前缀自减            | 从右到左 |
| 2    | +      | 取正              | 从右到左 |
| 2    | -      | 取负              | 从右到左 |
| 2    | !      | 逻辑否             | 从右到左 |
| 2    | ~      | 按位否             | 从右到左 |
| 2    | typeof | 取类型             | 从右到左 |
| 3    | *      | 相乘              | 从左到右 |
| 3    | /      | 相除              | 从左到右 |
| 3    | %      | 取模              | 从左到右 |
| 4    | +      | 相加              | 从左到右 |
| 4    | -      | 相减              | 从左到右 |
| 5    | <<     | 左移              | 从左到右 |
| 5    | \>\>   | 右移              | 从左到右 |
| 6    | <      | 小于              | 从左到右 |
| 6    | <=     | 小于等于            | 从左到右 |
| 6    | >      | 大于              | 从左到右 |
| 6    | \>=    | 大于等于            | 从左到右 |
| 7    | ==     | 相等              | 从左到右 |
| 7    | !=     | 不等              | 从左到右 |
| 8    | &      | 按位与             | 从左到右 |
| 9    | ^      | 异或              | 从左到右 |
| 10   | \|     | 按位或             | 从左到右 |
| 11   | &&     | 逻辑与             | 从左到右 |
| 12   | \|\|   | 逻辑或             | 从左到右 |
| 13   | ?:     | 三元操作符           | 从右到左 |
| 14   | =      | 赋值              | 从右到左 |
| 14   | +=     | 相加并赋值           | 从右到左 |
| 14   | -=     | 相减并赋值           | 从右到左 |
| 14   | *=     | 相乘并赋值           | 从右到左 |
| 14   | /=     | 相除并赋值           | 从右到左 |
| 14   | %=     | 取模并赋值           | 从右到左 |
| 14   | <<=    | 左移并赋值           | 从右到左 |
| 14   | \>\>=  | 右移并赋值           | 从右到左 |
| 14   | &=     | 按位与并赋值          | 从右到左 |
| 14   | ^=     | 异或并赋值           | 从右到左 |
| 14   | \|=    | 按位或并赋值          | 从右到左 |
| 15   | ,      | 逗号运算符           | 从左到右 |

#### 求值顺序

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


- 赋值

  `a=b`

  若`a`为标识符, 则直接对`b`求值, 然后把`b`的值赋给`a`.

  若`a`为表达式, 则先对`a`求值, 然后把`b`的值赋给`a`.

- 操作并赋值

  若`O`为操作符`+`, `-`, `*`, `/`, `%`, `<<`, `>>`, `|`, `^`, `&`中的一个.

  则`a O= b`相当于`a=a O b`.

### 语句

#### 语句块

语句块是由`{`和`}`包裹的若干个语句.

例如:

```
{
  a+=b;
  a++;
}
```

语句块中也可以没有任何语句, 如`{}`.

#### 变量声明语句

变量声明语句由`var`开始, 由`;`结束. 中间是由`,`分隔的若干个变量名.

如果变量需要初始化, 则可以在变量名的后面跟`=`,再跟一个表达式, 其值作为该变量的初始值.

下面是几个变量声明的例子:

```javascript
var a;
var b,c,d;
var e=5,f,g=7;
var h=a+b, g=a*c;
var s='abc'+'def';
```

#### 空语句

空语句就是只有一个`;`的语句

#### 表达式语句

任何一个合法的表达式的后面跟一个`;`, 都是一个表达式语句.

例如:

```javascript
a+b;
f(1,2,3);
a+f(5)*6;
array[1]+object.key*f(1)[1]["123"]+4;
```

#### if语句

if语句有两种形式:

- `if` `(` `表达式`  `)` `语句`

  如果`表达式`的值为`true`, 则执行`语句`.

- `if` `(` `表达式`  `)` `语句1` `else` `语句2`

  如果`表达式`的值为`true`, 则执行`语句1`, 否则执行`语句2`.

#### do-while语句

`do` `语句` `while` `(` `表达式`  `)` `;`

首先会执行`语句`, 然后对`表达式`求值, 如果为`true`, 则一直重复这个过程. 否则`do-while语句`结束.

#### while语句

`while` `(` `表达式` `)` `语句`

对`表达式`进行求值, 如果为`true`, 则执行`语句`, 然后重复这个过程. 否则`while语句`结束.

#### for语句

for语句有两种形式

- `for` `(` `声明语句` `表达式1` `;` `表达式2` `)` `语句`

  在`for语句`的作用域内用`声明语句`声明变量.

  然后对`表达式1`进行求值, 如果是`true`, 则执行`语句`, 然后执行`表达式2`. 一直重复这个过程, 直到`表达式1`的值为`false`.

- `for` `(` `表达式1` `;` `表达式2 ` `;` `表达式3` `)` `语句`

  首先执行`表达式1`.

  然后对`表达式2`进行求值, 如果是`true`, 则执行`语句`, 然后执行`表达式3`. 一直重复这个过程, 直到`表达式2`的值为`false`.

#### switch语句

`switch` `(` `表达式` `)` `{` 若干个case从句`}`

`case从句`有两种形式

- `case ` `表达式` `:` 若干个语句.
- `default` `:` 若干个语句.

`switch语句`会对`表达式进行求值`, 然后逐一与每个`case从句`中的`表达式`进行比较, 如果相等, 则执行该`case从句`中的若干个语句. 如果这若干个语句中没有`break语句`, 则执行完后会直接贯穿到下一个`case从句`中的若干个表达式中.

如果没有找到相匹配的`case从句`, 并且存在`default`, 就会执行`default`中的若干个语句, 否则结束`switch语句`.

于是`default`形式的`case从句`最多只能有一个.

#### continue语句

`continue` `;`

用于跳过当前的循环, 并进入下一次循环.

可以用于`do-while语句`, `while语句`和`for语句`.

#### break语句

`break` `;`

用于跳出`do-while语句`, `while语句`, `for语句`以及`switch语句`

#### return语句

return语句用于终止并返回函数, 有两种形式:

- `return` `表达式` `;`

  对`表达式`进行求值, 并将其作为函数的返回值.

- `return` `;`

  相当于 `return null;`

### 变量作用域

和大多数语言一样, 对于不同层次作用域的同名变量, 最内层的会生效, 也就是内层变量的优先级最高, 也叫做内层变量屏蔽外层变量.

#### 全局作用域

不在函数内, 不在语句内, 而是直接在最顶层声明的变量, 就是`全局变量`.

`全局变量`可以再任意位置被访问.

`内建函数`所依附的`全局对象`实际上就是一个内建的`全局变量`

#### 语句块作用域

在语句块中声明的变量拥有其自己的作用域.

#### for语句作用域

`for语句`的其中一种形式允许再`for语句`内声明变量, 这种变量的作用域就是`for语句`, 并且这个作用域位于`for语句`的语句块的作用域的上层.

#### 函数参数作用域

同`for语句`的作用域相似, 函数的参数的作用域位于函数的`语句块`作用域的上层.

### 内建函数

所有的内建函数都位于全局对象`system`中, 所以使用内建函数时, 必须从`system`中获取.

如内建函数`println`的调用方法为:

```javascript
system.println("hello world");
```

当然, 也可以把内建函数保存到其他的变量中, 然后直接调用:

```javascript
var pln = system.println;
pln("hello world again");
```

#### print

接受任一个参数, 将每个参数输出到`stdout`中, 多个参数的的输出之间用`,`隔开.

`system.print(1,'a',2); 将得到`1, a, 2`.

返回值恒为`null`.

#### println

相当于调用`print`之后再调用`print('\n')`.

#### scani

无参数, 从`stdin`中读取一个整数并返回.

#### scanf

无参数, 从`stdin`中读取一个实数并返回.

#### scans

无参数, 从`stdin`中读取一个字符串并返回.

#### exit

无参数, 作用是终止并退出程序

#### len

接受一个参数.

如果参数的类型是`String`, `Array`或`Object`, 则返回其元素的个数, 否则返回`1`.

#### push

接收最少1个参数.

第一个参数的类型必须是`Array`, 从第二个参数开始, 依次追加到第一个参数的数组中.

#### clear

接受1个参数, 参数类型必须是`Array`或`Object`, 会清空里面的所有元素.

#### resize

接受2个参数`a`和`i`.

`a`的类型必须是`Array`, 会将`a`的大小更新为`i`.

#### get_keys

接受一个参数, 参数类型必须是`Array`或`Object`.

如果参数类型是`Array`, 则会得到一个从`0`开始依次递增的大小为数组大小的一个数组.

如果参数类型是`Objecg`, 则会得到一个包含所有键值的字符串数组.

如果是其他类型, 则返回空数组.

#### gc

无参数, 返回`null`.

调用虚拟机的垃圾回收功能.

#### each

接受两个参数`o`和`f`.

`f`必须是一个接收两个参数的函数, 每个元素`v`及其键值`k`将会传个`f`调用, 于是有多少个元素, `f`就会被调用多少次.

如果`o`的类型是`Array`, 键值就是值对应的整数下标.

如果`o`的类型是`Object`, 键值就是值对应的字符串键值.

