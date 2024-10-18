#include "json_eval.h"

int main(int argc, char *argv[])
{
    Parser parser;
    char c;
    while (in >> c)
        parser.setJsonExp(c);
    std::string json_exp = parser.getJsonExp();
    std::cout << json_exp << "\n";
    // std::string expresion = argv[1];
    std::string expresion = "size(a.b[a.b[1]].c)";
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

    std::cout
        << parser.solveBoth(expresion, json_exp) << '\n';
    return 0;
}
