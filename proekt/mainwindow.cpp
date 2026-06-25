#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QIntValidator>
#include <QFile>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , money(1000)
    , currentBet(10)
    , gameActive(false)
    , isSplitMode(false)
    , isSplitPhase(false)
    , currentHandIndex(0)
{
    ui->setupUi(this);
    animationTimer = new QTimer(this);
    animationTimer->setSingleShot(true);

    // Создаем поле для ввода своей ставки
    QHBoxLayout* betLayout = qobject_cast<QHBoxLayout*>(ui->betLayout);
    if (betLayout) {
        QSpacerItem* spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        betLayout->insertSpacerItem(betLayout->count(), spacer);

        QLabel* customLabel = new QLabel("Своя:");
        customLabel->setStyleSheet("color: white; font-size: 14px; font-weight: bold;");
        betLayout->addWidget(customLabel);

        customBetEdit = new QLineEdit();
        customBetEdit->setPlaceholderText("Введите сумму");
        customBetEdit->setFixedWidth(100);
        customBetEdit->setValidator(new QIntValidator(1, 99999, this));
        customBetEdit->setStyleSheet(
            "QLineEdit {"
            "background: white; "
            "border: 2px solid #b8860b; "
            "border-radius: 5px; "
            "padding: 5px; "
            "font-size: 14px; "
            "color: #1a1a1a; "
            "}"
            "QLineEdit:focus {"
            "border: 2px solid #ffd700; "
            "}"
            );
        betLayout->addWidget(customBetEdit);

        QPushButton* setBetButton = new QPushButton("Установить");
        setBetButton->setFixedWidth(100);
        setBetButton->setStyleSheet(
            "QPushButton {"
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "stop:0 #4CAF50, stop:1 #388E3C);"
            "border: 2px solid #2E7D32;"
            "border-radius: 5px;"
            "padding: 5px 10px;"
            "font-size: 14px;"
            "font-weight: bold;"
            "color: white;"
            "}"
            "QPushButton:hover {"
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "stop:0 #66BB6A, stop:1 #43A047);"
            "}"
            "QPushButton:pressed {"
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "stop:0 #388E3C, stop:1 #2E7D32);"
            "}"
            );
        betLayout->addWidget(setBetButton);

        connect(setBetButton, &QPushButton::clicked, this, &MainWindow::onCustomBetButtonClicked);
        connect(customBetEdit, &QLineEdit::returnPressed, this, &MainWindow::onCustomBetButtonClicked);
    }

    // Подключаем сигналы кнопок
    connect(ui->dealButton, &QPushButton::clicked, this, &MainWindow::onDealButtonClicked);
    connect(ui->hitButton, &QPushButton::clicked, this, &MainWindow::onHitButtonClicked);
    connect(ui->standButton, &QPushButton::clicked, this, &MainWindow::onStandButtonClicked);
    connect(ui->doubleButton, &QPushButton::clicked, this, &MainWindow::onDoubleButtonClicked);
    connect(ui->splitButton, &QPushButton::clicked, this, &MainWindow::onSplitButtonClicked);
    connect(ui->newGameButton, &QPushButton::clicked, this, &MainWindow::onNewGameButtonClicked);

    connect(ui->bet10Button, &QPushButton::clicked, this, &MainWindow::onBetButtonClicked);
    connect(ui->bet25Button, &QPushButton::clicked, this, &MainWindow::onBetButtonClicked);
    connect(ui->bet50Button, &QPushButton::clicked, this, &MainWindow::onBetButtonClicked);
    connect(ui->bet100Button, &QPushButton::clicked, this, &MainWindow::onBetButtonClicked);

    ui->playerLayout->setSpacing(10);
    ui->playerLayout->setContentsMargins(10, 10, 10, 10);

    initializeGame();
    updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeGame() {
    deck.reset();
    deck.shuffle();
    playerHand.clear();
    dealerHand.clear();
    splitHands.clear();
    currentHandIndex = 0;
    isSplitMode = false;
    isSplitPhase = false;
    gameActive = false;
    money = 1000;
    currentBet = 10;

    clearLayout(ui->playerLayout);
    clearLayout(ui->dealerLayout);

    if (customBetEdit) {
        customBetEdit->clear();
        customBetEdit->setEnabled(true);
    }

    ui->statusLabel->setText("Сделайте ставку");
    ui->statusLabel->setStyleSheet("color: #ffd700; background: rgba(0,0,0,0.5); border-radius: 10px; padding: 10px;");
    ui->dealerScoreLabel->setText("Очки дилера: 0");
    ui->playerScoreLabel->setText("Очки игрока: 0");

    updateMoneyDisplay();
    updateUI();
}

void MainWindow::onCustomBetButtonClicked() {
    if (gameActive) {
        showMessage("Нельзя менять ставку во время игры!");
        return;
    }

    if (!customBetEdit) return;

    QString text = customBetEdit->text();
    if (text.isEmpty()) {
        showMessage("Введите сумму ставки!");
        return;
    }

    bool ok;
    int bet = text.toInt(&ok);
    if (!ok || bet <= 0) {
        showMessage("Введите корректную сумму!");
        return;
    }

    if (bet > money) {
        showMessage("Недостаточно денег! Баланс: " + QString::number(money));
        return;
    }

    currentBet = bet;
    ui->betLabel->setText(QString("💰 Ставка: %1").arg(currentBet));
    ui->dealButton->setEnabled(true);
    showMessage("Ставка установлена: " + QString::number(currentBet));
    customBetEdit->clear();
    updateUI();
}

void MainWindow::displaySplitHands() {
    clearLayout(ui->playerLayout);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->setSpacing(40);
    mainLayout->setContentsMargins(20, 10, 20, 10);

    for (int i = 0; i < splitHands.size(); ++i) {
        QVBoxLayout* handLayout = new QVBoxLayout();
        handLayout->setSpacing(5);
        handLayout->setAlignment(Qt::AlignCenter);

        QFrame* handFrame = new QFrame();
        if (i == currentHandIndex && splitHands[i].isActive && isSplitPhase) {
            handFrame->setStyleSheet("QFrame { background: rgba(0, 255, 0, 0.1); border: 2px solid #00ff00; border-radius: 10px; padding: 10px; }");
        } else if (!splitHands[i].isActive) {
            if (splitHands[i].isBust()) {
                handFrame->setStyleSheet("QFrame { background: rgba(255, 0, 0, 0.1); border: 2px solid #ff4444; border-radius: 10px; padding: 10px; }");
            } else {
                handFrame->setStyleSheet("QFrame { background: rgba(0, 255, 0, 0.05); border: 2px solid #00aa00; border-radius: 10px; padding: 10px; }");
            }
        } else {
            handFrame->setStyleSheet("QFrame { background: rgba(255, 215, 0, 0.05); border: 2px solid #ffd700; border-radius: 10px; padding: 10px; }");
        }

        QVBoxLayout* frameLayout = new QVBoxLayout();
        frameLayout->setSpacing(5);

        QLabel* titleLabel = new QLabel();
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setFixedHeight(30);

        if (i == currentHandIndex && splitHands[i].isActive && isSplitPhase) {
            titleLabel->setText("🎯 РУКА #" + QString::number(i + 1) + " (ХОД)");
            titleLabel->setStyleSheet("color: #00ff00; font-size: 16px; font-weight: bold;");
        } else if (!splitHands[i].isActive) {
            if (splitHands[i].isBust()) {
                titleLabel->setText("❌ РУКА #" + QString::number(i + 1) + " (ПЕРЕБОР)");
                titleLabel->setStyleSheet("color: #ff4444; font-size: 16px; font-weight: bold;");
            } else {
                titleLabel->setText("✅ РУКА #" + QString::number(i + 1) + " (ГОТОВО)");
                titleLabel->setStyleSheet("color: #00ff00; font-size: 16px; font-weight: bold;");
            }
        } else {
            titleLabel->setText("⏳ РУКА #" + QString::number(i + 1));
            titleLabel->setStyleSheet("color: #ffd700; font-size: 16px; font-weight: bold;");
        }
        frameLayout->addWidget(titleLabel);

        QHBoxLayout* cardsLayout = new QHBoxLayout();
        cardsLayout->setSpacing(5);
        cardsLayout->setAlignment(Qt::AlignCenter);

        for (int j = 0; j < splitHands[i].cards.size(); ++j) {
            Card cardCopy = splitHands[i].cards[j];
            cardCopy.faceUp = true;

            QLabel* cardLabel = new QLabel();
            cardLabel->setFixedSize(80, 120);
            cardLabel->setAlignment(Qt::AlignCenter);
            cardLabel->setScaledContents(true);

            QString imagePath = cardCopy.getImagePath();
            QPixmap pixmap;
            if (pixmap.load(imagePath)) {
                cardLabel->setPixmap(pixmap.scaled(80, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                if (cardCopy.faceUp) {
                    QString text = cardCopy.getRankString() + "\n" + cardCopy.getSuitSymbol();
                    cardLabel->setText(text);
                    QString style = QString(
                                        "background: white; "
                                        "border: 2px solid %1; "
                                        "border-radius: 8px; "
                                        "font-size: 16px; "
                                        "color: %1; "
                                        "font-weight: bold; "
                                        "padding: 2px;"
                                        ).arg(cardCopy.getSuitColor());
                    cardLabel->setStyleSheet(style);
                }
            }

            QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(cardLabel);
            cardLabel->setGraphicsEffect(effect);
            QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
            animation->setDuration(300);
            animation->setStartValue(0);
            animation->setEndValue(1);
            animation->start(QAbstractAnimation::DeleteWhenStopped);

            cardsLayout->addWidget(cardLabel);
        }
        frameLayout->addLayout(cardsLayout);

        QHBoxLayout* infoLayout = new QHBoxLayout();
        infoLayout->setSpacing(20);
        infoLayout->setAlignment(Qt::AlignCenter);

        QLabel* scoreLabel = new QLabel();
        int total = splitHands[i].getTotal();
        if (splitHands[i].isBust()) {
            scoreLabel->setText("💀 " + QString::number(total));
            scoreLabel->setStyleSheet("color: #ff4444; font-size: 18px; font-weight: bold;");
        } else {
            scoreLabel->setText("⭐ " + QString::number(total));
            scoreLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
        }
        infoLayout->addWidget(scoreLabel);

        QLabel* betLabel = new QLabel();
        betLabel->setText("💰 " + QString::number(splitHands[i].bet));
        betLabel->setStyleSheet("color: #ffd700; font-size: 16px; font-weight: bold;");
        infoLayout->addWidget(betLabel);

        frameLayout->addLayout(infoLayout);

        handFrame->setLayout(frameLayout);
        handLayout->addWidget(handFrame);
        mainLayout->addLayout(handLayout);
    }

    ui->playerLayout->addLayout(mainLayout);
}

void MainWindow::dealCards() {
    if (money < currentBet) {
        showMessage("Недостаточно денег для ставки!");
        return;
    }

    checkAndShuffle();
    money -= currentBet;
    updateMoneyDisplay();

    playerHand.clear();
    dealerHand.clear();
    splitHands.clear();
    currentHandIndex = 0;
    isSplitMode = false;
    isSplitPhase = false;

    qDebug() << "=== ПРОВЕРКА РЕСУРСОВ ===";
    qDebug() << "back.png:" << QFile::exists(":/images/back.png");
    qDebug() << "fire_2.png:" << QFile::exists(":/images/fire_2.png");
    qDebug() << "water_10.png:" << QFile::exists(":/images/water_10.png");
    qDebug() << "earth_mag.png:" << QFile::exists(":/images/earth_mag.png");

    clearLayout(ui->playerLayout);
    clearLayout(ui->dealerLayout);

    if (customBetEdit) {
        customBetEdit->setEnabled(false);
    }

    playerHand.addCard(deck.drawCard());
    dealerHand.addCard(deck.drawCard());
    playerHand.addCard(deck.drawCard());
    dealerHand.addCard(deck.drawCard());

    playerHand.bet = currentBet;

    // Отображаем карты игрока
    QHBoxLayout* playerCardsLayout = new QHBoxLayout();
    playerCardsLayout->setSpacing(10);
    playerCardsLayout->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < playerHand.cards.size(); ++i) {
        Card cardCopy = playerHand.cards[i];
        cardCopy.faceUp = true;

        QLabel* label = new QLabel();
        label->setFixedSize(100, 150);
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(true);

        QString imagePath = cardCopy.getImagePath();
        QPixmap pixmap;
        if (pixmap.load(imagePath)) {
            label->setPixmap(pixmap.scaled(100, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            QString text = cardCopy.getRankString() + "\n" + cardCopy.getSuitSymbol();
            label->setText(text);
            QString style = QString(
                                "background: white; "
                                "border: 3px solid %1; "
                                "border-radius: 10px; "
                                "font-size: 20px; "
                                "color: %1; "
                                "font-weight: bold; "
                                "padding: 5px;"
                                ).arg(cardCopy.getSuitColor());
            label->setStyleSheet(style);
        }

        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(label);
        label->setGraphicsEffect(effect);
        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
        animation->setDuration(300);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->start(QAbstractAnimation::DeleteWhenStopped);

        playerCardsLayout->addWidget(label);
    }
    ui->playerLayout->addLayout(playerCardsLayout);

    // Отображаем карты дилера
    QHBoxLayout* dealerCardsLayout = new QHBoxLayout();
    dealerCardsLayout->setSpacing(10);
    dealerCardsLayout->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < dealerHand.cards.size(); ++i) {
        Card cardCopy = dealerHand.cards[i];
        cardCopy.faceUp = (i == 0);

        QLabel* label = new QLabel();
        label->setFixedSize(100, 150);
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(true);

        QString imagePath = cardCopy.getImagePath();
        QPixmap pixmap;
        if (pixmap.load(imagePath)) {
            label->setPixmap(pixmap.scaled(100, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            if (cardCopy.faceUp) {
                QString text = cardCopy.getRankString() + "\n" + cardCopy.getSuitSymbol();
                label->setText(text);
                QString style = QString(
                                    "background: white; "
                                    "border: 3px solid %1; "
                                    "border-radius: 10px; "
                                    "font-size: 20px; "
                                    "color: %1; "
                                    "font-weight: bold; "
                                    "padding: 5px;"
                                    ).arg(cardCopy.getSuitColor());
                label->setStyleSheet(style);
            } else {
                label->setText("🂠");
                label->setStyleSheet(
                    "background: #2a2a2a; "
                    "border: 3px solid #444; "
                    "border-radius: 10px; "
                    "font-size: 48px;"
                    );
            }
        }

        dealerCardsLayout->addWidget(label);
    }
    ui->dealerLayout->addLayout(dealerCardsLayout);

    ui->playerScoreLabel->setText(QString("Очки игрока: %1").arg(playerHand.getTotal()));
    ui->dealerScoreLabel->setText("Очки дилера: ?");

    gameActive = true;
    ui->statusLabel->setText("Игра началась!");
    ui->statusLabel->setStyleSheet("color: #00ff00; background: rgba(0,0,0,0.5); border-radius: 10px; padding: 10px;");

    if (playerHand.isBlackjack() && dealerHand.isBlackjack()) {
        showMessage("Ничья! У обоих Blackjack!");
        money += playerHand.bet;
        gameActive = false;
        if (customBetEdit) customBetEdit->setEnabled(true);
        updateUI();
        return;
    } else if (playerHand.isBlackjack()) {
        showMessage("Blackjack! Вы выиграли!");
        money += playerHand.bet * 2.5;
        gameActive = false;
        if (customBetEdit) customBetEdit->setEnabled(true);
        updateUI();
        return;
    } else if (dealerHand.isBlackjack()) {
        showMessage("У дилера Blackjack! Вы проиграли!");
        gameActive = false;
        if (customBetEdit) customBetEdit->setEnabled(true);
        updateUI();
        return;
    }

    updateUI();
}

void MainWindow::onDealButtonClicked() {
    dealCards();
}

void MainWindow::onHitButtonClicked() {
    if (!gameActive) {
        showMessage("Игра не активна! Нажмите 'Раздать'");
        return;
    }

    if (isSplitMode && isSplitPhase) {
        if (currentHandIndex < splitHands.size()) {
            Hand& hand = splitHands[currentHandIndex];
            if (!hand.isActive) return;

            hand.addCard(deck.drawCard());
            displaySplitHands();
            ui->playerScoreLabel->setText(QString("Очки руки #%1: %2").arg(currentHandIndex + 1).arg(hand.getTotal()));

            if (hand.isBust()) {
                hand.isActive = false;
                showMessage("Рука #" + QString::number(currentHandIndex + 1) + " - ПЕРЕБОР!");
                currentHandIndex++;
                if (currentHandIndex >= splitHands.size()) {
                    isSplitPhase = false;
                    dealerPlay();
                } else {
                    ui->statusLabel->setText("Собираем РУКУ #" + QString::number(currentHandIndex + 1));
                    displaySplitHands();
                    ui->playerScoreLabel->setText(QString("Очки руки #%1: %2").arg(currentHandIndex + 1).arg(splitHands[currentHandIndex].getTotal()));
                }
            } else if (hand.getTotal() == 21) {
                hand.isActive = false;
                showMessage("Рука #" + QString::number(currentHandIndex + 1) + " - 21!");
                currentHandIndex++;
                if (currentHandIndex >= splitHands.size()) {
                    isSplitPhase = false;
                    dealerPlay();
                } else {
                    ui->statusLabel->setText("Собираем РУКУ #" + QString::number(currentHandIndex + 1));
                    displaySplitHands();
                    ui->playerScoreLabel->setText(QString("Очки руки #%1: %2").arg(currentHandIndex + 1).arg(splitHands[currentHandIndex].getTotal()));
                }
            }
            updateUI();
        }
    } else if (isSplitMode && !isSplitPhase) {
        showMessage("Все руки собраны!");
    } else {
        playerHand.addCard(deck.drawCard());

        clearLayout(ui->playerLayout);
        QHBoxLayout* playerCardsLayout = new QHBoxLayout();
        playerCardsLayout->setSpacing(10);
        playerCardsLayout->setAlignment(Qt::AlignCenter);

        for (int i = 0; i < playerHand.cards.size(); ++i) {
            Card cardCopy = playerHand.cards[i];
            cardCopy.faceUp = true;

            QLabel* label = new QLabel();
            label->setFixedSize(100, 150);
            label->setAlignment(Qt::AlignCenter);
            label->setScaledContents(true);

            QString imagePath = cardCopy.getImagePath();
            QPixmap pixmap;
            if (pixmap.load(imagePath)) {
                label->setPixmap(pixmap.scaled(100, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                QString text = cardCopy.getRankString() + "\n" + cardCopy.getSuitSymbol();
                label->setText(text);
                QString style = QString(
                                    "background: white; "
                                    "border: 3px solid %1; "
                                    "border-radius: 10px; "
                                    "font-size: 20px; "
                                    "color: %1; "
                                    "font-weight: bold; "
                                    "padding: 5px;"
                                    ).arg(cardCopy.getSuitColor());
                label->setStyleSheet(style);
            }

            QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(label);
            label->setGraphicsEffect(effect);
            QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
            animation->setDuration(300);
            animation->setStartValue(0);
            animation->setEndValue(1);
            animation->start(QAbstractAnimation::DeleteWhenStopped);

            playerCardsLayout->addWidget(label);
        }
        ui->playerLayout->addLayout(playerCardsLayout);

        ui->playerScoreLabel->setText(QString("Очки игрока: %1").arg(playerHand.getTotal()));

        if (playerHand.isBust()) {
            showMessage("Перебор! Вы проиграли!");
            gameActive = false;
            if (customBetEdit) customBetEdit->setEnabled(true);
            updateUI();
        } else if (playerHand.getTotal() == 21) {
            ui->statusLabel->setText("21! Ход дилера...");
            QTimer::singleShot(1000, this, &MainWindow::dealerPlay);
        }
        updateUI();
    }
}

void MainWindow::onStandButtonClicked() {
    if (!gameActive) {
        showMessage("Игра не активна!");
        return;
    }

    if (isSplitMode && isSplitPhase) {
        if (currentHandIndex < splitHands.size()) {
            splitHands[currentHandIndex].isActive = false;
            showMessage("Рука #" + QString::number(currentHandIndex + 1) + " - Стоп!");
            currentHandIndex++;
            if (currentHandIndex >= splitHands.size()) {
                isSplitPhase = false;
                dealerPlay();
            } else {
                ui->statusLabel->setText("Собираем РУКУ #" + QString::number(currentHandIndex + 1));
                displaySplitHands();
                ui->playerScoreLabel->setText(QString("Очки руки #%1: %2").arg(currentHandIndex + 1).arg(splitHands[currentHandIndex].getTotal()));
            }
            updateUI();
        }
    } else if (isSplitMode && !isSplitPhase) {
        dealerPlay();
    } else {
        ui->statusLabel->setText("Ход дилера...");
        QTimer::singleShot(1000, this, &MainWindow::dealerPlay);
        updateUI();
    }
}

void MainWindow::onDoubleButtonClicked() {
    if (!gameActive || isSplitMode) {
        showMessage("Недоступно при Split!");
        return;
    }

    if (playerHand.cards.size() != 2) {
        showMessage("Удвоить можно только на первых двух картах!");
        return;
    }

    if (money < playerHand.bet) {
        showMessage("Недостаточно денег для удвоения!");
        return;
    }

    money -= playerHand.bet;
    playerHand.bet *= 2;
    playerHand.isDoubled = true;
    updateMoneyDisplay();

    showMessage("Ставка удвоена! Новая ставка: " + QString::number(playerHand.bet));

    playerHand.addCard(deck.drawCard());

    clearLayout(ui->playerLayout);
    QHBoxLayout* playerCardsLayout = new QHBoxLayout();
    playerCardsLayout->setSpacing(10);
    playerCardsLayout->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < playerHand.cards.size(); ++i) {
        Card cardCopy = playerHand.cards[i];
        cardCopy.faceUp = true;

        QLabel* label = new QLabel();
        label->setFixedSize(100, 150);
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(true);

        QString imagePath = cardCopy.getImagePath();
        QPixmap pixmap;
        if (pixmap.load(imagePath)) {
            label->setPixmap(pixmap.scaled(100, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            QString text = cardCopy.getRankString() + "\n" + cardCopy.getSuitSymbol();
            label->setText(text);
            QString style = QString(
                                "background: white; "
                                "border: 3px solid %1; "
                                "border-radius: 10px; "
                                "font-size: 20px; "
                                "color: %1; "
                                "font-weight: bold; "
                                "padding: 5px;"
                                ).arg(cardCopy.getSuitColor());
            label->setStyleSheet(style);
        }

        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(label);
        label->setGraphicsEffect(effect);
        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
        animation->setDuration(300);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->start(QAbstractAnimation::DeleteWhenStopped);

        playerCardsLayout->addWidget(label);
    }
    ui->playerLayout->addLayout(playerCardsLayout);

    ui->playerScoreLabel->setText(QString("Очки игрока: %1").arg(playerHand.getTotal()));

    if (playerHand.isBust()) {
        showMessage("Перебор! Вы проиграли!");
        gameActive = false;
        if (customBetEdit) customBetEdit->setEnabled(true);
        updateUI();
    } else {
        ui->statusLabel->setText("Удвоение! Ход дилера...");
        QTimer::singleShot(1000, this, &MainWindow::dealerPlay);
    }
    updateUI();
}

void MainWindow::onSplitButtonClicked() {
    if (!gameActive || isSplitMode) {
        showMessage("Недоступно!");
        return;
    }
    if (!playerHand.canSplit()) {
        showMessage("Нельзя разделить эти карты!");
        return;
    }
    if (money < playerHand.bet) {
        showMessage("Недостаточно денег для Split!");
        return;
    }

    isSplitMode = true;
    isSplitPhase = true;
    money -= playerHand.bet;
    updateMoneyDisplay();

    Hand hand1(playerHand.bet);
    Hand hand2(playerHand.bet);

    hand1.addCard(playerHand.cards[0]);
    hand2.addCard(playerHand.cards[1]);

    hand1.addCard(deck.drawCard());
    hand2.addCard(deck.drawCard());

    splitHands.clear();
    splitHands.append(hand1);
    splitHands.append(hand2);
    currentHandIndex = 0;

    displaySplitHands();
    ui->playerScoreLabel->setText(QString("Очки руки #1: %1").arg(splitHands[0].getTotal()));
    ui->statusLabel->setText("Собираем РУКУ #1");
    ui->statusLabel->setStyleSheet("color: #00ff00; background: rgba(0,0,0,0.5); border-radius: 10px; padding: 10px;");

    ui->splitButton->setEnabled(false);

    updateUI();
}

void MainWindow::dealerPlay() {
    if (!gameActive) return;

    // Открываем все карты дилера
    clearLayout(ui->dealerLayout);

    QHBoxLayout* dealerCardsLayout = new QHBoxLayout();
    dealerCardsLayout->setSpacing(10);
    dealerCardsLayout->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < dealerHand.cards.size(); ++i) {
        Card cardCopy = dealerHand.cards[i];
        cardCopy.faceUp = true;

        QLabel* label = new QLabel();
        label->setFixedSize(100, 150);
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(true);

        QString imagePath = cardCopy.getImagePath();
        QPixmap pixmap;
        if (pixmap.load(imagePath)) {
            label->setPixmap(pixmap.scaled(100, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            QString text = cardCopy.getRankString() + "\n" + cardCopy.getSuitSymbol();
            label->setText(text);
            QString style = QString(
                                "background: white; "
                                "border: 3px solid %1; "
                                "border-radius: 10px; "
                                "font-size: 20px; "
                                "color: %1; "
                                "font-weight: bold; "
                                "padding: 5px;"
                                ).arg(cardCopy.getSuitColor());
            label->setStyleSheet(style);
        }

        dealerCardsLayout->addWidget(label);
    }
    ui->dealerLayout->addLayout(dealerCardsLayout);
    ui->dealerScoreLabel->setText(QString("Очки дилера: %1").arg(dealerHand.getTotal()));

    // Дилер берет карты до 17
    while (dealerHand.getTotal() < 17) {
        dealerHand.addCard(deck.drawCard());

        clearLayout(ui->dealerLayout);

        QHBoxLayout* newDealerLayout = new QHBoxLayout();
        newDealerLayout->setSpacing(10);
        newDealerLayout->setAlignment(Qt::AlignCenter);

        for (int i = 0; i < dealerHand.cards.size(); ++i) {
            Card cardCopy = dealerHand.cards[i];
            cardCopy.faceUp = true;

            QLabel* label = new QLabel();
            label->setFixedSize(100, 150);
            label->setAlignment(Qt::AlignCenter);
            label->setScaledContents(true);

            QString imagePath = cardCopy.getImagePath();
            QPixmap pixmap;
            if (pixmap.load(imagePath)) {
                label->setPixmap(pixmap.scaled(100, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                QString text = cardCopy.getRankString() + "\n" + cardCopy.getSuitSymbol();
                label->setText(text);
                QString style = QString(
                                    "background: white; "
                                    "border: 3px solid %1; "
                                    "border-radius: 10px; "
                                    "font-size: 20px; "
                                    "color: %1; "
                                    "font-weight: bold; "
                                    "padding: 5px;"
                                    ).arg(cardCopy.getSuitColor());
                label->setStyleSheet(style);
            }

            newDealerLayout->addWidget(label);
        }
        ui->dealerLayout->addLayout(newDealerLayout);
        ui->dealerScoreLabel->setText(QString("Очки дилера: %1").arg(dealerHand.getTotal()));

        QCoreApplication::processEvents();
        QTimer::singleShot(500, this, [](){});
    }

    // Оценка результатов
    if (isSplitMode) {
        if (dealerHand.isBust()) {
            showMessage("Дилер перебрал! Все руки выиграли!");
            for (const Hand& hand : splitHands) {
                money += hand.bet * 2;
            }
        } else {
            for (int i = 0; i < splitHands.size(); ++i) {
                const Hand& hand = splitHands[i];
                if (hand.isBust()) {
                    showMessage("Рука #" + QString::number(i + 1) + " - проиграла (перебор)");
                } else if (hand.getTotal() > dealerHand.getTotal()) {
                    money += hand.bet * 2;
                    showMessage("Рука #" + QString::number(i + 1) + " - ВЫИГРАЛА! (+" + QString::number(hand.bet * 2) + ")");
                } else if (hand.getTotal() == dealerHand.getTotal()) {
                    money += hand.bet;
                    showMessage("Рука #" + QString::number(i + 1) + " - ничья (возврат ставки)");
                } else {
                    showMessage("Рука #" + QString::number(i + 1) + " - проиграла");
                }
            }
        }
    } else {
        if (dealerHand.isBust()) {
            showMessage("Дилер перебрал! Вы выиграли!");
            money += playerHand.bet * 2;
        } else {
            if (playerHand.isBust()) {
                showMessage("Перебор! Вы проиграли!");
            } else if (playerHand.getTotal() > dealerHand.getTotal()) {
                showMessage("Вы выиграли!");
                money += playerHand.bet * 2;
            } else if (playerHand.getTotal() == dealerHand.getTotal()) {
                showMessage("Ничья!");
                money += playerHand.bet;
            } else {
                showMessage("Дилер выиграл!");
            }
        }
    }

    gameActive = false;
    isSplitMode = false;
    isSplitPhase = false;
    if (customBetEdit) customBetEdit->setEnabled(true);
    updateUI();
}

void MainWindow::onNewGameButtonClicked() {
    initializeGame();
    ui->statusLabel->setText("Новая игра!");
    updateUI();
}

void MainWindow::onBetButtonClicked() {
    if (gameActive) return;

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString text = button->text();
    QString numberStr;
    for (QChar ch : text) {
        if (ch.isDigit()) {
            numberStr += ch;
        }
    }
    int bet = numberStr.toInt();
    if (bet > 0 && money >= bet) {
        currentBet = bet;
        ui->betLabel->setText(QString("💰 Ставка: %1").arg(currentBet));
        ui->dealButton->setEnabled(true);
        if (customBetEdit) customBetEdit->clear();
        showMessage("Ставка установлена: " + QString::number(currentBet));
    } else {
        showMessage("Недостаточно денег для такой ставки!");
    }
    updateUI();
}

void MainWindow::updateUI() {
    bool canSplit = gameActive && !isSplitMode && playerHand.canSplit() && money >= playerHand.bet;

    ui->dealButton->setEnabled(!gameActive && money >= currentBet);
    ui->hitButton->setEnabled(gameActive);
    ui->standButton->setEnabled(gameActive);
    ui->doubleButton->setEnabled(gameActive && !isSplitMode && playerHand.cards.size() == 2 && money >= playerHand.bet);
    ui->splitButton->setEnabled(canSplit);

    ui->bet10Button->setEnabled(!gameActive && money >= 10);
    ui->bet25Button->setEnabled(!gameActive && money >= 25);
    ui->bet50Button->setEnabled(!gameActive && money >= 50);
    ui->bet100Button->setEnabled(!gameActive && money >= 100);

    if (customBetEdit) {
        customBetEdit->setEnabled(!gameActive);
    }

    updateMoneyDisplay();
}

void MainWindow::updateMoneyDisplay() {
    ui->moneyLabel->setText(QString("💰 Баланс: %1").arg(money));
    if (money < 10) {
        ui->moneyLabel->setStyleSheet("color: #ff4444; font-size: 16px; font-weight: bold;");
    } else {
        ui->moneyLabel->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    }
}

void MainWindow::showMessage(const QString& msg) {
    ui->statusLabel->setText(msg);
    if (msg.contains("выиграли") || msg.contains("Blackjack") || msg.contains("ВЫИГРАЛА")) {
        ui->statusLabel->setStyleSheet("color: #00ff00; background: rgba(0,0,0,0.5); border-radius: 10px; padding: 10px; font-size: 18px;");
    } else if (msg.contains("проиграли") || msg.contains("Перебор") || msg.contains("ПЕРЕБОР")) {
        ui->statusLabel->setStyleSheet("color: #ff4444; background: rgba(0,0,0,0.5); border-radius: 10px; padding: 10px; font-size: 18px;");
    } else if (msg.contains("ничья") || msg.contains("Ничья")) {
        ui->statusLabel->setStyleSheet("color: #ffd700; background: rgba(0,0,0,0.5); border-radius: 10px; padding: 10px; font-size: 18px;");
    } else {
        ui->statusLabel->setStyleSheet("color: #ffd700; background: rgba(0,0,0,0.5); border-radius: 10px; padding: 10px; font-size: 18px;");
    }
}

void MainWindow::clearLayout(QLayout* layout) {
    if (!layout) return;
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (item->widget()) {
            delete item->widget();
        }
        if (item->layout()) {
            clearLayout(item->layout());
        }
        delete item;
    }
}

void MainWindow::checkAndShuffle() {
    if (deck.needShuffle()) {
        deck.reset();
        deck.shuffle();
        showMessage("Колода перетасована!");
    }
}
