var haha=(3*(4+5))<<2;
var println=system.println;
var print=system.print;
var scani=system.scani;
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

print("请输入一个在[10,15]之间的整数\n");
var tmp;
while(!((tmp=scani())>=10&&tmp<=15))println("请输入一个在[10,15]之间的整数");
println("我知道..用do写会更优雅一些...上面我只是为了测试while...");
println("于是我用do再来一次");
do println("请输入一个在[10,15]之间的整数");while(!((tmp=scani())>=10&&tmp<=15));
if(1+1>2)println("1+1>2");
else println("1+1不大于2");
switch(scani())
{
    default:
        println("默认...");
    case 1:
        println("case 1");
        break;
    case 3:
        println("case 3");
}

print('来打个素数表试试\'\n');
for(var i=2;i<1000;i++)
{
    var j;
    for(j=2;j*j<=i;j++)if(i%j==0)break;
    if(j*j>i)print(i),print(" ");
}
println("");
/*
for(var i=0;i<4;i++)println(a[i]);
var len;
var each=function(a,f){
    for(var i=0;i<len(a);i++)f(a[i]);
};
var display;
each(a,function(a){
    display(a);
});


/*
test
/*nest comment
/*
nest*/
asdgasdgasdg
*/
*/
for(var i=0;i<keys.len;i++)
{
    var key=keys[i];
    display(key);
    display(b[key]);
}
var c;
a+=b;
c=b+c;
a>>b;
c<<=1;
var d,f;
d/=f;
~a+c;
display("END");
*/