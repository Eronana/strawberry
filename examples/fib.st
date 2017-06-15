var fib = function (n) {
  if (n < 2) return n;
  return callee(n - 1) + callee(n - 2);
};

system.print('你要求斐波那契数列的第几项?');
var a=system.scani();
system.println('斐波那契数列的第',a,'项是:',fib(a));
