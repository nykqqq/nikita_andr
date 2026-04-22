#ifndef MAGE_H
#define MAGE_H

#include "npc.h"

class Mage : public NPC
{
private:
    QString element;
    int mana;

public:
    Mage(QString n, QString el, int m, int h, Armor a);

    QString Craft() const override;

    QString getElement() const;
    int getMana() const;
};

#endif
