#include <QGraphicsView>
#include <QKeyEvent>

class Tetris_Game_View : public QGraphicsView {
    Q_OBJECT

public:
    explicit Tetris_Game_View(QWidget* parent = nullptr) : QGraphicsView(parent) {}

protected:
    void keyPressEvent(QKeyEvent* event);

signals:
    void move(QKeyEvent* event);
};

