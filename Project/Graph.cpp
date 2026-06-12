#include "Graph.h"
#include <QSet>
#include <QPair>
#include <queue>
#include <functional>

void Graph::addNode(const Restaurant& r)
{
    m_nodes.append(r);
    m_adj[r.id]; // 빈 리스트 초기화
}

void Graph::addEdge(int from, int to, double weight)
{
    m_adj[from].append({to,   weight});
    m_adj[to  ].append({from, weight});
}

Graph::DijkstraResult Graph::dijkstra(int startId) const
{
    const double INF = std::numeric_limits<double>::infinity();
    int n = m_nodes.size();

    DijkstraResult res;
    res.dist.fill(INF, n);
    res.prev.fill(-1,  n);
    res.dist[startId] = 0.0;

    // (거리, 노드id) 최소 힙
    using P = QPair<double, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0.0, startId});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();

        if (d > res.dist[u]) continue;

        for (const Edge& e : m_adj[u]) {
            double nd = res.dist[u] + e.weight;
            if (nd < res.dist[e.to]) {
                res.dist[e.to] = nd;
                res.prev[e.to] = u;
                pq.push({nd, e.to});
            }
        }
    }
    return res;
}

QVector<int> Graph::getPath(const DijkstraResult& res, int endId) const
{
    QVector<int> path;
    for (int cur = endId; cur != -1; cur = res.prev[cur])
        path.prepend(cur);
    return path;
}
