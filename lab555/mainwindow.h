#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MorseCodec.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnEncode_clicked();      // П.1 Кодирование
    void on_btnTransmit_clicked();    // П.2 Передача (внести ошибку)
    void on_btnDecode_clicked();      // П.3 Декодирование
    void on_btnLoadText_clicked();    // Загрузить текст из файла
    void on_btnSaveText_clicked();    // Сохранить текст в файл
    void on_btnLoadMorse_clicked();   // Загрузить морзе из файла
    void on_btnSaveMorse_clicked();   // Сохранить морзе в файл

private:
    Ui::MainWindow *ui;
    MorseCodec m_codec;

    // Вспомогательные
    QString openFileDialog();
    QString saveFileDialog();
    void showVariants(const QStringList& variants);
};

#endif // MAINWINDOW_H
