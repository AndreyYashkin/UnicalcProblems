/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "UnicalcServer.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationVersion("1.0.1");
    QCommandLineParser parser;
    parser.setApplicationDescription("UnicalcServer");
    parser.addVersionOption();
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for UnicalcServer [default: 25565]."),
            QCoreApplication::translate("main", "port"), QLatin1Literal("25565"));
    parser.addOption(portOption);
    QCommandLineOption numberOfStoredFilesOption(QStringList() << "f" << "files",
            QCoreApplication::translate("main", "Number of .uni files from clients to store [default: 0]."),
            QCoreApplication::translate("main", "files"), QLatin1Literal("0"));
    parser.addOption(numberOfStoredFilesOption);

    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    unsigned long port = parser.value(portOption).toULong();
    int files= parser.value(numberOfStoredFilesOption).toInt();;
    UnicalcServer *server = new UnicalcServer(static_cast<quint16> (port), debug, files);
    QObject::connect(server, &UnicalcServer::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
