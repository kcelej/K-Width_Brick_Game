#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_main_window.h"
#include "NetworkGame.h"
#include "QThread"

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMap>
#include <QDialog>



class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);
    ~main_window();

    void symulateServerConfiguration();//simulates +/- what the server user has to do before starting the game 
    
    void checkNetworkConfig();//test
    QString getHostIpAddress() const;
    QString getPlayerIpById(int id) const;

private:
    QTcpServer* server;
    QList<QTcpSocket*> clients;
    QTcpSocket* socket;
    QLineEdit* portInput;
    QLineEdit* ipInput;
    QPushButton* hostButton;
    QPushButton* joinButton;
    QPushButton* startGameButton;
    QPushButton* singleplayerGameButton;
    QLabel* statusLabel;
    QListWidget* playerList;

    QMap<QTcpSocket*, int> playerIds;
    int nextPlayerId = 2;  // We start from 2 because the host gets ID=1
    QString hostIp;  // Host IP

    Ui::main_windowClass ui;

    
    NetworkGame* game;
    
    
private slots:
    void on_testGameButton_clicked();
    void setupNetworkMenu();
    void startServer();
    void handleNewConnection();
    void removeClient();
    void updatePlayerList();
    void connectToHost();
    void onConnected();
    void readData();
    void startGame();
    void assignPortsAndIPs();
    void singleplayer();
};

class GameInfoWindow : public QDialog {
public:
    GameInfoWindow(int listenPort, int sendPort, QString sendIp, QWidget* parent = nullptr)
        : QDialog(parent) {
        setWindowTitle("Your connection settings");
        setFixedSize(300, 150);

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(new QLabel("Listening port: " + QString::number(listenPort), this));
        layout->addWidget(new QLabel("Sending port: " + QString::number(sendPort), this));
        layout->addWidget(new QLabel("Destination IP: " + sendIp, this));
    }
};