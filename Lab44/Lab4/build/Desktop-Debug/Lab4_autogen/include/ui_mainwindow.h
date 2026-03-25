/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *saveButton;
    QPushButton *resetButton;
    QLineEdit *authorEdit;
    QLineEdit *titleEdit;
    QLineEdit *codeEdit;
    QLineEdit *ratioEdit;
    QRadioButton *radioPaper;
    QRadioButton *radioElectronic;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(369, 298);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName("saveButton");
        saveButton->setGeometry(QRect(100, 220, 91, 21));
        resetButton = new QPushButton(centralwidget);
        resetButton->setObjectName("resetButton");
        resetButton->setGeometry(QRect(210, 220, 91, 21));
        authorEdit = new QLineEdit(centralwidget);
        authorEdit->setObjectName("authorEdit");
        authorEdit->setGeometry(QRect(100, 10, 141, 31));
        titleEdit = new QLineEdit(centralwidget);
        titleEdit->setObjectName("titleEdit");
        titleEdit->setGeometry(QRect(100, 50, 141, 31));
        codeEdit = new QLineEdit(centralwidget);
        codeEdit->setObjectName("codeEdit");
        codeEdit->setGeometry(QRect(100, 90, 141, 31));
        ratioEdit = new QLineEdit(centralwidget);
        ratioEdit->setObjectName("ratioEdit");
        ratioEdit->setGeometry(QRect(100, 130, 141, 31));
        radioPaper = new QRadioButton(centralwidget);
        radioPaper->setObjectName("radioPaper");
        radioPaper->setGeometry(QRect(100, 170, 141, 31));
        radioPaper->setToolTipDuration(-5);
        radioElectronic = new QRadioButton(centralwidget);
        radioElectronic->setObjectName("radioElectronic");
        radioElectronic->setGeometry(QRect(200, 170, 141, 31));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 10, 41, 31));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 60, 61, 16));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(50, 100, 31, 16));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 130, 71, 21));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 180, 71, 16));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 369, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        resetButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\261\321\200\320\276\321\201", nullptr));
        radioPaper->setText(QCoreApplication::translate("MainWindow", "\320\221\321\203\320\274\320\260\320\266\320\275\320\276\320\265", nullptr));
        radioElectronic->setText(QCoreApplication::translate("MainWindow", "\320\255\320\273\320\265\320\272\321\202\321\200\320\276\320\275\320\275\320\276\320\265", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\220\320\262\321\202\320\276\321\200", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\264", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\242\320\270\320\277 \320\270\320\267\320\264\320\260\320\275\320\270\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
