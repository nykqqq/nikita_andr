#include "enemy.h"

Enemy::Enemy(QString n, QString r, int d, int h, Armor a)
    : NPC(n, h, a), rarity(r), damage(d)
{
}

QString Enemy::Craft() const
{
    return "Имя врага: " + name +
           "\nРедкость: " + rarity +
           "\nХП: " + QString::number(hp) +
           "\nУрон: " + QString::number(damage);
}

QString Enemy::getRarity() const
{
    return rarity;
}

int Enemy::getDamage() const
{
    return damage;
}
