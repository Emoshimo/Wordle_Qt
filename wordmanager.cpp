#include "wordmanager.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
WordManager::WordManager(const string & filename)
{
    ifstream file(filename);
    if (file.is_open()) {
        string word;
        while (getline(file, word)) {
            words.push_back(word);
        }
        file.close();
    }

}

string WordManager::getRandomWord() const {
    if (words.empty()) {
        return "shimo";
    }
    int i = rand() % words.size() - 1;

    return words[i];
}

vector<string> WordManager::getWords() const {
    return words;
}
