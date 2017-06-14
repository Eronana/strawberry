// 快速排序
var sort = function (data, cmp) {
  var swap = function (a, b) {
    var t = data[a];
    data[a] = data[b];
    data[b] = t;
  };
  (function (l, r) {
    if (l >= r) return;
    var m = l;
    for(var i = l + 1; i < r; i++) if (cmp(data[i], data[l])) swap(++m, i);
    swap(m, l);
    callee(l, m);
    callee(m + 1, r);
  })(0, system.len(data));
};

var scani = system.scani;
var new_array = system.new_array;
var println = system.println;
var push = system.push;

var N = 200005;
var Edge = function () {
  this.u = scani();
  this.v = scani();
  this.w = scani();
};

var edgeCmp = function (a, b) {
  return a.w < b.w;
};

var Father = new_array(N);

var Find = function (a) {
  return Father[a]?Father[a]=Find(Father[a]):a;
};



var n=scani(),m=scani(),k=scani();
var edge = [];
for(var i=0;i<m;i++)push(edge,Edge.new());
sort(edge,edgeCmp);
var ans=0;
for(var i=0;i<m;i++)
{
    var x=Find(edge[i].u);
    var y=Find(edge[i].v);
    if(x!=y)
    {
        ans+=edge[i].w;
        Father[x]=y;
    }
}
var cnt=n,x=Find(k);
for(var i=1;i<=n;i++)if(Find(i)==x)cnt--;
if(cnt)ans=cnt;
println(ans);

