#include "Solution.h"
#include <assert.h>
#include <stack>

ExpressionInfo::ExpressionInfo(size_t size)
{
    canBeSubWord.assign(size, std::vector<bool>(size, false));
    canBeSubSuffix.assign(size, std::vector<bool>(size, false));
}

ExpressionInfo ExpressionInfo::constructEpsilon(size_t length) {
    ExpressionInfo epsilonExpression(length + 1);
    for (size_t i = 0; i < length + 1; ++i) {
        epsilonExpression.canBeSubWord[i][i] = true;
        epsilonExpression.canBeSubSuffix[i][i] = true;
    }
    return epsilonExpression;
}

ExpressionInfo::ExpressionInfo(std::string word, char c)
{
    if (c == '1') {
        *this = constructEpsilon(word.length());
        return;
    }
    assert(Solution::isInAlphabet(c));
    canBeSubWord.assign(word.length() + 1, std::vector<bool>(word.length() + 1, false));
    for (size_t i = 0; i < word.length(); ++i)
    {
        canBeSubWord[i][i + 1] = word[i] == c;
    }
    canBeSubSuffix.assign(word.length() + 1, std::vector<bool>(word.length() + 1, false));
    for (size_t i = 0; i < word.length(); ++i)
    {
        canBeSubSuffix[i][i + 1] = word[i] == c;
        // Любое пустое подслово - это суффикс рассматриваемого слова.
        canBeSubSuffix[i][i] = true;
    }
}

ExpressionInfo ExpressionInfo::operator+(const ExpressionInfo &other)
{
    ExpressionInfo sumInfo(canBeSubWord.size());
    for (size_t begin = 0; begin < this->canBeSubWord.size(); ++begin)
    {
        for (size_t end = 0; end < this->canBeSubWord[begin].size(); ++end)
        {
            sumInfo.canBeSubWord[begin][end] = this->canBeSubWord[begin][end] || other.canBeSubWord[begin][end];
            sumInfo.canBeSubSuffix[begin][end] = this->canBeSubSuffix[begin][end] || other.canBeSubSuffix[begin][end];
        }
    }
    return sumInfo;
}

ExpressionInfo ExpressionInfo::operator*(const ExpressionInfo &other) {
    ExpressionInfo multInfo(canBeSubWord.size());
    for (size_t begin = 0; begin < this->canBeSubWord.size(); ++begin)
    {
        for (size_t end = 0; end < this->canBeSubWord[begin].size(); ++end)
        {
            if (this->canBeSubWord[begin][end] /*&& end != this->canBeSubWord.size() - 1*/)
            {
                for (size_t copy = end/* + 1*/; copy < other.canBeSubWord.size(); ++copy)
                {
                    multInfo.canBeSubWord[begin][copy] = multInfo.canBeSubWord[begin][copy] || other.canBeSubWord[end][copy];
                }
            }
        }
    }
    for (size_t begin = 0; begin < this->canBeSubSuffix.size(); ++begin)
    {
        for (size_t end = 0; end < this->canBeSubSuffix.size(); ++end)
        {
            // Все суффиксы второго выражения будут также суффиксами произведения.
            multInfo.canBeSubSuffix[begin][end] = multInfo.canBeSubSuffix[begin][end] || other.canBeSubSuffix[begin][end];
            if (this->canBeSubSuffix[begin][end] /*&& end != this->canBeSubSuffix.size() - 1*/)
            {
                for (size_t copy = end; copy < other.canBeSubSuffix.size(); ++copy)
                {
                    multInfo.canBeSubSuffix[begin][copy] = multInfo.canBeSubSuffix[begin][copy] || other.canBeSubWord[end][copy];
                }
            }
        }
    }
    return multInfo;
}

ExpressionInfo ExpressionInfo::kleeneStar() {
    ExpressionInfo sumInfo = *this;
    ExpressionInfo power = *this;
    for (size_t i = 0; i < canBeSubWord.size(); ++i)
    {
        power = power * (*this);
        sumInfo = sumInfo + power;
    }
    for (size_t i = 0; i < canBeSubWord.size(); ++i)
    {
        sumInfo.canBeSubWord[i][i] = true;
        //sumInfo.canBeSubSuffix[i][i - 1] = true;
    }
    return sumInfo;
}

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

bool Solution::runSolution(const std::string &regularExpression, const std::string &word, size_t &maxSuffixLength) {
    std::stack<ExpressionInfo> parsingStack;
    for (size_t i = 0; i < regularExpression.length(); ++i)
    {
        switch (regularExpression[i]) {
            case 'a':
            case 'b':
            case 'c':
            case '1':
                parsingStack.push(ExpressionInfo(word, regularExpression[i]));
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
//        std::cout << parsingStack.top();
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
                maxSuffixLength = tableSize - i - 1;
                return true;
            }
        }
    }
    else
    {
        return false;
    }
}



bool Solution::checkWord(std::string word) {
    for (char c : word)
    {
        if (!isInAlphabet(c))
        {
            return false;
        }
    }
    return true;
}

bool Solution::isInAlphabet(char c) {
    return c == 'a' || c == 'b' || c == 'c';
}