#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <stack>
#include <sstream>
class Parser
{
private:
    std::string json_exp;
    std::map<std::string, std::pair<std::string, std::string>> mapping;

public:
    Parser(const std::string &input = "") : json_exp(input) {}
    void setJsonExp(const char c);
    std::string getJsonExp() const;
    std::map<std::string, std::pair<std::string, std::string>> getJsonMapping(std::string json_exp);
    int CountArrayElements(std::string expresion);
    std::pair<std::string, std::string> ArrayProcessing(std::string expresion, int parameter);
    std::string TrivialOP(std::string expresion, std::string json_exp);
    std::string SubscriptOP(std::string expresion, std::string json_exp, std::vector<std::string> &key);
    std::string solveBoth(std::string expresion, std::string json_exp);
    bool checkParantheses(std::string expresion);
    int ImplementSize(std::string expresion, std::string json_exp);
    std::vector<std::string> getArrayElements(std::string expresion);
    int ImplementMin(std::string expresion, std::string json_exp);
    int ImplementMax(std::string expresion, std::string json_exp);
};
