/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "UnicalcClient.hpp"
#include "ui_UCMainWindow.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include "OutputWindow.hpp"
#include "ForUniAndVal.hpp"
#include "UniFileViewer.hpp"

#ifdef WEBVERSION//добавлять иконки вручную надо только в вбе версии
    #include <QIcon>
#endif

using namespace UnicalcClientLib;
using namespace DoNotTouchItInYourProgram;


UnicalcClient::UnicalcClient(const UCProblemsFactory &generator, const QUrl& defaultServerUrl, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::UCMainWindow),
    problem(generator.createProblem(this)),
    connected(false),
    serverUrl(defaultServerUrl),
    connectionDialogPtr(nullptr),
    aboutDialogPtr(nullptr)
{
    setAttribute(Qt::WA_DeleteOnClose);//или лучше так http://doc.qt.io/qt-5/qtwidgets-mainwindows-application-example.html#close-event-handler
    ui->setupUi(this);
    setWindowTitle(problem->getProblemName());
    ui->scrollArea->setWidget(problem->getInputWidget());

    //Добавим примеры в загрузку, если они есть или уберем меню загрузки примеров
    if(problem->getExamplesList().empty())
        ui->loadExampleSubmenu->menuAction()->setVisible(false);
    else//наполняем список примеров
        for(ExamplesConstIterator i=problem->getExamplesList().begin(); i<problem->getExamplesList().end(); i++) // или !=
        {
            QAction *loadExampleAction=new QAction(ui->loadExampleSubmenu);
            loadExampleAction->setObjectName(QString("%1").arg(i-problem->getExamplesList().begin()));//название это номер примера
            loadExampleAction->setText(i->name);
            ui->loadExampleSubmenu->addAction(loadExampleAction);
            connect(loadExampleAction, &QAction::triggered, this, &UnicalcClient::on_actionOpenExample_triggered);
        }

    //сделаем невидимыми недоступные элементы
    if(!problem->isSettingsDialogAvailable())
        ui->actionOpenSettings->setVisible(false);
    if(!problem->isUserGuideDialogAvailable() && !problem->isAboutDialogAvailable())
        ui->infoMenu->menuAction()->setVisible(false);
    else
    {
        if(!problem->isUserGuideDialogAvailable())
            ui->actionOpenUserGuide->setVisible(false);
        if(!problem->isAboutDialogAvailable())
            ui->actionOpenAbout->setVisible(false);
    }

    connect(&server, &QWebSocket::connected, this, &UnicalcClient::onConnected);
    connect(&server, &QWebSocket::disconnected, this, &UnicalcClient::onDisconnected);
    connect(&server, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));//тут перегуженная функция: удобнее старым синтаксисом

    connectToServer(serverUrl);
    #ifndef WEBVERSION
        problem->resetUNIFile();//начальное значение
    #else
        if(!problem->getExamplesList().empty())//Пусть в начале отображается первый пример, если такой имеется
        {
            QString uniFile;
            loadUniFile(problem->getExamplesList().first().uniFilePath, uniFile);
            problem->setUNIFile(uniFile);
        }
        else
            problem->resetUNIFile();//начальное значение будет пустой моделью
        //Работа с диком в wasm версии Qt все-равно не работает(Qt 5.12.0)
        ui->actionLoadFromDisk->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionSaveAs->setEnabled(false);
    #endif//WEBVERSION
    /*
     * Проблема с отсутствием стандартных сисемных иконок может возникнуть и под linux, поэтому лучше всего убрать макрос и сделать проверку того что все иконки, которые используются в .ui файле доступны.
     * Если какая-то из них недоступна, то сделать загрузку своих или еще проще всегда загружать свои.
     */
    #if defined(WEBVERSION) || defined(Q_OS_WIN)
        //теперь нужно установить свои иконки для действий вместо отсутствующих системных или они будут пустые
        ui->actionCalculate->setIcon(QIcon(":/icons/Images/menu icons for web/calcuate.png"));
        ui->actionChangeServer->setIcon(QIcon(":/icons/Images/menu icons for web/connect.png"));
        ui->actionExit->setIcon(QIcon(":/icons/Images/menu icons for web/exit.png"));
        ui->actionLoadFromDisk->setIcon(QIcon(":/icons/Images/menu icons for web/open.png"));
        ui->actionMewModel->setIcon(QIcon(":/icons/Images/menu icons for web/new.png"));
        ui->actionOpenAbout->setIcon(QIcon(":/icons/Images/menu icons for web/about.png"));
        ui->actionOpenSettings->setIcon(QIcon(":/icons/Images/menu icons for web/settings.png"));
        ui->actionOpenUserGuide->setIcon(QIcon(":/icons/Images/menu icons for web/help.png"));
        ui->actionSave->setIcon(QIcon(":/icons/Images/menu icons for web/save.png"));
        ui->actionSaveAs->setIcon(QIcon(":/icons/Images/menu icons for web/saveAs.png"));
        ui->actionViewUniFile->setIcon(QIcon(":/icons/Images/menu icons for web/viewUni.png"));
     #endif//defined(WEBVERSION) || defined(Q_OS_WIN)

}

