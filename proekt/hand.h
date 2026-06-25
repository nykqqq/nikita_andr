#ifndef HAND_H
#define HAND_H

#include <QVector>
#include "card.h"

class Hand {
public:
    QVector<Card> cards;
    int bet;
    bool isSplit;
    bool isDoubled;
    bool isActive;

    Hand(int b = 0);
    void addCard(const Card& card);
    int getTotal() const;
    bool isBlackjack() const;
    bool isBust() const;
    bool canSplit() const;
    void clear();
};

#endif // HAND_H
