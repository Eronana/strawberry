var a=[1,"222",3.1315926,4];
var each=function(a,f){
    for(var i=0;i<len(a);i++)f(a[i]);
};

each(a,function(a){
    display(a);
});

var b={
    "string":"world",
    "int":1234f,
    "float":222.123123,
    "array":[1,2,3,"xxx"],
    "function":function(){
        display("hello");
    },
    "object":{
        "nico":"maki",
        "kotiri":2.3333
    }
};

var keys=get_keys(b);//test comment
/*
test
/*nest comment
/*
nest*/
asdgasdgasdg
*/
*/
for(var i=0;i<len(keys);i++)
{
    var key=keys[i];
    display(key);
    display(b[key]);
}

a+=b;
c=b+c;
a>>b;
c<<=1;
d/=f;
~a+c;
display("END");
