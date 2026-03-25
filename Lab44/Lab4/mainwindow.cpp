#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Сброс выбора радиокнопок
    ui->radioPaper->setAutoExclusive(false);
    ui->radioElectronic->setAutoExclusive(false);
    ui->radioPaper->setChecked(false);
    ui->radioElectronic->setChecked(false);
    ui->radioPaper->setAutoExclusive(true);
    ui->radioElectronic->setAutoExclusive(true);

    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSaveClicked()
{
    QString errorMsg;
    if (!validateFields(errorMsg))
    {
        QMessageBox::critical(this, "Ошибка", errorMsg);
        return;
    }

    QString type;
    if (ui->radioPaper->isChecked())
        type = ui->radioPaper->text();
    else if (ui->radioElectronic->isChecked())
        type = ui->radioElectronic->text();

    Book item(ui->authorEdit->text().trimmed(),
                  ui->titleEdit->text().trimmed(),
                  ui->codeEdit->text().trimmed(),
                  ui->ratioEdit->text().trimmed(),
                  type);

    if (item.saveToFile("result.txt"))
    {
        QMessageBox::information(this, "Успех", "Данные сохранены в result.txt");
        onResetClicked();
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл");
    }
}

void MainWindow::onResetClicked()
{
    ui->authorEdit->clear();
    ui->titleEdit->clear();
    ui->codeEdit->clear();
    ui->ratioEdit->clear();
    ui->radioPaper->setChecked(false);
    ui->radioElectronic->setChecked(false);
}

bool MainWindow::validateFields(QString &errorMessage)
{
    // Обязательные поля
    if (ui->authorEdit->text().trimmed().isEmpty())
    {
        errorMessage = "Поле 'Автор' не заполнено.";
        return false;
    }
    if (ui->titleEdit->text().trimmed().isEmpty())
    {
        errorMessage = "Поле 'Название' не заполнено.";
        return false;
    }
    if (ui->codeEdit->text().trimmed().isEmpty())
    {
        errorMessage = "Поле 'Код' не заполнено.";
        return false;
    }
    if (ui->ratioEdit->text().trimmed().isEmpty())
    {
        errorMessage = "Поле 'Наполнение' не заполнено.";
        return false;
    }
    if (!ui->radioPaper->isChecked() && !ui->radioElectronic->isChecked())
    {
        errorMessage = "Поле 'Тип издания' не выбрано.";
        return false;
    }

    // Проверка формата "Автор"
    QRegularExpression authorRegex("^[А-ЯЁ][а-яё]+\\s+[А-ЯЁ]{1,2}$");
    if (!authorRegex.match(ui->authorEdit->text().trimmed()).hasMatch())
    {
        errorMessage = "Автор: формат 'Иванов ИВ' (первая буква заглавная, затем одна или две заглавные буквы через пробел).";
        return false;
    }

    // Название в кавычках
    QRegularExpression titleRegex("^\"[^\"]*\"$");
    if (!titleRegex.match(ui->titleEdit->text().trimmed()).hasMatch())
    {
        errorMessage = "Название должно быть в двойных кавычках, например: \"Война и мир\".";
        return false;
    }

    // Код:
    QRegularExpression codeRegex("^\\d+-\\d+$");
    if (!codeRegex.match(ui->codeEdit->text().trimmed()).hasMatch())
    {
        errorMessage = "Код должен быть в формате: 123-12345.";
        return false;
    }

    // Наполнение: число/число, второе меньше первого
    QRegularExpression ratioRegex("^(\\d+)/(\\d+)$");
    auto match = ratioRegex.match(ui->ratioEdit->text().trimmed());
    if (!match.hasMatch())
    {
        errorMessage = "'Наполнение' должно быть в формате: число/число, например 2/1.";
        return false;
    }
    int num1 = match.captured(1).toInt();
    int num2 = match.captured(2).toInt();
    if (num2 >= num1)
    {
        errorMessage = "В 'Наполнение' второе число должно быть меньше первого.";
        return false;
    }

    return true;
}