UnicalcClient::~UnicalcClient()
{
    delete ui;
    delete problem;
    /*
     * Судя по всему, в документации врут и ничего он сам не закрывает при уничтожении, что вызывает каким-то образом segmentation fault
     * http://doc.qt.io/qt-5/qwebsocket.html#dtor.QWebSocket
     * А может, реальный виновник падения в другом месете и это, просто, совпадение, что при ручном закрытии все работает
     */
    if(connected)
        server.close();
}

void UnicalcClient::enableActionCalculate()
{
    ui->actionCalculate->setEnabled(true);
    disconnect(&server, &QWebSocket::textMessageReceived, static_cast <OutputWindow*> (sender()), &OutputWindow::onResultReceived);//Он все-равно больше не слушает
}


void UnicalcClient::onConnected()
{
    connected=true;
    connectionsHistory+="CONNECTED\n";
    ui->statusBar->showMessage("Connected to server");
    if(connectionDialogPtr)
        connectionDialogPtr->updateConnectionsHistory(connectionsHistory);
}

void UnicalcClient::onDisconnected()
{
    connected=false;
    ui->statusBar->showMessage("No connection to server");
    connectionsHistory+="DISCONNECTED\n";
    if(connectionDialogPtr)
        connectionDialogPtr->updateConnectionsHistory(connectionsHistory);
}

void UnicalcClient::onError()
{
    connectionsHistory+=server.errorString()+'\n';
    if(connectionDialogPtr)
        connectionDialogPtr->updateConnectionsHistory(connectionsHistory);
}

void UnicalcClient::onAboutDialogDestroyed()
{
    aboutDialogPtr=nullptr;
}

void UnicalcClient::onConnectionDialogDestroyed()
{
    connectionDialogPtr=nullptr;
}

void UnicalcClient::connectToServer(const QUrl& _serverUrl)
{
    serverUrl=_serverUrl;
    server.open(serverUrl);
}

void UnicalcClient::on_actionMewModel_triggered()
{
    problem->resetUNIFile();
    lastUniFilePath=QString();//теперь работаем с новой моделью
}

bool UnicalcClient::loadUniFile(const QString& path, QString& uniFile)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox *errorMessage= new QMessageBox(QMessageBox::Critical, "Опс!", file.errorString(), QMessageBox::Close, this);
        errorMessage->exec();
        return false;
    }
    QTextStream in(&file);
    uniFile=in.readAll();
    return true;
}

bool UnicalcClient::confirmLoadFile(const QString& uniFile)
{
    if(uniFile.indexOf(QString("(*%1*)").arg(problem->getProblemName()))==-1)//если не удлось найти запись, говорящую о том, что этот .uni файл относится к нашей проблеме
    {
        QMessageBox confirm(QMessageBox::Warning, "Опс...", "Не удалось найти запись, говорящую о том, что этот .uni файл относится к данной проблеме!", QMessageBox::Open|QMessageBox::Cancel, this);
        int des=confirm.exec();
        if(des==QMessageBox::Open)
            return true;
        else
            return false;
    }
    return true;
}

void UnicalcClient::on_actionLoadFromDisk_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открытие файла модели задачи", QString(), "Модель задачи (*.uni)");
    if(fileName.isEmpty())
        return;//Файл не выбран: ничего делать не надо
    QString uniFile;
    if(loadUniFile(fileName, uniFile))//Если успечно прочитали файл
        if(confirmLoadFile(uniFile))
            problem->setUNIFile(uniFile);
}

