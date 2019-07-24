/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "OutputWidget.hpp"
#include "ui_OutputWidget.h"

#include "DisplayTableWidget.hpp"
#include "UnicalcClient/ForUniAndVal.hpp"
#include "UnicalcClient/Global.hpp"

OutputWidget::OutputWidget(const QString &ValFile, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputWidget),
    aTable(new QStringTableModel(false)), ipTable(new QStringTableModel(false)), bTable(new QStringTableModel(false)), ixTable(new QStringTableModel(false)), iuTable(new QStringTableModel(false)), ifTable(new QStringTableModel(false))
{
    ui->setupUi(this);
    int m, n, t;
    using namespace UnicalcClientLib;
    m=valueOf("M", ValFile, FileType::VAL).toInt();
    n=valueOf("N", ValFile, FileType::VAL).toInt();
    t=valueOf("T", ValFile, FileType::VAL).toInt();

    ui->m->setText(valueOf("M", ValFile, FileType::VAL));
    ui->n->setText(valueOf("N", ValFile, FileType::VAL));
    ui->t->setText(valueOf("T", ValFile, FileType::VAL));

    //подгоняем размер таблиц
    aTable->setTableModelSize(m, 1);
    ipTable->setTableModelSize(m, 1);
    bTable->setTableModelSize(n, 1);
    ixTable->setTableModelSize(n, n+m);
    iuTable->setTableModelSize(t, 1);
    ifTable->setTableModelSize(n, t);

    for(int i=0; i<m; i++)
        aTable->setData(aTable->index(i, 0), valueOf(QString("a[%1]").arg(i+1), ValFile, FileType::VAL, 3));

    for(int i=0; i<n; i++)
        bTable->setData(bTable->index(i, 0), valueOf(QString("b[%1]").arg(i+1), ValFile, FileType::VAL, 3));

    for(int i=0; i<n; i++)
        for(int j=0; j<n+m;j++)
            ixTable->setData(ixTable->index(i, j), valueOf(QString("ix[%1,%2]").arg(i+1).arg(j+1), ValFile, FileType::VAL, 3));

    for(int i=0; i<m; i++)
       ipTable->setData(ipTable->index(i, 0), valueOf(QString("ip[%1]").arg(i+1), ValFile, FileType::VAL, 3));

    for(int i=0; i<t; i++)
        iuTable->setData(iuTable->index(i, 0), valueOf(QString("iu[%1]").arg(i+1), ValFile, FileType::VAL, 3));

    for(int i=0; i<n; i++)
        for(int j=0; j<t;j++)
           ifTable->setData(ifTable->index(i, j), valueOf(QString("if[%1,%2]").arg(i+1).arg(j+1), ValFile, FileType::VAL));
}

OutputWidget::~OutputWidget()
{
    delete ui;

    delete aTable;
    delete ipTable;
    delete bTable;
    delete ixTable;
    delete iuTable;
    delete ifTable;
}

void OutputWidget::onOpenDisplayTableDialogPushed()
{
    DisplayTableWidget *d;
    QString title, info;
    if(sender()==ui->aToolButton)
    {
        title="Объемы производства энергоресурсов";
        info="Объемы производства энергоресурсов(a[i]), тыс кВт*ч";
        d=new DisplayTableWidget(title, info, aTable);
    }
    else if(sender()==ui->ipToolButton)
    {
        title="Вероятности аварии у производителей ресурсов";
        info="Безразмерные вероятности аварии у производителей ресурсов(ip[i])";
        d=new DisplayTableWidget(title, info, ipTable);
    }
    else if(sender()==ui->bToolButton)
    {
        title="Объемы потребления энергоресурсов";
        info="Объемы потребления энергоресурсов(b[i]), тыс кВт*ч";
        d=new DisplayTableWidget(title, info, bTable);
    }
    else if(sender()==ui->ixToolButton)
    {
        title="Объемы энергоресурсов, поставляемых потребителям";
        info="Объемы энергоресурсов, поставляемых потребителям(ix[i,j]), тыс кВт*ч";
        d=new DisplayTableWidget(title, info, ixTable);
    }
    else if(sender()==ui->iuToolButton)
    {
        title="Объемы энергоресурсов в резервных хранилищах";
        info="Объемы энергоресурсов в резервных хранилищах(iu[i]), тыс кВт*ч";
        d=new DisplayTableWidget(title, info, iuTable);
    }
    else //if(sender()==ui->ifToolButton)
    {
        title="Веса связей между потребителями, поставщиками и пунктами резервного хранения";
        info="Веса (приоритеты) связей между потребителями, поставщиками и пунктами резервного хранения ;\nВведем веса по 5-бальной шкале:\n1-очень удобно; 2-удобно; 3-ни то ни сё; 4-неудобно; 5-очень неудобно";
        d=new DisplayTableWidget(title, info, ifTable);
    }
    connect(this, &QObject::destroyed, d, &QWidget::close);
    UnicalcClientLib::MACROSHOW(d);
}
