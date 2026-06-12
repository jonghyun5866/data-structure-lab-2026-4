#include "MapWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetrics>
#include <cmath>

static const double NODE_RADIUS = 22.0;

MapWidget::MapWidget(Graph* graph, QWidget* parent)
    : QWidget(parent), m_graph(graph)
{
    setMinimumSize(820, 600);
    setMouseTracking(true);
}

void MapWidget::setStartNode(int id) { m_startId = id; update(); }
void MapWidget::setEndNode(int id) { m_endId = id; update(); }
void MapWidget::clearSelection() { m_startId = m_endId = -1; m_path.clear(); update(); }
void MapWidget::highlightPath(const QVector<int>& path) { m_path = path; update(); }

int MapWidget::nodeAt(const QPointF& pos) const
{
    const auto& nodes = m_graph->nodes();
    for (const auto& r : nodes) {
        double dx = pos.x() - r.pos.x();
        double dy = pos.y() - r.pos.y();
        if (std::sqrt(dx * dx + dy * dy) <= NODE_RADIUS + 4)
            return r.id;
    }
    return -1;
}

void MapWidget::mousePressEvent(QMouseEvent* e)
{
    int id = nodeAt(e->pos());
    if (id >= 0) emit nodeClicked(id);
}

void MapWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.fillRect(rect(), QColor("#FFF8F0"));

    auto drawArea = [&](const QString& label, QRectF rc, QColor c) {
        p.setBrush(QColor(c.red(), c.green(), c.blue(), 30));
        p.setPen(QPen(c, 1.5, Qt::DashLine));
        p.drawRoundedRect(rc, 12, 12);
        p.setPen(c);
        QFont f = p.font(); f.setBold(true); f.setPointSize(9); p.setFont(f);
        p.drawText(rc.adjusted(8, 6, 0, 0), Qt::AlignTop | Qt::AlignLeft, label);
        };
    drawArea("한옥마을", QRectF(40, 60, 280, 280), QColor("#E07B54"));
    drawArea("객리단길", QRectF(350, 40, 220, 180), QColor("#5B8DD9"));
    drawArea("전주시청 인근", QRectF(590, 180, 200, 200), QColor("#4CAF82"));
    drawArea("전북대 인근", QRectF(310, 300, 250, 240), QColor("#9B59B6"));

    drawEdges(p);
    drawPath(p);
    drawNodes(p);
}

void MapWidget::drawEdges(QPainter& p)
{
    QFont f = p.font(); f.setPointSize(7); p.setFont(f);

    const auto& nodes = m_graph->nodes();
    int n = nodes.size();

    QSet<QPair<int, int>> drawn;
    for (const auto& r : nodes) {
        for (const Edge& e : m_graph->adj(r.id)) {
            if (e.to < 0 || e.to >= n) continue;
            int a = qMin(r.id, e.to), b = qMax(r.id, e.to);
            if (drawn.contains({ a, b })) continue;
            drawn.insert({ a, b });

            const Restaurant& to = nodes[e.to];
            QPointF mid = (r.pos + to.pos) / 2;

            p.setPen(QPen(QColor("#CCBBAA"), 1.8));
            p.drawLine(r.pos, to.pos);

            p.setPen(QColor("#999"));
            p.drawText(mid + QPointF(-8, -4),
                QString::number((int)e.weight) + "분");
        }
    }
}

void MapWidget::drawPath(QPainter& p)
{
    if (m_path.size() < 2) return;
    const auto& nodes = m_graph->nodes();
    int n = nodes.size();

    QPen pen(QColor("#FF6B35"), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p.setPen(pen);
    for (int i = 0; i < m_path.size() - 1; ++i) {
        int idA = m_path[i], idB = m_path[i + 1];
        if (idA < 0 || idA >= n || idB < 0 || idB >= n) continue;
        p.drawLine(nodes[idA].pos, nodes[idB].pos);
    }
}

void MapWidget::drawNodes(QPainter& p)
{
    QFont labelFont = p.font();
    labelFont.setPointSize(8);
    labelFont.setBold(true);

    for (const auto& r : m_graph->nodes()) {
        bool isStart = (r.id == m_startId);
        bool isEnd = (r.id == m_endId);
        bool inPath = m_path.contains(r.id);

        p.setBrush(QColor(0, 0, 0, 40));
        p.setPen(Qt::NoPen);
        p.drawEllipse(r.pos + QPointF(3, 3), NODE_RADIUS, NODE_RADIUS);

        QColor fill, border;
        if (isStart) { fill = QColor("#FF6B35"); border = QColor("#CC4400"); }
        else if (isEnd) { fill = QColor("#4CAF50"); border = QColor("#2E7D32"); }
        else if (inPath) { fill = QColor("#FFB347"); border = QColor("#E67E22"); }
        else { fill = QColor("#3D7ECC"); border = QColor("#1A5276"); }

        p.setBrush(fill);
        p.setPen(QPen(border, 2.5));
        p.drawEllipse(r.pos, NODE_RADIUS, NODE_RADIUS);

        p.setFont(labelFont);
        p.setPen(Qt::white);
        p.drawText(QRectF(r.pos.x() - NODE_RADIUS, r.pos.y() - NODE_RADIUS,
            NODE_RADIUS * 2, NODE_RADIUS * 2),
            Qt::AlignCenter, r.name.left(1));

        QFont nameFont = labelFont;
        nameFont.setBold(false); nameFont.setPointSize(8);
        p.setFont(nameFont);
        p.setPen(QColor("#333"));
        p.drawText(QRectF(r.pos.x() - 55, r.pos.y() + NODE_RADIUS + 2, 110, 30),
            Qt::AlignTop | Qt::AlignHCenter, r.name);
    }
}