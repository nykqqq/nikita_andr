#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "npc.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    std::vector<NPC*> list;

    void loadData(QString fileName);
    void refreshTable();
};

#endif
