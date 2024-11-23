#include "Tetris_Game_View.h"

void Tetris_Game_View::keyPressEvent(QKeyEvent* event) {
	emit move(event);
}