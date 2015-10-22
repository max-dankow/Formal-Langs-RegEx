#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <assert.h>

struct SubExpressionInfo
{
    SubExpressionInfo(){}
    SubExpressionInfo(size_t size);
    SubExpressionInfo(std::string word, char c);
    SubExpressionInfo operator + (const SubExpressionInfo &other);
    SubExpressionInfo operator * (const SubExpressionInfo &other);
    std::vector<std::vector<bool>> canBeSubWord;
    std::string expression;
};

std::ostream& operator << (std::ostream &output, const SubExpressionInfo &info)
{
    for (size_t begin = 0; begin < info.canBeSubWord.size(); ++begin)
    {
        for (size_t end = 0; end < info.canBeSubWord.size(); ++end)
        {
            output << info.canBeSubWord[begin][end] << ' ';
        }
        output << '\n';
    }
    output << '\n';
    return  output;
}

bool isInAlphabet(char c)
{
    // todo: сделать унивесальный алфавит.
    return c == 'a' || c == 'b' || c == 'c';
}

SubExpressionInfo::SubExpressionInfo(size_t size)
{
    canBeSubWord.assign(size, std::vector<bool>(size, false));
}

SubExpressionInfo::SubExpressionInfo(std::string word, char c) {
    assert(isInAlphabet(c));
    canBeSubWord.assign(word.length(), std::vector<bool>(word.length(), false));
    for (size_t i = 0; i < word.length(); ++i)
    {
        canBeSubWord[i][i] = word[i] == c;
    }
}


SubExpressionInfo SubExpressionInfo::operator+(const SubExpressionInfo &other) {
    SubExpressionInfo sumInfo(canBeSubWord.size());
    for (size_t begin = 0; begin < this->canBeSubWord.size(); ++begin)
    {
        for (size_t end = 0; end < this->canBeSubWord.size(); ++end)
        {
            sumInfo.canBeSubWord[begin][end] = this->canBeSubWord[begin][end] || other.canBeSubWord[begin][end];
        }
    }
    return sumInfo;
}
SubExpressionInfo SubExpressionInfo::operator*(const SubExpressionInfo &other) {
    SubExpressionInfo multInfo(canBeSubWord.size());
    for (size_t begin = 0; begin < this->canBeSubWord.size(); ++begin)
    {
        for (size_t end = 0; end < this->canBeSubWord.size(); ++end)
        {
            if (this->canBeSubWord[begin][end] && end != this->canBeSubWord.size() - 1)
            {
                for (size_t copy = end; copy < other.canBeSubWord.size(); ++ copy)
                {
                    multInfo.canBeSubWord[begin][copy] = multInfo.canBeSubWord[begin][copy] || other.canBeSubWord[end + 1][copy];
                }
            }
        }
    }
    return multInfo;
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

bool runSolution(const std::string &regularExpression, const std::string &word)
{
    std::stack<SubExpressionInfo> parsingStack;
    for (size_t i = 0; i < regularExpression.length(); ++i)
    {
        switch (regularExpression[i]) {
            // todo: Объединить как-то эти три одинаковых случая с целью DRY.
            case 'a':
                parsingStack.push(SubExpressionInfo(word, 'a'));
                break;
            case 'b':
                parsingStack.push(SubExpressionInfo(word, 'b'));
                break;
            case 'c':
                parsingStack.push(SubExpressionInfo(word, 'c'));
                break;
            case '1':
                parsingStack.push(SubExpressionInfo(word.length()));
                break;
            case '.': {
                if (parsingStack.size() < 2) {
                    return false;
                }
                SubExpressionInfo first = parsingStack.top();
                parsingStack.pop();
                SubExpressionInfo second = parsingStack.top();
                parsingStack.pop();
                parsingStack.push(first + second);
                break;
            }
            case '+': {
                if (parsingStack.size() < 2)
                {
                    return false;
                }
                SubExpressionInfo first = parsingStack.top();
                parsingStack.pop();
                SubExpressionInfo second = parsingStack.top();
                parsingStack.pop();
                parsingStack.push(first + second);
                break;
            }
            case '*':
                break;
            default:
                assert(false);
        }
        std::cout << parsingStack.top();
    }
}

int main() {
    std::ifstream inputFile("input.txt");
    std::string regularExpression;
    std::string word;
    readInput(inputFile, regularExpression, word);
    assert(checkWord(word));
    std::cout << regularExpression << "    " << word << "\n";
    runSolution(regularExpression, word);
    return 0;
}
