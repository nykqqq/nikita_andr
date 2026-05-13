// Program1MainWindow.cpp (фрагменты)
#include "Program1MainWindow.h"
#include "ui_Program1MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QJsonDocument>
#include <QDebug>

Program1MainWindow::Program1MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Program1MainWindow)
{
    ui->setupUi(this);
    connect(ui->loadJsonBtn, &QPushButton::clicked, this, &Program1MainWindow::onLoadJsonClicked);
    connect(ui->saveJsonBtn, &QPushButton::clicked, this, &Program1MainWindow::onSaveJsonClicked);
    connect(ui->loadTxtBtn, &QPushButton::clicked, this, &Program1MainWindow::onLoadTxtClicked);
    connect(ui->addPersonBtn, &QPushButton::clicked, this, &Program1MainWindow::onAddPersonClicked);
    clearForm();
}

Program1MainWindow::~Program1MainWindow() { delete ui; }

void Program1MainWindow::clearForm() {
    ui->nameEdit->clear();
    ui->surnameEdit->clear();
    ui->heightSpin->setValue(0);
    ui->weightSpin->setValue(0.0);
}

void Program1MainWindow::onLoadJsonClicked() {
    QString path = QFileDialog::getOpenFileName(this, "Выбрать JSON файл", "", "JSON (*.json)");
    if (path.isEmpty()) return;
    loadJsonFromFile(path);
    m_currentJsonPath = path;
    QMessageBox::information(this, "Загрузка", "JSON загружен. Можно добавлять записи (в конец).");
}

void Program1MainWindow::loadJsonFromFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        m_personsArray = QJsonArray();
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isArray()) {
        m_personsArray = doc.array();
    } else {
        m_personsArray = QJsonArray();
    }
    file.close();
}

void Program1MainWindow::onSaveJsonClicked() {
    QString path = m_currentJsonPath.isEmpty() ?
                   QFileDialog::getSaveFileName(this, "Сохранить JSON", "", "JSON (*.json)") :
                   m_currentJsonPath;
    if (path.isEmpty()) return;
    saveJsonToFile(path);
    m_currentJsonPath = path;
}

void Program1MainWindow::saveJsonToFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }
    QJsonDocument doc(m_personsArray);
    file.write(doc.toJson());
    file.close();
    QMessageBox::information(this, "Сохранение", "Данные сохранены");
}

void Program1MainWindow::onLoadTxtClicked() {
    QString path = QFileDialog::getOpenFileName(this, "Выбрать TXT файл (объект)", "", "TXT (*.txt)");
    if (path.isEmpty()) return;
    loadTxtAndFillForm(path);
}

void Program1MainWindow::loadTxtAndFillForm(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось прочитать TXT");
        return;
    }
    QTextStream stream(&file);
    QString content = stream.readAll().trimmed();
    file.close();

    // Формат: Вася/Петров/174/43,0   (разделитель /)
    // Заменяем запятую на точку для веса
    content.replace(',', '.');
    QStringList parts = content.split('/');
    if (parts.size() >= 4) {
        ui->nameEdit->setText(parts[0]);
        ui->surnameEdit->setText(parts[1]);
        ui->heightSpin->setValue(parts[2].toInt());
        ui->weightSpin->setValue(parts[3].toFloat());
        QMessageBox::information(this, "Импорт", "Данные из TXT загружены в форму.\nТеперь можно редактировать и добавить.");
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный формат TXT. Ожидается: Имя/Фамилия/Рост/Вес");
    }
}

void Program1MainWindow::onAddPersonClicked() {
    QString name = ui->nameEdit->text().trimmed();
    QString surname = ui->surnameEdit->text().trimmed();
    int height = ui->heightSpin->value();
    float weight = ui->weightSpin->value();

    Person p(name, surname, height, weight);
    if (!p.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля корректно: имя, фамилия, рост>0, вес>0");
        return;
    }

    QJsonObject newObj = p.toJson();
    m_personsArray.append(newObj);
    clearForm();
    QMessageBox::information(this, "Добавлено", "Объект добавлен в конец текущего JSON массива.");
}