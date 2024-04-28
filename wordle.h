#ifndef WORDLE_H
#define WORDLE_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLineEdit>
#include <QObject>
#include <QMessageBox>
#include <QTimer>
#include <QColor>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "wordmanager.h"
#include "gameoverdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Wordle; }
QT_END_NAMESPACE

class Wordle : public QMainWindow
{
    Q_OBJECT

public:
    explicit Wordle(QWidget *parent = nullptr);
    ~Wordle();
protected:
    void keyPressEvent(QKeyEvent * event) override {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            enterPressed();
            return;
        }

        // Check if Backspace key is pressed
        if (event->key() == Qt::Key_Backspace) {
            deletePressed();
            return;
        }

        // Check if it's a letter key
        if (event->text().length() == 1 && event->text().at(0).isLetter()) {
            QString letter = event->text().toUpper(); // Convert to uppercase letter
            processLetterInput(letter);
        }
    }


private:
    Ui::Wordle *ui;
    GameOverDialog *gameOverDialog;
    WordManager wordManager;
    QString correctWordQString;
    string correctWord;
    vector<string> validWords;
    string getAnswerWord();
    bool validateWord(const string word) const;
    bool canPressEnter();
    bool eventFilter(QObject* watched, QEvent * event);
    bool isEnglishLetter(QChar c) const;
    vector<QColor> compareWords(const string & userWord) const;
    void showNotification(const QString& message);
    void getNextLineEdit(QLineEdit * currentLineEdit);
    void modifyRowColors(const vector<QColor> & colors);
    void showGameOverDialog(const bool didPlayerWin);
    void processLetterInput (const QString & letter);


private slots:
    void handleTextChanged(const QString &text);
    void onLineEditTextEdited(const QString & text);
    void onLetterButtonClicked();
    void enterPressed();
    void deletePressed();
    void restartGame();


};
#endif // WORDLE_H
