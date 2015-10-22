#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <assert.h>

struct SubExpressionInfo
{
    SubExpressionInfo(std::string word);
    SubExpressionInfo(std::string word, char c);
    std::vector<std::vector<bool>> canBeSubWord;
    std::string expression;
};

bool isInAlphabet(char c)
{
    // todo: сделать унивесальный алфавит.
    return c == 'a' || c == 'b' || c == 'c';
}

SubExpressionInfo::SubExpressionInfo(std::string word)
{
    canBeSubWord.assign(word.length(), std::vector<bool>(word.length(), false));
}

SubExpressionInfo::SubExpressionInfo(std::string word, char c) {
    assert(isInAlphabet(c));
    canBeSubWord.assign(word.length(), std::vector<bool>(word.length(), false));
    for (size_t i = 0; i < word.length(); ++i)
    {
        canBeSubWord[i][i] = word[i] == c;

    }
}

bool checkWord(std::string word)
{
    for (char c : word)
    {
        if (!isInAlphabet(c))
        {
            return false;
        }
    }
    return true;
}

void readInput(std::istream &input, std::string &regularExpression, std::string &word)
{
    input >> regularExpression >> word;
}

void runSolution(const std::string &regularExpression, const std::string &word)
{
    std::stack<SubExpressionInfo> parsingStack;
    for (size_t i = 0; i < regularExpression.length(); ++i)
    {
        switch (regularExpression[i])
        {
            case 'a':
                break;
            case 'b':
                break;
            case 'c':
                break;
            case '1':
                break;
            case '.':
                break;
            case '+':
                break;
            case '*':
                break;
            default:
                assert(false);
        }
    }
}

int main() {
    std::ifstream inputFile("input.txt");
    std::string regularExpression;
    std::string word;
    readInput(inputFile, regularExpression, word);
    assert(checkWord(word));
    std::cout << regularExpression << "    " << word << "\n";
    return 0;
}
