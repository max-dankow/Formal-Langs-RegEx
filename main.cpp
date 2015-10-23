#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <assert.h>

struct ExpressionInfo
{
    ExpressionInfo(){}
    ExpressionInfo(size_t size);
    ExpressionInfo(std::string word, char c);
    ExpressionInfo operator + (const ExpressionInfo &other);
    ExpressionInfo operator * (const ExpressionInfo &other);
    ExpressionInfo kleeneStar();
    std::vector<std::vector<bool>> canBeSubWord;
    std::vector<std::vector<bool>> canBeSubSuffix;
};

std::ostream& operator << (std::ostream &output, const ExpressionInfo &info)
{
    for (size_t begin = 0; begin < info.canBeSubWord.size(); ++begin)
    {
        for (size_t end = 0; end < info.canBeSubWord.size(); ++end)
        {
            output << info.canBeSubWord[begin][end] << ' ';
        }
        output << " | ";
        for (size_t end = 0; end < info.canBeSubSuffix.size(); ++end)
        {
            output << info.canBeSubSuffix[begin][end] << ' ';
        }
        output << "\n";
    }
    output << '\n';
    return  output;
}

bool isInAlphabet(char c)
{
    // todo: сделать унивесальный алфавит.
    return c == 'a' || c == 'b' || c == 'c';
}

ExpressionInfo::ExpressionInfo(size_t size)
{
    canBeSubWord.assign(size, std::vector<bool>(size, false));
    canBeSubSuffix.assign(size, std::vector<bool>(size, false));
}

ExpressionInfo::ExpressionInfo(std::string word, char c)
{
    assert(isInAlphabet(c));
    canBeSubWord.assign(word.length(), std::vector<bool>(word.length(), false));
    for (size_t i = 0; i < word.length(); ++i)
    {
        canBeSubWord[i][i] = word[i] == c;
    }
    canBeSubSuffix.assign(word.length(), std::vector<bool>(word.length(), false));
    for (size_t i = 0; i < word.length(); ++i)
    {
        canBeSubSuffix[i][i] = word[i] == c;
    }
}

ExpressionInfo ExpressionInfo::operator+(const ExpressionInfo &other)
{
    ExpressionInfo sumInfo(canBeSubWord.size());
    for (size_t begin = 0; begin < this->canBeSubWord.size(); ++begin)
    {
        for (size_t end = begin; end < this->canBeSubWord.size(); ++end)
        {
            sumInfo.canBeSubWord[begin][end] = this->canBeSubWord[begin][end] || other.canBeSubWord[begin][end];
            sumInfo.canBeSubSuffix[begin][end] = this->canBeSubSuffix[begin][end] || other.canBeSubSuffix[begin][end];
        }
    }
    return sumInfo;
}
ExpressionInfo ExpressionInfo::operator*(const ExpressionInfo &other) {
    ExpressionInfo multInfo(canBeSubWord.size());
//    std::cout << *this << "******";
//    std::cout << other;
    for (size_t begin = 0; begin < this->canBeSubWord.size(); ++begin)
    {
        for (size_t end = begin; end < this->canBeSubWord.size(); ++end)
        {
            if (this->canBeSubWord[begin][end] && end != this->canBeSubWord.size() - 1)
            {
                for (size_t copy = end/* + 1*/; copy < other.canBeSubWord.size(); ++copy)
                {
                    multInfo.canBeSubWord[begin][copy] = multInfo.canBeSubWord[begin][copy] || other.canBeSubWord[end + 1][copy];
                }
            }
        }
    }
    for (size_t begin = 0; begin < this->canBeSubSuffix.size(); ++begin)
    {
        for (size_t end = begin; end < this->canBeSubSuffix.size(); ++end)
        {
            multInfo.canBeSubSuffix[begin][end] = multInfo.canBeSubSuffix[begin][end] || other.canBeSubSuffix[begin][end];
            if (this->canBeSubSuffix[begin][end] && end != this->canBeSubSuffix.size() - 1)
            {
                for (size_t copy = end; copy < other.canBeSubSuffix.size(); ++copy)
                {
                    multInfo.canBeSubSuffix[begin][copy] = multInfo.canBeSubSuffix[begin][copy] || other.canBeSubWord[end + 1][copy];
                }
            }
        }
    }
    return multInfo;
}

ExpressionInfo ExpressionInfo::kleeneStar() {
    ExpressionInfo sumInfo = *this;
    ExpressionInfo power = *this;
//    std::cout << power;
    for (size_t i = 0; i < canBeSubWord.size(); ++i)
    {
        power = power * (*this);
//        std::cout << power;
        sumInfo = sumInfo + power;
//        std::cout << sumInfo;
    }
    for (size_t i = 1; i < canBeSubWord.size(); ++i)
    {
        sumInfo.canBeSubWord[i][i - 1] = true;
    }
    return sumInfo;
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

bool runSolution(const std::string &regularExpression, const std::string &word, size_t &maxSuffixLength)
{
    std::stack<ExpressionInfo> parsingStack;
    for (size_t i = 0; i < regularExpression.length(); ++i)
    {
        switch (regularExpression[i]) {
            // todo: Объединить как-то эти три одинаковых случая с целью DRY.
            case 'a':
                parsingStack.push(ExpressionInfo(word, 'a'));
                break;
            case 'b':
                parsingStack.push(ExpressionInfo(word, 'b'));
                break;
            case 'c':
                parsingStack.push(ExpressionInfo(word, 'c'));
                break;
            case '1':
                parsingStack.push(ExpressionInfo(word.length()));
                break;
            case '.': {
                if (parsingStack.size() < 2) {
                    return false;
                }
                ExpressionInfo first = parsingStack.top();
                parsingStack.pop();
                ExpressionInfo second = parsingStack.top();
                parsingStack.pop();
                parsingStack.push(second * first);
                break;
            }
            case '+': {
                if (parsingStack.size() < 2) {
                    return false;
                }
                ExpressionInfo first = parsingStack.top();
                parsingStack.pop();
                ExpressionInfo second = parsingStack.top();
                parsingStack.pop();
                parsingStack.push(first + second);
                break;
            }
            case '*': {
                if (parsingStack.size() < 1) {
                    return false;
                }
                ExpressionInfo first = parsingStack.top();
                parsingStack.pop();
                parsingStack.push(first.kleeneStar());
                break;
            }
            default:
                assert(false);
        }
        std::cout << parsingStack.top();
    }
    maxSuffixLength = 0;
    if (parsingStack.size() == 1)
    {
        ExpressionInfo entireExpressionInfo = parsingStack.top();
        size_t tableSize = entireExpressionInfo.canBeSubSuffix.size();
        for (size_t i = 0; i < entireExpressionInfo.canBeSubSuffix.size(); ++i)
        {
            if (entireExpressionInfo.canBeSubSuffix[i][tableSize - 1])
            {
                maxSuffixLength = tableSize - i;
                return true;
            }
        }
    }
    return false;
}

int main() {
    std::ifstream inputFile("input.txt");
    std::string regularExpression;
    std::string word;
    readInput(inputFile, regularExpression, word);
    assert(checkWord(word));
//    std::cout << regularExpression << "    " << word << "\n";
    size_t answer = 0;
    if (runSolution(regularExpression, word, answer))
    {
        std::cout << "All right. Expression (" << regularExpression << ") is correct.\n";
        std::cout << "The longest suffix of '" << word << "', what is a suffix of a word from language, is " << answer << "\n";
    }
    else
    {
        std::cout << "ERROR occurred! Expression (" << regularExpression << ") is WRONG!\n";
    }
    return 0;
}
