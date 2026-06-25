#ifndef CARDDECK_H
#define CARDDECK_H

#include <QVector>
#include "card.h"

class CardDeck {
private:
    QVector<Card> cards;
    int currentIndex;

public:
    CardDeck();
    void shuffle();
    Card drawCard();
    int remainingCards() const;
    bool needShuffle() const;
    void reset();
};

#endif // CARDDECK_H
