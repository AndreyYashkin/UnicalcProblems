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
#ifndef INPUTWIDGET_HPP
#define INPUTWIDGET_HPP

#include <QWidget>

#include <QTableView>
#include <QAbstractTableModel>
#include "QStringTableModel.hpp"
#include "UnicalcClient/ForUniAndVal.hpp"



namespace Ui
{
    class InputWidget;
}

class InputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InputWidget(QWidget *parent = nullptr);
    ~InputWidget();

    void setUNIFile(const QString& UniFile, UnicalcClientLib::FileType fileType);
    QString getUNIFile() const;
    void resetUNIFile();

private slots:
    void onQtyChanged();
    void onOpenDisplayTableDialogPushed();
private:
    Ui::InputWidget *ui;
    int m, n, t;
    QStringTableModel *aTable, *ipTable, *bTable, *ixTable, *iuTable, *ifTable;
    //потом запомнаить указатели, чтобы не клепать клоны диалогов, если старый еще не закрыт
    //если сделать так, то можно при замене уни файлов таблицы удалять и ставить на их место новые, одновременно закрывая ока, которые на них смотрят
    //DisplayTableDialog *aDialog, *ipDialog, *bDialog, *ixDialog, *iuDialog, *ifDialog;
};

#endif // INPUTWIDGET_HPP
