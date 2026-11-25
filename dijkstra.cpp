struct Dijkstra {
    vector<vector<pii>> adj; // {to, weight}
    vector<ll> dist;
    Dijkstra(int n) : adj(n), dist(n, LINF) {}
    void add_edge(int u, int v, ll w) {
        adj[u].pb({v, w});
    }
    void run(int src) {
        priority_queue<pll, vector<pll>, greater<pll>> pq; // {dist, node}
        dist[src] = 0;
        pq.push({0, src});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto [v, w] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
    }
};