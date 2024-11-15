#include <QtWidgets>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>

#include "board.h"
#include "brick.h"

/*
class Tetris_Game : QWidget {
public:
    Tetris_Game();
    ~Tetris_Game();
    void Game();

private:
    QGraphicsScene* gameScene = nullptr;
    QGraphicsView* gameView = nullptr;
    QGraphicsPixmapItem* backgroundImageItem = nullptr;
    board* gameBoard = nullptr;
    brick* player = nullptr;

    void end_of_life_of_brick();
    void add_new_brick();

protected:
    void keyPressEvent(QKeyEvent* key_event) override;
};
*/