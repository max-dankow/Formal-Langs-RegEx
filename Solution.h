#ifndef REGULAREXPRESSIONS_SOLUTION_H
#define REGULAREXPRESSIONS_SOLUTION_H

#include <vector>
#include <string>
#include <iostream>

struct ExpressionInfo {
    ExpressionInfo() { }

    ExpressionInfo(size_t size);

    ExpressionInfo(std::string word, char c);

    ExpressionInfo operator+(const ExpressionInfo &other);

    ExpressionInfo operator*(const ExpressionInfo &other);

    ExpressionInfo kleeneStar();

    ExpressionInfo constructEpsilon(size_t length);

    // Ячейка [i][j] соответствует подстроке [i,j).
    std::vector<std::vector<bool>> canBeSubWord;
    std::vector<std::vector<bool>> canBeSubSuffix;
};

class Solution {
public:
    static bool runSolution(const std::string &regularExpression, const std::string &word, size_t &maxSuffixLength);

    static bool isInAlphabet(char c);

    static bool checkWord(std::string word);
};


#endif //REGULAREXPRESSIONS_SOLUTION_H
