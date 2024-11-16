#pragma once
#include "Tetris_Game.h"
#include "other_functions.h"

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

            if (!player->can_be_still_moved) {

                how_many++; // Increment the number of blocks processed.

                // If the current block can no longer be moved:
                place_brick(game_ended);
                break; // Exit the inner loop to spawn a new block.
            }

           //make a line for line deletion test
            if (how_many == 1 && temp == 1) {
                temp++;
                player->movement(2);
                player->movement(1);
                player->movement(1);
                player->movement(1);
                player->movement(1);
            }
            if (how_many == 3 && temp == 2) {
                temp++;
                player->movement(0);
            }
            if (how_many == 4 && temp == 3) {
                temp++;
                player->movement(2);
                player->movement(0);
                player->movement(0);
                player->movement(0);
                player->movement(0);
            }
            if (how_many == 5 && temp == 4) {
                temp++;
                player->movement(2);
                player->movement(1);
                player->movement(1);
                player->movement(1);
                player->movement(1);
            }
            if (how_many == 6 && temp == 5) {
                temp++;
                player->movement(2);
                player->movement(0);
                player->movement(0);
                player->movement(0);
                player->movement(0);
            }

            if (frameCounter++ >= 60) {
                // If 60 frames have elapsed (approximately 1 second at 60 FPS):
                player->movement(3); // Move the block down by one step.
                frameCounter = 0; // Reset the frame counter.
            }

            player->draw(); // Redraw the current state of the block.
            waitForNextFrame(timer, frameDelay); // Wait for the next frame to maintain FPS.
        }
  
        if (game_ended) break;
        // After the inner loop, chceck board.
        gameBoard->check_board();
        // After the inner loop, spawn a new block.
        add_new_brick();
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

        // Uncomment the following line and fix the issue to connect the keyboard input to the brick's handler.
        // QObject::connect(this, &Tetris_Game::keyPressd, player, &brick::onKeyPress);

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



