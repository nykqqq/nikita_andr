#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Новый объект");

    // Проверка, что ui элементы созданы
    if (!ui->loadButton || !ui->saveButton) {
        qDebug() << "UI elements not found!";
        return;
    }

    connect(ui->loadButton, &QPushButton::clicked,
            this, &MainWindow::on_loadButton_clicked);
    connect(ui->saveButton, &QPushButton::clicked,
            this, &MainWindow::on_saveButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Выберите файл с данными",
                                                    "",
                                                    "Текстовые файлы (*.txt);;Все файлы (*)");

    if (filePath.isEmpty())
        return;

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);

    if (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList data = line.split("/");

        if (data.size() >= 4)
        {
            ui->name_text->setText(data[0]);
            ui->description_text->setPlainText(data[1]);
            ui->coef_text->setValue(data[2].toDouble());

            int typeIndex = ui->type_combo->findText(data[3]);
            if (typeIndex != -1)
                ui->type_combo->setCurrentIndex(typeIndex);
        }
        else
        {
            QMessageBox::warning(this, "Ошибка",
                                 "Ожидается формат: название/описание/коэффициент/тип_защиты");
        }
    }

    file.close();
}

void MainWindow::on_saveButton_clicked()
{
    QString name = ui->name_text->text();
    QString description = ui->description_text->toPlainText();
    double coefficient = ui->coef_text->value();
    QString type = ui->type_combo->currentText();

    QJsonObject jsonObject;
    jsonObject["name"] = name;
    jsonObject["description"] = description;
    jsonObject["coefficient"] = coefficient;
    jsonObject["protection_type"] = type;

    QJsonDocument jsonDoc(jsonObject);

    QString filePath = "data.json";

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка",
                             "Не удалось создать файл: " + filePath);
        return;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();

    QMessageBox::information(this, "Успех",
                             "Данные успешно сохранены в " + filePath);
}
