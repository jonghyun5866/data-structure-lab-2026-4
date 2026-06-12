#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QScrollArea>
#include <QMessageBox>
#include <QFrame>
#include <algorithm>

// ════════════════════════════════════════════
//  그래프 데이터 초기화 (노드 & 엣지 하드코딩)
// ════════════════════════════════════════════
void MainWindow::buildGraph()
{
    // ── 노드 정의 ──────────────────────────────────
    // id, name, area, category, menu, price, feature, pos(x,y)
    QVector<Restaurant> restaurants = {
        // ▶ 한옥마을
        { 0, "한일관",    "한옥마을", "한정식",   "전주비빔밥",   "12,000원", "전주비빔밥 원조 맛집, 50년 전통",   {130, 120} },
        { 1, "고궁",      "한옥마을", "한정식",   "한정식 코스",  "25,000원", "한옥 분위기, 관광객 필수 코스",     {200,  90} },
        { 2, "베테랑",    "한옥마을", "칼국수",   "칼국수",       " 8,000원", "현지인 단골, 구수한 사골 육수",      { 90, 200} },
        { 3, "풍년제과",  "한옥마을", "베이커리",  "초코파이",     " 2,500원", "전주 수제 초코파이 원조, 줄 필수",   {180, 230} },
        { 4, "삼백집",    "한옥마을", "콩나물국밥","콩나물국밥",   " 8,000원", "24시간 운영, 해장의 성지",           {270, 180} },
        // ▶ 객리단길
        { 5, "왱이집",    "객리단길", "막걸리",   "막걸리+빈대떡","15,000원", "전통 막걸리 맛집, 야경 명소",         {400, 80} },
        { 6, "남부시장야시장","객리단길","분식·야시장","비빔밥볶음","  5,000원", "금·토 야시장, 다양한 길거리 음식",  {490, 140} },
        { 7, "오모가리김치찌개","객리단길","김치찌개","김치찌개",  "10,000원", "돼지고기 넉넉, 뚝배기 한 가득",       {430, 190} },
        // ▶ 전주시청 인근
        { 8, "전주중앙회관","시청 인근","삼겹살",  "삼겹살+냉면", "15,000원", "고기 두께가 남달라, 회식 명소",       {640, 220} },
        { 9, "짬뽕순두부", "시청 인근","두부요리",  "짬뽕순두부",  " 9,000원", "국물이 칼칼, 순두부계 인기 1위",     {700, 300} },
        // ▶ 전북대 인근
        {10, "교동반점",   "전북대 인근","중식",   "짜장면",       " 7,000원", "학생들의 소울푸드, 두툼한 면발",     {360, 360} },
        {11, "청학동순대국","전북대 인근","순대국",  "순대국밥",     " 8,000원", "뽀얀 국물, 술국으로도 인기",         {440, 420} },
        {12, "봉순이김밥",  "전북대 인근","분식",   "충무김밥",     " 4,000원", "시험기간 필수, 가성비 최고",          {320, 450} },
        {13, "학현가",     "전북대 인근","한식",   "제육볶음",     " 9,000원", "푸짐한 반찬, 학생 할인 있음",         {510, 340} },
    };

    for (const auto& r : restaurants)
        m_graph->addNode(r);

    // ── 엣지 (도보 시간, 분) ─────────────────────────
    // 한옥마을 내부
    m_graph->addEdge( 0,  1,  3);
    m_graph->addEdge( 0,  2,  5);
    m_graph->addEdge( 0,  3,  7);
    m_graph->addEdge( 1,  3,  5);
    m_graph->addEdge( 2,  4,  6);
    m_graph->addEdge( 3,  4,  4);
    // 한옥마을 ↔ 객리단길
    m_graph->addEdge( 4,  5, 10);
    m_graph->addEdge( 4,  7,  8);
    m_graph->addEdge( 3,  6, 12);
    // 객리단길 내부
    m_graph->addEdge( 5,  6,  6);
    m_graph->addEdge( 6,  7,  5);
    m_graph->addEdge( 5,  7,  7);
    // 객리단길 ↔ 시청 인근
    m_graph->addEdge( 6,  8, 11);
    m_graph->addEdge( 7,  8,  9);
    m_graph->addEdge( 8,  9,  6);
    // 객리단길 ↔ 전북대
    m_graph->addEdge( 7, 13, 10);
    m_graph->addEdge( 6, 11, 12);
    // 전북대 내부
    m_graph->addEdge(10, 11,  5);
    m_graph->addEdge(10, 12,  4);
    m_graph->addEdge(11, 13,  6);
    m_graph->addEdge(12, 13,  7);
    m_graph->addEdge(11, 12,  6);
    // 시청 ↔ 전북대
    m_graph->addEdge( 9, 13,  9);
}

