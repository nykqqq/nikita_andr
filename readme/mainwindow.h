#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include "BinaryTree.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadTree();
    void onShowTraversal();
    void onSaveToFile();

private:
    void drawTree();
    void drawNode(Node* node, qreal x, qreal y, qreal xOffset);
    void ensureSampleFileExists();   // создаёт tree.txt при первом запуске

    BinaryTree tree;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QTextEdit* logArea;
    QLabel* heightLabel;

    const int NODE_RADIUS = 25;
    const int VERTICAL_SPACING = 80;
};

#endif
