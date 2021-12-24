#ifndef UNTITLED3_STATEMENT_H
#define UNTITLED3_STATEMENT_H

#include "system/books.h"

class Statement{
public:
    Account accountSystem;
    Books bookSystem;
    Diary diarySystem;
    std::string diaryLine;
    std::string cmdLine;
    int wordNum = 0;//vector中非空字符数量
    std::vector<std::string> words;//储存每句语法被拆解出的单词
    Statement();
    Statement(std::string &cmd);
    void separateCmd(std::string &cmd);
    virtual ~Statement();
    virtual void execute() = 0;//每个派生类中均需要单独实现execute函数，功能用当前priority区分
};

class Quit: public Statement {
public:
    Quit(std::string &cmd);
    virtual void execute();
};

class Exit: public Statement {
public:
    Exit(std::string &cmd);
    virtual void execute();
};

class Su:public Statement{
public:
    Su(std::string &cmd,std::string &line);
    virtual void execute();
};

class Logout:public Statement{
public:
    Logout(std::string &cmd, std::string &line);
    virtual void execute();
};

class Register:public Statement{
public:
    Register(std::string &cmd,std::string &line);
    virtual void execute();
};

class Passwd:public Statement{
public:
    Passwd(std::string &cmd,std::string &line);
    virtual void execute();
};

class Useradd:public Statement{
public:
    Useradd(std::string &cmd, std::string &line);
    virtual void execute();
};

class Delete:public Statement{
public:
    Delete(std::string &cmd, std::string &line);
    virtual void execute();
};

class Show:public Statement{
public:
    Show(std::string &cmd,std::string &line);
    virtual void execute();
};

class Buy:public Statement{
public:
    Buy(std::string &cmd, std::string &line);
    virtual void execute();
};

class Select:public Statement{
public:
    Select(std::string &cmd, std::string &line);
    virtual void execute();
};

class Modify:public Statement{
public:
    Modify(std::string &cmd, std::string &line);
    virtual void execute();
};

class Import:public Statement{
public:
    Import(std::string &cmd, std::string &line);
    virtual void execute();
};

class ReportMyself:public Statement{
public:
    ReportMyself(std::string &cmd, std::string &line);
    virtual void execute();
};

class ShowFinance:public Statement{
public:
    ShowFinance(std::string &cmd, std::string &line);
    virtual void execute();
};

class ReportFinance:public Statement{
public:
    ReportFinance(std::string &cmd, std::string &line);
    virtual void execute();
};

class ReportEmployee:public Statement{
public:
    ReportEmployee(std::string &cmd, std::string &line);
    virtual void execute();
};

class log:public Statement{
public:
    log(std::string &cmd, std::string &line);
    virtual void execute();
};

#endif //UNTITLED3_STATEMENT_H