// ════════════════════════════════════════════
//  UI 구성
// ════════════════════════════════════════════
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("🍜 전주 맛집 추천 지도 - 그래프 데이터 구조");
    resize(1200, 680);

    m_graph = new Graph();
    buildGraph();

    // ── 지도 위젯 ──────────────────────────────
    m_map = new MapWidget(m_graph);
    connect(m_map, &MapWidget::nodeClicked, this, &MainWindow::onNodeClicked);

    // ── 오른쪽 패널 ────────────────────────────
    auto* rightPanel = new QWidget();
    rightPanel->setFixedWidth(310);
    rightPanel->setStyleSheet("background:#FFFDF9;");

    auto* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(10);
    rightLayout->setContentsMargins(12, 12, 12, 12);

    // 제목
    m_titleLabel = new QLabel("📍 전주 맛집 지도");
    m_titleLabel->setStyleSheet(
        "font-size:16px; font-weight:bold; color:#C0392B;"
        "padding:6px; border-bottom:2px solid #E07B54;");
    rightLayout->addWidget(m_titleLabel);

    // ── 맛집 정보 ──
    auto* infoGroup = new QGroupBox("🍽 맛집 정보");
    infoGroup->setStyleSheet(
        "QGroupBox{font-weight:bold;color:#5D4037;border:1px solid #DDD;"
        "border-radius:6px;margin-top:6px;padding-top:6px;}"
        "QGroupBox::title{subcontrol-origin:margin;left:8px;}");
    auto* infoLayout = new QVBoxLayout(infoGroup);
    m_infoLabel = new QLabel("지도에서 맛집을 클릭하세요.");
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setStyleSheet("font-size:11px; color:#555; padding:4px;");
    m_infoLabel->setMinimumHeight(120);
    infoLayout->addWidget(m_infoLabel);
    rightLayout->addWidget(infoGroup);

    // ── 경로 탐색 ──
    auto* pathGroup = new QGroupBox("🗺 최단 경로 탐색");
    pathGroup->setStyleSheet(
        "QGroupBox{font-weight:bold;color:#1A5276;border:1px solid #DDD;"
        "border-radius:6px;margin-top:6px;padding-top:6px;}"
        "QGroupBox::title{subcontrol-origin:margin;left:8px;}");
    auto* pathLayout = new QVBoxLayout(pathGroup);

    m_startCombo = new QComboBox();
    m_endCombo   = new QComboBox();
    for (const auto& r : m_graph->nodes()) {
        m_startCombo->addItem(r.area + " - " + r.name, r.id);
        m_endCombo  ->addItem(r.area + " - " + r.name, r.id);
    }
    m_endCombo->setCurrentIndex(6);

    pathLayout->addWidget(new QLabel("출발지:"));
    pathLayout->addWidget(m_startCombo);
    pathLayout->addWidget(new QLabel("도착지:"));
    pathLayout->addWidget(m_endCombo);

    m_findBtn = new QPushButton("🔍 경로 탐색");
    m_findBtn->setStyleSheet(
        "QPushButton{background:#3D7ECC;color:white;border-radius:5px;"
        "padding:6px;font-weight:bold;}"
        "QPushButton:hover{background:#2980B9;}");
    connect(m_findBtn, &QPushButton::clicked, this, &MainWindow::onFindPath);
    pathLayout->addWidget(m_findBtn);

    m_resultLabel = new QLabel("");
    m_resultLabel->setWordWrap(true);
    m_resultLabel->setStyleSheet(
        "font-size:11px;color:#1A5276;background:#EAF4FB;"
        "padding:6px;border-radius:4px;");
    m_resultLabel->setMinimumHeight(50);
    pathLayout->addWidget(m_resultLabel);

    rightLayout->addWidget(pathGroup);

    // ── 주변 맛집 추천 ──
    auto* recGroup = new QGroupBox("⭐ 주변 맛집 추천");
    recGroup->setStyleSheet(
        "QGroupBox{font-weight:bold;color:#2E7D32;border:1px solid #DDD;"
        "border-radius:6px;margin-top:6px;padding-top:6px;}"
        "QGroupBox::title{subcontrol-origin:margin;left:8px;}");
    auto* recLayout = new QVBoxLayout(recGroup);

    m_recommendBtn = new QPushButton("📍 현재 선택 위치에서 추천");
    m_recommendBtn->setStyleSheet(
        "QPushButton{background:#4CAF50;color:white;border-radius:5px;"
        "padding:6px;font-weight:bold;}"
        "QPushButton:hover{background:#388E3C;}");
    connect(m_recommendBtn, &QPushButton::clicked, this, &MainWindow::onRecommend);
    recLayout->addWidget(m_recommendBtn);

    m_recommendList = new QListWidget();
    m_recommendList->setStyleSheet(
        "font-size:10px;border:none;background:#F9FFF9;");
    m_recommendList->setMaximumHeight(140);
    recLayout->addWidget(m_recommendList);

    rightLayout->addWidget(recGroup);

    // ── 초기화 버튼 ──
    m_clearBtn = new QPushButton("🔄 초기화");
    m_clearBtn->setStyleSheet(
        "QPushButton{background:#7F8C8D;color:white;border-radius:5px;"
        "padding:6px;}"
        "QPushButton:hover{background:#636E72;}");
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::onClear);
    rightLayout->addWidget(m_clearBtn);
    rightLayout->addStretch();

    // ── 분할 레이아웃 ──
    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(m_map);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);
    setCentralWidget(splitter);
}

