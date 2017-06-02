var println=system.println;
var print=system.print;
var scani=system.scani;
var each=system.each;



println("test string");
var sa="0123456789";
var sb=sa[0]+sa[8];
var sc="0123"[0]+"abcd"[1];
println(sb,sc);

println("test function");
var func=function(a,b)
{
    return a+b;
};
println("1+2="+func(1,2));


var eachArray=[1,2,3,"a",1.2345,true];
var eachObject={
    "hello":"world",
    "fuck":123456,
};
var printx=function(x)
{
    return function(k,v){
        println(x,k,v);
    };
};

println("each object");
each(eachObject,printx("OBJECT: "));

println("each array");
each(eachArray,function(k,v){
    println(k,v);
});





println("test closure");
var People=function(name)
{
    return {
        "say":function(s){println(name,"say:",s);},
        "love":function(a,b){println(name,"say:",a+' love '+b);},
    };
};
var ming=People("小明");
var hong=People("小红");
ming.say("I am a SB");
hong.say("I am a SB too");
hong.love('小明','我');

var cx=function(a)
{
    println("a:",a);
    return function(b){
        println("a:",a,"b:",b);
        return function(){
            println("a:",a,"b:",b);
            return a++ + b++;
        };
    };
};
var cxx=cx(1000)(100);
println(cxx());
println(cxx());
println(cxx());
println(cxx());
println("========");


var fib=function(n)
{
    if(n>=2)return fib(n-1)+fib(n-2);
    return n;
};

println("fib 0 to 19");
for(var i=0;i<20;i++)print(fib(i),"");
println("");
var counter=function(begin)
{
    return function(){return begin++;};
};

var c1=counter(100);
var c2=counter(0);
for(var i=0;i<20;i++)
{
    print(c1(),"");
    print(c2(),", ");
}
println("\n------");
var counter2=function(begin)
{
    return function(){return begin.counter++;};
};
var begin={"counter":0};
var c3=counter2(begin);
var c4=counter2(begin);
for(var i=0;i<20;i++)
{
    print(c3(),"");
    print(c4(),", ");
}
println("\n------");


println("test GC");
for(var i=0;i<1000000;i++)
{
    var x="hello"+"hello"+"hello"+"hello"+"hello";
    x=[[]];
}



var t=true,f=false;
var t1=t&&f;
var t2=t||f;
var t3=!t;
var t4=!f;
println(t,f,t1,t2,t3,t4);
var haha=(3*(4+5))<<2;
println("haha: "+haha);
haha%=7;
println("haha mo 7: "+haha);
for(var i=0;i<haha;i++)
{
    system.println(i*i);
}

println("来打印一个9*9乘法表");


for(var i=1;i<=9;i++)
{
    for(var j=1;j<=i;j++)
    {
        print(j);
        print("*");
        print(i);
        print("=");
        print(i*j);
        print("  ");
    }
    println("");
}

println("现在开始测试..树煮");
var a=[1,"222",3.1315926,4];
a[1]+="sss";
var push=system.push;
var len=system.len;
println("----------------");
for(var i=0;i<len(a);i++)println(a[i]);
push(push(a),"fuck the world","膜蛤","魔法师","66666");
if(push(a,"666"))for(var i=0;i<system.len(a);i++)println(a[i]);
if(!push(1))println("不能push非树煮");


var b={
    "string":"world",
    "int":1234f,
    "float":222.123123,
    "array":[1,2,3,"xxx",{"wtf":[1234,[6666,77777]]}],
    "object":{
        "nico":"maki",
        "kotiri":2.3333
    }
};

var get_keys=system.get_keys;
var keys=get_keys(b);

for(var i=0;i<len(keys);i++)println(keys[i]);
b.array[4].wtf[1][1]++;
var bwtf=b.array[4].wtf[1];
push(bwtf,"1",2222,"hahaha");
for(var i=0;i<len(bwtf);i++)println(bwtf[i]);
println("asadgasd");

switch(scani())
{
    default:
        println("默认...");
    case 1:
        println("case 1");
    case 3:
        println("case 3");
        break;
}

print("请输入一个在[10,15]之间的整数\n");
var tmp;

while(!((tmp=scani())>=10&&tmp<=15))println("请输入一个在[10,15]之间的整数");
println("我知道..用do写会更优雅一些...上面我只是为了测试while...");
println("于是我用do再来一次");
do println("请输入一个在[10,15]之间的整数");while(!((tmp=scani())>=10&&tmp<=15));
if(1+1>2)println("1+1>2");
else println("1+1不大于2");



print('来打个素数表试试\'\n');
for(var i=2;i<1000;i++)
{
    var j;
    for(j=2;j*j<=i;j++)if(i%j==0)break;
    if(j*j>i)print(i),print(" ");
}
println();
println('测试prototype和this');
system.prototypes.Number.sq=function(){
    return this*this;  
};

println("(fib(15)^2)^2 is",(function(n){
    if(n<2)return n;
    return callee(n-1)+callee(n-2);
})(15).sq().sq());

println('测试this指向');
(function(){
    println(this.prototypes.Number.sq);
    println(callee);
})();


var Foo=function(name,age){
    this.name=name;
    this.age=age;
};
Foo.prototype.hello=function(){
    system.println(this.name,"is",this.age,"years old");
};

var bar={__proto__:Foo.prototype};
system.call(bar,Foo,"小明",66);
bar.hello();


var tsetnew=system.new(Foo,"小红",88);
tsetnew.hello();
println('测试constructor');
println(tsetnew.constructor==Foo);

println('目前没有fuck属性');
println(bar.fuck);
println(tsetnew.fuck);
Foo.prototype.fuck=function(name){
    println(this.name,"fuck",name);
};
println('修改了函数的原型后就有了fuck属性');
println(bar.fuck);
println(tsetnew.fuck);
bar.fuck('小红');
tsetnew.fuck('小明');
system.prototypes.Function.new=system.__this_new__;

var ttt=tsetnew.constructor.new("小章",233);
ttt.hello();
ttt.fuck("小明");

println("\nEND");
