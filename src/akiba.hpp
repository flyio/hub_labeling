#include "labeling.hpp"

template<class T> inline bool chmin(T &minval, const T &newval) { if (minval>newval) { minval=newval; return 1; } return 0; }

struct Graph {
  typedef pair<int,int> pii;
  typedef pii Edge;  // (cost, to)
  typedef vector<vector<Edge>> Graph_;

  vector<Graph_> G; int V; int E;
  Labeling L;

  Graph() : V(0), E(0) {}
  Graph(int V) : L(V), G(2,Graph_(V)), V(V), E(0) {}
  void add_edge(int from, int to, int cost){ G[1][from].push_back({cost,to}); G[0][to].push_back({cost,from}); E++; }

  void resize(int V){
    this->V = V;
    G.resize(2,Graph_(V));
    L.resize(V);
  }

  template <class iStream>
  void load(iStream &stream){
    string line;
    string _1, _2;
    int V, E; int from, to, cost;
    while(getline(stream, line)){
      istringstream s(line);
      switch(line[0]){
        case 'c': break;
        case 'p':
          s >> _1 >> _2 >> V >> E;
          this->resize(V);
        case 'a':
          s >> _1 >> from >> to >> cost;
          add_edge(from-1,to-1,cost);
      }
    }
  }
  void load(string filename){
    ifstream ifs(filename);
    load(ifs);
  }
  template <class oStream>
  void save(oStream &out){
    cout << "p sp " << V << " " << E << endl;
    bool forward = true;
    for(int v=0;v<V;++v){
      for(Edge e : G[forward][v]){
        // dimacs format requires node IDs 1-based
        cout << "a " << v+1 << " " << e.second+1 << " " << e.first << endl;
      }
    }
  }
};

// Akiba's 2-hop indexing (SIGMOD'14)
struct Akiba : public Graph {
  typedef tuple<int,int,int> piii;

  vector<int> order;

  Akiba() {}
  Akiba(int V) : Graph(V) {}
  
  void pruned_bfs(int i, bool forward, vector<int> &order, vector<int> &dist)
  {
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    fill(dist.begin(), dist.end(), numeric_limits<int>::max());
    int v = order[i]; dist[v] = 0;
    pq.push({0,v});
    while(!pq.empty()){
      auto [d,u] = pq.top(); pq.pop();
      if(dist[u]<d) continue;  /* Note: this is needed because priority_queue allows multiple keys
                                (sometimes old keys with worse cost remains in the queue...) */
      L.add(u,i,d,!forward);
      for(auto [c,w] : G[forward][u]){
        if(dist[u]+c < dist[w] && dist[u]+c < L.query(v,w,forward)){
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
