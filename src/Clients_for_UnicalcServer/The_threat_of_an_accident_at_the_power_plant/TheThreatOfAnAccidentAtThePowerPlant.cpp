/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "TheThreatOfAnAccidentAtThePowerPlant.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include "OutputWidget.hpp"
#include "AboutDialog.hpp"

using namespace UnicalcClientLib;

TheThreatOfAnAccidentAtThePowerPlant::TheThreatOfAnAccidentAtThePowerPlant(UnicalcClient *parentOfWidgets):
    UCProblem(QString::fromUtf8("Угроза «Авария на электростанции»"), parentOfWidgets)
{
    ex.insert(0, Example("Пример 1", ":/res/examples/example1"));
}

TheThreatOfAnAccidentAtThePowerPlant::~TheThreatOfAnAccidentAtThePowerPlant()
{
}

QWidget* TheThreatOfAnAccidentAtThePowerPlant::getInputWidget()
{
    return &in;
}

QWidget* TheThreatOfAnAccidentAtThePowerPlant::createOutputWidget(const QString& valFile, QWidget *parent) const
{
    return new OutputWidget(valFile, parent);
}

QDialog* TheThreatOfAnAccidentAtThePowerPlant::createAboutDialog() const
{
    return new AboutDialog();
}

bool TheThreatOfAnAccidentAtThePowerPlant::isAboutDialogAvailable() const
{
    return true;
}

void TheThreatOfAnAccidentAtThePowerPlant::setUNIFile(const QString& UniFile, FileType fileType)
{
    in.setUNIFile(UniFile, fileType);
}

QString TheThreatOfAnAccidentAtThePowerPlant::getUNIFile() const
{
   return in.getUNIFile();
}

void TheThreatOfAnAccidentAtThePowerPlant::resetUNIFile()
{
    in.resetUNIFile();
}

const ExamplesList& TheThreatOfAnAccidentAtThePowerPlant::getExamplesList() const
{
    return ex;
}
