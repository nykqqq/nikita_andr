#include "hand.h"

Hand::Hand(int b)
    : bet(b), isSplit(false), isDoubled(false), isActive(true) {}

void Hand::addCard(const Card& card) {
    cards.append(card);
}

int Hand::getTotal() const {
    int total = 0;
    int aces = 0;

    for (const Card& card : cards) {
        total += card.value;
        if (card.isAce()) {
            aces++;
        }
    }

    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }

    return total;
}

bool Hand::isBlackjack() const {
    return cards.size() == 2 && getTotal() == 21;
}

bool Hand::isBust() const {
    return getTotal() > 21;
}

bool Hand::canSplit() const {
    return cards.size() == 2 && cards[0].rank == cards[1].rank;
}

void Hand::clear() {
    cards.clear();
    isActive = true;
    isDoubled = false;
}
