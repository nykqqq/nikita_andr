#include "mage.h"

Mage::Mage(QString n, QString el, int m, int h, Armor a)
    : NPC(n, h, a), element(el), mana(m)
{
}

QString Mage::Craft() const
{
    return "Имя мага: " + name +
           "\nСтихия: " + element +
           "\nХП: " + QString::number(hp) +
           "\nМана: " + QString::number(mana);
}

QString Mage::getElement() const
{
    return element;
}

int Mage::getMana() const
{
    return mana;
}
