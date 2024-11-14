#include <QtWidgets>
#include <QTimer>
#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>

#include "board.h"
#include "brick.h"

class Tetris_Game {
public:
    Tetris_Game();

    ~Tetris_Game();

    bool Game();
private:
    QGraphicsScene* gameScene = nullptr;
    QGraphicsView* gameView = nullptr;
    QGraphicsPixmapItem* backgroundImageItem = nullptr;
    board* gameBoard = nullptr;
    brick* player = nullptr;

    void Events();
    void Update();
    void Render();
};

//Maybe a new class that is only responsible for the player's input? This class would be an element of the Tetris_Game class