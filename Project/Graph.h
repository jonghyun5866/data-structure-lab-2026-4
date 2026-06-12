#pragma once
#include <QString>
#include <QVector>
#include <QMap>
#include <QPointF>
#include <limits>

struct Restaurant {
    int     id;
    QString name;
    QString area;
    QString category;
    QString menu;
    QString price;
    QString feature;
    QPointF pos;
};

struct Edge {
    int    to;
    double weight;
};

class Graph {
public:
    Graph() = default;

    void addNode(const Restaurant& r);
    void addEdge(int from, int to, double weight);

    const QVector<Restaurant>& nodes() const { return m_nodes; }

    QVector<Edge> adj(int id) const {
        if (m_adj.contains(id)) return m_adj[id];
        return {};
    }

    int nodeCount() const { return m_nodes.size(); }

    struct DijkstraResult {
        QVector<double> dist;
        QVector<int>    prev;
    };
    DijkstraResult dijkstra(int startId) const;
    QVector<int>   getPath(const DijkstraResult& res, int endId) const;

private:
    QVector<Restaurant>      m_nodes;
    QMap<int, QVector<Edge>> m_adj;
};
