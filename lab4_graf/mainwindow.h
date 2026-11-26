#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QVector>
#include <QDebug>
#include <queue>


struct Node {
    int id;
    QString name;
    QPointF pos;
};


struct Edge {
    int from;
    int to;
    double baseLength;
    double loadFactor;
    QColor color;
};

class MapWidget : public QWidget {
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);
    void setGraphData(const QVector<Node>& nodes, const QVector<Edge>& edges);
    void setRoute(const QVector<int>& route);
    void resetRoute();
    void setBackground(const QPixmap& pixmap);
    int getFirstSelected() const { return m_firstSelected; }
    int getSecondSelected() const { return m_secondSelected; }

signals:
    void nodeSelected(int id);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPixmap m_background;
    QVector<Node> m_nodes;
    QVector<Edge> m_edges;
    QVector<int> m_currentRoute;
    int m_firstSelected = -1;
    int m_secondSelected = -1;

    int findNodeAt(QPointF pos);
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadTraffic();
    void onFindRoute();
    void onSaveRoute();
    void onNodeSelected(int id);

private:
    void initGraph();

    MapWidget *mapWidget;
    QComboBox *comboStart;
    QComboBox *comboEnd;
    QPushButton *btnTraffic;
    QPushButton *btnFind;
    QPushButton *btnSave;

    QVector<Node> nodes;
    QVector<Edge> edges;
    QVector<int> lastCalculatedRoute;

    QColor getColorForLoad(double load);
};
#endif
