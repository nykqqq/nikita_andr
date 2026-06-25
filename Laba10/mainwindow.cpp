#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QRegularExpression>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Регистрация");
    setFixedSize(450, 600);

    connect(ui->registerBtn, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(ui->loadPhotoBtn, &QPushButton::clicked, this, &MainWindow::onLoadPhotoClicked);

    setupDoubleClickClear();

    selectedPhotoPath = "";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDoubleClickClear()
{
    ui->nameEdit->installEventFilter(this);
    ui->surnameEdit->installEventFilter(this);
    ui->nicknameEdit->installEventFilter(this);
    ui->idKeyEdit->installEventFilter(this);
    ui->emailEdit->installEventFilter(this);
    ui->passwordEdit->installEventFilter(this);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (obj == ui->nameEdit) {
            ui->nameEdit->clear();
            return true;
        }
        if (obj == ui->surnameEdit) {
            ui->surnameEdit->clear();
            return true;
        }
        if (obj == ui->nicknameEdit) {
            ui->nicknameEdit->clear();
            return true;
        }
        if (obj == ui->idKeyEdit) {
            ui->idKeyEdit->clear();
            return true;
        }
        if (obj == ui->emailEdit) {
            ui->emailEdit->clear();
            return true;
        }
        if (obj == ui->passwordEdit) {
            ui->passwordEdit->clear();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onLoadPhotoClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите фото", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!fileName.isEmpty()) {
        selectedPhotoPath = fileName;
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            ui->photoLabel->setPixmap(pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

QString MainWindow::encryptPassword(const QString& password)
{
    const QString key = "SecretKey2024";
    QString encrypted;

    for (int i = 0; i < password.length(); ++i) {
        encrypted.append(QChar(password[i].unicode() ^ key[i % key.length()].unicode()));
    }

    return encrypted.toUtf8().toBase64();
}

bool MainWindow::validateIdKey(const QString& id)
{
    QRegularExpression regex("^[А-ЯA-Z][0-9]{3}-[0-9]{2}[А-ЯA-Z]-[А-ЯA-Z]$");
    return regex.match(id).hasMatch();
}

bool MainWindow::isNicknameAvailable(const QString& nickname)
{
    QString accountPath = getDesktopPath() + "/Accounts/" + nickname;
    return !QDir(accountPath).exists();
}

bool MainWindow::validateFields()
{
    if (ui->nameEdit->text().trimmed().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Поле 'Имя' не может быть пустым!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                             "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();
        ui->nameEdit->setFocus();
        return false;
    }

    if (ui->surnameEdit->text().trimmed().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Поле 'Фамилия' не может быть пустым!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                             "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();
        ui->surnameEdit->setFocus();
        return false;
    }

    if (ui->nicknameEdit->text().trimmed().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Поле 'Никнейм' не может быть пустым!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                             "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();
        ui->nicknameEdit->setFocus();
        return false;
    }

    if (!isNicknameAvailable(ui->nicknameEdit->text())) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Указанный никнейм уже занят!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                             "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();
        ui->nicknameEdit->setFocus();
        ui->nicknameEdit->clear();
        return false;
    }

    if (!validateIdKey(ui->idKeyEdit->text())) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("ID-Key должен быть в формате: А111-11А-А");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                             "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();
        ui->idKeyEdit->setFocus();
        ui->idKeyEdit->clear();
        return false;
    }

    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(ui->emailEdit->text()).hasMatch()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Введите корректный email!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                             "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();
        ui->emailEdit->setFocus();
        ui->emailEdit->clear();
        return false;
    }

    if (ui->passwordEdit->text().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Поле 'Пароль' не может быть пустым!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                             "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();
        ui->passwordEdit->setFocus();
        return false;
    }

    if (ui->passwordEdit->text().length() < 6) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Пароль должен содержать минимум 6 символов!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                             "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();
        ui->passwordEdit->setFocus();
        ui->passwordEdit->clear();
        return false;
    }

    if (!ui->europeCheck->isChecked() && !ui->asiaCheck->isChecked() && !ui->americaCheck->isChecked()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Выберите хотя бы один сервер!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                             "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();
        return false;
    }

    return true;
}

QString MainWindow::getDesktopPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void MainWindow::saveToJson(const QString& nickname)
{
    QString jsonPath = getDesktopPath() + "/Accounts/" + nickname + "/userdata.json";

    QJsonObject jsonObj;
    jsonObj["Имя"] = ui->nameEdit->text();
    jsonObj["Фамилия"] = ui->surnameEdit->text();
    jsonObj["Никнейм"] = ui->nicknameEdit->text();
    jsonObj["ID-Key"] = ui->idKeyEdit->text();
    jsonObj["Почта"] = ui->emailEdit->text();

    QStringList servers;
    if (ui->europeCheck->isChecked()) servers.append("Европа");
    if (ui->asiaCheck->isChecked()) servers.append("Азия");
    if (ui->americaCheck->isChecked()) servers.append("Америка");
    jsonObj["Сервера"] = servers.join(", ");

    QJsonDocument doc(jsonObj);

    QFile file(jsonPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::saveToTxt(const QString& email, const QString& password)
{
    QString txtPath = getDesktopPath() + "/Accounts/" + ui->nicknameEdit->text() + "/password.txt";

    QString encryptedPassword = encryptPassword(password);
    QString data = email + "/" + encryptedPassword;

    QFile file(txtPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;
        file.close();
    }
}

void MainWindow::copyPhoto(const QString& nickname)
{
    QString destPath = getDesktopPath() + "/Accounts/" + nickname + "/photo.jpg";

    if (!selectedPhotoPath.isEmpty() && QFile::exists(selectedPhotoPath)) {
        QFile::copy(selectedPhotoPath, destPath);
    } else {
        QFile defaultFile(destPath);
        if (defaultFile.open(QIODevice::WriteOnly)) {
            defaultFile.close();
        }
    }
}

void MainWindow::createAccountDirectory(const QString& nickname)
{
    QString accountPath = getDesktopPath() + "/Accounts/" + nickname;
    QDir dir;
    if (!dir.exists(accountPath)) {
        dir.mkpath(accountPath);
    }
}

void MainWindow::onRegisterClicked()
{
    if (validateFields()) {
        QString nickname = ui->nicknameEdit->text();

        createAccountDirectory(nickname);
        saveToJson(nickname);
        saveToTxt(ui->emailEdit->text(), ui->passwordEdit->text());
        copyPhoto(nickname);

        QMessageBox msgBox;
        msgBox.setWindowTitle("Успех");
        msgBox.setText(QString("Регистрация успешно завершена!\n"
                               "Данные сохранены в: %1/Accounts/%2")
                              .arg(getDesktopPath()).arg(nickname));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                                          "QLabel{ color: black; font-size: 20px}");
        msgBox.exec();


        ui->nameEdit->clear();
        ui->surnameEdit->clear();
        ui->nicknameEdit->clear();
        ui->idKeyEdit->clear();
        ui->emailEdit->clear();
        ui->passwordEdit->clear();
        ui->europeCheck->setChecked(false);
        ui->asiaCheck->setChecked(false);
        ui->americaCheck->setChecked(false);
        ui->photoLabel->setText("Фото");
        ui->photoLabel->setPixmap(QPixmap());
        selectedPhotoPath = "";
    }
}
