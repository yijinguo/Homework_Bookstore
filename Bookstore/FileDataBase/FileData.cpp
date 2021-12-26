#include "FileData.h"

Stack::Stack() = default;

Stack::~Stack() {
    vecStack.clear();
}

void Stack::push(std::string content){

    vecStack.push_back(content);
}

std::string Stack::pop() {
    vecStack.pop_back();
    if (vecStack.empty()) {
        throw DealException();
    } else {
        return vecStack.back();
    }
}

bool Stack::judgeNull() const {
    if (vecStack.empty()) return true;
    return false;
}

bool Stack::InStack(std::string index) {
    auto it = vecStack.begin();
    while (it != vecStack.end()) {
        if (*it == index) return true;
        it++;
    }
    return false;
}




DiaryRecord::DiaryRecord(){
    diaryRecord.open("fileDiaryRecord",std::fstream::in);
    if (!diaryRecord) {
        diaryRecord.open("fileDiaryRecord",std::fstream::out);
        staffRecord.open("fileStaffRecord",std::fstream::out);
        staffRecord.close();
    }
    diaryRecord.close();
    staffRecord.open("fileStaffRecord");
    diaryRecord.open("fileDiaryRecord");
}

DiaryRecord::~DiaryRecord(){
    diaryRecord.close();
    staffRecord.close();
}

void DiaryRecord::write(int &priority,std::string &name, std::string &content){
    diaryRecord.seekp(0,std::ios::end);
    diaryRecord << priority << " " << name << " " << content << '\n';
}
//生成员工操作,注意生成最后需要多输出一行'\n'作为划分
void DiaryRecord::returnIndex(std::string index) {
    int _priority;
    std::string _name;
    std::string _content;
    diaryRecord.seekg(0);
    staffRecord.seekp(0,std::ios::end);
    staffRecord << index << "'s record" <<'\n';
    while (diaryRecord.peek() != EOF) {
        diaryRecord >> _priority >> _name >> _content;
        if (_priority == 3 && _name == index) {
            staffRecord << _content << '\n';
        }
    }
    std::cout << '\n';
}

void DiaryRecord::clear(){
    diaryRecord.close();
    diaryRecord.open("fileDiaryRecord",std::fstream::out);
    diaryRecord.close();
    diaryRecord.open("fileDiaryRecord");
}



FinanceRecord::FinanceRecord(){
    Information initial;
    initial.time = 0;
    strcpy(initial.income, "0.00");
    strcpy(initial.expense,"0.00");
    file.open("fileFinanceRecord",std::fstream::in);
    if (!file) {
        Time = 0;
        strcpy(inAll, "0.00");
        strcpy(outAll,"0.00");
        file.open("fileFinanceRecord",std::fstream::out);
        file.write(reinterpret_cast<char*>(&initial),sizeInf);
        file.close();
    } else {
        file.read(reinterpret_cast<char*>(&initial), sizeInf);
        file.close();
        Time = initial.time;
        strcpy(inAll,initial.income);
        strcpy(outAll, initial.expense);
    }
    file.open("fileFinanceRecord");
}

FinanceRecord::~FinanceRecord(){
    Information last;
    last.time = Time;
    strcpy(last.income,inAll);
    strcpy(last.expense,outAll);
    file.seekp(0);
    file.write(reinterpret_cast<char*>(&last),sizeInf);
    file.close();
}

void FinanceRecord::addRecord(std::string income, std::string expense){
    Information add;
    Time++;
    add.time = Time;
    strcpy(add.income, income.c_str());
    strcpy(add.expense, expense.c_str());
    double in_all = stringToDouble(inAll) + stringToDouble(income);
    double out_all = stringToDouble(outAll) + stringToDouble(expense);
    strcpy(inAll, doubleToString(in_all).c_str());
    strcpy(outAll, doubleToString(out_all).c_str());
    file.seekp(0, std::ios::end);
    file.write(reinterpret_cast<char*>(&add),sizeInf);
}

void FinanceRecord::printTime(int time){
    if (time > Time) throw BasicException();
    double incomeAll = 0, expenseAll = 0;
    for (int i = 1; i <= time; ++i) {
        Information tmp;
        file.seekg((Time - i + 1) * sizeInf);
        file.read(reinterpret_cast<char*>(&tmp),sizeInf);
        incomeAll += stringToDouble(tmp.income);
        expenseAll += stringToDouble(tmp.expense);
    }
    std::string in = doubleToString(incomeAll);
    std::string out = doubleToString(expenseAll);
    std::cout << "+ " << in << " - " << out << '\n';
}

void FinanceRecord::printAll(){
    printTime(Time);
}


TradeRecord::TradeRecord(){
    tradeRecord.open("fileTradeRecord",std::fstream::in);
    if (!tradeRecord) {
        tradeRecord.open("fileTradeRecord",std::fstream::out);
    }
    tradeRecord.close();
    tradeRecord.open("fileTradeRecord");
}

TradeRecord::~TradeRecord(){
    tradeRecord.close();
}

void TradeRecord::buyBook(std::string _user_id, std::string isbn,std::string _book_name,int _quantity,std::string cost){
    tradeRecord << _user_id << " buy " << isbn << " " << _book_name << " " << _quantity << " " << cost << '\n';
}

void TradeRecord::importBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,std::string cost){
    tradeRecord << _user_id << " import " << isbn << " " << _book_name << " " << _quantity << " " << cost << '\n';
}

void TradeRecord::writeTotal(FinanceRecord &a){

    tradeRecord << a.Time << "  + " << a.inAll << " - " << a.outAll << '\n';
}

