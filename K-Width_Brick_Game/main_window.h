#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_main_window.h"
#include "NetworkGame.h"


class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);
    ~main_window();

private:
    Ui::main_windowClass ui;

    //do testow, pozniej usunac
    NetworkGame g1;
    NetworkGame g2;
    NetworkGame g3;
    NetworkGame g4;

private slots:
    void on_testGameButton_clicked();
};

//concept moved to new class Tetris_Game
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