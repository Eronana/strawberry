var len = system.len;
var println = system.println;
var scani = system.scani;
var scans = system.scans;
var new_array = system.new_array;


var N=100005;
var CLR=function(a,v){
  var l=len(a);
  for(var i=0;i<l;i++)a[i]=v;
};


var tree=new_array(N<<2);
var lazy=new_array(N<<2);
var push_up=function(rt)
{
  tree[rt]=tree[(rt<<1)]+tree[((rt<<1)|1)];
};
var push_down=function(l,r,rt)
{
  var m=(l+r)>>1;
  tree[(rt<<1)]+=lazy[rt]*(m-l+1);
  tree[((rt<<1)|1)]+=lazy[rt]*(r-m);
  lazy[(rt<<1)]+=lazy[rt];
  lazy[((rt<<1)|1)]+=lazy[rt];
  lazy[rt]=0;
};
var build=function(l,r,rt)
{
  if(l==r)return tree[rt]=scani();
  var m=(l+r)>>1;
  build(l,m,(rt<<1));
  build(m+1,r,((rt<<1)|1));
  push_up(rt);
};
var update=function(v,ll,rr,l,r,rt)
{
  if(ll<=l&&rr>=r)
  {
    tree[rt]+=(r-l+1)*v;
    lazy[rt]+=v;
    return;
  }
  if(lazy[rt])push_down(l,r,rt);
  var m=(l+r)>>1;
  if(ll<=m)update(v,ll,rr,l,m,(rt<<1));
  if(rr>m)update(v,ll,rr,m+1,r,((rt<<1)|1));
  push_up(rt);
};

var query=function(ll,rr,l,r,rt)
{
  if(ll<=l&&rr>=r)return tree[rt];
  if(lazy[rt])push_down(l,r,rt);
  var m=(l+r)>>1;
  var ret=0;
  if(ll<=m)ret+=query(ll,rr,l,m,(rt<<1));
  if(rr>m)ret+=query(ll,rr,m+1,r,((rt<<1)|1));
  return ret;
};


var n=scani(),q=scani();
CLR(lazy,0);
build(1,n,1);
while(q--)
{
  var s=scans();
  var a=scani(),b=scani();
  if(s[0]=='Q') println(query(a,b,1,n,1));
  else update(scani(),a,b,1,n,1);
}

