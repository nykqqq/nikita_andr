#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QGraphicsSimpleTextItem>   // <-- надёжный текстовый элемент
#include <QDir>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    resize(900, 700);

    QHBoxLayout* controlLayout = new QHBoxLayout();
    QPushButton* btnLoad = new QPushButton("Загрузить из файла (tree.txt)");
    QPushButton* btnTraversal = new QPushButton("Префиксный обход (УЛП)");
    QPushButton* btnSave = new QPushButton("Сохранить в result.txt");
    heightLabel = new QLabel("Высота: -");
    heightLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    controlLayout->addWidget(btnLoad);
    controlLayout->addWidget(btnTraversal);
    controlLayout->addWidget(btnSave);
    controlLayout->addWidget(heightLabel);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    logArea = new QTextEdit();
    logArea->setReadOnly(true);
    logArea->setMaximumHeight(100);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(view);
    mainLayout->addWidget(logArea);

    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadTree);
    connect(btnTraversal, &QPushButton::clicked, this, &MainWindow::onShowTraversal);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveToFile);

    // Создаём tree.txt, если его нет, и сразу грузим
    ensureSampleFileExists();
    onLoadTree();
}

MainWindow::~MainWindow() {}

// Создаёт tree.txt с примером из задания при первом запуске
void MainWindow::ensureSampleFileExists() {
    QString path = QDir::currentPath() + "/tree.txt";
    QFile f(path);
    if (f.exists()) return;

    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);
        out << "1\n";
        out << "4 3\n";
        out << "2 - 9 7\n";
        out << "- - - - - - - -\n";
        f.close();
        logArea->append("Создан файл tree.txt с примером: " + path);
    }
}

void MainWindow::onLoadTree() {
    if (tree.loadFromFile("tree.txt")) {
        logArea->append("Дерево успешно загружено.");
        drawTree();
        heightLabel->setText(QString("Высота: %1").arg(tree.getHeight()));
    } else {
        logArea->append("Ошибка: не удалось открыть tree.txt");
    }
}

void MainWindow::drawTree() {
    scene->clear();
    if (!tree.getRoot()) return;

    // Центр сцены — 0,0 неудобен, сместим начальную точку
    qreal startX = 0;
    qreal startY = 0;
    qreal offset = 280;

    drawNode(tree.getRoot(), startX, startY, offset);

    // Подгоняем вид под содержимое
    scene->setSceneRect(scene->itemsBoundingRect());
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::drawNode(Node* node, qreal x, qreal y, qreal xOffset) {
    if (!node) return;

    // 1. Линии к детям (рисуем их ПЕРВЫМИ, чтобы они были ПОД кружками)
    QPen linePen(Qt::black);
    linePen.setWidth(2);

    if (node->left) {
        qreal childX = x - xOffset;
        qreal childY = y + VERTICAL_SPACING;
        scene->addLine(x, y + NODE_RADIUS, childX, childY - NODE_RADIUS, linePen);
        drawNode(node->left, childX, childY, xOffset / 2);
    }
    if (node->right) {
        qreal childX = x + xOffset;
        qreal childY = y + VERTICAL_SPACING;
        scene->addLine(x, y + NODE_RADIUS, childX, childY - NODE_RADIUS, linePen);
        drawNode(node->right, childX, childY, xOffset / 2);
    }

    // 2. Кружок
    QPen pen(Qt::black);
    pen.setWidth(2);
    QBrush brush(Qt::white);
    scene->addEllipse(x - NODE_RADIUS, y - NODE_RADIUS,
                      NODE_RADIUS * 2, NODE_RADIUS * 2, pen, brush);

    // 3. Текст — QGraphicsSimpleTextItem + точное центрирование
    QFont font;
    font.setPointSize(14);
    font.setBold(true);
    QGraphicsSimpleTextItem* text = scene->addSimpleText(QString::number(node->data), font);
    text->setBrush(QBrush(Qt::black));

    QRectF tb = text->boundingRect();
    text->setPos(x - tb.width() / 2.0, y - tb.height() / 2.0);
}

void MainWindow::onShowTraversal() {
    if (!tree.getRoot()) {
        logArea->append("Дерево пустое.");
        return;
    }
    QString result = tree.getPreorderTraversal();
    logArea->append("Префиксный обход (УЛП): " + result);
    heightLabel->setText(QString("Высота: %1").arg(tree.getHeight()));
}

// Сохраняет результат в result.txt рядом с программой (без диалога)
void MainWindow::onSaveToFile() {
    if (!tree.getRoot()) {
        logArea->append("Дерево пустое — сохранять нечего.");
        return;
    }

    QString path = QDir::currentPath() + "/result.txt";
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Префиксный обход (УЛП): " << tree.getPreorderTraversal() << "\n";
        out << "Высота дерева: " << tree.getHeight() << "\n";
        file.close();
        logArea->append("Результат сохранён в: " + path);
    } else {
        logArea->append("Не удалось сохранить result.txt");
    }
}
