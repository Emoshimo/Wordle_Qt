#ifndef WORDLE_H
#define WORDLE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Wordle; }
QT_END_NAMESPACE

class Wordle : public QMainWindow
{
    Q_OBJECT

public:
    Wordle(QWidget *parent = nullptr);
    ~Wordle();

private:
    Ui::Wordle *ui;
};
#endif // WORDLE_H
