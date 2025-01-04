#include "NetworkUtils.h"


QString NetworkUtils::getLocalIP() {
    // Uzyskaj nazwê hosta (komputera)
    QString hostName = QHostInfo::localHostName();

    // Pobierz informacje o hoœcie
    QHostInfo hostInfo = QHostInfo::fromName(hostName);

    // Iteruj po adresach hosta
    foreach(const QHostAddress & address, hostInfo.addresses()) {
        // SprawdŸ, czy adres jest IPv4
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            return address.toString(); // Zwróæ adres w formacie tekstowym
        }
    }
    return ""; 
}

QString NetworkUtils::getLocalHost()
{
    // Zwróæ standardowy adres localhost (IPv4)
    return "127.0.0.1";
}

quint16 NetworkUtils::getDefaultGameMessageListeningPort()
{
    // Zwróæ domyœlny port do nas³uchiwania wiadomoœci gry
    return 10001;  // Mo¿na zmieniæ na dowolny port
}

quint16 NetworkUtils::getDefaultGameMessageSendingPort()
{
    // Zwróæ domyœlny port do wysy³ania wiadomoœci gry
    return 10000;  // Mo¿na zmieniæ na dowolny port
}

quint16 NetworkUtils::getDefaultServerPort()
{
    // Zwróæ domyœlny port serwera
    return 12345;  // Mo¿na zmieniæ na dowolny port
}