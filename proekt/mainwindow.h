#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLayout>
#include <QLabel>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include "carddeck.h"
#include "hand.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDealButtonClicked();
    void onHitButtonClicked();
    void onStandButtonClicked();
    void onDoubleButtonClicked();
    void onSplitButtonClicked();
    void onNewGameButtonClicked();
    void onBetButtonClicked();
    void onCustomBetButtonClicked();
    void updateUI();

private:
    Ui::MainWindow *ui;
    CardDeck deck;
    Hand playerHand;
    Hand dealerHand;
    QVector<Hand> splitHands;
    int currentHandIndex;
    int money;
    int currentBet;
    bool gameActive;
    bool isSplitMode;
    bool isSplitPhase;
    QTimer* animationTimer;
    QLineEdit* customBetEdit;

    void initializeGame();
    void dealCards();
    void dealerPlay();
    void displaySplitHands();
    void clearLayout(QLayout* layout);
    void checkAndShuffle();
    void updateMoneyDisplay();
    void showMessage(const QString& msg);
};

#endif // MAINWINDOW_H
