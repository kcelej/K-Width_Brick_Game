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

const int MAX_PLAYERS = 4; // Player limit

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

    singleplayerGameButton = new QPushButton("Singleplayer", this);
    layout->addWidget(singleplayerGameButton);
    connect(singleplayerGameButton, &QPushButton::clicked, this, &main_window::singleplayer);

    statusLabel = new QLabel("Status: Waiting", this);
    layout->addWidget(statusLabel);

    playerList = new QListWidget(this);
    layout->addWidget(playerList);
}

QString main_window::getHostIpAddress() const {
    return hostIp;
}

void main_window::singleplayer() {
    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();
    config.hostport = 15550;
    hostIp = "127.0.0.1";
    startServer();
    startGame();

}

void main_window::startServer() {
    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();
    qDebug() << "\nPortInput:\n" << portInput << "'"  ;
    qDebug() << "\nIpInput:\n" << ipInput << "'";
    
    if (!portInput->text().isEmpty() && !ipInput->text().isEmpty()) {
        config.hostport = portInput->text().toInt();
        hostIp = ipInput->text().trimmed();

       
    }
    qDebug() << "\nIpHosta:\n" << hostIp << "'";
    startGameButton->setEnabled(true);
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &main_window::handleNewConnection);

    if (server->listen(QHostAddress::Any, config.hostport)) {
        statusLabel->setText("Status: Hosting on " + hostIp + ":" + QString::number(config.hostport));
        qDebug() << "\nIpHosta:\n" << hostIp << "'";
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
    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();
    config.PLAYER_COUNT = clients.size() + 1; // Liczymy równie¿ hosta
    QString playerListMessage = QString("PLAYERS:%1;COUNT:%2").arg(playerInfoList.join(";")).arg(config.PLAYER_COUNT);

    for (QTcpSocket* socket : std::as_const(clients)) {
        socket->write(playerListMessage.toUtf8());
        socket->flush();
    }

    statusLabel->setText("Status: Connected players: " + QString::number(clients.size() + 1));
    //startGameButton->setEnabled(!clients.isEmpty());
}

void main_window::connectToHost() {
    QString ip = ipInput->text().trimmed();
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


    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();
    qDebug() << "Recieved data from client:" << client->peerAddress().toString() << "->" << message;

    if (message.startsWith("PLAYERS:")) {
        // Aktualizacja listy graczy
        QStringList parts = message.split(";");
        QStringList players = parts[0].mid(8).split(";");

        playerList->clear();
        for (const QString& player : players) {
            playerList->addItem(player);
        }

        // **Odczyt liczby graczy**
        for (const QString& part : parts) {
            if (part.startsWith("COUNT:")) {
                config.PLAYER_COUNT = part.mid(6).toInt();
                
                qDebug() << "Ustawiono PLAYER_COUNT na:" << config.PLAYER_COUNT;
            }
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
    /*if (clients.isEmpty()) {
        statusLabel->setText("Not enough players to start game!");
        return;
    }*/
    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();
    config.PLAYER_COUNT = clients.size() + 1;
    qDebug() << "Player count:" << config.PLAYER_COUNT;

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
    playerIps[1] = hostIp;

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
    qDebug() << "\nIpHosta:\n" << hostIp << "'";
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