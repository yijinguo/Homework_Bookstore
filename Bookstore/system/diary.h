#ifndef UNTITLED3_DIARY_H
#define UNTITLED3_DIARY_H

#include "account.h"

//在指定交易笔数下的交易总额
struct SumIncome {
    int time;
    double in = 0;
    double out = 0;
    friend std::ostream &operator<<(std::ostream &os, const SumIncome &b){
        os << "+ " << b.in << " - " << b.out << '\n';
        return os;
    }
    bool operator<(const SumIncome &b) const {
        if (time < b.time) return true;
        return false;
    }
    bool operator>(const SumIncome &b) const {
        if (time > b.time) return true;
        return false;
    }
    friend bool operator<(const SumIncome &a,const SumIncome &b){
        if (a.time < b.time) return true;
        return false;
    }
    friend bool operator>(const SumIncome &a,const SumIncome &b){
        if (a.time > b.time) return true;
        return false;
    }
    friend bool operator==(const SumIncome &a, const SumIncome &b){
        if (a.time == b.time) return true;
        return false;
    }
};

class Diary {
private:
    FileData<SumIncome> finance;
    SumIncome latest;//最近一次交易总额
    int Time;//交易总笔数
    std::fstream diary;//这是一个直接可读的文件,是所有人操作的记录,格式：权限 ID 操作 \n
public:
    Diary();
    ~Diary();
    //将某用户的某次操作记录写入文件
    void write(int &priority,std::string &name, std::string &content);
    //收入
    void buyBook(double cost);
    //支出
    void importBook(double total_cost);

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
