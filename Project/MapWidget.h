#pragma once
#include <QWidget>
#include "Graph.h"

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(Graph* graph, QWidget* parent = nullptr);

    void setStartNode(int id);   // 출발지 설정
    void setEndNode(int id);     // 도착지 설정
    void clearSelection();
    void highlightPath(const QVector<int>& path);

signals:
    void nodeClicked(int id);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

private:
    Graph*       m_graph;
    int          m_startId  = -1;
    int          m_endId    = -1;
    int          m_hoverId  = -1;
    QVector<int> m_path;

    int  nodeAt(const QPointF& pos) const;
    void drawEdges(QPainter& p);
    void drawNodes(QPainter& p);
    void drawPath(QPainter& p);
};
