#ifndef UNTITLED3_DIARY_H
#define UNTITLED3_DIARY_H

#include "account.h"

class Diary {
private:

    DiaryRecord diary;//记录所有操作
    TradeRecord trade;//记录所有交易
    FinanceRecord deal;//记录交易总额

public:

    //Diary();
    //~Diary();

    //将某用户的某次操作记录写入文件
    void write(int &priority,std::string &name, std::string &content);
    //收入
    void buyBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,std::string cost);
    //支出
    void importBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,std::string cost);

    //{3}reportMyself
    void reportMyself(std::string &index);
    //{7}财务记录查询
    void showFinance(int time);
    //{7}生成财务记录报告
    void reportFinance();
    //{7}生成全体员工工作情况报告
    void reportEmployee();
    //{7}生成日志
    void LogRecord();

};
#endif //UNTITLED3_DIARY_H
