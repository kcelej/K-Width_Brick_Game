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

        brick* player = nullptr;
        int how_many = 1;
        while (how_many <= 6) { //for now spawn only 6 blocks
            // example brick for testing
            player = new brick(z++, 2, gameBoard);
           // player = new brick(random(0, 6), random(0, 6), gameBoard);
            //player = new brick(random(0, 6), z++, gameBoard);

            /*
            0 ->
            1 <-
            2 /\
            3 \/
            */

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