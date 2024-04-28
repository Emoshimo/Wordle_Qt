#include "wordle.h"
#include "ui_wordle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QKeyEvent>
#include <iostream>
#include <algorithm>

int rowIndex = 1;
int colIndex = 1;
bool gameOver = false;
int guessTries = 0;
Wordle::Wordle(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Wordle)
    , wordManager("C:\\Users\\user\\Documents\\Qt Projects\\Wordle\\words.txt")
{
    ui->setupUi(this);
    //Connecting line edit inputs to text handle
    for (int row = 1; row < 7; ++row) {
        for (int col = 1; col < 6; ++col) {
            QString lineEditName = QString("lineEdit_%1_%2").arg(row).arg(col);
            QLineEdit * lineEdit = findChild<QLineEdit *>(lineEditName);

            lineEdit->setFocusPolicy(Qt::NoFocus);
            lineEdit->installEventFilter(this);
            connect(lineEdit, &QLineEdit::textChanged, this,
                    Wordle::handleTextChanged);
            connect(lineEdit, &QLineEdit::textEdited, this,
                    &Wordle::onLineEditTextEdited);

        }
    }

    // Connecting virtual keyboard
    for (char c = 'A'; c<='Z'; ++c) {
        QString letterButtonName = QString("Button_%1").arg(c);
        QPushButton * letterButton = findChild<QPushButton *>(letterButtonName);
        connect(letterButton, &QPushButton::clicked, this, &Wordle::onLetterButtonClicked);
    }
    // Connecting DEL and Enter Buttons
    connect(ui->Button_DEL, &QPushButton::clicked, this, &Wordle::deletePressed);
    connect(ui->Button_Enter, &QPushButton::clicked, this, &Wordle::enterPressed);

    // Getting Random word from word manager
    correctWord = wordManager.getRandomWord();
    correctWordQString = QString::fromStdString(correctWord);
    validWords = wordManager.getWords();
    cout << correctWord << endl;

}

Wordle::~Wordle()
{
    delete ui;
}

void Wordle::handleTextChanged(const QString &text)
{
    QLineEdit *senderLineEdit = qobject_cast<QLineEdit*>(sender());
    if (senderLineEdit) {
        if (text.length() >= 1)
        {
           getNextLineEdit(senderLineEdit);
        }

    }
}

void Wordle::onLineEditTextEdited(const QString & text) {
    if (text.length() == 1){
        QLineEdit * senderLineEdit = qobject_cast<QLineEdit *>(sender());
        if (senderLineEdit) {
             senderLineEdit->setText(text.toUpper());
        }
    }

}

void Wordle::onLetterButtonClicked() {
    if (canPressEnter()) {
        return;
    }
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QString text = button->text();
    if (button && !text.trimmed().isEmpty() && !text.at(0).isSpace()) {
        QString lineEditName = QString("lineEdit_%1_%2").arg(rowIndex).arg(colIndex);
        QLineEdit * lineEditWidget = findChild<QLineEdit *>(lineEditName);
        if (lineEditWidget ) {
            lineEditWidget->setText(button->text().toUpper());
            getNextLineEdit(lineEditWidget);
        }


    }
}

void Wordle::processLetterInput(const QString & letter) {
    if (canPressEnter()) {
        return;
    }

    // Find the current line edit widget based on rowIndex and colIndex
    QString lineEditName = QString("lineEdit_%1_%2").arg(rowIndex).arg(colIndex);
    QLineEdit *lineEditWidget = findChild<QLineEdit *>(lineEditName);

    if (lineEditWidget) {
        // Set the text to the uppercase letter
        lineEditWidget->setText(letter);

        // Move to the next line edit
        getNextLineEdit(lineEditWidget);
    }
}

void Wordle::getNextLineEdit(QLineEdit * currentLineEdit) {
    QString lineEditName = currentLineEdit->objectName();
    QStringList parts = lineEditName.split("_");
    rowIndex = parts[1].toInt();
    colIndex = parts[2].toInt();
    QLineEdit * currLineEdit = findChild<QLineEdit *>(lineEditName);
    if (currLineEdit->text().length() != 1){
        return;
    }
    if (colIndex < 5) {
        colIndex++;
    }

}

void Wordle::deletePressed() {
    QString lastTypedLineEditName = QString("lineEdit_%1_%2").arg(rowIndex).arg(colIndex);
    QLineEdit *lastTypedLineEdit = findChild<QLineEdit *>(lastTypedLineEditName);

    if (lastTypedLineEdit) {
        if (lastTypedLineEdit->text().isEmpty()) {
            if (colIndex > 1) {
                QString previousLineEditName = QString("lineEdit_%1_%2").arg(rowIndex).arg(colIndex - 1);
                QLineEdit *previousLineEdit = findChild<QLineEdit *>(previousLineEditName);

                if (previousLineEdit) {
                    previousLineEdit->setText("");
                    colIndex--; // Update colIndex to the previous column
                }
            }
        } else {
            lastTypedLineEdit->setText("");
        }


    }
}

bool Wordle::canPressEnter() {
    QString refName = QString("lineEdit_%1_5").arg(rowIndex);
    QLineEdit * lastLineEditOfRow = findChild<QLineEdit *>(refName);
    if (lastLineEditOfRow->text().length() >0) {
        return true;
    }
    return false;
}

