#include "NetworkUtils.h"


QString NetworkUtils::getLocalIP() {
    // Uzyskaj nazw� hosta (komputera)
    QString hostName = QHostInfo::localHostName();

    // Pobierz informacje o ho�cie
    QHostInfo hostInfo = QHostInfo::fromName(hostName);

    // Iteruj po adresach hosta
    foreach(const QHostAddress & address, hostInfo.addresses()) {
        // Sprawd�, czy adres jest IPv4
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            return address.toString(); // Zwr�� adres w formacie tekstowym
        }
    }
    return ""; 
}

QString NetworkUtils::getLocalHost()
{
    // Zwr�� standardowy adres localhost (IPv4)
    return "127.0.0.1";
}

quint16 NetworkUtils::getDefaultGameMessageListeningPort()
{
    // Zwr�� domy�lny port do nas�uchiwania wiadomo�ci gry
    return 10001;  // Mo�na zmieni� na dowolny port
}

quint16 NetworkUtils::getDefaultGameMessageSendingPort()
{
    // Zwr�� domy�lny port do wysy�ania wiadomo�ci gry
    return 10000;  // Mo�na zmieni� na dowolny port
}

quint16 NetworkUtils::getDefaultServerPort()
{
    // Zwr�� domy�lny port serwera
    return 12345;  // Mo�na zmieni� na dowolny port
}