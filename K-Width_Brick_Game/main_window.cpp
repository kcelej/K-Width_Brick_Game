#include "main_window.h"
#include <QtWidgets>

#include <QPixMap>
#include <QGraphicsPixmapItem>
#include<QTimer>

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

bool checkCollision(brick* brick_shape, QGraphicsScene* scene) {
    // Iterate over each of the brick's four tiles
    for (int i = 0; i < 4; i++) {
        // Map each tile's local coordinates to scene coordinates
        QPointF tileScenePos = brick_shape->mapToScene(
            brick_shape->coordinates[i].x * brick_shape->tile_size,
            brick_shape->coordinates[i].y * brick_shape->tile_size
        );

        // Check if the tile is at or below the bottom boundary (y = 390)
        if (tileScenePos.y() + brick_shape->tile_size >= 390) {
            return true;  // Collision at the bottom of the game field
        }
    }

    // No collision detected
    return false;
}



void main_window::on_testGameButton_clicked() {
    try {
        

        QGraphicsScene* game_scene = new QGraphicsScene();
        QGraphicsView* game_view = new QGraphicsView(game_scene);
        game_view->setWindowTitle("Brick Game");
        game_view->setFocusPolicy(Qt::StrongFocus);  // Allow key events to go to the items in the scene
        this->setFocusPolicy(Qt::NoFocus);            // Disable focus on the main window to possibly avoid conflict

        QPixmap background_image_pixmap(":/resources/textures/background.png");
        QGraphicsPixmapItem* background_image_item = new QGraphicsPixmapItem();
        background_image_item->setPixmap(background_image_pixmap);
        background_image_item->setScale(1);
        background_image_item->setFlag(QGraphicsItem::ItemIsFocusable, false);  // Background shouldn't take focus, please Qt, I beg you

        
        game_scene->addItem(background_image_item);

        brick *brick_shape = new brick(tile_size, 3);
        game_scene->addItem(brick_shape);
        game_view->setFocus();            // Set focus to the view


        game_scene->addItem(brick_shape);
        game_scene->setFocusItem(brick_shape);  // Explicitly set the brick as the focused item in the scene
        brick_shape->setFocus();    // Then set focus to the brick after adding it to the scene

        game_view->show();
        game_view->setFixedSize(game_window_width, game_window_height);
        game_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        game_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [brick_shape, game_scene, timer]() {
            // Check if the brick collides with the bottom or other blocks
            if (!checkCollision(brick_shape, game_scene)) {
                brick_shape->setY(brick_shape->y() + brick_shape->tile_size);  // Move down by one tile
            }
            else {
                timer->stop();  // Stop the timer when the brick collides with the bottom or other bricks

                // To be done:
                // 1. Lock the brick in place (this happens implicitly)
                // 2. Check for full lines and clear them if needed
                // 3. Spawn a new brick
            }
            });
        timer->start(1000);  // Move brick down every second

    }
    catch (...) {
        QMessageBox::information(this, "Error", "Error during game test");
    }
}