// ════════════════════════════════════════════
//  슬롯 구현
// ════════════════════════════════════════════
void MainWindow::onNodeClicked(int id)
{
    m_selectedId = id;
    updateInfoPanel(id);
    m_map->setStartNode(id);
    m_map->highlightPath({});
    m_resultLabel->clear();
    m_recommendList->clear();
}

void MainWindow::updateInfoPanel(int id)
{
    const Restaurant& r = m_graph->nodes()[id];
    QString html = QString(
        "<b style='font-size:13px;color:#C0392B;'>%1</b><br>"
        "<span style='color:#777;'>📌 %2</span><br><br>"
        "🍽 <b>종류:</b> %3<br>"
        "🥢 <b>대표 메뉴:</b> %4<br>"
        "💰 <b>가격대:</b> %5<br>"
        "✨ <b>특징:</b> %6"
    ).arg(r.name, r.area, r.category, r.menu, r.price, r.feature);
    m_infoLabel->setText(html);
}

void MainWindow::onFindPath()
{
    int startId = m_startCombo->currentData().toInt();
    int endId   = m_endCombo->currentData().toInt();

    if (startId == endId) {
        QMessageBox::information(this, "알림", "출발지와 도착지가 같습니다.");
        return;
    }

    auto res  = m_graph->dijkstra(startId);
    auto path = m_graph->getPath(res, endId);

    m_map->setStartNode(startId);
    m_map->setEndNode(endId);
    m_map->highlightPath(path);

    if (res.dist[endId] == std::numeric_limits<double>::infinity()) {
        m_resultLabel->setText("⚠️ 경로를 찾을 수 없습니다.");
        return;
    }

    // 경로 이름 조합
    QString pathStr;
    for (int i = 0; i < path.size(); ++i) {
        if (i) pathStr += " → ";
        pathStr += m_graph->nodes()[path[i]].name;
    }

    m_resultLabel->setText(
        QString("🚶 경로: %1\n⏱ 총 도보 시간: %2분")
            .arg(pathStr)
            .arg((int)res.dist[endId])
    );
}

void MainWindow::onRecommend()
{
    if (m_selectedId < 0) {
        QMessageBox::information(this, "알림", "지도에서 맛집을 먼저 클릭하세요.");
        return;
    }
    updateRecommendList(m_selectedId);
}

void MainWindow::updateRecommendList(int startId)
{
    auto res = m_graph->dijkstra(startId);

    // (거리, id) 정렬
    QVector<QPair<double, int>> sorted;
    for (int i = 0; i < m_graph->nodeCount(); ++i) {
        if (i == startId) continue;
        if (res.dist[i] < std::numeric_limits<double>::infinity())
            sorted.append({res.dist[i], i});
    }
    std::sort(sorted.begin(), sorted.end());

    m_recommendList->clear();
    int rank = 1;
    for (const auto& [dist, id] : sorted) {
        const Restaurant& r = m_graph->nodes()[id];
        QString text = QString("%1위  %2 (%3)  %4분 도보")
                           .arg(rank++).arg(r.name).arg(r.category).arg((int)dist);
        m_recommendList->addItem(text);
    }
}

void MainWindow::onClear()
{
    m_selectedId = -1;
    m_map->clearSelection();
    m_infoLabel->setText("지도에서 맛집을 클릭하세요.");
    m_resultLabel->clear();
    m_recommendList->clear();
}
