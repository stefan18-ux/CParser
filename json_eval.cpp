#include "json_eval.h"

int main(int argc, char *argv[])
{
    Parser parser;
    char c;
    if (argc < 3)
    {
        std::cout << "You must introduce a query and a json expresion!\n";
        return 0;
    }
    std::ifstream in(argv[1]);
    while (in >> c)
        parser.setJsonExp(c);
    std::string json_exp = parser.getJsonExp();
    std::string expresion = argv[2];
    if (!parser.checkParantheses(expresion))
    {
        return 0;
    }
    if (expresion.substr(0, 4) == "size")
    {
        int ans = parser.ImplementSize(expresion, json_exp);
        if (ans == -1)
        {
            std::cout << "Error: Query does not have size.\n";
            return 0;
        }
        std::cout << ans << '\n';
        return 0;
    }
    else if (expresion.substr(0, 3) == "min")
    {
        int ans = parser.ImplementMin(expresion, json_exp);
        if (ans == -1)
        {
            std::cout << "Error: Query does not have min.\n";
            return 0;
        }
        std::cout << ans << '\n';
        return 0;
    }
    else if (expresion.substr(0, 3) == "max")
    {
        int ans = parser.ImplementMax(expresion, json_exp);
        if (ans == -1)
        {
            std::cout << "Error: Query does not have max.\n";
            return 0;
        }
        std::cout << ans << '\n';
        return 0;
    }

    std::cout << parser.solveBoth(expresion, json_exp) << '\n';
    return 0;
}
