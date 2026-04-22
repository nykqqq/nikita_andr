#ifndef NPC_H
#define NPC_H

#include <QString>

struct Armor
{
    int helmet;
    int chest;
    int boots;
};

class NPC
{
protected:
    QString name;
    int hp;
    Armor armor;

public:
    NPC(QString n, int h, Armor a);
    virtual ~NPC();

    virtual QString Craft() const = 0;

    QString getName() const;
    int getHP() const;
    Armor getArmor() const;
};

#endif
