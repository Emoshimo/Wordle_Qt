#include "wordle.h"
#include <QApplication>
#include <ctime>
#include <cstdlib>
int main(int argc, char *argv[])
{
        srand((unsigned int)time(NULL));
        QApplication a(argc, argv);
        Wordle w;
        w.show();
        return a.exec();
}
