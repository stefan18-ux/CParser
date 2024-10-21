#include "json_eval.h"

void Parser::setJsonExp(const char c)
{
    json_exp.push_back(c);
    return;
}

std::string Parser::getJsonExp() const
{
    std::string aux = json_exp;
    for (int i = 1; i < aux.size(); i++)
    {
        if (aux[i - 1] == ':' && aux[i] >= '0' && aux[i] <= '9')
        {
            aux.insert(i, "\"");
            i++;
            while (aux[i] >= '0' && aux[i] <= '9')
                i++;
            aux.insert(i, "\"");
        }
    }
    return aux;
}

std::map<std::string, std::pair<std::string, std::string>> Parser::getJsonMapping(std::string json_exp)
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
int Parser::CountArrayElements(std::string expresion)
{
    std::string type;
    int bracketLevel = 0;
    int braceLevel = 0;
    int parameter = 0;
    int i = 1;
    while (i < expresion.size() - 1)
    {
        type = "";
        if (expresion[i] == '{')
        {
            type = "object";
            braceLevel = 1;
            i++;
            while (braceLevel)
            {
                if (expresion[i] == '{')
                    braceLevel++;
                else if (expresion[i] == '}')
                    braceLevel--;
                i++;
            }
            i++;
            parameter++;
        }
        type = "";
        if (expresion[i] == '[')
        {
            type = "array";
            bracketLevel = 1;
            i++;
            while (bracketLevel)
            {
                if (expresion[i] == '[')
                    bracketLevel++;
                else if (expresion[i] == ']')
                    bracketLevel--;
                i++;
            }
            i++;
            parameter++;
        }
        if (!type.size())
        {
            type = "string";
            while (expresion[i] != ',' && i < expresion.size() - 1)
                i++;
            i++;
            parameter++;
        }
    }
    return parameter;
}
std::pair<std::string, std::string> Parser::ArrayProcessing(std::string expresion, int parameter)
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

std::string Parser::TrivialOP(std::string expresion, std::string json_exp)
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

                if (type == "string")
                    break;

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
                    break;
            }
            i++;
        }
        if (expresion[i] == ']' && i == expresion.size() - 1)
            return value;
        if (i < expresion.size())
            value = "Error: There is no \"" + expresion + "\" field in this json!";
        return value;
    }
    return "";
}

std::string Parser::SubscriptOP(std::string expresion, std::string json_exp, std::vector<std::string> &key)
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

std::string Parser::solveBoth(std::string expresion, std::string json_exp)
{
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
        return TrivialOP(expresion, json_exp);
    }
    else
    {
        std::map<std::string, std::string> key;
        std::vector<std::string> tmp;
        std::string str = SubscriptOP(expresion, json_exp, tmp);
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
            {
                key[next] = TrivialOP(next, json_exp);
                std::string aux = key[next];
                if (aux.substr(0, 6) == "Error:")
                    return "Error: There is no \"" + i + "\" field in this json!";
            }
            else
            {
                std::string value = key[new_string];
                next.erase(idx, len);
                std::string ins = "[" + value + "]";
                next.insert(idx, ins);
                key[i] = TrivialOP(next, json_exp);
                std::string aux = key[i];
                if (aux.substr(0, 6) == "Error:")
                    return "Error: There is no \"" + i + "\" field in this json!";
            }
        }
        return key[expresion];
    }
}

bool Parser::checkParantheses(std::string expresion)
{
    int bracket = 0;
    for (int j = 0; j < expresion.size(); j++)
    {
        char i = expresion[j];
        if (i == '[')
        {
            bracket++;
            if (j < expresion.size() - 1)
                if (expresion[j + 1] == ']' || expresion[j + 1] == '[')
                {
                    std::cout << "There's an error in your query." << '\n';
                    return false;
                }
        }
        else if (i == ']')
            bracket--;
    }
    if (bracket)
    {
        std::cout << "Unmatched parantheses." << '\n';
        return false;
    }
    return true;
}

