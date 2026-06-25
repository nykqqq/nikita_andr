#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>

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

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onRegisterClicked();
    void onLoadPhotoClicked();

private:
    Ui::MainWindow *ui;
    QString selectedPhotoPath;

    bool validateFields();
    bool validateIdKey(const QString& id);
    bool isNicknameAvailable(const QString& nickname);
    void createAccountDirectory(const QString& nickname);
    void saveToJson(const QString& nickname);
    void saveToTxt(const QString& email, const QString& password);
    void copyPhoto(const QString& nickname);
    QString getDesktopPath();
    QString encryptPassword(const QString& password);
    void setupDoubleClickClear();
};

#endif // MAINWINDOW_H
