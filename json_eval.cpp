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
    std::string expresion = "a.b.c";
    // a.b[a.b[1]].c
    /*
    a.b[a.b[a.b[0]]].c
    */
    int type = 0;
    for (int i = 0; i < expresion.size(); i++)
    {
        if (expresion[i] == '[' && (expresion[i + 1] < '0' || expresion[i + 1] > '9'))
        {
            type = 1;
            break;
        }
    }
    if (!type)
    {
        std::cout << parser.TrivialOP(expresion, json_exp) << '\n';
        return 0;
    }
    if (type == 1)
    {
        std::map<std::string, std::string> key;
        std::vector<std::string> tmp;
        std::string str = parser.SubscriptOP(expresion, json_exp, tmp);
        tmp.push_back(expresion);
        for (auto i : tmp)
        {
            std::string next = i;
            std::string new_string;
            int idx = -1, len = 0;
            for (int i = 0; i < next.size() - 1; i++)
            {
                if (next[i] == '[' && (next[i + 1] < '0' || next[i + 1] > '9'))
                {
                    idx = i;
                    len++;
                    i++;
                    int bracket = 1;
                    while (bracket)
                    {
                        len++;
                        new_string.push_back(next[i]);
                        if (next[i] == '[')
                            bracket++;
                        else if (next[i] == ']')
                            bracket--;
                        i++;
                    }
                    new_string.pop_back();
                    break;
                }
            }
            if (idx == -1)
                key[next] = parser.TrivialOP(next, json_exp);
            else
            {
                std::string value = key[new_string];
                next.erase(idx, len);
                std::string ins = "[" + value + "]";
                next.insert(idx, ins);
                key[i] = parser.TrivialOP(next, json_exp);
            }
        }
        std::cout << key[expresion] << '\n';
        return 0;
    }
}
