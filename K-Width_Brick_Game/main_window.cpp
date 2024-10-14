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
        game_view->setFocusPolicy(Qt::StrongFocus);  // Allow key events to go to the items in the scene.
        this->setFocusPolicy(Qt::NoFocus);            // Optionally, disable focus on the main window to avoid conflict.

        QPixmap background_image_pixmap(":/resources/textures/background.png");
        QGraphicsPixmapItem* background_image_item = new QGraphicsPixmapItem();
        background_image_item->setPixmap(background_image_pixmap);
        background_image_item->setScale(1);
        background_image_item->setFlag(QGraphicsItem::ItemIsFocusable, false);  // Background shouldn't take focus.

        
        game_scene->addItem(background_image_item);

        brick *brick_shape = new brick(tile_size, 3);
        game_scene->addItem(brick_shape);
        game_view->setFocus();            // Set focus to the view itself.


        game_scene->addItem(brick_shape);
        game_scene->setFocusItem(brick_shape);  // Explicitly set the brick as the focused item in the scene.
        brick_shape->setFocus();    // Then set focus to the brick after adding it to the scene.

        game_view->show();
        game_view->setFixedSize(game_window_width, game_window_height);
        game_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        game_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    }
    catch (...) {
        QMessageBox::information(this, "Error", "Error during game test");
    }
}