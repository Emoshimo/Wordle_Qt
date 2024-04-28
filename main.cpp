#include "wordle.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Wordle w;
    w.show();
    return a.exec();
}
