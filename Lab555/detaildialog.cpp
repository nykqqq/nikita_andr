#include "detaildialog.h"
#include "ui_detaildialog.h"
#include "mage.h"
#include "enemy.h"
#include <QPixmap>

DetailDialog::DetailDialog(NPC* npc, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::DetailDialog)
{
    ui->setupUi(this);

    QString htmlText;

    if (Mage* m = dynamic_cast<Mage*>(npc))
    {
        QString elementColor = "black";

        if (m->getElement().toLower() == "fire" ||
            m->getElement().toLower() == "огонь")
            elementColor = "orange";
        else if (m->getElement().toLower() == "water" ||
                 m->getElement().toLower() == "вода")
            elementColor = "blue";
        else if (m->getElement().toLower() == "earth" ||
                 m->getElement().toLower() == "земля")
            elementColor = "green";

        htmlText =
            "<b>Имя мага:</b> " + m->getName() + "<br>"
                                                 "<b>Стихия:</b> <span style='color:" + elementColor + ";'>"
            + m->getElement() + "</span><br>"
                                "<b>Мана:</b> <span style='color:blue;'>"
            + QString::number(m->getMana()) + "</span><br>"
                                              "<b>ХП:</b> <span style='color:green;'>"
            + QString::number(m->getHP()) + "</span>";

        ui->labelImage->setPixmap(
            QPixmap(":/images/mag.png")
                .scaled(120, 120, Qt::KeepAspectRatio)
            );
    }
    else if (Enemy* e = dynamic_cast<Enemy*>(npc))
    {
        QString rarityColor = "black";

        QString rarity = e->getRarity().toLower();

        if (rarity == "rare" || rarity == "редкий")
            rarityColor = "blue";
        else if (rarity == "common" || rarity == "обычный")
            rarityColor = "green";
        else if (rarity == "legend" || rarity == "легенда")
            rarityColor = "orange";

        htmlText =
            "<b>Имя врага:</b> <span style='color:" + rarityColor + ";'>"
            + e->getName() + "</span><br>"
                             "<b>Редкость:</b> " + e->getRarity() + "<br>"
                               "<b>Урон:</b> " + QString::number(e->getDamage()) + "<br>"
                                                "<b>ХП:</b> <span style='color:red;'>"
            + QString::number(e->getHP()) + "</span>";

        ui->labelImage->setPixmap(
            QPixmap(":/images/vrag.png")
                .scaled(120, 120, Qt::KeepAspectRatio)
            );
    }

    ui->labelInfo->setText(htmlText);
    ui->labelInfo->setTextFormat(Qt::RichText);
}

DetailDialog::~DetailDialog()
{
    delete ui;
}

void DetailDialog::on_btnPrint_clicked()
{
    accept();
}

void DetailDialog::on_btnCancel_clicked()
{
    reject();
}
