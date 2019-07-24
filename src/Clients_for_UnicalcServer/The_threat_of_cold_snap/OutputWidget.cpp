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

#include "UnicalcClient/ForUniAndVal.hpp"

OutputWidget::OutputWidget(const QString &ValFile, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputWidget)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    using namespace UnicalcClientLib;
    int i=valueOf("i", ValFile, FileType::VAL).toInt();
    switch(i)
    {
        case 1: ui->month->setText("Январь"); break;
        case 2: ui->month->setText("Февраль"); break;
        case 3: ui->month->setText("Март"); break;
        case 4: ui->month->setText("Апрель"); break;
        case 5: ui->month->setText("Май"); break;
        case 6: ui->month->setText("Июнь"); break;
        case 7: ui->month->setText("Июль"); break;
        case 8: ui->month->setText("Август"); break;
        case 9: ui->month->setText("Сентябрь"); break;
        case 10: ui->month->setText("Октябрь"); break;
        case 11: ui->month->setText("Ноябрь"); break;
        case 12: ui->month->setText("Декабрь"); break;
    }
    ui->tsr->setText(valueOf(QString("tsr[%1]").arg(i), ValFile, FileType::VAL, 3));
    ui->tvn->setText(valueOf("tvn", ValFile, FileType::VAL, 3));
    ui->dt->setText(valueOf(QString("dt[%1]").arg(i), ValFile, FileType::VAL, 3));//месяц только что записали
    ui->bnt->setText(valueOf("bnt", ValFile, FileType::VAL, 3));
    ui->dbt->setText(valueOf("dbt", ValFile, FileType::VAL, 3));
    ui->dtk->setText(valueOf("dtk", ValFile, FileType::VAL, 3));
    ui->dtt->setText(valueOf("dtt", ValFile, FileType::VAL, 3));
    ui->bdeft->setText(valueOf("bdeft", ValFile, FileType::VAL, 3));
    ui->bne->setText(valueOf("bne", ValFile, FileType::VAL, 3));
    ui->dbe->setText(valueOf("dbe", ValFile, FileType::VAL, 3));
    ui->det->setText(valueOf("det", ValFile, FileType::VAL, 3));
    ui->bdefe->setText(valueOf("bdefe", ValFile, FileType::VAL, 3));
}

OutputWidget::~OutputWidget()
{
    delete ui;
}

