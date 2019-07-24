/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "UnicalcServer.hpp"

#include <QtGlobal>
//#include <QProcess>
#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QDir>
#include <QDateTime>
#include <QStringList>

#include <cstdlib>


//rem и true комманды делают ничего, но нужны, чтобы сервер ждал завершения работы уникалька перед открытием .val файла
#if defined(Q_OS_WIN)
    #define RUNUNICALC "Uc3wl_en model.uni>model.val && rem"
#else
    #define RUNUNICALC "wine ./Uc3wl_en.exe model.uni>model.val && true"
#endif

UnicalcServer::UnicalcServer(quint16 port, bool debug, int numberOfStoredFiles, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Unicacl Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_debug(debug),
    numberOfStoredFiles(numberOfStoredFiles)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {

        qInfo() << "UnicalcServer is listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &UnicalcServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &UnicalcServer::closed);
    }
    else
        qInfo() << "Cannot listenen on port" << port;
}

UnicalcServer::~UnicalcServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void UnicalcServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    if (m_debug)
        qInfo() << "socketConnection:" << pSocket;
    connect(pSocket, &QWebSocket::textMessageReceived, this, &UnicalcServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &UnicalcServer::socketDisconnected);

    m_clients << pSocket;
}

void UnicalcServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        Calculate(message);
        pClient->sendTextMessage(message);
    }
}

void UnicalcServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qInfo() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void UnicalcServer::Calculate(QString& message)
{
    QFile::remove("model.uni");//если по какой-то причине лежит мусор
    QFile::remove("model.val");
    QFile* file=new QFile("model.uni");
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        message="Can not write in uni file.";
        qInfo() << "Can not write in uni file.";
        return;
    }
    QTextStream out(file);
    out << message;
    delete file;

    //Это должно было бы решить проблему необходимости DoIt.* , но получаем какой-то unknow error
    /*
    QProcess pr;
    //вар 1
    pr.setStandardOutputFile("model.val");
    QStringList arguments;
    arguments << "./Uc3wl_en.exe" << "model.uni";
    pr.execute("wine", arguments);
    //вар 2
    QString com="wine ./Uc3wl_ru.exe model.uni>model.val";
    pr.start(com);
    //pr.waitForFinished(-1);
    */
    std::system(RUNUNICALC);

    file = new QFile("model.val");
    if (!file->open(QIODevice::Truncate | QIODevice::ReadOnly | QIODevice::Text))
    {
        message="Can not open val file.";
        qInfo() << "Can not open val file. Is a working floder right?";
        return;
    }
    QTextStream in(file);
    QString ans=in.readAll();
    delete file;
    if(numberOfStoredFiles!=0)//если надо что-то складировать
    {
        QDir thisDir;
        thisDir.rename("model.val", QString("storage/") + QDateTime::currentDateTime().toString() + ".val");//проблема с windows скорее всего в том, что надо использовать симовл '\'
        thisDir.cd("storage");
        thisDir.setFilter(QDir::Files);
        thisDir.setSorting(QDir::Time);
        QStringList files=thisDir.entryList();
        if(files.size()>numberOfStoredFiles)//если склад переполнен
            for(int i=0; i<files.size()-numberOfStoredFiles; i++)
            {
                QFile::remove(QString("storage/") + files.last());
                files.removeLast();
            }
    }
    message=ans;//ответное сообщение
}