bool Wordle::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent)
        {
            QString text = keyEvent->text();
            if (text.length() == 1 && isEnglishLetter(text.at(0)))
            {
                // Allow English letters (both uppercase and lowercase)
                return QObject::eventFilter(obj, event); // Allow the event to propagate
            }
            else
            {
                // Block non-letter characters
                return true; // Block the key press event
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

string Wordle::getAnswerWord() {
    string currentWord;
    for (int col = 1; col <=5; ++col) {
        QString currentLineEditName = QString("lineEdit_%1_%2").arg(rowIndex).arg(col);
        QLineEdit * currentLineEdit = findChild<QLineEdit *>(currentLineEditName);
        if (currentLineEdit) {
            currentWord = currentWord + currentLineEdit->text().toLower().toStdString();
        }
    }
    return currentWord;
}

bool Wordle::validateWord(const string word) const {
    return binary_search(validWords.begin(), validWords.end(), word);
}

void Wordle::showNotification(const QString & message) {
    QMessageBox* notification = new QMessageBox(this);
    notification->setText(message);
    notification->setWindowTitle("");
    notification->setStandardButtons(QMessageBox::NoButton);

    // Show the notification for 2 seconds
    QTimer::singleShot(2000, notification, &QMessageBox::accept);

    notification->exec();
}

void Wordle::enterPressed() {
    vector<QColor> colors;
    if (canPressEnter()) {
        string word = getAnswerWord();
        if (validateWord(word)){
            guessTries++;
            qDebug() << guessTries;
            colors = compareWords(word);
            modifyRowColors(colors);
            if (gameOver) {
                showGameOverDialog(true);
            }
            else if (guessTries == 6) {
                // Implement lose game over
                showGameOverDialog(false);
                qDebug() << "Allo allo";

            }
            else {
                rowIndex++;
                colIndex = 1;
            }

        }
        else {
            showNotification("Word not in word list.");
        }
    } else {
       showNotification("Not enough letters.");
    }
}

void Wordle::showGameOverDialog(const bool didPlayerWin) {
    GameOverDialog gameOverDialog(this, didPlayerWin, QString::fromStdString(correctWord));
    connect(&gameOverDialog, &GameOverDialog::restartRequested, this, &Wordle::restartGame);
    connect(&gameOverDialog, &GameOverDialog::quitRequested, qApp, &QApplication::quit);
    gameOverDialog.exec();
}


void Wordle::restartGame() {
    rowIndex = 1;
    colIndex = 1;
    guessTries = 0;
    correctWord = wordManager.getRandomWord();
    cout << correctWord << endl;
    for (int row = 1; row <= 6; ++row) {
        for (int col = 1; col <= 5; ++col) {
            QString lineEditName = QString("lineEdit_%1_%2").arg(row).arg(col);
            QLineEdit *lineEdit = findChild<QLineEdit *>(lineEditName);
            if (lineEdit) {
                lineEdit->clear();
                lineEdit->setStyleSheet(""); // Reset style
            }
        }
    }

}

void Wordle::modifyRowColors(const vector<QColor> & colors) {
    for (unsigned int col = 1; col <= 5; ++col) {
        QString lineEditName = QString("lineEdit_%1_%2").arg(rowIndex).arg(col);
        QLineEdit *lineEdit = findChild<QLineEdit *>(lineEditName);
        if (lineEdit && col <= colors.size()) {
            lineEdit->setStyleSheet(QString("background-color: %1; color: white;").arg(colors[col - 1].name()));
        }
    }
}
vector<QColor> Wordle::compareWords(const string & userWord) const {
    vector<QColor> colorVector(5, QColor("#787C7E")); // Initialize vector with gray color
    bool isGameOver = true;


    unordered_map<char, vector<int>> correctIndices;
    for (unsigned int i = 0; i < correctWord.size(); ++i) {
        correctIndices[correctWord[i]].push_back(i);
    }


    unordered_set<int> usedIndices;

    // Determine if each letter in userWord is correct and in the correct position
    for (int i = 0; i < 5; ++i) {
        if (userWord[i] == correctWord[i]) {
            colorVector[i] = QColor("#6AAA64"); // Green for correct letter in correct position
            usedIndices.insert(i); // Mark index as used
        }
    }

    // Determine if each letter in userWord is correct but in the wrong position
    for (int i = 0; i < 5; ++i) {
        if (userWord[i] != correctWord[i]) {
            isGameOver = false;
            char userChar = userWord[i];
            if (correctIndices.count(userChar) > 0) {
                for (int idx : correctIndices[userChar]) {
                    if (usedIndices.count(idx) == 0) {
                        colorVector[i] = QColor("#C9B458"); // Yellow for correct letter in wrong position
                        usedIndices.insert(idx); // Mark index as used
                        break; // Stop after marking the letter as yellow
                    }
                }
            }
        }
    }

    gameOver = isGameOver;
    return colorVector;
}

bool Wordle::isEnglishLetter(QChar c) const
{
    // Check if the character is an English letter (a-z or A-Z)
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
