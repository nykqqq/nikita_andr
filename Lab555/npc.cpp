#include "npc.h"

NPC::NPC(QString n, int h, Armor a)
    : name(n), hp(h), armor(a)
{
}

NPC::~NPC()
{
}

QString NPC::getName() const
{
    return name;
}

int NPC::getHP() const
{
    return hp;
}

Armor NPC::getArmor() const
{
    return armor;
}
