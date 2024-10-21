#include "json_eval.h"
#define FILE_IN "test.json"
std::ifstream in(FILE_IN);

int main(int argc, char *argv[])
{
    Parser parser;
    char c;
    while (in >> c)
        parser.setJsonExp(c);
    std::string json_exp = parser.getJsonExp();
    if (argc < 2)
    {
        std::cout << "You must introduce a query!\n";
        return 0;
    }
    std::string expresion = argv[1];
    if (!parser.checkParantheses(expresion))
    {
        return 0;
    }
    if (expresion.substr(0, 4) == "size")
    {
        std::cout << parser.ImplementSize(expresion, json_exp) << '\n';
        return 0;
    }
    else if (expresion.substr(0, 3) == "min")
    {
        std::cout << parser.ImplementMin(expresion, json_exp) << '\n';
        return 0;
    }
    else if (expresion.substr(0, 3) == "max")
    {
        std::cout << parser.ImplementMax(expresion, json_exp) << '\n';
        return 0;
    }

    std::cout << parser.solveBoth(expresion, json_exp) << '\n';
    return 0;
}
