/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "TheThreatOfColdSnap.hpp"
#include "OutputWidget.hpp"
#include "AboutDialog.hpp"

using namespace UnicalcClientLib;

TheThreatOfColdSnap::TheThreatOfColdSnap(UnicalcClient *parentOfWidgets):
    UCProblem("Угроза похолодания", parentOfWidgets)
{
    ex.insert(0, Example(QString::fromUtf8("Пример 1"), ":/res/examples/example1"));
}

TheThreatOfColdSnap::~TheThreatOfColdSnap()
{
}

QWidget* TheThreatOfColdSnap::getInputWidget()
{
    return &in;
}

QWidget* TheThreatOfColdSnap::createOutputWidget(const QString& valFile, QWidget *parent) const
{
    return new OutputWidget(valFile, parent);
}

void TheThreatOfColdSnap::setUNIFile(const QString& file, FileType fileType)
{
    in.setUNIFile(file, fileType);
}

QString TheThreatOfColdSnap::getUNIFile() const
{
   return in.getUNIFile();
}

void TheThreatOfColdSnap::resetUNIFile()
{
    in.resetUNIFile();
}

QDialog* TheThreatOfColdSnap::createAboutDialog() const
{
    return new AboutDialog();
}

bool TheThreatOfColdSnap::isAboutDialogAvailable() const
{
    return true;
}

const ExamplesList& TheThreatOfColdSnap::getExamplesList() const
{
    return ex;
}
