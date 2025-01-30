#include "main_window.h"
#include "Tetris_Game.h"


#include <QtWidgets>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "brick.h"
#include "board.h"
//
#include <QThread>
#include <QCoreApplication>
#include "NetworkGame.h"
#include "NetworkUtils.h"
#include "iostream"

#include "PlayerNetworkConfig.h"
const int MAX_PLAYERS = 2; // Player limit

main_window::main_window(QWidget *parent)
    : QMainWindow(parent), server(nullptr), socket(nullptr)
{
    ui.setupUi(this);
    setupNetworkMenu();
}

main_window::~main_window()
{
    delete game;
}

//old
/*
void main_window::on_testGameButton_clicked() {
    try {
        QGraphicsScene* gameScene = new QGraphicsScene();
        QGraphicsView* gameView = new QGraphicsView(gameScene);
        gameView->setWindowTitle("Brick Game");

        QPixmap backgroundImagePixmap(":/resources/textures/background.png");
        QGraphicsPixmapItem* backgroundImageItem = new QGraphicsPixmapItem();
        backgroundImageItem->setPixmap(backgroundImagePixmap);
        backgroundImageItem->setScale(1);
        backgroundImageItem->setFlag(QGraphicsItem::ItemIsFocusable, false);  // background hopefully shouldn't take focus

        gameScene->addItem(backgroundImageItem);
        board* gameBoard = new board(gameScene, gameView);

        gameView->show();
        gameView->setFixedSize(gameWindowWidth, gameWindowHeight);
        gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        brick* player = nullptr;
        int how_many = 1;
        while (how_many <= 6) { //for now spawn only 6 blocks
            // example brick for testing
            player = new brick(z++, 2, gameBoard);
           // player = new brick(random(0, 6), random(0, 6), gameBoard);
            //player = new brick(random(0, 6), z++, gameBoard);

            
            //0 ->
            //1 <-
            //2 /\
            //3 \/
            

            // example movement downwards for testing
            int i = 0;
            while (true) {
                if (i == 0) {
                    player->movement(2);
                    delayOneSecond();
                    player->movement(2);
                    delayOneSecond();
                    player->movement(1);
                    delayOneSecond();
                    player->movement(1);
                    delayOneSecond();
                }
                if (i == 1) {
                    
                }
                if (i == 2) {
                    //player->change_rotation();
                }
                if (i == 3) {
                  // player->change_rotation();
                }
                if (!player->move_down()) {
                    how_many++;
                    player->draw();
                    //player = nullptr; // memory leak FIX LATER!!!
                    delete(player);
                    break;
                }

                //if (!(i % 4)) player->move_left();
                //if (!(i % 4)) player->move_right();
                ++i;
                player->draw();
                delayOneSecond();
            }
        }
    }

    catch (...) {
        QMessageBox::information(this, "Error", "Error during game test");
    }
}
*/

void main_window::setupNetworkMenu() {
    QVBoxLayout* layout = new QVBoxLayout();
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    QLabel* label = new QLabel("Enter Host Port:", this);
    layout->addWidget(label);

    portInput = new QLineEdit(this);
    layout->addWidget(portInput);

    QLabel* ipLabel = new QLabel("Enter Host IP:", this);
    layout->addWidget(ipLabel);

    ipInput = new QLineEdit(this);
    layout->addWidget(ipInput);

    hostButton = new QPushButton("Host", this);
    layout->addWidget(hostButton);
    connect(hostButton, &QPushButton::clicked, this, &main_window::startServer);

    joinButton = new QPushButton("Join", this);
    layout->addWidget(joinButton);
    connect(joinButton, &QPushButton::clicked, this, &main_window::connectToHost);

    startGameButton = new QPushButton("Start Game", this);
    startGameButton->setEnabled(false);
    layout->addWidget(startGameButton);
    connect(startGameButton, &QPushButton::clicked, this, &main_window::startGame);

    statusLabel = new QLabel("Status: Waiting", this);
    layout->addWidget(statusLabel);

    playerList = new QListWidget(this);
    layout->addWidget(playerList);
}

QString main_window::getHostIpAddress() const {
    return hostIp;
}

void main_window::startServer() {
    int port = portInput->text().toInt();
    hostIp = ipInput->text().trimmed();

    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();
    config.hostport = port;
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &main_window::handleNewConnection);

    if (server->listen(QHostAddress::Any, port)) {
        statusLabel->setText("Status: Hosting on " + hostIp + ":" + QString::number(port));
        updatePlayerList();
    }
    else {
        statusLabel->setText("Status: Hosting error");
    }
}

void main_window::handleNewConnection() {
    
    QTcpSocket* newSocket = server->nextPendingConnection();
    if (clients.size() >= MAX_PLAYERS - 1) { // -1 because the host also counts as a player
        newSocket->write("SERVER_FULL");
        newSocket->flush();
        newSocket->disconnectFromHost();
        return;
    }
    
    
    clients.append(newSocket);

    // Assigning ID to new player
    playerIds[newSocket] = nextPlayerId++;

    connect(newSocket, &QTcpSocket::disconnected, this, &main_window::removeClient);
    connect(newSocket, &QTcpSocket::readyRead, this, &main_window::readData);
    updatePlayerList();
}

