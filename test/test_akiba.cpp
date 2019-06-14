#include "../src/akiba.hpp"
#include "../src/graph_io.hpp"


template <class Matrix>
void warshall_floyd(Matrix &d, int V){
    // [input] d: 隣接行列 (size: V x V) d[i][j] は (i,j) のコスト
    //         (エッジがない場合「無限大」にする．オーバーフローを避けるため
    //          numeric_limits<int>::max() を使ってはいけない)
    // [output] d[i][j] が (i,j) の最短経路長となる
    for(int k=0;k<V;++k)
        for(int i=0;i<V;++i)
            for(int j=0;j<V;++j)
                d[i][j] = min(d[i][j], d[i][k]+d[k][j]);
}

int test_akiba(int V){
  // check if the Akiba-labeling is implemented correctly using all pairs shortest path
  Akiba g;
  random_graph(V,0.2,g);
  vector<int> order(V);
  for(int i=0;i<V;++i) order[i] = i;

  double dur1 = 0.0;
  auto start1 = chrono::system_clock::now();
  g.build_index(order);
  auto end1 = chrono::system_clock::now();
  dur1 += chrono::duration_cast<chrono::microseconds>(end1-start1).count();

  vector<vector<int>> M(V,vector<int>(V,10000000));
  bool forward = true;
  for(int v=0;v<V;++v){
    M[v][v] = 0;
    for(auto [cost, u] : g.G[forward][v]){
      M[v][u] = cost;
    }
  }
  double dur2 = 0.0;
  auto start2 = chrono::system_clock::now();
  warshall_floyd(M,V);
  auto end2 = chrono::system_clock::now();
  dur2 += chrono::duration_cast<chrono::microseconds>(end2-start2).count();

  bool ok = true;
  for(int u=0;u<V;++u){
    for(int v=0;v<V;++v){
      start1 = chrono::system_clock::now();
      int d = g.L.query(u,v);
      end1 = chrono::system_clock::now();
      dur1 += chrono::duration_cast<chrono::microseconds>(end1-start1).count();
      if(d != M[u][v]) {
        cerr << "(u,v) = " << u << "," << v << ", Akiba: " << d << " WF: " << M[u][v] << endl;
        ok = false;
      }
    }
  }
  if(ok) cerr << "[info] test_akiba(" << V << ") => The result equals to Warshall-Floyd method;)" << endl;

  cerr << "Akiba: " << dur1 << " [microsec]" << endl;
  cerr << "Warshall-Floyd: " << dur2 << " [microsec]" << endl;
}

int main(){
  test_akiba(32); test_akiba(256);
  //test_io();
}