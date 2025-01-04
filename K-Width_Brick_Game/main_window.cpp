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

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //do testow. pozniej usunac
    g1.setUserId(1);
    g2.setUserId(2);
    g3.setUserId(3);
    g4.setUserId(4);

    g1.configNetworkNeighbors(NetworkUtils::getLocalHost(), 15551, 15550);
    g2.configNetworkNeighbors(NetworkUtils::getLocalHost(), 15552, 15551);
    g3.configNetworkNeighbors(NetworkUtils::getLocalHost(), 15553, 15552);
    g4.configNetworkNeighbors(NetworkUtils::getLocalHost(), 15550, 15553);

    g1.startListening();
    g2.startListening();
    g3.startListening();
    g4.startListening();
    QThread::msleep(1000);
    g1.connectToNext();
    g2.connectToNext();
    g3.connectToNext();
    g4.connectToNext();

    g1.startGame();
    g2.startGame();
    g3.startGame();
    g4.startGame();

    QThread::msleep(1000);
}

main_window::~main_window()
{
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
    Tetris_Game Tetris;
    Tetris.Game();

   //idk
   return;
}