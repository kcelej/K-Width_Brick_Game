#include "Tetris_Game.h"

Tetris_Game::Tetris_Game() {
    gameScene = new QGraphicsScene();
    gameView = new QGraphicsView(gameScene);
    gameView->setWindowTitle("Brick Game");

    QPixmap backgroundImagePixmap(":/resources/textures/background.png");
    backgroundImageItem = new QGraphicsPixmapItem();
    backgroundImageItem->setPixmap(backgroundImagePixmap);
    backgroundImageItem->setScale(1);
    backgroundImageItem->setFlag(QGraphicsItem::ItemIsFocusable, false);  // background hopefully shouldn't take focus

    gameScene->addItem(backgroundImageItem);
    gameBoard = new board(gameScene, gameView);

    gameView->show();
    gameView->setFixedSize(gameWindowWidth, gameWindowHeight);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    player = nullptr;
};

Tetris_Game::~Tetris_Game() {
    if (gameScene) {
        delete(gameScene);
        QGraphicsScene* gameScene = nullptr;
    }
    if (gameView) {
        delete(gameView);
        QGraphicsView* gameView = nullptr;
    }
    if (backgroundImageItem) {
        delete(backgroundImageItem);
        QGraphicsPixmapItem* backgroundImageItem = nullptr;
    }
    if (gameBoard) {
        delete(gameBoard);
        board* gameBoard = nullptr;
    }
    if (player) {
        delete(player);
        brick* player = nullptr;
    }
}

void Tetris_Game::Events() {

}

void Tetris_Game::Update() {

}

void Tetris_Game::Render() {

}

bool Tetris_Game::Game() {

    return true;
}