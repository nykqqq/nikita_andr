// Program1MainWindow.h
#ifndef PROGRAM1MAINWINDOW_H
#define PROGRAM1MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>
#include "Person.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Program1MainWindow; }
QT_END_NAMESPACE

class Program1MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Program1MainWindow(QWidget *parent = nullptr);
    ~Program1MainWindow();

private slots:
    void onLoadJsonClicked();
    void onSaveJsonClicked();
    void onLoadTxtClicked();
    void onAddPersonClicked();

private:
    void loadJsonFromFile(const QString &path);
    void saveJsonToFile(const QString &path);
    void loadTxtAndFillForm(const QString &path);
    void clearForm();

    Ui::Program1MainWindow *ui;
    QJsonArray m_personsArray; // текущий загруженный массив (или пустой для нового)
    QString m_currentJsonPath;
};

#endif // PROGRAM1MAINWINDOW_H