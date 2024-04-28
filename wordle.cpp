#include "wordle.h"
#include "ui_wordle.h"

Wordle::Wordle(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Wordle)
{
    ui->setupUi(this);
}

Wordle::~Wordle()
{
    delete ui;
}

