#include "gameoverdialog.h"

GameOverDialog::GameOverDialog(QWidget *parent, bool didPlayerWin, const QString & correctWord) : QDialog(parent)
{
    setWindowTitle("Game Over");

        restartButton = new QPushButton("Restart");
        quitButton = new QPushButton("Quit");

        QVBoxLayout *layout = new QVBoxLayout;
        if (didPlayerWin) {
             layout->addWidget(new QLabel("Congratulations! You Won!"));
             layout->addWidget(new QLabel("Play again?"));

         } else {
             layout->addWidget(new QLabel("Game Over!"));
             layout->addWidget(new QLabel("Correct Word was: " + correctWord));
             layout->addWidget(new QLabel("Better luck next time."));
         }
        layout->addWidget(restartButton);
        layout->addWidget(quitButton);

        setLayout(layout);

        connect(restartButton, &QPushButton::clicked, this, &GameOverDialog::restartGame);
        connect(quitButton, &QPushButton::clicked, this, &GameOverDialog::quitGame);
}
void GameOverDialog::restartGame() {
    emit restartRequested();
    close();
}
void GameOverDialog::quitGame() {
    emit quitRequested();
}
GameOverDialog::~GameOverDialog()
{
}
