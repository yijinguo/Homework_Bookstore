#ifndef UNTITLED3_PARSER_H
#define UNTITLED3_PARSER_H

#include "statement.h"

//返回第一个单词，剩余cmd
static std::string takeFirstWord(std::string &cmd){
    int index = 0;
    int firstWordIndex;
    std::string firstWord;
    bool haveBlank = false;
    while (cmd[index] == ' ') {
        haveBlank = true;
        ++index;
    }
    if (cmd[index] != '\0') {
        if (haveBlank) throw BasicException();
        firstWordIndex = index;
        while (cmd[index] != ' ' && cmd[index] != '\0') {++index;}
        char word[index - firstWordIndex + 1];
        for (int i = 0; i < index - firstWordIndex; ++i) {word[i] = cmd[i + firstWordIndex];}
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


//生成所执行的语法类型
void execute(Statement &stmt, std::string &cmd) {
    try {
        std::string line = cmd;
        std::string order = takeFirstWord(cmd);
        if (order == "quit") {
            stmt.Quit();
        } else if (order == "exit") {
            stmt.Exit();
        } else if (order == "su") {
            stmt.define(cmd, line);
            stmt.Su();
        } else if (order == "logout") {
            stmt.define(cmd, line);
            stmt.Logout();
        } else if (order == "register") {
            stmt.define(cmd, line);
            stmt.Register();
        } else if (order == "passwd") {
            stmt.define(cmd, line);
            stmt.Passwd();
        } else if (order == "useradd") {
            stmt.define(cmd, line);
            stmt.Useradd();
        } else if (order == "delete") {
            stmt.define(cmd, line);
            stmt.Delete();
        } else if (order == "show") {
            if (checkShowFinance(cmd)) {
                stmt.define(cmd, line);
                stmt.ShowFinance();
            } else {
                stmt.define(cmd, line);
                stmt.Show();
            }
        } else if (order == "buy") {
            stmt.define(cmd, line);
            stmt.Buy();
        } else if (order == "select") {
            stmt.define(cmd, line);
            stmt.Select();
        } else if (order == "modify") {
            stmt.define(cmd, line);
            stmt.Modify();
        } else if (order == "import") {
            stmt.define(cmd, line);
            stmt.Import();
        } else if (order == "log") {
            stmt.define(cmd, line);
            stmt.log();
        } else if (order == "report") {
            std::string order_second = takeFirstWord(cmd);
            if (order_second == "myself") {
                stmt.define(cmd, line);
                stmt.ReportMyself();
            } else if (order_second == "finance") {
                stmt.define(cmd, line);
                stmt.ReportFinance();
            } else if (order_second == "employee") {
                stmt.define(cmd, line);
                stmt.ReportEmployee();
            } else {
                throw BasicException();
            }
        } else {
            throw BasicException();
        }
    } catch (BasicException &ex) {
        throw BasicException();
    } catch (BlankLineException &ex) {
        throw BlankLineException();
    }
}

#endif //UNTITLED3_PARSER_H
