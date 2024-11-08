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
    void on_testGameButton_clicked();
};

//main concept for game loop
// add fps = 60
//  int frame_passed = 0
// while(!game_over() || victory()){
//  Events()
//  Update()
//  Render()
//  if(frame_passed == 60){
//      frame_passed = 0;
//      move the block independently of the player one block down
//  }
// frame_passed++;
//}