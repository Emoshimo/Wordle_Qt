#ifndef WORDMANAGER_H
#define WORDMANAGER_H

#include <vector>
#include <string>
#include <random>
using namespace std;
class WordManager
{
public:
    WordManager(const std::string& filename);
    string getRandomWord() const;
    vector<string> getWords() const;
private:
    vector<string> words;
};

#endif // WORDMANAGER_H
