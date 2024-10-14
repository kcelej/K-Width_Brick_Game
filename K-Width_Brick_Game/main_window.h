#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_main_window.h"

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);
    ~main_window();

private:
    Ui::main_windowClass ui;

private slots:
    void on_testGameButton_clicked();   // Launch the game for testing purposes
};
