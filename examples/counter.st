var Counter = function (init, inc) {
  init -= inc;
  return function () {
    return init += inc;
  };
};

var println = system.println;
var a = Counter(0,1);
println(a());
println(a());
println(a());
println(a());
println(a());

var b = Counter(100,10);
println(b());
println(b());
println(b());
println(b());
println(b());
