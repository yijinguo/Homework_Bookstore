#ifndef UNTITLED3_STATEMENT_H
#define UNTITLED3_STATEMENT_H

#include "system/books.h"

class Statement{
private:
    Account accountSystem;
    Books bookSystem;
    Diary diarySystem;
    std::string diaryLine;
    std::string cmdLine;
    int wordNum = 0;//vector中非空字符数量
    std::vector<std::string> words;//储存每句语法被拆解出的单词
public:
    Statement();
    ~Statement();
    void define(std::string &cmd, std::string &line);
    void separateCmd(std::string &cmd);
    void Quit();
    void Exit();
    void Su();
    void Logout();
    void Register();
    void Passwd();
    void Useradd();
    void Delete();
    void Show();
    void Buy();
    void Select();
    void Modify();
    void Import();
    void ReportMyself();
    void ShowFinance();
    void ReportFinance();
    void ReportEmployee();
    void log();
};


#endif //UNTITLED3_STATEMENT_H
