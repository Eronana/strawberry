var Human = function (name, age) {
  this.name = name;
  this.age = age;
};

Human.prototype.hello = function () {
  system.println("Hello, I am " + this.name + ", I'am " + this.age + " years old.");
};

Human.prototype.fight = function (x) {
  system.println(this.name + " is the best. I will beat you, " + x.name + ".");
};

var a = Human.new('小明', 15);
var b = a.constructor.new('小李', 16);
a.hello();
b.hello();
a.fight(b);

b.name = '小红';
a.name = '小绿';
b.fight(a);

// 手动使用原型构建对象
var c = {
  name: '大红',
  age: 666,
  __proto__: b.constructor.prototype,
};

c.hello();
c.fight(b);