void main_window::removeClient() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        clients.removeAll(client);
        playerIds.remove(client);
        client->deleteLater();
        updatePlayerList();
    }
}

void main_window::updatePlayerList() {
    playerList->clear();
    QStringList playerInfoList;

    // Adding host to player list
    QString hostInfo = QString("ID: 1 | IP: %1 (Host)").arg(hostIp);
    playerList->addItem(hostInfo);
    playerInfoList.append(hostInfo);

    // Adding clients to the player list
    for (QTcpSocket* socket : std::as_const(clients)) {
        int playerId = playerIds.value(socket, -1);
        QString ip = socket->peerAddress().toString();
        QString playerInfo = QString("ID: %1 | IP: %2").arg(playerId).arg(ip);
        playerList->addItem(playerInfo);
        playerInfoList.append(playerInfo);
    }

    // Sending the player list to everyone
    QString playerListMessage = "PLAYERS:" + playerInfoList.join(";");
    for (QTcpSocket* socket : std::as_const(clients)) {
        socket->write(playerListMessage.toUtf8());
    }

    statusLabel->setText("Status: Connected players: " + QString::number(clients.size() + 1));
    startGameButton->setEnabled(!clients.isEmpty());
}

void main_window::connectToHost() {
    QString ip = ipInput->text();
    int port = portInput->text().toInt();
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &main_window::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &main_window::readData);
    socket->connectToHost(QHostAddress(ip), port);
}

void main_window::onConnected() {
    statusLabel->setText("Status: Connected with host");
}

void main_window::readData() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QByteArray data = client->readAll();
    QString message = QString::fromUtf8(data);

    qDebug() << "Recieved data from client:" << client->peerAddress().toString() << "->" << message;

    if (message.startsWith("PLAYERS:")) {
        // Updating player list
        QStringList players = message.mid(8).split(";");
        playerList->clear();
        for (const QString& player : players) {
            playerList->addItem(player);
        }
    }
    if (message.startsWith("SERVER_FULL")) {
        
        QMetaObject::invokeMethod(this, [this]() {
            statusLabel->setText("Status: Server is full!");
            }, Qt::QueuedConnection);
    }

    if (message.startsWith("START_GAME")) {
        QStringList parts = message.split(";");
        QMap<QString, QString> gameData;

        for (const QString& part : parts) {
            QStringList keyValue = part.split(":");
            if (keyValue.size() == 2) {
                gameData[keyValue[0].trimmed()] = keyValue[1].trimmed();
            }
        }

        

        if (gameData.contains("PLAYER_ID") && gameData.contains("LISTEN_PORT")
            && gameData.contains("SEND_PORT") && gameData.contains("SEND_IP")) {

            PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();
            config.playerId = gameData["PLAYER_ID"].toInt();
            config.listenPort = gameData["LISTEN_PORT"].toInt();
            config.sendPort = gameData["SEND_PORT"].toInt();
            config.sendIp = gameData["SEND_IP"];

            qDebug() << "Assigned values ??to PlayerNetworkConfig:";
            qDebug() << "playerId:" << config.playerId;
            qDebug() << "listenPort:" << config.listenPort;
            qDebug() << "sendPort:" << config.sendPort;
            qDebug() << "sendIp:" << config.sendIp;

            qDebug() << "Creating a GameInfoWindow for the player:" << config.listenPort << config.sendPort << config.sendIp;
            GameInfoWindow* infoWindow = new GameInfoWindow(config.listenPort, config.sendPort, config.sendIp);
            infoWindow->show();
            symulateServerConfiguration();

        }
    }
}

void main_window::startGame() {
    if (clients.isEmpty()) {
        statusLabel->setText("Not enough players to start game!");
        return;
    }
    assignPortsAndIPs();

    statusLabel->setText("The game has started!");
    symulateServerConfiguration();
}

QString main_window::getPlayerIpById(int id) const {
    if (id == 1) {
        return hostIp;  // Host always has ID = 1
    }

    for (QTcpSocket* socket : playerIds.keys()) {
        if (playerIds.value(socket) == id) {
            return socket->peerAddress().toString();
        }
    }
    return "Player nod found";
}

