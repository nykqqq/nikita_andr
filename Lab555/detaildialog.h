#ifndef DETAILDIALOG_H
#define DETAILDIALOG_H

#include <QDialog>
#include "npc.h"

namespace Ui {
class DetailDialog;
}

class DetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetailDialog(NPC* npc, QWidget *parent = nullptr);
    ~DetailDialog();

private slots:
    void on_btnPrint_clicked();
    void on_btnCancel_clicked();

private:
    Ui::DetailDialog *ui;
};

#endif
