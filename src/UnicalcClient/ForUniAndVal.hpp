/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef FORUNIANDVAL_HPP
#define FORUNIANDVAL_HPP

#include "Global.hpp"
#include <QString>
#include <QList>

namespace UnicalcClientLib
{
    using  IntList = QList<int>;

    enum UNICALCCLIENTLIB_EXPORT FileType {UNI, VAL};
    UNICALCCLIENTLIB_EXPORT const QString valueOf(QString par, const QString& file, FileType fileType, int numberOfSymbolsAfterPoint=-1);

    UNICALCCLIENTLIB_EXPORT void cutFractionalPart(QString& str, int numberOfSymbolsAfterPointInOutput);


   // UNICALCCLIENTLIB_EXPORT QString valueOf1(const QString& file,  const QString& par, IntList list, FileType fileType, int numberOfSymbolsAfterPoint);
}

#endif // FORUNIANDVAL_HPP
