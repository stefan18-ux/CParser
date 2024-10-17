#include "json_eval.h"

int main(int argc, char *argv[])
{
    Parser parser;
    char c;
    while (in >> c)
        parser.setJsonExp(c);
    std::string json_exp = parser.getJsonExp();
    std::cout << json_exp << "\n";
    // ToDo: tot ce vine dupa virgula trebuie sa fie convertit in "" doar daca e numar de fapt
    // std::string expresion = argv[1];
    std::string expresion = "a.b[a.b[a.b[3]]]";
    // a.b[a.b[1]].c
    /*
    a.b[a.b[a.b[0]]].c
    */
    std::cout << parser.solveBoth(expresion, json_exp) << '\n';
    // ToDo : binding to solve for both(not that hard)
}
