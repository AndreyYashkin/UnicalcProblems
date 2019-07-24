/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "Global.hpp"
#include "OutputWindow.hpp"
#include "ui_OutputWindow.h"

#include <QMessageBox>

using namespace UnicalcClientLib;
using namespace DoNotTouchItInYourProgram;

OutputWindow::OutputWindow(UCProblem* problemPtr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputWindow),
    problemPtr(problemPtr),
    isWaitngForAnswer(true)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}

OutputWindow::~OutputWindow()
{
    if(isWaitngForAnswer)
        emit IsNotWaitingForServerAnswerAnymore();//надо сообщить клиенту, что ответа больше никто не ждет и можно разблокировать кнопку отправки модели на сервер
    else
        if(ui->stackedWidget->currentIndex()==Index::RESULTPAGE)
            if(ui->checkBox->isChecked())//пользователь поставил галачку экспортировать в inputWidget
                problemPtr->setUNIFile(val, FileType::VAL);
    delete ui;
}

void OutputWindow::onResultReceived(const QString& valFile)
{
    if(valFile.indexOf("Inconsistent system")!=-1 ||
            valFile.indexOf("Compiling errors")!=-1 ||
            /* Какие-то роблемы серверной части */
            valFile.indexOf("Can not write in uni file")!=-1 ||
            valFile.indexOf("Can not open val file")!=-1)
    {
        ui->errorView->setPlainText(valFile);
        ui->stackedWidget->setCurrentIndex(Index::ERRORPAGE);//покажем ошибку
    }
    else
    {
        val=valFile;//Запомним на сл, если понадобится экспортировать его в inputWidget
        ui->scrollArea->setWidget(problemPtr->createOutputWidget(valFile, ui->scrollArea));
        ui->stackedWidget->setCurrentIndex(Index::RESULTPAGE);//покажем реузьтат работы
    }
    isWaitngForAnswer=false;
    emit IsNotWaitingForServerAnswerAnymore();//UnicalcClient::unclockCalculateButton должен разорвать соединение сигнала и слота и больше в этот диалог ничего не отправлять
}

void OutputWindow::onDisconnected()
{
    if(isWaitngForAnswer)
    {
        //или лучше перевести на ERRORPAGE и сказать об этом там?
        QMessageBox *errorMessage= new QMessageBox(QMessageBox::Critical, "Опс!", "Соединение с сервером было потеряно до получения результатов вычислений.", QMessageBox::Close, this);
        MACROEXEC(errorMessage);
        emit close();//Результат вычислений на уже не получить...
    }
}
