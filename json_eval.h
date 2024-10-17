#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
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

    std::pair<std::string, std::string> ArrayProcessing(std::string expresion, int parameter)
    {
        std::string currentElement, type;
        int bracketLevel = 0;
        int braceLevel = 0;
        parameter++;
        int i = 1;
        while (i < expresion.size() - 1)
        {
            currentElement = "", type = "";
            if (expresion[i] == '{')
            {
                type = "object";
                braceLevel = 1;
                i++;
                currentElement.push_back('{');
                while (braceLevel)
                {
                    currentElement.push_back(expresion[i]);
                    if (expresion[i] == '{')
                        braceLevel++;
                    else if (expresion[i] == '}')
                        braceLevel--;
                    i++;
                }
                i++;
                parameter--;
                if (!parameter)
                    return {currentElement, type};
            }
            currentElement = "", type = "";
            if (expresion[i] == '[')
            {
                type = "array";
                bracketLevel = 1;
                i++;
                currentElement.push_back('[');
                while (bracketLevel)
                {
                    currentElement.push_back(expresion[i]);
                    if (expresion[i] == '[')
                        bracketLevel++;
                    else if (expresion[i] == ']')
                        bracketLevel--;
                    i++;
                }
                i++;
                parameter--;
                if (!parameter)
                    return {currentElement, type};
            }
            if (!type.size())
            {
                type = "string";
                while (expresion[i] != ',' && i < expresion.size() - 1)
                    currentElement.push_back(expresion[i]), i++;
                i++;
                parameter--;
                if (!parameter)
                    return {currentElement, type};
            }
        }
        if (parameter)
            currentElement = "", type = "";
        return {currentElement, type};
    }

    std::string TrivialOP(std::string expresion, std::string json_exp)
    {
        size_t pos_dot = expresion.find('.');
        size_t pos_bracket = expresion.find('[');

        std::map<std::string, std::pair<std::string, std::string>> map = getJsonMapping(json_exp);

        // Answering simple queries : <name>
        if (pos_dot == std::string::npos && pos_bracket == std::string::npos)
        {
            if (map[expresion].first.size())
                return map[expresion].first;
            else if (map[expresion].first.size() == 0)
                return "Error: There is no \"" + expresion + "\" field in this json!";
        }

        // Answering queries containing only "." and no "[]"
        if (pos_dot != std::string::npos && pos_bracket == std::string::npos)
        {
            int i = 0;
            std::string value;
            while (i < expresion.size())
            {
                std::string key;
                while (expresion[i] != '.' && i < expresion.size())
                    key.push_back(expresion[i]), i++;
                i++;

                value = map[key].first;

                if (!value.size())
                    return "Error: There is no \"" + expresion + "\" field in this json!";

                if (map[key].second == "string")
                    break;
                map = getJsonMapping(value);
            }
            if (i < expresion.size())
                value = "Error: There is no \"" + expresion + "\" field in this json!";
            return value;
        }

        // Answering queries containing both "." and "[]"
        if (pos_dot && pos_bracket)
        {
            int i = 0, parameter = 0;
            std::string value, type = "object", key;
            while (i < expresion.size())
            {
                if ((expresion[i] == '.' && type == "array") || (expresion[i] == '[' && type == "object"))
                    break;
                if (type == "object")
                {
                    key = "";
                    if (expresion[i] == '.')
                        i++;
                    while (expresion[i] != '.' && expresion[i] != '[' && i < expresion.size())
                        key.push_back(expresion[i]),
                            i++;

                    value = map[key].first;
                    type = map[key].second;

                    if (!value.size())
                        return "Error: There is no \"" + expresion + "\" field in this json!";

                    map = getJsonMapping(value);
                }
                if (key.size() && expresion[i] == '[' && i < expresion.size())
                {
                    // processing string, object - kinda easy
                    // processing array - hard
                    if (type == "string" || type == "object")
                        break;

                    i++;
                    parameter = 0;
                    while (expresion[i] != ']')
                        parameter = parameter * 10 + expresion[i] - '0', i++;
                    // it's an array;
                    std::pair<std::string, std::string> ans = ArrayProcessing(value, parameter);
                    value = ans.first;
                    type = ans.second;
                    if (value == "" && type == "")
                        return "Error: There is no \"" + expresion + "\" field in this json!";

                    if (type == "string")
                        return value;
                }
                i++;
            }
            if (i < expresion.size())
                value = "Error: There is no \"" + expresion + "\" field in this json!";
            return value;
        }
        return "";
    }

    std::string SubscriptOP(std::string expresion, std::string json_exp, std::vector<std::string> &key)
    {
        std::string new_exp;
        bool type = false;
        for (int i = 0; i < expresion.size() - 1; i++)
        {
            if (expresion[i] == '[' && (expresion[i + 1] < '0' || expresion[i + 1] > '9'))
            {
                type = true;
                i++;
                int bracket = 1;
                while (bracket)
                {
                    new_exp.push_back(expresion[i]);
                    if (expresion[i] == '[')
                        bracket++;
                    else if (expresion[i] == ']')
                        bracket--;
                    i++;
                }
                new_exp.pop_back();
                break;
            }
        }
        if (type)
        {
            std::string next = SubscriptOP(new_exp, json_exp, key);
            key.push_back(next);
        }
        return expresion;
    }
};
