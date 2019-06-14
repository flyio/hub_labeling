#include <bits/stdc++.h>
using namespace std;

template <class Graph>
void random_graph(int V, double p, Graph &g){
  g.resize(V);
  int P = (int)(p*10000);
  for(int i=0;i<V;++i){
    for(int j=0;j<V;++j){
      if(i!=j && (rand() % 10000)<P) g.add_edge(i,j,5+rand()%5);
    }
  }
}