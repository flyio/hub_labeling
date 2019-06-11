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
  void add(int v, int h, int d, bool backward){
    L[v][backward].push_back({h,d});
    sorted[v][backward] = false;
  }
  void sort_all() {
    for(int u=0; u<L.size(); ++u){
      sort(L[u][0].begin(), L[u][0].end());
      sort(L[u][1].begin(), L[u][1].end());
    }
  }
  int query_safe(int u, int v){
    if(!sorted[u][0]) sort(L[u][0].begin(), L[u][0].end());
    if(!sorted[v][1]) sort(L[v][1].begin(), L[v][1].end());
    return query(u,v);
  }
  int query(int u, int v){
    auto &Lf = L[u][0]; auto &Lb = L[v][1];
    int i=0; int j=0;
    int m=Lf.size(); int n=Lb.size();
    while(i<m && j<n){
      int h = Lf[i].h; int g = Lb[j].h;
      if(h==g) return Lf[i].d + Lb[j].d;
      else if(h<g) i++;
      else j++;
    }
    return -1;  // no hub found
  }

  // I/O
  template <class OutStream>
  void write(OutStream &out) {
    out << L.size() << endl;
    for(int u=0;u<L.size();++u){
      out << L[u][0] << endl;  // forward
      out << L[u][1] << endl;  // backward
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