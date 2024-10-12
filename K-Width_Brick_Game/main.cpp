#include "main_window.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    main_window w;
    w.setWindowTitle("Brick Game Lobby");
    w.show();
    return a.exec();
}
