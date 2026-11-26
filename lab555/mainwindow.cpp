#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Лабораторная №5 — Азбука Морзе");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------- Вспомогательные ----------

QString MainWindow::openFileDialog()
{
    return QFileDialog::getOpenFileName(this, "Открыть файл", "",
                                        "Текстовые файлы (*.txt);;Все файлы (*)");
}

QString MainWindow::saveFileDialog()
{
    return QFileDialog::getSaveFileName(this, "Сохранить файл", "",
                                        "Текстовые файлы (*.txt);;Все файлы (*)");
}

// ---------- П.1 Кодирование ----------

void MainWindow::on_btnEncode_clicked()
{
    QString text = ui->textInput->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите текст для кодирования.");
        return;
    }
    QString morse = m_codec.encode(text);
    ui->morseOutput->setPlainText(morse);
}

// ---------- П.2 Передача (внести ошибку) ----------

void MainWindow::on_btnTransmit_clicked()
{
    QString morse = ui->morseOutput->toPlainText().trimmed();
    if (morse.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             "Сначала выполните кодирование (п.1).");
        return;
    }
    QString corrupted = m_codec.applyNoviceError(morse);
    ui->morseOutput->setPlainText(corrupted);
    QMessageBox::information(this, "Передача",
                             "В код внесены ошибки радиста-новичка.\n"
                             "Проверьте поле вывода.");
}

// ---------- П.3 Декодирование ----------

void MainWindow::on_btnDecode_clicked()
{
    QString morse = ui->morseOutput->toPlainText().trimmed();
    if (morse.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет кода Морзе для декодирования.");
        return;
    }

    QStringList variants = m_codec.decode(morse);

    if (variants.size() == 1 && !variants[0].contains("Не удалось")) {
        ui->textDecoded->setPlainText(variants[0]);
        ui->listVariants->clear();
        return;
    }

    // Если есть неоднозначность — показываем пользователю список (П.4)
    showVariants(variants);
}

// ---------- П.4 Восстановление (показ вариантов) ----------

void MainWindow::showVariants(const QStringList& variants)
{
    ui->listVariants->clear();
    for (const QString& v : variants) {
        ui->listVariants->addItem(v);
    }

    if (variants.isEmpty()) {
        QMessageBox::warning(this, "Декодирование",
                             "Не удалось декодировать код.");
        return;
    }

    // Пользователь выбирает вариант двойным кликом
    bool ok = false;
    QString chosen = QInputDialog::getItem(
        this, "Восстановление",
        "Выберите вариант декодирования (П.4):",
        variants, 0, false, &ok);

    if (ok && !chosen.isEmpty()) {
        ui->textDecoded->setPlainText(chosen);

        // Сохраняем выбранный вариант в файл (по требованию П.4)
        QString path = saveFileDialog();
        if (!path.isEmpty()) {
            QFile file(path);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << chosen;
                file.close();
                QMessageBox::information(this, "Сохранено",
                                         "Вариант сохранён в файл.");
            } else {
                QMessageBox::critical(this, "Ошибка",
                                      "Не удалось сохранить файл.");
            }
        }
    }
}

// ---------- Загрузка / сохранение ----------

void MainWindow::on_btnLoadText_clicked()
{
    QString path = openFileDialog();
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }
    QTextStream in(&file);
    ui->textInput->setPlainText(in.readAll());
    file.close();
}

void MainWindow::on_btnSaveText_clicked()
{
    QString path = saveFileDialog();
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл.");
        return;
    }
    QTextStream out(&file);
    out << ui->textDecoded->toPlainText();
    file.close();
    QMessageBox::information(this, "OK", "Текст сохранён.");
}

void MainWindow::on_btnLoadMorse_clicked()
{
    QString path = openFileDialog();
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }
    QTextStream in(&file);
    ui->morseOutput->setPlainText(in.readAll());
    file.close();
}

void MainWindow::on_btnSaveMorse_clicked()
{
    QString path = saveFileDialog();
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл.");
        return;
    }
    QTextStream out(&file);
    out << ui->morseOutput->toPlainText();
    file.close();
    QMessageBox::information(this, "OK", "Код Морзе сохранён.");
}
