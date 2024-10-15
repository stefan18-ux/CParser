#include "json_eval.h"

int main()
{
    Parser parser;
    char c;
    while (in >> c)
        parser.setJsonExp(c);
    std::string json_exp = parser.getJsonExp();
    std::cout << json_exp << '\n';
    std::string expresion = "a.b[1]";
    std::map<std::string, std::pair<std::string, std::string>> map = parser.getJsonMapping(json_exp);
    for (auto i : map)
        std::cout << i.first << " : " << i.second.first << '\n';

    return 0;
}
