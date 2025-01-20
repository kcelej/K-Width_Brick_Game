#include "main_window.h"
#include "Tetris_Game.h"

//delete later
#include <QtWidgets>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "brick.h"
#include "board.h"
//
#include <QThread>
#include <QCoreApplication>
#include "NetworkGame.h"
#include "NetworkUtils.h"
#include "iostream"

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

main_window::~main_window()
{
    delete game;
}

//old
/*
void main_window::on_testGameButton_clicked() {
    try {
        QGraphicsScene* gameScene = new QGraphicsScene();
        QGraphicsView* gameView = new QGraphicsView(gameScene);
        gameView->setWindowTitle("Brick Game");

        QPixmap backgroundImagePixmap(":/resources/textures/background.png");
        QGraphicsPixmapItem* backgroundImageItem = new QGraphicsPixmapItem();
        backgroundImageItem->setPixmap(backgroundImagePixmap);
        backgroundImageItem->setScale(1);
        backgroundImageItem->setFlag(QGraphicsItem::ItemIsFocusable, false);  // background hopefully shouldn't take focus

        gameScene->addItem(backgroundImageItem);
        board* gameBoard = new board(gameScene, gameView);

        gameView->show();
        gameView->setFixedSize(gameWindowWidth, gameWindowHeight);
        gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        brick* player = nullptr;
        int how_many = 1;
        while (how_many <= 6) { //for now spawn only 6 blocks
            // example brick for testing
            player = new brick(z++, 2, gameBoard);
           // player = new brick(random(0, 6), random(0, 6), gameBoard);
            //player = new brick(random(0, 6), z++, gameBoard);

            
            //0 ->
            //1 <-
            //2 /\
            //3 \/
            

            // example movement downwards for testing
            int i = 0;
            while (true) {
                if (i == 0) {
                    player->movement(2);
                    delayOneSecond();
                    player->movement(2);
                    delayOneSecond();
                    player->movement(1);
                    delayOneSecond();
                    player->movement(1);
                    delayOneSecond();
                }
                if (i == 1) {
                    
                }
                if (i == 2) {
                    //player->change_rotation();
                }
                if (i == 3) {
                  // player->change_rotation();
                }
                if (!player->move_down()) {
                    how_many++;
                    player->draw();
                    //player = nullptr; // memory leak FIX LATER!!!
                    delete(player);
                    break;
                }

                //if (!(i % 4)) player->move_left();
                //if (!(i % 4)) player->move_right();
                ++i;
                player->draw();
                delayOneSecond();
            }
        }
    }

    catch (...) {
        QMessageBox::information(this, "Error", "Error during game test");
    }
}
*/

//new
void main_window::on_testGameButton_clicked() {

    symulateServerConfiguration();

   //idk
   return;
}

void main_window::symulateServerConfiguration() {
    game = new NetworkGame();
    // Get the listening port
    bool ok = false;
    int listeningPort = QInputDialog::getInt(this, "Server Configuration",
        "Enter listening port:",
        0, 0, 65535, 1, &ok);
    if (!ok) return; // User canceled

    // Get the sending port
    int sendingPort = QInputDialog::getInt(this, "Server Configuration",
        "Enter sending port:",
        0, 0, 65535, 1, &ok);
    if (!ok) return;

    // Get the sending IP
    QString sendingIP = QInputDialog::getText(this, "Server Configuration",
        "Enter sending IP (v4):",
        QLineEdit::Normal, "127.0.0.1", &ok);
    if (!ok) return;

    // Get the user ID
    int userId = QInputDialog::getInt(this, "Server Configuration",
        "Enter user ID:",
        0, 0, 10000, 1, &ok);
    if (!ok) return;

    // Confirmation
    QMessageBox::StandardButton confirm = QMessageBox::question(this,
        "Confirm",
        QString("Listening Port: %1\nSending Port: %2\nSending IP: %3\nUser ID: %4\nDo you confirm?")
        .arg(listeningPort)
        .arg(sendingPort)
        .arg(sendingIP)
        .arg(userId),
        QMessageBox::Yes | QMessageBox::No);

    if (confirm != QMessageBox::Yes) return;

    // Configure the game
    game->setUserId(userId);
    game->configNetworkNeighbors(sendingIP.toStdString().c_str(), sendingPort, listeningPort);
    game->startListening();

    // Delay for simulation
    QThread::msleep(5000);

    // Connect to the next node and start the game
    game->connectToNext();
    hide();
    game->startGame();
    show();
}