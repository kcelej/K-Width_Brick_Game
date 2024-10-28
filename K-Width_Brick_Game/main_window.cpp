#include "main_window.h"

//Avoiding magic numbers, in future move to Defines.h file
#define direction_down 0
#define direction_left 1
#define direction_right 2

#include <QtWidgets>

#include <QTimer>
#include <QCoreApplication>
#include <QDebug>

#include <QKeyEvent>

#include <QPixMap>
#include <QGraphicsPixmapItem>
#include <QTimer>

#include "brick.h"
#include "board.h"

//only for tests
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
        brick* player = new brick(random(0, 6), 0, gameBoard);
        //brick* player = new brick(random(0, 6), random(0, 6), gameBoard);
        //brick* player = new brick(random(0, 6), z++, gameBoard);
   
        // example movement downwards for testing
        int i = 0;
        while (true) {
            if (player->collision(direction_down)) {
                player->draw();
                break;
            }
           if (i == 3) {
               player->set_rotation(1);
           }
           if (i == 8) {
                player->set_rotation(2);
           }
           if (i == 13) {
              player->set_rotation(3);
           }
           if (i == 18) {
               //player->set_rotation(0);
           }
           player->reset_entire_brick();
           for (int j = 0; j < 8; j++) {
              player->coordinates[j].i++; // lowers the brick by one tile
           }
          ++i;
          player->draw();    // changes the game board tiles to the brick shape
         delayOneSecond();
        }   
        
    brick* player1 = new brick(random(0, 6), 1, gameBoard);
   // brick* player1 = new brick(random(0, 6), random(0, 6), gameBoard);
   
    // example movement downwards for testing
        i = 0;
    while (true) {
        if (player1->collision(direction_down)) break;
        if (i == 3) {
            player1->set_rotation(1);
        }
        if (i == 8) {
            player1->set_rotation(2);
        }
        if (i == 13) {
            // player1->set_rotation(3);
        }
        if (i == 18) {
            //player1->set_rotation(0);
        }
        player1->reset_entire_brick();
        for (int j = 0; j < 8; j++) {
            player1->coordinates[j].i++; // lowers the brick by one tile
        }
        ++i;
        player1->draw();    // changes the game board tiles to the brick shape
        //delayOneSecond();
    }
    

    }
    catch (...) {
        QMessageBox::information(this, "Error", "Error during game test");
    }
}