void UnicalcClient::on_actionOpenExample_triggered()
{
    //sender()->objectName().toInt();//равен номеру примера(loadExampleAction так создается)
    ExamplesConstIterator examleIndex=problem->getExamplesList().begin()+sender()->objectName().toInt();
    QString uniFile;
    if(loadUniFile(examleIndex->uniFilePath, uniFile))
        problem->setUNIFile(uniFile);
    lastUniFilePath=QString();//теперь работаем с другой моделью
}

void UnicalcClient::on_actionSave_triggered()
{
    if(lastUniFilePath.isEmpty())
        return on_actionSaveAs_triggered();//сейчас ниакакой файл с диска не открыт
    QFile file(lastUniFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox *errorMessage= new QMessageBox(QMessageBox::Critical, "Опс!", file.errorString(), QMessageBox::Close, this);
        errorMessage->exec();
        lastUniFilePath=QString();//последний файл более недоступен
        return;
    }
    QTextStream out(&file);
    out << QString("(*%1*)\n\n\n").arg(problem->getProblemName()) << problem->getUNIFile();
}

void UnicalcClient::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранение файла модели задачи", QString(), "Модель задачи (*.uni)");
    if(fileName.isEmpty())
        return;//Файл не выбран: ничего делать не надо
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox *errorMessage= new QMessageBox(QMessageBox::Critical, "Опс!", file.errorString(), QMessageBox::Close, this);
        errorMessage->exec();
        return;
    }
    lastUniFilePath=fileName;
    QTextStream out(&file);
    out << QString("(*%1*)\n\n\n").arg(problem->getProblemName()) << problem->getUNIFile();
}

void UnicalcClient::on_actionExit_triggered()
{
    emit close();
}

void UnicalcClient::on_actionChangeServer_triggered()
{
    if(!connectionDialogPtr)
    {
        connectionDialogPtr=new ConnectionDialog(serverUrl.toString(), connectionsHistory, this);
        connect(connectionDialogPtr, &ConnectionDialog::destroyed, this, &UnicalcClient::onConnectionDialogDestroyed);
        connect(connectionDialogPtr, &ConnectionDialog::connectToServer, this, &UnicalcClient::connectToServer);
    }
    MACROSHOW(connectionDialogPtr);
}

void UnicalcClient::on_actionOpenSettings_triggered()
{
    MACROEXEC(problem->createSettingsDialog());
}

void UnicalcClient::on_actionOpenUserGuide_triggered()
{
    MACROSHOW(problem->createUserGuideDialog());
}

void UnicalcClient::on_actionOpenAbout_triggered()
{
    if(!aboutDialogPtr)
    {
        aboutDialogPtr=problem->createAboutDialog();
        connect(aboutDialogPtr, &QObject::destroyed, this, &UnicalcClient::onAboutDialogDestroyed);
    }
    MACROSHOW(aboutDialogPtr);
}

void UnicalcClientLib::UnicalcClient::on_actionCalculate_triggered()
{
    if(connected)
    {
        server.sendTextMessage(QString("(*%1*)\n\n\n").arg(problem->getProblemName()) + problem->getUNIFile());
        /*
         * Заблокируем кнопку повтороной отправки запроса на сервер, пока есть ожидающий ответа диалог, чтобы не было двухсмысленности какому диалого именно прдназанчется сообжение.
         * А что если будет смена сервера до получения ответа? Тогда сигнала разрыва соединения нет: кнопка не разблокируется, а ответ не прийдет. Маловероятное событие, но вызвать ошибку может.
         */
        ui->actionCalculate->setEnabled(false);
        OutputWindow* outputWindow = new OutputWindow(problem);
        connect(&server, &QWebSocket::textMessageReceived, outputWindow , &OutputWindow::onResultReceived);
        connect(outputWindow , &OutputWindow::IsNotWaitingForServerAnswerAnymore, this, &UnicalcClient::enableActionCalculate);
        connect(this, &UnicalcClient::destroyed, outputWindow , &OutputWindow::close);//После закрытия приложения все окна тоже должны быть закрыты
        MACROSHOW(outputWindow );
    }
    else
        on_actionChangeServer_triggered();
}

void UnicalcClientLib::UnicalcClient::on_actionViewUniFile_triggered()
{
    UniFileViewer * uniFileViewer = new UniFileViewer(problem->getUNIFile());
    connect(this, &UnicalcClient::destroyed, uniFileViewer , &UniFileViewer::close);//После закрытия приложения все окна тоже должны быть закрты
    MACROSHOW(uniFileViewer);
}
