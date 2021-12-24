#include "diary.h"

Diary::Diary(){

}
Diary::~Diary(){

}

void Diary::write(int &priority,std::string &name, std::string &content){
    diary.write(priority,name,content);
}

//struct trade{
//    //详细交易情况
//    char userID[30];
//    char cmd[6];
//    char ISBN[20];
//    int quantity;
//    char income[13];
//    char expense[13];
//    //交易总额
//    char inAll[13];
//    char outAll[13];
//};
void Diary::buyBook(std::string _user_id, std::string isbn,std::string _book_name,int _quantity,std::string cost) {
    trade tradeRecord;
    strcpy(tradeRecord.userID,_user_id.c_str());
    strcpy(tradeRecord.cmd,"buy");
    strcpy(tradeRecord.ISBN,isbn.c_str());
    strcpy(tradeRecord.bookName,_book_name.c_str());
    tradeRecord.quantity = _quantity;
    strcpy(tradeRecord.income,cost.c_str());
    strcpy(tradeRecord.expense,"0");
}

void importBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,std::string cost){

}
//根据ID为索引，找出该用户的所有操作
//{3}reportMyself
void Diary::reportMyself(std::string &index){

}

//注意判断time和私有Time的大小//若返回最新一笔，则time = -1;
//{7}财务记录查询
void Diary::showFinance(std::string time){

}

//{7}生成财务记录报告
void Diary::reportFinance(){

}

//{7}生成全体员工工作情况报告
void Diary::reportEmployee(){

}

//{7}生成日志
void Diary::LogRecord(){

}
