#include "mainwindow.h"
#include <algorithm>



MapWidget::MapWidget(QWidget *parent) : QWidget(parent) {

    setFixedSize(562, 321);
}

void MapWidget::setBackground(const QPixmap& pixmap) {
    m_background = pixmap;
    update();
}

void MapWidget::setGraphData(const QVector<Node>& nodes, const QVector<Edge>& edges) {
    m_nodes = nodes;
    m_edges = edges;
    update();
}

void MapWidget::setRoute(const QVector<int>& route) {
    m_currentRoute = route;
    update();
}

void MapWidget::resetRoute() {
    m_currentRoute.clear();
    m_firstSelected = -1;
    m_secondSelected = -1;
    update();
}

int MapWidget::findNodeAt(QPointF pos) {
    for (const auto& node : m_nodes) {
        if (QLineF(node.pos, pos).length() < 15) {
            return node.id;
        }
    }
    return -1;
}

void MapWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        int id = findNodeAt(event->pos());
        if (id != -1) {
            emit nodeSelected(id);
        }
    }
}

void MapWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    if (!m_background.isNull()) {
        painter.drawPixmap(0, 0, m_background.scaled(562, 321, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }


    for (const auto& edge : m_edges) {

        if (edge.from >= m_nodes.size() || edge.to >= m_nodes.size()) continue;

        QPen pen(edge.color);
        pen.setWidth(3);
        painter.setPen(pen);

        QPointF p1 = m_nodes[edge.from].pos;
        QPointF p2 = m_nodes[edge.to].pos;
        painter.drawLine(p1, p2);


        QPointF midPoint = (p1 + p2) / 2;
        QString label = QString("%1(%2)").arg(edge.baseLength).arg(edge.baseLength * edge.loadFactor, 0, 'f', 1);

        painter.setPen(Qt::black);
        QRectF textRect(midPoint.x() - 25, midPoint.y() - 10, 50, 20);
        painter.fillRect(textRect, QColor(255, 255, 255, 200));
        painter.drawText(textRect, Qt::AlignCenter, label);
    }


    if (m_currentRoute.size() > 1) {
        QPen routePen(Qt::blue);
        routePen.setWidth(5);
        routePen.setStyle(Qt::DashLine);
        painter.setPen(routePen);
        for (int i = 0; i < m_currentRoute.size() - 1; ++i) {
            painter.drawLine(m_nodes[m_currentRoute[i]].pos, m_nodes[m_currentRoute[i+1]].pos);
        }
    }


    for (const auto& node : m_nodes) {
        painter.setBrush(Qt::gray);
        painter.setPen(Qt::black);
        if (node.id == m_firstSelected) painter.setBrush(Qt::green);
        if (node.id == m_secondSelected) painter.setBrush(Qt::red);

        painter.drawEllipse(node.pos, 8, 8);


        painter.setPen(Qt::black);
        painter.drawText(node.pos + QPointF(12, 4), node.name);
    }
}



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    QVBoxLayout *controls = new QVBoxLayout;
    comboStart = new QComboBox;
    comboEnd = new QComboBox;
    btnTraffic = new QPushButton("Имитация пробок");
    btnFind = new QPushButton("Маршрут");
    btnSave = new QPushButton("Сохранить маршрут");

    controls->addWidget(new QLabel("Точка А:"));
    controls->addWidget(comboStart);
    controls->addWidget(new QLabel("Точка К:"));
    controls->addWidget(comboEnd);
    controls->addSpacing(20);
    controls->addWidget(btnTraffic);
    controls->addWidget(btnFind);
    controls->addWidget(btnSave);
    controls->addStretch();

    mapWidget = new MapWidget;

    // Загрузка карты
    QPixmap bg(":/new/prefix1/map1.jpg");
    if (bg.isNull()) {
        qDebug() << "ОШИБКА: Карта не загружена! Проверь путь в resources.qrc";
        bg = QPixmap(562, 321);
        bg.fill(Qt::white);
    } else {
        qDebug() << "Карта загружена, размер:" << bg.size();
    }
    mapWidget->setBackground(bg);

    mainLayout->addLayout(controls, 1);
    mainLayout->addWidget(mapWidget, 4);
    setCentralWidget(central);


    try {
        initGraph();
        mapWidget->setGraphData(nodes, edges);

        for (const auto& node : nodes) {
            comboStart->addItem(node.name, node.id);
            comboEnd->addItem(node.name, node.id);
        }
    } catch (...) {
        qDebug() << "Ошибка при инициализации графа!";
    }

    connect(btnTraffic, &QPushButton::clicked, this, &MainWindow::onLoadTraffic);
    connect(btnFind, &QPushButton::clicked, this, &MainWindow::onFindRoute);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveRoute);
    connect(mapWidget, &MapWidget::nodeSelected, this, &MainWindow::onNodeSelected);
}

MainWindow::~MainWindow() {}

