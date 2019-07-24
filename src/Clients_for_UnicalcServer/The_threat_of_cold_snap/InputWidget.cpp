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


InputWidget::InputWidget(QWidget *parent) : QWidget(parent),
  ui(new Ui::InputWidget)
{
    ui->setupUi(this);
}

InputWidget::~InputWidget()
{
    delete ui;
}

void InputWidget::setUNIFile(const QString& UniFile, UnicalcClientLib::FileType fileType)
{

    using namespace UnicalcClientLib;

    ui->January->setValue(valueOf("tsr[0]", UniFile, fileType, 3).toDouble());
    ui->February->setValue(valueOf("tsr[1]",  UniFile, fileType, 3).toDouble());
    ui->March->setValue(valueOf("tsr[2]", UniFile, fileType, 3).toDouble());
    ui->April->setValue(valueOf("tsr[3]", UniFile, fileType, 3).toDouble());
    ui->May->setValue(valueOf("tsr[4]", UniFile, fileType, 3).toDouble());
    ui->June->setValue(valueOf("tsr[5]", UniFile, fileType, 3).toDouble());
    ui->July->setValue(valueOf("tsr[6]", UniFile, fileType, 3).toDouble());
    ui->August->setValue(valueOf("tsr[7]", UniFile, fileType, 3).toDouble());
    ui->September->setValue(valueOf("tsr[8]", UniFile, fileType, 3).toDouble());
    ui->October->setValue(valueOf("tsr[9]", UniFile, fileType, 3).toDouble());
    ui->November->setValue(valueOf("tsr[10]", UniFile, fileType, 3).toDouble());
    ui->February->setValue(valueOf("tsr[11]", UniFile, fileType, 3).toDouble());

    ui->month->setCurrentIndex(valueOf("i", UniFile, fileType, 3).toInt()-1);
    ui->tvn->setText(valueOf("tvn", UniFile, fileType, 3));
    ui->dt->setText(valueOf(QString("dt[%1]").arg(ui->month->currentIndex() +1), UniFile, fileType, 3));//месяц только что записали
    ui->bnt->setText(valueOf("bnt", UniFile, fileType, 3));
    ui->dbt->setText(valueOf("dbt", UniFile, fileType, 3));
    ui->dtk->setText(valueOf("dtk", UniFile, fileType, 3));
    ui->dtt->setText(valueOf("dtt", UniFile, fileType, 3));
    ui->bdeft->setText(valueOf("bdeft", UniFile, fileType, 3));
    ui->bne->setText(valueOf("bne", UniFile, fileType, 3));
    ui->dbe->setText(valueOf("dbe", UniFile, fileType, 3));
    ui->det->setText(valueOf("det", UniFile, fileType, 3));
    ui->bdefe->setText(valueOf("bdefe", UniFile, fileType, 3));
}

QString InputWidget::getUNIFile() const
{
    QString uni;
    uni+=QString("tsr[0] = %1;\n").arg(ui->January->value());
    uni+=QString("tsr[1] = %1;\n").arg(ui->February->value());
    uni+=QString("tsr[2] = %1;\n").arg(ui->March->value());
    uni+=QString("tsr[3] = %1;\n").arg(ui->April->value());
    uni+=QString("tsr[4] = %1;\n").arg(ui->April->value());
    uni+=QString("tsr[5] = %1;\n").arg(ui->May->value());
    uni+=QString("tsr[6] = %1;\n").arg(ui->June->value());
    uni+=QString("tsr[7] = %1;\n").arg(ui->July->value());
    uni+=QString("tsr[8] = %1;\n").arg(ui->September->value());
    uni+=QString("tsr[9] = %1;\n").arg(ui->October->value());
    uni+=QString("tsr[10] = %1;\n").arg(ui->November->value());
    uni+=QString("tsr[11] = %1;\n").arg(ui->February->value());

    ;
    uni+=QString("i = %1;\n").arg(ui->month->currentIndex()+1);//месяц
    uni+=QString("tvn = %1;\n").arg(ui->tvn->text());//расчетная температура воздуха отапливаемых помещений
    uni+=QString("dt[%1] = %2;\n").arg(ui->month->currentIndex()+1).arg(ui->dt->text());//отклонение среднемесячной наружной в m-ом месяце
    uni+=QString("bnt = %1;\n").arg(ui->bnt->text());//нормальное потребление теплоэнергии
    uni+=QString("dbt = %1;\n").arg(ui->dbt->text());//изменения потребления теплоэнергии %
    uni+=QString("dtk = %1;\n").arg(ui->dtk->text());//увеличение выработки тепла на котельных
    uni+=QString("dtt = %1;\n").arg(ui->dtt->text());//увеличение выработки тепла на ТЭЦ
    uni+=QString("bdeft = %1;\n").arg(ui->bdeft->text());//дефицит теплоэнергии
    uni+=QString("bne = %1;\n").arg(ui->bne->text());//нормальное потребление электроэнергии
    uni+=QString("dbe = %1;\n").arg(ui->dbe->text());//изменения потребления электроэнергии %
    uni+=QString("det = %1;\n").arg(ui->det->text());//увеличение выработки электроэнергии на ТЭС
    uni+=QString("bdefe = %1;\n").arg(ui->bdefe->text());//дефицит электроэнергии

    uni+=QString("dbt= dt[i]/( tvn- tsr[i]);\ndbe=0.3*dbt;\nbdeft= bnt* dbt - dtk - dtt;\nbdefe= bne* dbe - det;\n");
    return uni;
}

void InputWidget::resetUNIFile()
{
    ui->January->setValue(0);
    ui->February->setValue(0);
    ui->March->setValue(0);
    ui->April->setValue(0);
    ui->May->setValue(0);
    ui->June->setValue(0);
    ui->July->setValue(0);
    ui->August->setValue(0);
    ui->September->setValue(0);
    ui->October->setValue(0);
    ui->November->setValue(0);
    ui->February->setValue(0);

    ui->month->setCurrentIndex(0);
    ui->tvn->setText("");
    ui->dt->setText("");
    ui->bnt->setText("");
    ui->dbt->setText("");
    ui->dtk->setText("");
    ui->dtt->setText("");
    ui->bdeft->setText("");
    ui->bne->setText("");
    ui->dbe->setText("");
    ui->det->setText("");
    ui->bdefe->setText("");
}

