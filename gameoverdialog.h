#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
class GameOverDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GameOverDialog(QWidget *parent = nullptr);
    explicit GameOverDialog(QWidget * parent, bool didPlayerWin, const QString & correctWord);
    ~GameOverDialog();

signals:
    void restartRequested();
    void quitRequested();
private:
    QPushButton * restartButton;
    QPushButton * quitButton;
private slots:
    void restartGame();
    void quitGame();


};

#endif // GAMEOVERDIALOG_H
