#include "mainwindow.h"
#include <QApplication>

EvaluationContext pro_data;
QMap<int, QString> basic_program;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
