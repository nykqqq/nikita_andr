#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mage.h"
#include "enemy.h"
#include "detaildialog.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(
        {"Имя", "Тип", "Параметр", "ХП"}
        );

    connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::on_pushButton_clicked);

    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked,
            this, &MainWindow::on_tableWidget_cellDoubleClicked);
}

MainWindow::~MainWindow()
{
    for (auto x : list)
        delete x;

    delete ui;
}

void MainWindow::loadData(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка",
                             "Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        int code;
        QString name;
        QString text;
        int param;
        int hp;
        int helmet;
        int chest;
        int boots;

        in >> code >> name >> text
            >> param >> hp
            >> helmet >> chest >> boots;

        if (in.status() != QTextStream::Ok)
            break;

        Armor armor{helmet, chest, boots};

        if (code == 1)
        {
            list.push_back(
                new Mage(name, text, param, hp, armor)
                );
        }
        else if (code == 2)
        {
            list.push_back(
                new Enemy(name, text, param, hp, armor)
                );
        }
    }

    file.close();
}

void MainWindow::refreshTable()
{
    ui->tableWidget->setRowCount(0);

    for (int i = 0; i < static_cast<int>(list.size()); i++)
    {
        ui->tableWidget->insertRow(i);

        ui->tableWidget->setItem(i, 0,
                                 new QTableWidgetItem(list[i]->getName()));

        if (Mage* m = dynamic_cast<Mage*>(list[i]))
        {
            ui->tableWidget->setItem(i, 1,
                                     new QTableWidgetItem("Маг"));

            ui->tableWidget->setItem(i, 2,
                                     new QTableWidgetItem(m->getElement()));

            ui->tableWidget->setItem(i, 3,
                                     new QTableWidgetItem(
                                         QString::number(m->getHP())
                                         ));
        }
        else if (Enemy* e = dynamic_cast<Enemy*>(list[i]))
        {
            ui->tableWidget->setItem(i, 1,
                                     new QTableWidgetItem("Враг"));

            ui->tableWidget->setItem(i, 2,
                                     new QTableWidgetItem(e->getRarity()));

            ui->tableWidget->setItem(i, 3,
                                     new QTableWidgetItem(
                                         QString::number(e->getHP())
                                         ));
        }
    }

    ui->tableWidget->clearSelection();
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Выберите файл",
        QDir::currentPath(),
        "Text files (*.txt)"
        );

    if (fileName.isEmpty())
        return;

    for (auto x : list)
        delete x;

    list.clear();

    loadData(fileName);

    QMessageBox::information(
        this,
        "Загрузка",
        "Загружено записей: " +
            QString::number(list.size())
        );

    refreshTable();
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= static_cast<int>(list.size()))
        return;

    DetailDialog dlg(list[row], this);

    if (dlg.exec() == QDialog::Accepted)
    {
        if (row >= 0 && row < static_cast<int>(list.size()))
        {
            delete list[row];
            list.erase(list.begin() + row);
            refreshTable();
        }
    }
}
