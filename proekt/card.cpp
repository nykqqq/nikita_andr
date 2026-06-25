#include "card.h"

Card::Card(Rank r, Suit s, bool up)
    : rank(r), suit(s), faceUp(up), value(0)
{
    calculateValue();
}

void Card::calculateValue() {
    if (rank == Rank::Mag || rank == Rank::Witch || rank == Rank::Monarch) {
        value = 10;
    }
    else if (rank == Rank::Sovereign) {
        value = 11;
    }
    else if (rank == Rank::Ten) {
        value = 10;
    }
    else {
        value = static_cast<int>(rank) + 2;
    }
}

bool Card::isAce() const {
    return rank == Rank::Sovereign;
}

QString Card::getRankString() const {
    switch(rank) {
    case Rank::Two: return "2";
    case Rank::Three: return "3";
    case Rank::Four: return "4";
    case Rank::Five: return "5";
    case Rank::Six: return "6";
    case Rank::Seven: return "7";
    case Rank::Eight: return "8";
    case Rank::Nine: return "9";
    case Rank::Ten: return "10";
    case Rank::Mag: return "mag";
    case Rank::Witch: return "witch";
    case Rank::Monarch: return "monarch";
    case Rank::Sovereign: return "sovereign";
    }
    return "";
}

QString Card::getSuitString() const {
    switch(suit) {
    case Suit::Fire: return "fire";
    case Suit::Water: return "water";
    case Suit::Earth: return "earth";
    case Suit::Air: return "air";
    }
    return "";
}

QString Card::getSuitSymbol() const {
    switch(suit) {
    case Suit::Fire: return "🔥";
    case Suit::Water: return "💧";
    case Suit::Earth: return "🌍";
    case Suit::Air: return "💨";
    }
    return "";
}

QString Card::getSuitColor() const {
    switch(suit) {
    case Suit::Fire: return "#FF4444";
    case Suit::Water: return "#4488FF";
    case Suit::Earth: return "#44BB44";
    case Suit::Air: return "#FFD700";
    }
    return "";
}

QString Card::getImagePath() const {
    if (!faceUp) {
        return ":/images/back.png";
    }
    return QString(":/images/%1_%2.png").arg(getSuitString()).arg(getRankString());
}
