/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "InputWidget.hpp"
#include "ui_InputWidget.h"

#include "DisplayTableWidget.hpp"
#include "SpinBoxDelegate.hpp"
#include "UnicalcClient/ForUniAndVal.hpp"
#include "UnicalcClient/Global.hpp"

InputWidget::InputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputWidget),
    m(1), n(1), t(1),
    aTable(new QStringTableModel(true)), ipTable(new QStringTableModel(true)), bTable(new QStringTableModel(true)), ixTable(new QStringTableModel(true)), iuTable(new QStringTableModel(true)), ifTable(new QStringTableModel(true))
{
    //aDialog=ipDialog=bDialog=ixDialog=iuDialog=ifDialog=nullptr;
    ui->setupUi(this);
}

InputWidget::~InputWidget()
{
    delete ui;

    delete aTable;
    delete ipTable;
    delete bTable;
    delete ixTable;
    delete iuTable;
    delete ifTable;
}

void InputWidget::setUNIFile(const QString& UniFile, UnicalcClientLib::FileType fileType)
{
    using namespace UnicalcClientLib;
    aTable->resetData();
    ipTable->resetData();
    bTable->resetData();
    ixTable->resetData();
    iuTable->resetData();
    ifTable->resetData();

    ui->mSpinBox->setValue(valueOf("M", UniFile, fileType).toInt());
    ui->nSpinBox->setValue(valueOf("N", UniFile, fileType).toInt());
    ui->tSpinBox->setValue(valueOf("T", UniFile, fileType).toInt());

    onQtyChanged();//подгоняем размер таблиц

    for(int i=0; i<m; i++)
        aTable->setData(aTable->index(i, 0), valueOf(QString("a[%1]").arg(i+1), UniFile, fileType));

    for(int i=0; i<n; i++)
        bTable->setData(bTable->index(i, 0), valueOf(QString("b[%1]").arg(i+1), UniFile, fileType));

    for(int i=0; i<n; i++)
        for(int j=0; j<n+m;j++)
            ixTable->setData(ixTable->index(i, j), valueOf(QString("ix[%1,%2]").arg(i+1).arg(j+1), UniFile, fileType));

    for(int i=0; i<m; i++)
       ipTable->setData(ipTable->index(i, 0), valueOf(QString("ip[%1]").arg(i+1), UniFile, fileType));

    for(int i=0; i<t; i++)
        iuTable->setData(iuTable->index(i, 0), valueOf(QString("iu[%1]").arg(i+1), UniFile, fileType));

    for(int i=0; i<n; i++)
        for(int j=0; j<t;j++)
           ifTable->setData(ifTable->index(i, j), valueOf(QString("if[%1,%2]").arg(i+1).arg(j+1), UniFile, fileType));
}

QString InputWidget::getUNIFile() const
{
    QString uni;
    uni+=QString("M = %1;\n").arg(m);
    for(int i=0; i<m; i++)
        uni+=QString("a[%1] = %2;\n").arg(i+1).arg(aTable->data(aTable->index(i, 0)).toString());
    uni+="sumA=sum( i = 1, 1, M; a[i] );\n";

    uni+=QString("N = %1;\n").arg(n);
    for(int i=0; i<n; i++)
        uni+=QString("b[%1] = %2;\n").arg(i+1).arg(bTable->data(bTable->index(i, 0)).toString());
    uni+="sumB=sum( i = 1, 1, N; b[i] );\nsumA=sumB;\n";

    for(int i=0; i<n; i++)
        for(int j=0; j<n+m;j++)
            uni+=QString("ix[%1,%2] = %3;\n").arg(i+1).arg(j+1).arg(ixTable->data(ixTable->index(i, j)).toString());
    for(int i=0; i<m; i++)
        uni+=QString("ip[%1] = %2;\n").arg(i+1).arg(ipTable->data(ipTable->index(i, 0)).toString());

    uni+=QString("T = %1;\n").arg(t);
    for(int i=0; i<t; i++)
        uni+=QString("iu[%1] = %2;\n").arg(i+1).arg(iuTable->data(iuTable->index(i, 0)).toString());
    uni+="for ( j = 1, 1, M;\nsum( i= 1, 1, N; ix[i,j+N]) = ( 1-ip[j] ) *a[j] );\n";

    for(int i=0; i<n; i++)
        for(int j=0; j<t;j++)
            uni+=QString("if[%1,%2] = %3;\n").arg(i+1).arg(j+1).arg(ifTable->data(ifTable->index(i, j)).toString());

    uni+="for (i = 1, 1, N;for( j = 1, 1, T;\niy[i,j] = [0,100] ) );\n";
    uni+="sum( i= 1, 1, T; iu[i] ) = sum( j = 1, 1, M; ip[j] *a[j] );\n";
    uni+="for (i = 1, 1, T;\nsum( j = 1, 1, N; iy[j,i] ) = iu[i] );\n";
    uni+="for (i = 1, 1, N;\nfor( j = 1, 1, T;  iz[i,j] = [0,1] ) );\n";
    uni+="for (i = 1, 1, N;\nfor( j = 1, 1, T; iy[i,j]=0 -> iz[i,j] = 0 ) );\n";
    uni+="for (i = 1, 1, N;\nfor( j = 1, 1, T; iy[i,j]>0 -> iz[i,j] = 1 ) );\n";
    uni+="for (i = 1, 1, N;\nb[i] + sum( j = 1, 1, N; ix[j,i] )  = sum( j = 1, 1, N+M; ix[i,j] ) +  sum( j = 1, 1, T; iy[i,j] ) );\n";
    uni+="igoal= sum( i= 1, 1, N;\nsum(  j=  1, 1, T; if[i,j]*iz[i,j] ) );\n";
    uni+="igoal>=0;\n";
    return uni;
}

void InputWidget::resetUNIFile()
{
    aTable->resetData();
    ipTable->resetData();
    bTable->resetData();
    ixTable->resetData();
    iuTable->resetData();
    ifTable->resetData();
}



void InputWidget::onQtyChanged()
{
    m=ui->mSpinBox->value();
    n=ui->nSpinBox->value();
    t=ui->tSpinBox->value();
    //начинаем подгонять размеры матриц под новые реали
    aTable->setTableModelSize(m, 1);
    ipTable->setTableModelSize(m, 1);
    bTable->setTableModelSize(n, 1);
    ixTable->setTableModelSize(n, n+m);
    iuTable->setTableModelSize(t, 1);
    ifTable->setTableModelSize(n, t);
}

void InputWidget::onOpenDisplayTableDialogPushed()
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
        SpinBoxDelegate *dd= new SpinBoxDelegate(1, 5, d);//parent=d -> удалится сам с d
        d->setDelegate(dd);
    }
    connect(this, &QObject::destroyed, d, &QWidget::close);
    UnicalcClientLib::MACROSHOW(d);
}
