/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef OUTPUTWINDOW_HPP
#define OUTPUTWINDOW_HPP

#include <QWidget>
#include "UCProblem.hpp"

/*
 * Возможно, лучше разбить OutputWindow на несколько отдельных окон: вывод успшного результата, класс наследник QProgressDialog, и QMessageBox для вывода ошибки.
 */

namespace UnicalcClientLib
{
    namespace DoNotTouchItInYourProgram
    {
        namespace Ui
        {
            class OutputWindow;
        }

        class OutputWindow : public QWidget
        {//костыль отображения
            Q_OBJECT

        public:
            explicit OutputWindow(UCProblem* problemPtr, QWidget *parent = nullptr);//получает виджет вывода как-то... Через слот для его получения!
            ~OutputWindow();
        signals:
            void IsNotWaitingForServerAnswerAnymore();
        public slots:
            void onResultReceived(const QString& valFile);
            void onDisconnected();

        private:
            enum Index{WAITINGPAGE, ERRORPAGE, RESULTPAGE};
            Ui::OutputWindow *ui;
            UCProblem * const problemPtr;
            bool isWaitngForAnswer;
            QString val;
        };
    }
}

#endif // OUTPUTWINDOW_HPP
