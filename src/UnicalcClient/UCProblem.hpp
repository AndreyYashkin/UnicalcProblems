/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#pragma once// -- какой-то баг. если снять комментарий предупреждение должно исчезнуть
#ifndef UCPROBLEM_HPP
#define UCPROBLEM_HPP

#include "Global.hpp"

#include <QWidget>
#include <QDialog>
#include <QString>
#include "Example.hpp"
#include "UCProblemsFactory.hpp"
#include "ForUniAndVal.hpp"

namespace UnicalcClientLib
{

    class UnicalcClient;//или поймаем неизвестный тип


    class UNICALCCLIENTLIB_EXPORT UCProblem//: public QObject
    {
    //Q_OBJECT
    public:
        virtual ~UCProblem();
        //модель
        virtual void setUNIFile(const QString& file, FileType fileType = FileType::UNI) = 0;
        virtual void resetUNIFile() = 0;//чистить только содержимое таблиц//переименовать в reset problem?
        virtual QString getUNIFile() const = 0;

        //жизненноважные виджеты. Без ниих работать никак нельзя
        virtual QWidget* getInputWidget() = 0;//должен возвращать указатель, а не создавать новый виджет 
        virtual QWidget* createOutputWidget(const QString& valFile, QWidget *parent) const = 0;//родитель другое окно, поэтому нужно указывать

        //отсутствие этих частей можно пережить ... на начальном этапе.
        //тут тоже надо как-то позаботится об отсутствии утечек памяти при close и закрытии
        virtual QDialog* createUserGuideDialog() const;
        virtual QDialog* createAboutDialog() const;
        virtual QDialog* createSettingsDialog();

        virtual bool isUserGuideDialogAvailable() const;
        virtual bool isAboutDialogAvailable() const;
        virtual bool isSettingsDialogAvailable() const;

        virtual const ExamplesList& getExamplesList() const;

        const QString& getProblemName() const;

    protected:
        explicit UCProblem(const QString& UCProblemName, UnicalcClient *unicalcClientPtr);

    protected:
        /*
         * UnicalcClient необходимо использовать как родитель всего, кроме OutputWidget
         * Если методам явно передавать указатель на родителя, то возникает вопрос как это совместить с единственностью inputWidget'а
         */
        UnicalcClient * const unicalcClientPtr;
        const ExamplesList examlesList;
    private:
        const QString UCProblemName;

    };

}

#endif // UCPROBLEM_HPP
