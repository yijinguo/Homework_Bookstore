#ifndef UNTITLED3_DIARY_H
#define UNTITLED3_DIARY_H

#include "account.h"

struct trade{
    //详细交易情况
    char userID[30];
    char cmd[6];
    char ISBN[20];
    char bookName[60];
    int quantity;
    char income[13];
    char expense[13];
    //交易总额
    char inAll[13];
    char outAll[13];
    void initialize(std::string _user_id,std::string _cmd,std::string isbn,std::string _book_name,int _quantity,std::string cost){

    }
};

class Diary {
private:
    FinanceRecord<trade> deal;//记录交易情况
    DiaryRecord diary;//记录所有操作
    int Time;//目前交易总笔数
    char inAll[13];//目前总收入
    char outAll[13];//目前总支出

public:

    Diary();
    ~Diary();
    //将某用户的某次操作记录写入文件
    void write(int &priority,std::string &name, std::string &content);
    //收入
    void buyBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,std::string cost);
    //支出
    void importBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,std::string cost);

    //{3}reportMyself
    void reportMyself(std::string &index);
    //{7}财务记录查询
    void showFinance(std::string time);
    //{7}生成财务记录报告
    void reportFinance();
    //{7}生成全体员工工作情况报告
    void reportEmployee();
    //{7}生成日志
    void LogRecord();

};
#endif //UNTITLED3_DIARY_H
