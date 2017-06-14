var println = system.println;
var print = system.print;
var each = system.each;
var len = system.len;
var rand = system.rand;
var push = system.push;
var TEST_SIZE = 20;

// 快速排序
var sort = function (data) {
  var swap = function (a, b) {
    var t = data[a];
    data[a] = data[b];
    data[b] = t;
  };
  (function (l, r) {
    if (l >= r) return;
    var m = l;
    for(var i = l + 1; i < r; i++) if (data[i] > data[l]) swap(++m, i);
    swap(m, l);
    callee(l, m);
    callee(m + 1, r);
  })(0, system.len(data));
};

var printArray = function (a) {
  for(var i = 0; i < len(a); i++) print('' + a[i] + ', ');
  print('\n');
};

var a = [];

for(var i = 0; i < TEST_SIZE; i++) push(a, rand()%10007);
println("排序前:");
printArray(a);
sort(a);
println("排序后:");
printArray(a);