void MainWindow::initGraph() {

    nodes = {
        {0, "A", {50, 23}},   {1, "B", {140, 28}},  {2, "C", {230, 28}},
        {3, "D", {50, 84}},  {4, "E", {138, 90}}, {5, "F", {227, 95}},
        {6, "G", {45, 170}},  {7, "H", {130, 184}}, {8, "I", {215, 197}},
        {9, "J", {326, 30}},  {10, "K", {320, 103}}, {11, "L", {412, 110}},
        {12, "M", {320, 200}},{13, "N", {407, 215}},{14, "O", {300, 270}},
        {15, "P", {206, 237}},{16, "Q", {125, 225}},{17, "R", {50, 215}}
    };

    edges = {
        {0, 1, 1.0, 1.0, Qt::green}, {1, 2, 1.2, 1.0, Qt::green},
        {0, 3, 1.0, 1.0, Qt::green}, {1, 4, 0.8, 1.0, Qt::green},
        {2, 5, 1.5, 1.0, Qt::green}, {2, 9, 2.0, 1.0, Qt::green},
        {3, 4, 1.0, 1.0, Qt::green}, {4, 5, 1.2, 1.0, Qt::green},
        {3, 6, 1.0, 1.0, Qt::green}, {4, 7, 1.0, 1.0, Qt::green},
        {5, 8, 1.5, 1.0, Qt::green}, {8, 12, 2.0, 1.0, Qt::green},
        {9, 10, 1.8, 1.0, Qt::green}, {10, 11, 1.0, 1.0, Qt::green},
        {11, 13, 1.5, 1.0, Qt::green}, {12, 13, 1.2, 1.0, Qt::green},
        {6, 7, 1.0, 1.0, Qt::green}, {7, 16, 0.9, 1.0, Qt::green},
        {8, 15, 1.2, 1.0, Qt::green}, {12, 14, 1.5, 1.0, Qt::green},
        {15, 16, 1.0, 1.0, Qt::green}, {16, 17, 1.8, 1.0, Qt::green},
        {14, 13, 1.0, 1.0, Qt::green}, {14, 15, 1.2, 1.0, Qt::green}
    };


    for(auto &e : edges) {
        if(e.from == 1 && e.to == 4) { e.loadFactor = 1.9; e.color = Qt::red; }
        if(e.from == 3 && e.to == 6) { e.loadFactor = 1.2; e.color = QColor(255, 165, 0); }
        if(e.from == 9 && e.to == 10) { e.loadFactor = 3.3; e.color = QColor(128, 0, 128); }
    }
}

QColor MainWindow::getColorForLoad(double load) {
    if (load < 1.2) return Qt::green;
    if (load < 1.5) return QColor(255, 165, 0);
    if (load < 1.8) return Qt::red;
    return QColor(128, 0, 128);
}

void MainWindow::onLoadTraffic() {
    for (auto& edge : edges) {
        double r = (double)(rand() % 100) / 100.0;
        edge.loadFactor = 1.0 + (r * 2.3);
        edge.color = getColorForLoad(edge.loadFactor);
    }
    mapWidget->setGraphData(nodes, edges);
    mapWidget->resetRoute();
    lastCalculatedRoute.clear();
}

void MainWindow::onNodeSelected(int id) {
    if (mapWidget->getFirstSelected() == -1) {
        int idx = comboStart->findData(id);
        if(idx != -1) comboStart->setCurrentIndex(idx);
    } else if (mapWidget->getSecondSelected() == -1) {
        int idx = comboEnd->findData(id);
        if(idx != -1) comboEnd->setCurrentIndex(idx);
    }
}

void MainWindow::onFindRoute() {
    if (nodes.isEmpty()) return;

    int startId = comboStart->currentData().toInt();
    int endId = comboEnd->currentData().toInt();

    if (startId == endId) {
        QMessageBox::warning(this, "Ошибка", "Выберите разные точки!");
        return;
    }

    int n = nodes.size();
    QVector<double> dist(n, 1e9);
    QVector<int> prev(n, -1);
    dist[startId] = 0;

    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;
    pq.push({0, startId});

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& edge : edges) {
            int v = -1;
            if (edge.from == u) v = edge.to;
            else if (edge.to == u) v = edge.from;
            else continue;

            double weight = edge.baseLength * edge.loadFactor;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[endId] >= 1e9) {
        QMessageBox::information(this, "Результат", "Маршрут не найден!");
        mapWidget->resetRoute();
        return;
    }

    QVector<int> path;
    for (int at = endId; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    lastCalculatedRoute = path;
    mapWidget->setRoute(path);

    QString msg = "Маршрут: ";
    for(int i=0; i<path.size(); ++i) {
        msg += nodes[path[i]].name;
        if(i < path.size()-1) msg += "-";
    }
    qDebug() << msg;
}

void MainWindow::onSaveRoute() {
    if (lastCalculatedRoute.empty()) {
        QMessageBox::warning(this, "Ошибка", "Сначала постройте маршрут!");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить маршрут", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Маршрут:\n";
        for (int i = 0; i < lastCalculatedRoute.size(); ++i) {
            out << nodes[lastCalculatedRoute[i]].name;
            if (i < lastCalculatedRoute.size() - 1) out << " -> ";
        }
        out << "\n";
        file.close();
        QMessageBox::information(this, "Успех", "Маршрут сохранен!");
    }
}
