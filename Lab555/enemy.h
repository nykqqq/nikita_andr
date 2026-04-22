#ifndef ENEMY_H
#define ENEMY_H

#include "npc.h"

class Enemy : public NPC
{
private:
    QString rarity;
    int damage;

public:
    Enemy(QString n, QString r, int d, int h, Armor a);

    QString Craft() const override;

    QString getRarity() const;
    int getDamage() const;
};

#endif
