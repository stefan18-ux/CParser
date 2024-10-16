#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#define FILE_IN "test.json"
std::ifstream in(FILE_IN);
class Parser
{
private:
    std::string json_exp;
    std::map<std::string, std::pair<std::string, std::string>> mapping;

public:
    Parser(const std::string &input = "") : json_exp(input) {}

    void setJsonExp(const char c)
    {
        json_exp.push_back(c);
        return;
    }

    std::string getJsonExp() const
    {
        return json_exp;
    }

    std::string evalMapping(std::string expresion)
    {
        return NULL;
    }

    std::map<std::string, std::pair<std::string, std::string>> getJsonMapping(std::string json_exp)
    {
        mapping.clear();
        std::string expresion = json_exp;
        int i = 0, found_obj = 0, count_pr = 0;
        std::string key;
        // array : []
        // object : {}
        // string : ""
        while (i < expresion.size())
        {
            if ((int)expresion[i] == 34 && !found_obj)
            {
                i++;
                key = "";
                while (int(expresion[i]) != 34)
                    key.push_back(expresion[i]), i++;
                found_obj = 1;
                i++;
            }
            if (found_obj && expresion[i] == '{')
            {
                i++;
                count_pr++;
                std::string value;
                value.push_back('{');
                while (count_pr)
                {
                    if (expresion[i] == '{')
                        count_pr++;
                    else if (expresion[i] == '}')
                        count_pr--;
                    if (!count_pr)
                        break;
                    value.push_back(expresion[i]);
                    i++;
                }
                value.push_back('}');
                mapping[key] = {value, "object"};
                found_obj = 0;
            }
            else if (found_obj && expresion[i] == '[')
            {
                i++;
                count_pr++;
                std::string value;
                value.push_back('[');
                while (count_pr)
                {
                    if (expresion[i] == '[')
                        count_pr++;
                    else if (expresion[i] == ']')
                        count_pr--;
                    if (!count_pr)
                        break;
                    value.push_back(expresion[i]);
                    i++;
                }
                value.push_back(']');
                mapping[key] = {value, "array"};
                found_obj = 0;
            }
            else if (found_obj && int(expresion[i]) == 34)
            {
                i++;
                std::string value;
                while (int(expresion[i]) != 34)
                    value.push_back(expresion[i]), i++;
                mapping[key] = {value, "string"};
                found_obj = 0;
            }
            i++;
        }
        return mapping;
    }
};