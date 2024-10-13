#include "main_window.h"
#include <QtWidgets>

#include <QPixMap>
#include <QGraphicsPixmapItem>

#include <brick.h>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

main_window::~main_window()
{
}

// GAME WINDOW DATA:
// Game field dimensions (in tiles) Width x Height: 10x20
// Game tile size (with borders): 18x18


const int game_window_width = 320;
const int game_window_height = 480;
const int tile_size = 18;

void main_window::on_testGameButton_clicked() {
    try {
        QGraphicsScene* game_scene = new QGraphicsScene();
        QGraphicsView* game_view = new QGraphicsView(game_scene);
        game_view->setWindowTitle("Brick Game");

        QPixmap background_image_pixmap(":/resources/textures/background.png");
        QGraphicsPixmapItem* background_image_item = new QGraphicsPixmapItem();
        background_image_item->setPixmap(background_image_pixmap);
        background_image_item->setScale(1);
        
        game_scene->addItem(background_image_item);

        brick *brick_shape = new brick(tile_size, 3);

        game_scene->addItem(brick_shape);

        game_view->show();
        game_view->setFixedSize(game_window_width, game_window_height);
        game_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        game_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    }
    catch (...) {
        QMessageBox::information(this, "Error", "Error during game test");
    }
}