/*
 * Copyright 2019 Andrey Yashkin <andreyyashkin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <QApplication>
#include "UnicalcClient/UnicalcClient.hpp"
#include "Factory.hpp"
#include "UnicalcClient/Global.hpp"

#include <QStyleFactory>
#include <QStringList>

using namespace UnicalcClientLib;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #if defined(Q_OS_WIN)
        QStringList availableStyles = QStyleFactory::keys();
        if(availableStyles.contains("Fusion"))
                   a.setStyle("Fusion");
        else if(availableStyles.contains("windowsvista"))
            a.setStyle("windowsvista");
        /*else
            a.setStyle("Windows");//Дефолтный стиль windows более напоминает xp или windows 95
        */
    #endif//defined(Q_OS_WIN)


    Q_INIT_RESOURCE(logo);//если библитоека статическая(а она статическая), без этого из нее могут не загрузиться ресурсы
    Q_INIT_RESOURCE(icons);

    Factory fac;
    //нельзя использовать не укзатель т.к UnicalcClient самоудаляется при событии close() и при при еще одном вызове неявного деструктора ничего хорошего не произойдет
    UnicalcClient *client= new UnicalcClient(fac, QUrl("ws://uniserv.iis.nsk.su:25565"));
    MACROSHOW(client);
    return a.exec();
}
