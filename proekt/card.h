#ifndef CARD_H
#define CARD_H

#include <QString>

enum class Suit { Fire, Water, Earth, Air };
enum class Rank { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Mag, Witch, Monarch, Sovereign };

struct Card {
    Rank rank;
    Suit suit;
    bool faceUp;
    int value;

    Card(Rank r = Rank::Two, Suit s = Suit::Fire, bool up = true);
    void calculateValue();
    bool isAce() const;
    QString getRankString() const;
    QString getSuitString() const;
    QString getSuitSymbol() const;
    QString getSuitColor() const;
    QString getImagePath() const;

    bool operator==(const Card& other) const {
        return rank == other.rank && suit == other.suit;
    }

    bool operator!=(const Card& other) const {
        return !(*this == other);
    }
};

#endif // CARD_H
