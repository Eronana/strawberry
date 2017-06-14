var scani = system.scani;
var len = system.len;
var new_array = system.new_array;
var push = system.push;
var println = system.println;

var Edge = function(v,w,next){
  this.v=v;
  this.w=w;
  this.next=next;
};
var Queue = function() {
  this.data = [];
  this.f=0;
};
Queue.prototype.front = function() {
  return this.data[this.f];
};
Queue.prototype.pop = function() {
  return this.data[this.f++];
};
Queue.prototype.push = function(e) {
  push(this.data,e);
};
Queue.prototype.size = function() {
  return len(this.data)-this.f;
};

var INF = 0x3f3f3f3f;

var n=scani(),m=scani();
var s=scani(),t=scani();
var N = n+10;
var M = N*10;

var edge = new_array(M*2);
var ehead = new_array(N);
var ecnt = 1;
var AddEdge = function(u,v,w) {
  edge[ecnt] = Edge.new(v,w,ehead[u]);
  ehead[u]=ecnt++;
};
for(var i=0;i<m;i++) {
  var u = scani(), v=scani(), w=scani();
  AddEdge(u,v,w);
}

var dist = [];
var inq = [];
var outq = [];
var q = Queue.new();
var PUSH = function(x) {
  inq[x]=true;
  q.push(x);
};
for(var i=0;i<N;i++) {
  push(dist,INF);
  push(inq,false);
  push(outq,0);
}
PUSH(s);
dist[s]=0;
while(q.size()) {
  var u=q.pop();
  inq[u]=false;
  if(++outq[u]>n) break;
  for(var i=ehead[u];i;i=edge[i].next)
  {
      var v=edge[i].v;
      var ndist=dist[u]+edge[i].w;
      if(ndist<dist[v])
      {
          dist[v]=ndist;
          if(!inq[v])PUSH(v);
      }
  }
}

println(dist[t]==INF?"todokanaikoi":dist[t]);