void main_window::assignPortsAndIPs() {
    struct PlayerInfo {
        int playerId;
        int listenPort;
        int sendPort;
        QString sendIp;
    };
    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();

    QList<PlayerInfo> assignedData;
    QList<int> playerIds;
    QMap<int, QString> playerIps;

    playerIds.append(1);
    playerIps[1] = QHostAddress(ipInput->text().trimmed()).toString();

    int currentId = 2;
    for (QTcpSocket* socket : clients) {
        QString ip = socket->peerAddress().toString();
        if (ip.startsWith("::ffff:")) {
            ip = ip.mid(7); // Removing prefix ::ffff:
        }
        playerIds.append(currentId);
        playerIps[currentId] = ip;
        qDebug() << "IP assigned for ID:" << currentId << "->" << playerIps[currentId];
        currentId++;
    }

    int maxPlayers = playerIds.size();
    int hostport = config.hostport+1;
    

    for (int i = 0; i < maxPlayers; i++) {
        int currentId = playerIds[i];
        int nextId = (i + 1 < maxPlayers) ? playerIds[i + 1] : playerIds[0];

        PlayerInfo info;
        info.playerId = currentId;
        info.listenPort = hostport + i;
        info.sendPort = hostport + ((i + 1) % maxPlayers);
        info.sendIp = playerIps.value(nextId, "Unknown IP");

        qDebug() << "Player ID:" << currentId << "will send to" << nextId << "(IP:" << info.sendIp << ")";

        assignedData.append(info);
    }

    // **Updating configuration for host (player ID 1)**
    
    config.playerId = assignedData[0].playerId;
    config.listenPort = assignedData[0].listenPort;
    config.sendPort = assignedData[0].sendPort;
    config.sendIp = assignedData[0].sendIp;

    qDebug() << "Host Configuration (PlayerNetworkConfig):";
    qDebug() << "playerId:" << config.playerId;
    qDebug() << "listenPort:" << config.listenPort;
    qDebug() << "sendPort:" << config.sendPort;
    qDebug() << "sendIp:" << config.sendIp;

    for (int i = 0; i < maxPlayers; i++) {
        QString message = QString("START_GAME;PLAYER_ID:%1;LISTEN_PORT:%2;SEND_PORT:%3;SEND_IP:%4")
            .arg(assignedData[i].playerId)
            .arg(assignedData[i].listenPort)
            .arg(assignedData[i].sendPort)
            .arg(assignedData[i].sendIp);

        qDebug() << "Sending a message to player ID:" << playerIds[i] << "->" << message;

        if (i == 0) {
            GameInfoWindow* infoWindow = new GameInfoWindow(
                assignedData[i].listenPort, assignedData[i].sendPort, assignedData[i].sendIp);
            infoWindow->show();
        }
        else {
            clients[i - 1]->write(message.toUtf8());
            clients[i - 1]->flush();
            qDebug() << "Message sent to client:" << clients[i - 1]->peerAddress().toString();
            connect(clients[i - 1], &QTcpSocket::readyRead, this, &main_window::readData);
        }
    }
}



//new
void main_window::on_testGameButton_clicked() {

    symulateServerConfiguration();

   //idk
   return;
}

void main_window::symulateServerConfiguration() {
    
    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();
    
    
    game = new NetworkGame();
    //// Get the listening port
    //bool ok = false;
    //int listeningPort = QInputDialog::getInt(this, "Server Configuration",
    //    "Enter listening port:",
    //    0, 0, 65535, 1, &ok);
    //if (!ok) return; // User canceled

    //// Get the sending port
    //int sendingPort = QInputDialog::getInt(this, "Server Configuration",
    //    "Enter sending port:",
    //    0, 0, 65535, 1, &ok);
    //if (!ok) return;

    //// Get the sending IP
    //QString sendingIP = QInputDialog::getText(this, "Server Configuration",
    //    "Enter sending IP (v4):",
    //    QLineEdit::Normal, "127.0.0.1", &ok);
    //if (!ok) return;

    //// Get the user ID
    //int userId = QInputDialog::getInt(this, "Server Configuration",
    //    "Enter user ID:",
    //    0, 0, 10000, 1, &ok);
    //if (!ok) return;
    int listeningPort = config.listenPort;
    int sendingPort = config.sendPort;
    QString sendingIP = config.sendIp;
    int userId = config.playerId;
    
    qDebug() << "Assigned values ??to PlayerNetworkConfig:";
    qDebug() << "playerId:" << config.playerId;
    qDebug() << "listenPort:" << config.listenPort;
    qDebug() << "sendPort:" << config.sendPort;
    qDebug() << "sendIp:" << config.sendIp;
    

    //// Confirmation
    //QMessageBox::StandardButton confirm = QMessageBox::question(this,
    //    "Confirm",
    //    QString("Listening Port: %1\nSending Port: %2\nSending IP: %3\nUser ID: %4\nDo you confirm?")
    //    .arg(listeningPort)
    //    .arg(sendingPort)
    //    .arg(sendingIP)
    //    .arg(userId),
    //    QMessageBox::Yes | QMessageBox::No);

    //if (confirm != QMessageBox::Yes) return;

    // Configure the game
    game->setUserId(userId);
    game->configNetworkNeighbors(sendingIP.toStdString().c_str(), sendingPort, listeningPort);
    game->startListening();

    // Delay for simulation
    QThread::msleep(5000);

    // Connect to the next node and start the game
    game->connectToNext();
    hide();
    game->startGame();
    show();
}
void main_window::checkNetworkConfig() {
    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();

    qDebug() << "Checking PlayerNetworkConfig:";
    qDebug() << "ListenPort:" << config.listenPort;
    qDebug() << "SendPort:" << config.sendPort;
    qDebug() << "SendIp:" << config.sendIp;

    if (config.listenPort <= 0 || config.sendPort <= 0 || config.sendIp.isEmpty()) {
        qDebug() << "ERROR: Invalid values ??in PlayerNetworkConfig!";
    }
}