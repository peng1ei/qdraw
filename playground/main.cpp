#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qSetMessagePattern("[%{type}] [%{file}] [%{line}] [%{function}] - %{message}");
    qSetMessagePattern("[%{function}] - %{message}");

    MainWindow w;
    w.showMaximized();
    return a.exec();
}
