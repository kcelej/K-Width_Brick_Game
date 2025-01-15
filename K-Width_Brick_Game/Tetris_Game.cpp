#pragma once
#include "Tetris_Game.h"
#include "other_functions.h"
#include "Tetris_Game_View.h"

Tetris_Game::Tetris_Game() {
    gameFreezed = false;
    gameScene = new QGraphicsScene();
    gameView = new Tetris_Game_View();
    gameView->setScene(gameScene);
    gameView->setWindowTitle("Brick Game");

    QPixmap backgroundImagePixmap(":/resources/textures/background.png");
    backgroundImageItem = new QGraphicsPixmapItem();
    backgroundImageItem->setPixmap(backgroundImagePixmap);
    backgroundImageItem->setScale(1);
    backgroundImageItem->setFlag(QGraphicsItem::ItemIsFocusable, false);  // background hopefully shouldn't take focus

    gameScene->addItem(backgroundImageItem);
    gameBoard = new board(gameScene);

    gameView->show();
    gameView->setFixedSize(gameWindowWidth, gameWindowHeight);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(static_cast<Tetris_Game_View*>(gameView), &Tetris_Game_View::move, this, &Tetris_Game::move);

    add_new_brick();

    // Create Buttons
    QPushButton* leftButton = new QPushButton("L");
    leftButton->setGeometry(20, 20, 20, 20);  // Set position and size
    connect(leftButton, &QPushButton::clicked, this, &Tetris_Game::moveLeft);

    QPushButton* rightButton = new QPushButton("R");
    rightButton->setGeometry(120, 20, 20, 20); // Set position and size
    connect(rightButton, &QPushButton::clicked, this, &Tetris_Game::moveRight);

    QPushButton* downButton = new QPushButton("D");
    downButton->setGeometry(220, 20, 20, 20); // Set position and size
    connect(downButton, &QPushButton::clicked, this, &Tetris_Game::moveDown);

    QPushButton* rotateButton = new QPushButton("S");
    rotateButton->setGeometry(320, 20, 20, 20); // Set position and size
    connect(rotateButton, &QPushButton::clicked, this, &Tetris_Game::rotateBrick);

    // Create buttons and add to scene as proxy widgets
    QGraphicsProxyWidget* leftButtonProxy = gameScene->addWidget(leftButton);
    QGraphicsProxyWidget* rightButtonProxy = gameScene->addWidget(rightButton);
    QGraphicsProxyWidget* downButtonProxy = gameScene->addWidget(downButton);
    QGraphicsProxyWidget* rotateButtonProxy = gameScene->addWidget(rotateButton);

    // Set positions of the buttons
    leftButtonProxy->setPos(20, 420);
    rightButtonProxy->setPos(40, 420);
    downButtonProxy->setPos(60, 420);
    rotateButtonProxy->setPos(80, 420);
    // Ensure buttons are on top of the background (set higher Z-value)
    leftButtonProxy->setZValue(1);
    rightButtonProxy->setZValue(1);
    downButtonProxy->setZValue(1);
    rotateButtonProxy->setZValue(1);

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
    bool game_ended = false; // Flag to indicate if the game has ended.

    int how_many = 1; // Temporary variable to limit the number of blocks spawned (for testing, remove later).

    const int targetFPS = 60; // Target frames per second for the game.
    const int frameDelay = 1000 / targetFPS; // Duration of one frame in milliseconds.
    QElapsedTimer timer; // Timer to measure the elapsed time between frames.
    timer.start(); // Start the timer.
    int frameCounter = 0; // Frame counter to track the number of frames elapsed.

    int temp = 1;

    // Main loop
    while (!game_ended) {
        // Inner loop to control the active player block.
        while (player) {
            if (!gameFreezed)
            {
                if (!player->can_be_still_moved) {
                  //  qDebug("4");
                    how_many++; // Increment the number of blocks processed.

                    // If the current block can no longer be moved:
                    place_brick(game_ended);
                    break; // Exit the inner loop to spawn a new block.
                }


                if (frameCounter++ >= 60) {
                   // qDebug("6");
                    // If 60 frames have elapsed (approximately 1 second at 60 FPS):
                    player->movement(3); // Move the block down by one step.
                    frameCounter = 0; // Reset the frame counter.
                }

                player->draw(); // Redraw the current state of the block.
                waitForNextFrame(timer, frameDelay); // Wait for the next frame to maintain FPS.
            }
            QCoreApplication::processEvents();
        }
        if (!gameFreezed) {
            if (game_ended) break;
            // After the inner loop, chceck board.
            gameBoard->check_board();
            // After the inner loop, spawn a new block.
            add_new_brick();
        }
        QCoreApplication::processEvents();
    }
    qDebug() << "Game ended";
}

void Tetris_Game::waitForNextFrame(QElapsedTimer& timer, int frameDelay) {
    // Wait until the time for one frame has elapsed.
    while (timer.elapsed() < frameDelay) {
        QCoreApplication::processEvents();
        // Process Qt events such as UI updates or input handling.
        // This prevents the application from freezing during the wait.
    }
    timer.restart();
    // Restart the timer to measure the time for the next frame.
}

void Tetris_Game::end_of_life_of_brick() {
    // Deletes the current brick if it exists.
    if (player) {
        delete(player); // Deallocate memory used by the brick object.
        player = nullptr; // Reset the pointer to avoid dangling references.
        qDebug() << "Brick deleted"; // Log the deletion for debugging purposes.
    }
    else {
        qDebug() << "No Brick to delete"; // Log if there is no brick to delete.
    }
}

void Tetris_Game::add_new_brick() {
    // Adds a new brick if there is no active brick currently.
    if (!player) {
        // Create a new brick with random properties, and assign it to the player pointer.
        player = new brick(random(0, 7), random(0, 6), gameBoard);
        //for tests only
        //player = new brick(random(0, 7), 0, gameBoard);

        qDebug() << "Added new brick"; // Log the creation of a new brick.
    }
    else {
        qDebug() << "Cannot add a new brick"; // Log if a new brick cannot be added because one already exists.
    }
}

void Tetris_Game::place_brick(bool& game_ended) {
    // Check if the brick exceeds the upper boundary of the board, causing a defeat condition.
    game_ended = player->chceck_for_defeat();

    if (!game_ended) {
        // If the game has not ended, place the brick's tiles on the board.
        player->draw();
    }

    // Perform cleanup for the current brick and prepare for the next one.
    end_of_life_of_brick();
}



void Tetris_Game::moveLeft() {
    if (player) {
        player->movement(1);  // Move brick left
    }
}

void Tetris_Game::moveRight() {
    if (player) {
        player->movement(0);  // Move brick right
    }
}

void Tetris_Game::moveDown() {
    if (player) {
        player->movement(3);  // Move brick down
    }
}

void Tetris_Game::rotateBrick() {
    if (player) {
        player->movement(2);  // Rotate brick
    }
}

board* Tetris_Game::getBoard() {
    return gameBoard;
}

void Tetris_Game::freeze() {
    //std::mutex m;
    //m.lock();
    gameFreezed = true;
    //m.unlock();
}
void Tetris_Game::unFreeze() {
    //std::mutex m;
    //m.lock();
    gameFreezed = false;
    //m.unlock();
}