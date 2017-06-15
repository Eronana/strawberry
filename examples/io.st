var println=system.println;
var print=system.print;
var each=system.each;

println('-'*10+'测试输出'+'-'*10);
var outData = {
  '字符串':'hello world',
  '整数':12345,
  '浮点数':123.45,
  'null':null,
  '布尔类型':true,
  '数组':[1,2,3],
  '对象':{a:6},
  '函数':function(){},
};
each(outData,function(v,k){
  println('输出['+k+']:',v);
});

println('-'*10+'测试输入'+'-'*10);
var inData = {
  '整数': system.scani,
  '浮点数': system.scanf,
  '字符串': system.scans,
};
each(inData,function(f,k){
  print('请输入一个['+k+']以供读入:');
  var v=f();
  println('你输入的['+k+']是:'+v);
});

println("测试完成");
