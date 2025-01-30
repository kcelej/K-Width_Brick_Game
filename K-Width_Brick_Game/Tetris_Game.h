#pragma once
#include <QtWidgets>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>

#include "board.h"
#include "brick.h"
#include "NetworkMessageManager.h"


class Tetris_Game : public QWidget {

    Q_OBJECT

public:
    /*
     * Constructor for the Tetris_Game class.
     * @details Initializes the game components, including the scene, view, game board, and background.
     *          It also sets up the game window properties and connects the keyboard input from
     *          `Tetris_Game_View` to the game's movement handler.
     */
    Tetris_Game();

    /*
     * Destructor for the Tetris_Game class.
     * @details Cleans up dynamically allocated memory to prevent memory leaks.
     */
    ~Tetris_Game();

    /*
     * Starts the Tetris game loop.
     * @details Controls the game flow, handling brick movement, collision detection, and board updates.
     */
    void Game();

    /*
     * Retrieves a pointer to the game board.
     * @return Pointer to the game board.
     */
    board* getBoard();

    /*
     * Retrieves a pointer to the game view.
     * @return Pointer to the game view.
     */
    QGraphicsView* getGameView();

    /*
     * Freezes the game, preventing player movement.
     */
    void freeze();

    /*
     * Unfreezes the game, allowing player movement.
     */
    void unFreeze();

private:
    bool gameFreezed;
    QGraphicsScene* gameScene = nullptr;              // Pointer to the game scene for rendering
    QGraphicsView* gameView = nullptr;                // Pointer to the view used to display the game scene
    QGraphicsPixmapItem* backgroundImageItem = nullptr; // Pointer to the background image item
    board* gameBoard = nullptr;                       // Pointer to the game board object
    brick* player = nullptr;                          // Pointer to the current player brick (falling piece)

    /*
     * Waits for the next frame to maintain a consistent frame rate.
     * @param timer Reference to QElapsedTimer to measure time elapsed since the last frame.
     * @param frameDelay The delay (in milliseconds) between frames to achieve the desired FPS.
     */
    void waitForNextFrame(QElapsedTimer& timer, int frameDelay);

    /*
     * Deletes the current brick.
     * @details If a brick exists, it is deleted, and the pointer is reset to avoid dangling references.
     */
    void end_of_life_of_brick();

    /*
     * Adds a new brick to the game.
     * @details A new brick is created if no active brick exists. The brick's properties are randomized.
     */
    void add_new_brick();

    /*
     * Places the current brick on the board and checks for game over.
     * @param game_ended Reference to the game-over flag.
     */
    void place_brick(bool& game_ended);

    /*
     * Handles keyboard input and passes it to the active brick.
     * @details This function receives a key event from `Tetris_Game_View` 
     *          and forwards it to the `player` object, allowing the brick to move accordingly.
     *          If no active player exists, a warning is logged to prevent errors.
     * @param event A pointer to the `QKeyEvent` containing key press details.
     */
    void move(QKeyEvent* event);
};
