#include "carddeck.h"
#include <random>
#include <algorithm>

CardDeck::CardDeck() {
    reset();
}

void CardDeck::reset() {
    cards.clear();
    currentIndex = 0;

    for (int deck = 0; deck < 4; ++deck) {
        for (int suit = 0; suit < 4; ++suit) {
            for (int rank = 0; rank < 13; ++rank) {
                cards.append(Card(static_cast<Rank>(rank), static_cast<Suit>(suit)));
            }
        }
    }
}

void CardDeck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
    currentIndex = 0;
}

Card CardDeck::drawCard() {
    if (currentIndex >= cards.size()) {
        reset();
        shuffle();
    }
    return cards[currentIndex++];
}

int CardDeck::remainingCards() const {
    return cards.size() - currentIndex;
}

bool CardDeck::needShuffle() const {
    return remainingCards() <= 52;
}
