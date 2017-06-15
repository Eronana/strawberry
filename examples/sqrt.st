// 使用牛顿法求平方根
// 令 x = sqrt(A)
// 则 x^2 = A
// 设 f(x) = x^2 - A
// 显然, 若f(x)为0, 则x为sqrt(A)
// 对f(x)求导, 得f'(x) = 2x
// 对于f(x)上的任意一点(x0, f(x0)), 其切线与x轴交于(x1, 0)
// 显然f(x0) / (x0 - x1) = f'(x0)成立
// 则x1 = (x0 + A/x0)/2
// 并且易证f(x1)比f(x0)更加接近于0

var abs = function (a) {
  return a > 0.0 ? a : -a;
};

var sqrt = function (a) {
  a = 1.0 * a;
  var x = a / 10;
  while(true) {
    var xx = (x + a / x) / 2;
    if (xx == x) return xx;
    x = xx;
  }
};

system.print('请输入一个数字:');
var a=system.scani();
system.println(a,'的平方根是:',sqrt(a));
