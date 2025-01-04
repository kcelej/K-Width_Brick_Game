#pragma once
#include <QHostInfo>
#include <QHostAddress>
#include <QDebug>
class NetworkUtils
{
public:
	static QString getLocalIP();
	static QString getLocalHost();
	static quint16 getDefaultGameMessageListeningPort();
	static quint16 getDefaultGameMessageSendingPort();
	static quint16 getDefaultServerPort();
};

