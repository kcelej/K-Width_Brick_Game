#include <QtWidgets>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>

#include "board.h"
#include "brick.h"


class Tetris_Game : public QWidget {

    Q_OBJECT

public:
    Tetris_Game();  // Constructor for the Tetris_Game class
    ~Tetris_Game(); // Destructor for cleaning up resources
    void Game();    // Main game loop

protected:
    void keyPressEvent(QKeyEvent* event) override; // Przechwytywanie zdarzeñ klawiatury

private:
    QGraphicsScene* gameScene = nullptr;              // Pointer to the game scene for rendering
    QGraphicsView* gameView = nullptr;                // Pointer to the view used to display the game scene
    QGraphicsPixmapItem* backgroundImageItem = nullptr; // Pointer to the background image item
    board* gameBoard = nullptr;                       // Pointer to the game board object
    brick* player = nullptr;                          // Pointer to the current player brick (falling piece)


    void waitForNextFrame(QElapsedTimer& timer, int frameDelay);

    /*
     * Handles the end of life for the current brick by deleting it and cleaning up resources.
     * @details This method checks if a brick object (`player`) currently exists in the game.
     *          If a brick is present, it deletes the brick and sets the pointer to `nullptr`,
     *          effectively removing the brick from the game and freeing up memory.
     *          If no brick is found, it logs a message indicating that no brick was available to delete.
     * @note This function ensures that memory leaks are prevented by properly managing
     *       the lifecycle of the dynamically allocated brick object.
     */
    void end_of_life_of_brick();

    /*
     * Adds a new brick to the game if one does not already exist.
     * @details This method checks if the `player` (the current brick) is `nullptr`. If it is,
     *          a new `brick` object is created using random shape and colour. After the brick is added, a debug message is logged indicating
     *          that a new brick has been added.
     *          If a brick already exists, a message is logged indicating that a new brick cannot be added.
     */
    void add_new_brick();

signals:
    void keyPressed(QKeyEvent* event); // Signal emitted when a key is pressed
};
