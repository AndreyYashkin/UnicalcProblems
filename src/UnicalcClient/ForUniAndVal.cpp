/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "ForUniAndVal.hpp"

#include <QRegularExpression>


const QString UnicalcClientLib::valueOf(QString par, const QString& file, FileType fileType, int numberOfSymbolsAfterPoint)
{
    QString separator;
    if(fileType==FileType::UNI)
        separator=";";
    else// if(fileType==FileType::VAL)
        separator="\n";
    par+= " = ";//приводим к виду в котором удобно искать
    int begining=file.indexOf(par)+par.length();
    int ending=file.indexOf(separator, begining);
    QString val=file.midRef(begining, ending-begining).toString();//это ровно выражение соотв par
    if(numberOfSymbolsAfterPoint!=-1)
        UnicalcClientLib::cutFractionalPart(val, numberOfSymbolsAfterPoint);
    return val;
}

void UnicalcClientLib::cutFractionalPart(QString& str, int numberOfSymbolsAfterPointInOutput)
{
    int i=0, j;
    while(i<str.length())//двигаемся по строке значения
    {
        i=str.indexOf('.', i);//найдем точку в слеледующем числе
        if(i==-1)
            break;
        for(j=i+1; '0' <= str[j+1] && str[j+1] <='9'; j++);//ищем конец числа

        if(j-i > numberOfSymbolsAfterPointInOutput)
            str.remove(i+1+numberOfSymbolsAfterPointInOutput, j-i-numberOfSymbolsAfterPointInOutput);//удаляем лишнее
         i++;
    }
}


/*QString UnicalcClientLib::valueOf1(const QString& file,  const QString& par, IntList list, FileType fileType, int numberOfSymbolsAfterPoint)
{
    QRegularExpression exp("");
    return QString();
}
*/
