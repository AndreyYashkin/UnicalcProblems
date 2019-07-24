/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "QStringTableModel.hpp"

QStringTableModel::QStringTableModel(bool editable):
    rows(1), columns(1), editable(editable)
{
}

int QStringTableModel::rowCount(const QModelIndex &) const
{
    return rows;
}

int QStringTableModel::columnCount(const QModelIndex &) const
{
    return columns;
}

QVariant QStringTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();
    if (index.row() >= rows || index.column() >= columns)
            return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return d[index.row()][index.column()];
    }
    return QVariant();
}

Qt::ItemFlags QStringTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    if(editable)
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    else
        return QAbstractItemModel::flags(index);
}

bool QStringTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        d[index.row()][index.column()]=value.toString();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool QStringTableModel::insertRows(int raw, int count, const QModelIndex &)
{
    if(raw!=rows-1)
        return false;
    beginInsertRows(QModelIndex(), raw, raw+count-1);
    rows+=count;
    endInsertRows();
    return true;
}

bool QStringTableModel::removeRows(int raw, int count, const QModelIndex &)
{
    if(raw+count!=rows-1)
        return false;
    beginRemoveRows(QModelIndex(), raw, raw+count-1);
    rows-=count;
    endRemoveRows();
    return true;
}

bool QStringTableModel::insertColumns(int column, int count, const QModelIndex &)
{
    if(column!=columns-1)
        return false;
    beginInsertColumns(QModelIndex(), column, column+count-1);
    columns+=count;
    endInsertColumns();
    return true;
}

bool QStringTableModel::removeColumns(int column, int count, const QModelIndex &)
{
    if(column+count!=columns-1)
        return false;
    beginRemoveColumns(QModelIndex(), column, column+count-1);
    columns-=count;
    endRemoveColumns();
    return true;
}


void QStringTableModel::setTableModelSize(int newRows, int newCollums)
{
    //вот если где-то тут return false, то это плохо
    if(rowCount() < newRows)
        insertRows(rowCount()-1, newRows-rowCount());
    else if(rowCount() > newRows)
        removeRows(newRows-1, rowCount()-newRows);

    if(columnCount() < newCollums)
         insertColumns(columnCount()-1, newCollums-columnCount());
    else if(columnCount() > newCollums)
        removeColumns(newCollums-1, columnCount()-newCollums);
}

void QStringTableModel::resetData()
{
    for(int i=0; i<MAXDIM; i++)
        for(int j=0; j<2*MAXDIM; j++)
            d[i][j]="";
}
