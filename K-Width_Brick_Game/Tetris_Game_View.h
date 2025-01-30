#include <QGraphicsView>
#include <QKeyEvent>

/*
 * Custom QGraphicsView subclass for handling Tetris game interactions.
 * @details This class extends `QGraphicsView` to capture keyboard events and emit a signal
 *          whenever a key is pressed. It allows external objects to respond to user input
 *          by connecting to the `move` signal.
 */
class Tetris_Game_View : public QGraphicsView {
    Q_OBJECT

public:
    /*
     * Constructor for Tetris_Game_View.
     * @details Initializes the QGraphicsView with an optional parent widget.
     * @param parent A pointer to the parent QWidget (default is nullptr).
     */
    explicit Tetris_Game_View(QWidget* parent = nullptr) : QGraphicsView(parent) {}

protected:
    /*
     * Handles key press events.
     * @details This function overrides the `keyPressEvent` method from `QGraphicsView`.
     *          When a key is pressed, it emits the `move` signal with the event details.
     * @param event A pointer to the QKeyEvent containing information about the key press.
     */
    void keyPressEvent(QKeyEvent* event);

signals:
    /*
     * Signal emitted when a key is pressed.
     * @details This signal carries the `QKeyEvent` data so that connected slots can process
     *          the key input accordingly.
     * @param event A pointer to the QKeyEvent associated with the key press.
     */
    void move(QKeyEvent* event);
};

