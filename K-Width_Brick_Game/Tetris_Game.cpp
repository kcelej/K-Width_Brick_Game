#pragma once
#include "Tetris_Game.h"
#include "other_functions.h"
#include "Tetris_Game_View.h"

Tetris_Game::Tetris_Game() {
    // Initially, the game is not frozen.
    gameFreezed = false;

    // Create the game scene.
    gameScene = new QGraphicsScene();

    // Create the game view and set its scene.
    gameView = new Tetris_Game_View();
    gameView->setScene(gameScene);
    gameView->setWindowTitle("Brick Game");
    // Hide the game view until the game starts.
    gameView->hide();

    // Load the background image.
    QPixmap backgroundImagePixmap(":/resources/textures/background.png");
    backgroundImageItem = new QGraphicsPixmapItem();
    backgroundImageItem->setPixmap(backgroundImagePixmap);
    // Maintain original size.
    backgroundImageItem->setScale(1);

    // Ensure the background does not take focus to avoid unintended interactions.
    backgroundImageItem->setFlag(QGraphicsItem::ItemIsFocusable, false);

    // Add the background image to the scene.
    gameScene->addItem(backgroundImageItem);

    // Initialize the game board and assign it to the scene.
    gameBoard = new board(gameScene);
    // Associate the board with the game view.
    gameBoard->setGameView(gameView);

    // Set the fixed size of the game window.
    gameView->setFixedSize(gameWindowWidth, gameWindowHeight);

    // Disable scroll bars to prevent unwanted movement.
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Connect keyboard input from Tetris_Game_View to the game's move function.
    connect(static_cast<Tetris_Game_View*>(gameView), &Tetris_Game_View::move, this, &Tetris_Game::move);

    // Spawn the first brick at the start of the game.
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
    gameView->show();
    bool game_ended = false; // Flag to indicate if the game has ended.
    const int targetFPS = 60; // Target frames per second.
    const int frameDelay = 1000 / targetFPS; // Duration of one frame in milliseconds.
    QElapsedTimer timer; // Timer to measure frame timing.
    timer.start(); // Start the timer.
    int frameCounter = 0; // Frame counter to track the number of frames elapsed.

    // Main loop
    while (!game_ended) {
        // Inner loop to handle active brick movement.
        while (player) {
            if (!gameFreezed)
            {
                if (!player->can_be_still_moved) {
                   // If the current block can no longer be moved.
                   place_brick(game_ended);
                   break; // Exit the inner loop to spawn a new block.
                }

                if (frameCounter++ >= 60) {
                   // Move brick down every 60 frames (~1s).
                    player->can_be_still_moved = player->move_down(); // Move the block down by one step.
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
    gameView->hide();
}

void Tetris_Game::waitForNextFrame(QElapsedTimer& timer, int frameDelay) {
    // Wait until the time for one frame has elapsed.
    while (timer.elapsed() < frameDelay) {
        // Process Qt events such as UI updates or input handling.
        // This prevents the application from freezing during the wait.
        QCoreApplication::processEvents();
    }
    // Restart the timer to measure the time for the next frame.
    timer.restart();
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

board* Tetris_Game::getBoard() {
    return gameBoard;
}

void Tetris_Game::freeze() {
    //std::mutex m;
    //m.lock();
    gameView->setDisabled(true);
    gameFreezed = true;
    //m.unlock();
}
void Tetris_Game::unFreeze() {
    //std::mutex m;
    //m.lock();
    gameView->setDisabled(false);
    gameFreezed = false;
    //m.unlock();
}

QGraphicsView* Tetris_Game::getGameView() {
    return gameView;
}

void Tetris_Game::move(QKeyEvent* event) {
    if (player) {
        player->movement(event);
    }
    else {
        qWarning("No active player brick to move.");
    }
}