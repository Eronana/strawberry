var println = system.println;

var o = {
  a: 'in o'
};

var a = { };
println(o.a);
println(a.a);
a.__proto__ = o;
println(a.a);
a.a = 'in a';
println(o.a);
println(a.a);

var s = "12345";
println(s.length);
system.prototypes.String.length = function () {
  return system.len(this);
};
println(s.length);
println(s.length());
s+='678';
println(s.length());
