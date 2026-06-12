#pragma once
#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include "Graph.h"
#include "MapWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onNodeClicked(int id);
    void onFindPath();
    void onRecommend();
    void onClear();

private:
    void buildGraph();
    void updateInfoPanel(int id);
    void updateRecommendList(int startId);

    Graph*       m_graph;
    MapWidget*   m_map;

    // 패널 위젯
    QLabel*      m_titleLabel;
    QLabel*      m_infoLabel;
    QListWidget* m_recommendList;
    QComboBox*   m_startCombo;
    QComboBox*   m_endCombo;
    QPushButton* m_findBtn;
    QPushButton* m_recommendBtn;
    QPushButton* m_clearBtn;
    QLabel*      m_resultLabel;

    int m_selectedId = -1;
};
