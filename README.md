mainwindown.cpp #include "mainwindow.h"
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
mainwindow.h #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_loadButton_clicked();
    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif
mainwindow.ui <?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>500</width>
    <height>400</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Новый объект</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <property name="spacing">
     <number>15</number>
    </property>
    <property name="leftMargin">
     <number>30</number>
    </property>
    <property name="topMargin">
     <number>30</number>
    </property>
    <property name="rightMargin">
     <number>30</number>
    </property>
    <property name="bottomMargin">
     <number>30</number>
    </property>
    <item>
     <layout class="QHBoxLayout" name="nameLayout">
      <item>
       <widget class="QLabel" name="nameLabel">
        <property name="text">
         <string>Название:</string>
        </property>
        <property name="minimumSize">
         <size>
          <width>120</width>
          <height>0</height>
         </size>
        </property>
       </widget>
      </item>
      <item>
       <widget class="QLineEdit" name="name_text">
        <property name="placeholderText">
         <string>Введите название</string>
        </property>
       </widget>
      </item>
     </layout>
    </item>
    <item>
     <layout class="QHBoxLayout" name="descLayout">
      <item>
       <widget class="QLabel" name="descLabel">
        <property name="text">
         <string>Описание:</string>
        </property>
        <property name="minimumSize">
         <size>
          <width>120</width>
          <height>0</height>
         </size>
        </property>
       </widget>
      </item>
      <item>
       <widget class="QTextEdit" name="description_text">
        <property name="maximumSize">
         <size>
          <width>16777215</width>
          <height>100</height>
         </size>
        </property>
        <property name="placeholderText">
         <string>Введите описание</string>
        </property>
       </widget>
      </item>
     </layout>
    </item>
    <item>
     <layout class="QHBoxLayout" name="coefLayout">
      <item>
       <widget class="QLabel" name="coefLabel">
        <property name="text">
         <string>Коэффициент защиты:</string>
        </property>
        <property name="minimumSize">
         <size>
          <width>120</width>
          <height>0</height>
         </size>
        </property>
       </widget>
      </item>
      <item>
       <widget class="QDoubleSpinBox" name="coef_text">
        <property name="minimum">
         <double>0.00</double>
        </property>
        <property name="maximum">
         <double>999.99</double>
        </property>
        <property name="singleStep">
         <double>0.10</double>
        </property>
        <property name="decimals">
         <number>2</number>
        </property>
       </widget>
      </item>
     </layout>
    </item>
    <item>
     <layout class="QHBoxLayout" name="typeLayout">
      <item>
       <widget class="QLabel" name="typeLabel">
        <property name="text">
         <string>Тип защиты:</string>
        </property>
        <property name="minimumSize">
         <size>
          <width>120</width>
          <height>0</height>
         </size>
        </property>
       </widget>
      </item>
      <item>
       <widget class="QComboBox" name="type_combo">
        <item>
         <property name="text">
          <string>Магическая</string>
         </property>
        </item>
        <item>
         <property name="text">
          <string>Универсальная</string>
         </property>
        </item>
        <item>
         <property name="text">
          <string>Силовая</string>
         </property>
        </item>
       </widget>
      </item>
     </layout>
    </item>
    <item>
     <layout class="QHBoxLayout" name="buttonsLayout">
      <property name="spacing">
       <number>20</number>
      </property>
      <item>
       <widget class="QPushButton" name="loadButton">
        <property name="text">
         <string>📂 Загрузить из .txt</string>
        </property>
        <property name="minimumSize">
         <size>
          <width>150</width>
          <height>35</height>
         </size>
        </property>
       </widget>
      </item>
      <item>
       <widget class="QPushButton" name="saveButton">
        <property name="text">
         <string>💾 Сохранить в JSON</string>
        </property>
        <property name="minimumSize">
         <size>
          <width>150</width>
          <height>35</height>
         </size>
        </property>
       </widget>
      </item>
     </layout>
    </item>
    <item>
     <spacer name="verticalSpacer">
      <property name="orientation">
       <enum>Qt::Vertical</enum>
      </property>
      <property name="sizeHint" stdset="0">
       <size>
        <width>20</width>
        <height>40</height>
       </size>
      </property>
     </spacer>
    </item>
   </layout>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>