int Parser::ImplementSize(std::string expresion, std::string json_exp)
{
    std::string aux;
    for (int i = 5; i < expresion.size() - 1; i++)
        aux.push_back(expresion[i]);
    expresion = aux;
    std::string ans = solveBoth(expresion, json_exp);
    if (ans.substr(0, 5) == "Error")
        return -1;
    if (ans[0] == '[')
    {
        return CountArrayElements(ans);
    }
    else if (ans[0] == '{')
    {
        std::string value = ans;
        std::map<std::string, std::pair<std::string, std::string>> map = getJsonMapping(value);
        return map.size();
    }
    else
        return ans.size();
}

std::vector<std::string> Parser::getArrayElements(std::string expresion)
{
    std::vector<std::string> ans;
    std::string currentElement;
    int i = 1;
    while (i < expresion.size() - 1)
    {
        currentElement = "";
        while (expresion[i] != ',' && i < expresion.size() - 1)
            currentElement.push_back(expresion[i]), i++;
        i++;
        ans.push_back(currentElement);
    }

    return ans;
}

int Parser::ImplementMin(std::string expresion, std::string json_exp)
{
    std::string aux;
    for (int i = 4; i < expresion.size() - 1; i++)
        aux.push_back(expresion[i]);
    expresion = aux;

    std::stringstream ss(expresion);
    std::string item;
    std::vector<std::string> elements;

    while (std::getline(ss, item, ','))
    {
        item.erase(0, item.find_first_not_of(" "));
        item.erase(item.find_last_not_of(" ") + 1);
        elements.push_back(item);
    }
    long long minimum = 0;
    bool first = false;
    for (const auto &elem : elements)
    {
        if (elem[0] >= '0' && elem[0] <= '9')
        {
            long long number = 0;
            for (auto i : elem)
                number = number * 10 + i - '0';
            if (!first)
                minimum = number, first = true;
            else
                minimum = std::min(minimum, number);
        }
        else
        {
            std::string ans = solveBoth(elem, json_exp);
            if (ans.substr(0, 5) == "Error")
                return -1;
            if (ans[0] == '[')
            {
                std::vector<std::string> array = getArrayElements(ans);
                for (auto i : array)
                {
                    long long number = 0;
                    for (auto j : i)
                        number = number * 10 + j - '0';
                    if (!first)
                        minimum = number, first = true;
                    else
                        minimum = std::min(minimum, number);
                }
            }
            else
            {
                long long number = 0;
                for (auto i : ans)
                    number = number * 10 + i - '0';
                if (!first)
                    minimum = number, first = true;
                else
                    minimum = std::min(minimum, number);
            }
        }
    }
    return minimum;
}

int Parser::ImplementMax(std::string expresion, std::string json_exp)
{
    std::string aux;
    for (int i = 4; i < expresion.size() - 1; i++)
        aux.push_back(expresion[i]);
    expresion = aux;

    std::stringstream ss(expresion);
    std::string item;
    std::vector<std::string> elements;

    while (std::getline(ss, item, ','))
    {
        item.erase(0, item.find_first_not_of(" "));
        item.erase(item.find_last_not_of(" ") + 1);
        elements.push_back(item);
    }
    long long maximum = 0;
    bool first = false;
    for (const auto &elem : elements)
    {
        if (elem[0] >= '0' && elem[0] <= '9')
        {
            long long number = 0;
            for (auto i : elem)
                number = number * 10 + i - '0';
            if (!first)
                maximum = number, first = true;
            else
                maximum = std::max(maximum, number);
        }
        else
        {
            std::string ans = solveBoth(elem, json_exp);
            if (ans.substr(0, 5) == "Error")
                return -1;
            if (ans[0] == '[')
            {
                std::vector<std::string> array = getArrayElements(ans);
                for (auto i : array)
                {
                    long long number = 0;
                    for (auto j : i)
                        number = number * 10 + j - '0';
                    if (!first)
                        maximum = number, first = true;
                    else
                        maximum = std::max(maximum, number);
                }
            }
            else
            {
                long long number = 0;
                for (auto i : ans)
                    number = number * 10 + i - '0';
                if (!first)
                    maximum = number, first = true;
                else
                    maximum = std::max(maximum, number);
            }
        }
    }
    return maximum;
}
