#include "Tetris_Game.h"

/*
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

    add_new_brick();
};

Tetris_Game::~Tetris_Game() {
    if (gameScene) {
        delete gameScene;
        qDebug() << "Deleted gameScene";
    }
    if (gameView) {
        delete gameView;
        qDebug() << "Deleted gameView";
    }
    if (backgroundImageItem) {
        delete backgroundImageItem;
        qDebug() << "Deleted backgroundImageItem";
    }
    if (gameBoard) {
        delete gameBoard;
        qDebug() << "Deleted gameBoard";
    }
    if (player) {
        delete player;
        qDebug() << "Deleted player";
    }
}

void Tetris_Game::Game() {

    while (true) {
        //movement
        if (player) {
            QApplication::processEvents();
        }
        
    }

}

void Tetris_Game::keyPressEvent(QKeyEvent* event){
    if (player) {
        qDebug() << "Brick accepted input";
        player->movement(event);
    }
    else qDebug() << "No brick to make movement";
}

void Tetris_Game::end_of_life_of_brick() {
    //delete
    if (player) {
        delete(player);
        qDebug() << "Brick deleted";
    }
    else qDebug() << "No Brick to delete";
}

void Tetris_Game::add_new_brick() {
    //add new brick
    if (!player) {
        player = new brick(random(0, 6), random(0, 6), gameBoard);
        qDebug() << "Added new brick";
    }
    else qDebug() << "Cannot add a new brick";
}
*/