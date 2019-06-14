#include <bits/stdc++.h>
using namespace std;

struct Labeling {
  struct Tuple {
    int h, d;  // hub (vertex) & distance
    bool operator<(const Tuple &t) const { return h<t.h; } 
  };
  vector<vector<vector<Tuple>>> L;  // L[v][backward][hubs]
  vector<vector<bool>> sorted;  // sorted[v][backward]: Is L[v][backward] already sorted?
  Labeling() {}
  Labeling(int V) : L(V, vector<vector<Tuple>>(2)), sorted(V, vector<bool>(2,true)) {}
  void resize(int V) {
    L.resize(V, vector<vector<Tuple>>(2));
    sorted.resize(V, vector<bool>(2,true));
  }
  void add(int v, int h, int d, bool forward){
    L[v][forward].push_back({h,d});
  }
  int query(int u, int v, bool forward=true){
    auto &Lf = L[u][forward]; auto &Lb = L[v][!forward];
    int i=0; int j=0; int ret = numeric_limits<int>::max();
    int m=Lf.size(); int n=Lb.size();
    while(i<m && j<n){
      int h = Lf[i].h; int g = Lb[j].h;
      if(h==g) ret = min(Lf[i++].d + Lb[j++].d, ret);
      else if(h<g) i++;
      else j++;
    }
    return ret;
  }

  // I/O
  template <class OutStream>
  void write(OutStream &out) {
    out << L.size() << endl;
    for(int u=0;u<L.size();++u){
      out << L[u][0] << endl;  // backward
      out << L[u][1] << endl;  // forward
    }
  }
  template <class InStream>
  void read(InStream &in) {
    int V; in >> V;
    L.resize(V, vector<vector<Tuple>>(2));
    sorted.resize(V, vector<bool>(2,false));
    for(int u=0;u<L.size();++u){
      in >> L[u][0]; in >> L[u][1];
    }
  }

  Tuple binary_search(int v, bool forward, int h){
    // Find hub h from L[v][forward]
    int a = 0; int b = L[v][forward].size()-1;
    while(a<b){
      int c = (a+b)/2;
      int xh = L[v][forward][c].h;
      if(xh == h) return L[v][forward][c];
      else if(xh < h) a = c+1;
      else b=c-1;
    }
    return {-1,-1}; // not found
  }
};

std::ostream& operator<<(std::ostream& stream, const vector<Labeling::Tuple> &L){
  stream << L.size() << " ";
  for(int i=0;i<L.size();++i){
    stream << L[i].h << " " << L[i].d << " ";
  }
  return stream;
}

std::istream& operator>>(std::istream& stream, vector<Labeling::Tuple> &L){
  int n; stream >> n;
  L.resize(n);
  for(int i=0;i<n;++i){
    stream >> L[i].h >> L[i].d;
  }
  return stream;
}