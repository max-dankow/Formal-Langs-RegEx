#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <assert.h>
#include "Solution.h"

void readInput(std::istream &input, std::string &regularExpression, std::string &word) {
    input >> regularExpression >> word;
}

int main() {
//    std::ifstream inputFile("input.txt");
    std::string regularExpression;
    std::string word;
//    readInput(inputFile, regularExpression, word);
    readInput(std::cin, regularExpression, word);
    assert(Solution::checkWord(word));
//    std::cout << regularExpression << "    " << word << "\n";
    size_t answer = 0;
    if (Solution::runSolution(regularExpression, word, answer))
    {
//        std::cout << "All right. Expression (" << regularExpression << ") is correct.\n";
//        std::cout << "The longest suffix of '" << word << "', what is a suffix of a word from language, is " << answer << "\n";
        std::cout << answer;
    }
    else
    {
//        std::cout << "ERROR occurred! Expression (" << regularExpression << ") is WRONG!\n";
        std::cout << "ERROR\n";
    }
    return 0;
}
