#include "json_eval.h"

int main()
{
    Parser parser;
    char c;
    while (in >> c)
        parser.setJsonExp(c);

    std::string json_exp = parser.getJsonExp();
    std::cout << json_exp << "\n";
    std::string expresion = "c";
    std::map<std::string, std::pair<std::string, std::string>> map = parser.getJsonMapping(json_exp);

    size_t pos_dot = expresion.find('.');
    size_t pos_bracket = expresion.find('[');

    // Answering simple queries : <name>
    if (pos_dot == std::string::npos && pos_bracket == std::string::npos)
    {
        if (map[expresion].first.size())
            std::cout << map[expresion].first << '\n';
        else if (map[expresion].first.size() == 0)
            std::cout << "Error: There is no \"" << expresion << "\" field in this json!\n";
    }
    // Answering queries containing only "." and no "[]"
    if (pos_dot != std::string::npos && pos_bracket == std::string::npos)
    {
    }
    // Answering queries containing both "." and "[]"
    if (pos_dot != std::string::npos && pos_bracket != std::string::npos)
    {
    }
    return 0;
}
