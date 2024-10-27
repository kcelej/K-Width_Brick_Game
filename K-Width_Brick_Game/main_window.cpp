#include "main_window.h"
#include <QtWidgets>

#include <QTimer>
#include <QCoreApplication>
#include <QDebug>

#include <QPixMap>
#include <QGraphicsPixmapItem>
#include <QTimer>

#include "brick.h"
#include "board.h"


using namespace std;
int z = 0;

int brickTestOrder(int var) {
    var++;
    if (var > 7) var = 0;
    return var;
}

void delayOneSecond()
{
    QTimer timer;
    timer.setSingleShot(true);  // Set the timer to only trigger once
    QObject::connect(&timer, &QTimer::timeout, []() {
        qDebug() << "1 second has passed.";
        });

    timer.start(1000);  // Start the timer with a 1-second interval
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Process events until the timer times out
    while (timer.isActive()) {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
}

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

main_window::~main_window()
{
}

int random(int min, int max) // range: [min, max]
{
    static bool first = true;
    if (first)
    {
        srand(time(NULL));
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

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



        // example brick for testing
       //brick* player = new brick(random(0, 6), 3);
        brick* player = new brick(random(0, 6), z++, gameBoard);


        // example position for testing
        player->coordinates[0].i = 0;
        player->coordinates[0].j = 4;
        player->coordinates[1].i = 0;
        player->coordinates[1].j = 5;
        player->coordinates[2].i = 1;
        player->coordinates[2].j = 4;
        player->coordinates[3].i = 1;
        player->coordinates[3].j = 5;
        player->coordinates[4].i = 2;
        player->coordinates[4].j = 4;
        player->coordinates[5].i = 2;
        player->coordinates[5].j = 5;
        player->coordinates[6].i = 3;
        player->coordinates[6].j = 4;
        player->coordinates[7].i = 3;
        player->coordinates[7].j = 5;
        player->draw();


        // example movement downwards for testing
        for (int i = 0; i < 30; i++) {
            if (player->collision(0) != 1) {
                if (i == 3) {
                    player->set_rotation(1);
                }
                if (i == 8) {
                    player->set_rotation(2);
                }
                    for (int j = 0; j < 8; j++) {
                        gameBoard->resetTile(player->coordinates[j].i, player->coordinates[j].j);   // changes the tiles to default texture
                        player->coordinates[j].i++; // lowers the brick by one tile
                    }
                    player->draw();    // changes the game board tiles to the brick shape
                }
           delayOneSecond();
        }
    //    //brick* player1 = new brick(random(0, 6), 5);
    //    brick* player1 = new brick(random(0, 6), random(0, 6), gameBoard);

    //// example position for testing
    //    player1->coordinates[0].i = 0;
    //    player1->coordinates[0].j = 4;
    //    player1->coordinates[1].i = 0;
    //    player1->coordinates[1].j = 5;
    //    player1->coordinates[2].i = 1;
    //    player1->coordinates[2].j = 4;
    //    player1->coordinates[3].i = 1;
    //    player1->coordinates[3].j = 5;
    //    player1->coordinates[4].i = 2;
    //    player1->coordinates[4].j = 4;
    //    player1->coordinates[5].i = 2;
    //    player1->coordinates[5].j = 5;
    //    player1->coordinates[6].i = 3;
    //    player1->coordinates[6].j = 4;
    //    player1->coordinates[7].i = 3;
    //    player1->coordinates[7].j = 5;
    //    player1->draw();

    //    // example movement downwards for testing
    //    for (int i = 0; i < 30; i++) {
    //        if (player1->collision(0) != 1) {
    //            for (int j = 0; j < 8; j++) {
    //                gameBoard->resetTile(player1->coordinates[j].i, player1->coordinates[j].j);   // changes the tiles to default texture
    //                player1->coordinates[j].i++; // lowers the brick by one tile
    //            }
    //            player1->draw();    // changes the game board tiles to the brick shape
    //        }

    //    }

    }
    catch (...) {
        QMessageBox::information(this, "Error", "Error during game test");
    }
}