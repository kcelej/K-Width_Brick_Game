#pragma once

#include "Player.h"
#include "NetworkMessageManager.h"
#include "Tetris_Game.h"
#include "NetworkUtils.h"
#include <QString>
#include <QObject>

class NetworkGame : public QObject
{
	Q_OBJECT
private:
	Player localPlayer;
	NetworkMessageManager* networkMessageManager;
	Tetris_Game* tetrisGame;
public:
	explicit NetworkGame(QObject* parent = nullptr);
	~NetworkGame();
	Q_INVOKABLE void startGame();
	Q_INVOKABLE void configNetworkNeighbors(QString nextNeighborIP, quint16 nextNeighborPort, quint16 previousNeighborPort);
	Q_INVOKABLE void setUserId(int id);
	Q_INVOKABLE void startListening();
	Q_INVOKABLE void connectToNext();
  
};

