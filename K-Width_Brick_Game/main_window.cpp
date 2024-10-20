#include "main_window.h"
#include <QtWidgets>

#include <QPixMap>
#include <QGraphicsPixmapItem>
#include <QTimer>

#include "brick.h"
#include "board.h"

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
        brick* player = new brick(random(0,6), random(0,6));


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
        player->draw(gameBoard);

        // example movement downwards for testing
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 8; j++) {
                gameBoard->resetTile(player->coordinates[j].i, player->coordinates[j].j);   // changes the tiles to default texture
                player->coordinates[j].i++; // lowers the brick by one tile
            }
            player->draw(gameBoard);    // changes the game board tiles to the brick shape
        }

    }
    catch (...) {
        QMessageBox::information(this, "Error", "Error during game test");
    }
}