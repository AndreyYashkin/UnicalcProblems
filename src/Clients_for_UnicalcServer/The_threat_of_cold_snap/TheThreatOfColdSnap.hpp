/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#pragma once
#ifndef THETHREATOFCOLDSNAP
#define THETHREATOFCOLDSNAP

#include "UnicalcClient/UCProblem.hpp"
#include "InputWidget.hpp"



class TheThreatOfColdSnap: public UnicalcClientLib::UCProblem
{
public:
    explicit TheThreatOfColdSnap(UnicalcClientLib::UnicalcClient *parentOfWidgetsPtr);//добавить ввод числа симоволов после запятой. А вообще это хороший пример для диалога настроек
    ~TheThreatOfColdSnap() override;

    void setUNIFile(const QString& file, UnicalcClientLib::FileType fileType = UnicalcClientLib::FileType::UNI) override;
    QString getUNIFile() const override;
    void resetUNIFile() override;

    QWidget* getInputWidget() override;
    QWidget* createOutputWidget(const QString& valFile, QWidget *parent) const override;

    QDialog* createAboutDialog() const override;
    bool isAboutDialogAvailable() const override;

    const UnicalcClientLib::ExamplesList& getExamplesList() const override;
private:
    InputWidget in;
    UnicalcClientLib::ExamplesList ex;
};

#endif // THETHREATOFACOLDSNAP

