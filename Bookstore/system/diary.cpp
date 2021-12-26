#include "diary.h"

void Diary::write(int &priority,std::string &name, std::string &content){
    diary.write(priority,name,content);
}


void Diary::buyBook(std::string _user_id, std::string isbn,std::string _book_name,int _quantity,double cost) {
    trade.buyBook(_user_id,isbn,_book_name,_quantity,cost);
    deal.addRecord(cost,0);
}

void Diary::importBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,double cost){
    trade.importBook(_user_id,isbn,_book_name,_quantity,cost);
    deal.addRecord(0,cost);
}

//根据ID为索引，生成该用户的所有操作
//{3}reportMyself
void Diary::reportMyself(std::string &userID){
    diary.clear();
    diary.returnIndex(userID);
}

//注意判断time和私有Time的大小//若返回最新一笔，则time = -1;
//{7}财务记录查询
void Diary::showFinance(int time) {
    try {
        if (time == -1) {
            deal.printAll();
        } else if (time == 0) {
            std::cout << '\n';
        } else {
            deal.printTime(time);
        }
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

//{7}生成财务记录报告
void Diary::reportFinance(){
    trade.writeTotal(deal);
}

//{7}生成全体员工工作情况报告
void Diary::reportEmployee(){
    for (int i = 0; i < Account::staffNum; ++i) {
        diary.returnIndex(Account::staffAll[i]);
    }
}

//{7}生成日志
void Diary::LogRecord(){
    trade.writeTotal(deal);
}
