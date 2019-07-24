/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef QSTRINGTABLEMODEL_HPP
#define QSTRINGTABLEMODEL_HPP

#include <QAbstractTableModel>
#include <QVariant>
#include <QString>

#define MAXDIM 99

class QStringTableModel : public QAbstractTableModel//переделать бы в SimpleTableModel
{
public:
    QStringTableModel(bool editable);//переделаь бы на enum

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role  = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;

    //будем вставлять и удалять только последний, данные не затираем(если снова увеличить размер таблицы, то все станет снова видимым)
    bool insertRows(int raw, int count, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int raw, int count, const QModelIndex &index = QModelIndex()) override;

    bool insertColumns(int column, int count, const QModelIndex &index = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &index = QModelIndex()) override;

    void setTableModelSize(int newRows, int newCollums);
    void resetData();
private:
    int rows, columns;
    bool editable;
    QString d[MAXDIM][2*MAXDIM];//самая простая реализация без наворотов. Нужно такой размера из-за таблицы ix
};

#endif // QSTRINGTABLEMODEL_HPP


//наследники по своему должны называть строки и столбцы
// QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

/*
 * QVariant QStringTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Horizontal = 0x1,
    //Vertical = 0x2
    if (role == Qt::DisplayRole)
    {
         return QString("Header #%1").arg(section);
    }
}
*/
