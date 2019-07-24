/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#pragma once//  -- какой-то баг. если снять комментарий предупреждение должно исчезнуть
#ifndef UNICALCCLIENT_HPP
#define UNICALCCLIENT_HPP

#include "Global.hpp"

#include "ConnectionDialog.hpp"
#include "UCProblemsFactory.hpp"
#include "UCProblem.hpp"
#include <QMainWindow>
#include <QDialog>
#include <QtWebSockets/QWebSocket>
#include <QString>

namespace UnicalcClientLib
{
    namespace DoNotTouchItInYourProgram
    {
        namespace Ui
        {
            class UCMainWindow;
        }
    }

    class UCProblemsFactory;//или поймаем неизвестный тип
    class UCProblem;

    class UNICALCCLIENTLIB_EXPORT UnicalcClient: public QMainWindow
    {
        Q_OBJECT
    public://strinf name
        explicit UnicalcClient(const UCProblemsFactory &generator, const QUrl& defaultServerUrl, QWidget *parent = nullptr);
        ~UnicalcClient();
    private:
        bool loadUniFile(const QString& path, QString& uniFile);
        bool confirmLoadFile(const QString& uniFile);
    private slots:
        void enableActionCalculate();
        void onConnected();
        void onDisconnected();
        void onError();
        void onAboutDialogDestroyed();
        void onConnectionDialogDestroyed();
        void connectToServer(const QUrl& serverUrl);

        //все эти действия могут быть вызваны только когда они доступны
        void on_actionMewModel_triggered();
        void on_actionLoadFromDisk_triggered();
        void on_actionExit_triggered();
        void on_actionChangeServer_triggered();
        void on_actionOpenSettings_triggered();//не проверял
        void on_actionOpenUserGuide_triggered();//не проверял
        void on_actionOpenAbout_triggered();
        void on_actionSave_triggered();
        void on_actionSaveAs_triggered();
        void on_actionOpenExample_triggered();
        void on_actionCalculate_triggered();
        void on_actionViewUniFile_triggered();

    private:

        DoNotTouchItInYourProgram::Ui::UCMainWindow *ui;
        UCProblem * problem;
        bool connected;//isValid() не работает в веб версии в Qt5.12.0
        QUrl serverUrl;
        QWebSocket server;
        QString connectionsHistory, lastUniFilePath;
        //чтобы не открывать по нескольку копий этих диалогов запоминаем открытый
        DoNotTouchItInYourProgram::ConnectionDialog * connectionDialogPtr;
        QDialog* aboutDialogPtr;
    };

}

#endif // UNICALCCLIENT_HPP
