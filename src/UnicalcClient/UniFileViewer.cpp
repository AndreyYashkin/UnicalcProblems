/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "UniFileViewer.hpp"
#include "ui_UniFileViewer.h"

//#include <LineNumberArea>

using namespace UnicalcClientLib;
using namespace DoNotTouchItInYourProgram;

UniFileViewer::UniFileViewer(const QString& uniFile, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UniFileViewer)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->uniFileText->setPlainText(uniFile);
}

UniFileViewer::~UniFileViewer()
{
    delete ui;
}