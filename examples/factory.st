var Human = function (name, age) {
  var human = {
    name: name,
    age: age,
    hello: function () {
      system.println("Hello, I am " + this.name + ", I'am " + this.age + " years old.");
    },
    fight: function (x) {
      system.println(this.name + " is the best. I will beat you, " + x.name + ".");
    },
  };
  return human;
};

var a = Human('小明', 15);
var b = Human('小李', 16);
a.hello();
b.hello();
a.fight(b);

b.name = '小红';
a.name = '小绿';
b.fight(a);
