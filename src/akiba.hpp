#include "labeling.hpp"

template<class T> inline bool chmin(T &minval, const T &newval) { if (minval>newval) { minval=newval; return 1; } return 0; }

// Akiba's 2-hop indexing (SIGMOD'14)
struct Akiba {
  typedef pair<int,int> pii;
  typedef tuple<int,int,int> piii;
  typedef pii Edge;  // (cost, to)
  typedef vector<vector<Edge>> Graph;

  vector<Graph> G; int V;
  vector<int> order;
  Labeling L;

  Akiba(int V) : G(2,Graph(V)), V(V), L(V) {}
  void add_edge(int from, int to, int cost){ G[1][from].push_back({cost,to}); G[0][to].push_back({cost,from}); }
  
  void pruned_bfs(int i, bool forward, vector<int> &order, vector<int> &dist)
  {
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    fill(dist.begin(), dist.end(), numeric_limits<int>::max());
    int v = order[i]; dist[v] = 0;
    pq.push({0,v});
    while(!pq.empty()){
      auto [d,u] = pq.top(); pq.pop();
      if(L.query(v,u)<=d) continue;
      L.add(u,i,d,!forward);
      for(auto [c,w] : G[forward][u]){
        if(dist[u]+c < dist[w]){
          dist[w] = dist[u] + c;
          pq.push({dist[w],w});
        }
      }
    }
  }

  void build_index(vector<int> &order){
    vector<int> dist(V);
    for(int i=0;i<V;++i){
      pruned_bfs(i, true, order, dist);
      pruned_bfs(i, false, order, dist);
    }
  }
};
