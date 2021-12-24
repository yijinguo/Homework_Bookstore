#ifndef UNTITLED3_PARSER_H
#define UNTITLED3_PARSER_H

#include "statement.h"

class parser{
private:

    //返回第一个单词，剩余cmd
    static std::string takeFirstWord(std::string &cmd){
        int index = 0;
        int firstWordIndex;
        std::string firstWord;
        while (cmd[index] == ' ') {++index;}
        if (cmd[index] != '\0') {
            firstWordIndex = index;
            while (cmd[index] != ' ' && cmd[index] != '\0') {++index;}
            char word[index - firstWordIndex + 1];
            for (int i = 0; i < index - firstWordIndex; ++i) {
                word[i] = cmd[i + firstWordIndex];
            }
            word[index - firstWordIndex] = '\0';
            std::string result = std::string(word);
            while (cmd[index] == ' ') {++index;}
            int l = cmd.length();
            if (l == index) {
                cmd[0] = '\0';
                return result;
            }
            for (int i = index; i < l; ++i) cmd[i - index] = cmd[i];
            cmd[l - index] = '\0';
            return result;
        } else {
            throw BlankLineException();
        }
    }

    static bool checkShowFinance(const std::string &cmd){
        char check[7];
        for (int i = 0; i < 7; ++i) {
            check[i] = cmd[i];
        }
        std::string check_word = std::string(check);
        if (check_word == "finance") return true;
        return false;
    }

public:

    parser() = default;

    ~parser() = default;

    //生成所执行的语法类型
    static Statement *parserWord(std::string &cmd) {
        std::string line = cmd;
        std::string order = takeFirstWord(cmd);
        if (order == "quit") {
            return new Quit(cmd);
        } else if (order == "exit") {
            return new Exit(cmd);
        } else if (order == "su") {
            return new Su(cmd, line);
        } else if (order == "logout") {
            return new Logout(cmd, line);
        } else if (order == "register") {
            return new Register(cmd, line);
        } else if (order == "passwd") {
            return new Passwd(cmd, line);
        } else if (order == "useradd") {
            return new Useradd(cmd, line);
        } else if (order == "delete") {
            return new Delete(cmd, line);
        } else if (order == "show") {
            if (checkShowFinance(cmd)) {
                return new ShowFinance(cmd, line);
            } else {
                return new Show(cmd,line);
            }
        } else if (order == "buy") {
            return new Buy(cmd, line);
        } else if (order == "select") {
            return new Select(cmd, line);
        } else if (order == "modify") {
            return new Modify(cmd, line);
        } else if (order == "import") {
            return new Import(cmd, line);
        } else if (order == "log") {
            return new log(cmd, line);
        } else if (order == "report") {
            std::string order_second = takeFirstWord(cmd);
            if (order_second == "myself") {
                return new ReportMyself(cmd, line);
            } else if (order_second == "finance") {
                return new ReportFinance(cmd, line);
            } else if (order_second == "employee") {
                return new ReportEmployee(cmd, line);
            } else {
                throw BasicException();
            }
        } else {
            throw BasicException();
        }
    }

};
#endif //UNTITLED3_PARSER_H
