#include "Headers/mainwindow.h"
#include<QFile>
#include <QApplication>
#include "Headers/databasehelper.h"

QString readTextFile(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        return in.readAll();
    }

    return "";
}
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QString qss = readTextFile(":/mystylesheet.qss");

    if(qss.length() > 0)
    {
        a.setStyleSheet(qss);
    }
    else
    {
        qDebug()<<"No Style Sheet";
    }
    MainWindow w;
    w.show();
    return a.exec();
}
