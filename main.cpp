#include "dialog.h"
#include <QApplication>
#include <QTextCodec>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font = a.font();
    font.setFamily("YaHei Consolas Hybrid");
    a.setFont(font);

    //解决中文乱码
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    Dialog w;
    w.show();

    return a.exec();
}
