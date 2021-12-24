#include "FileData.h"

Stack::Stack(){
    stackLog.open("fileStackLog");
}

Stack::~Stack() {
    stackLog.close();
    stackLog.open("fileStackLog",std::fstream::out);
    stackLog.close();
}

void Stack::push(std::string content){
    Information newPush;
    newPush.front = nowLoc;
    strcpy(newPush.index,content.c_str());
    lastLoc = nowLoc;
    stackLog.seekp(0,std::ios::end);
    nowLoc = stackLog.tellp() / sizeT;
    stackLog.write(reinterpret_cast<char*>(&newPush),sizeT);
}

std::string Stack::pop(){
    Information last;
    nowLoc = lastLoc;
    stackLog.seekg(lastLoc * sizeT);
    stackLog.read(reinterpret_cast<char*>(&last),sizeT);
    lastLoc = last.front;
    return std::string(last.index);
}

bool Stack::judgeNull() const {
    if (nowLoc == -1) return true;
    return false;
}

bool Stack::InStack(std::string index) {
    int tmp_nowLoc = nowLoc;
    Information tmp;
    while (tmp_nowLoc != -1) {
        stackLog.seekg(tmp_nowLoc);
        stackLog.read(reinterpret_cast<char *>(&tmp), sizeT);
        if (index == std::string(tmp.index)) return true;
        tmp_nowLoc = tmp.front;
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
    file.open("fileFinanceRecord",std::fstream::in);
    if (!file) {
        file.open("fileFinanceRecord",std::fstream::out);
    }
    file.close();
    file.open("fileFinanceRecord");
    Time = 0;
    strcpy(inAll, "0");
    strcpy(outAll,"0");
}

FinanceRecord::~FinanceRecord(){
    file.close();
}

void FinanceRecord::addRecord(std::string income, std::string expense){
    Information add;
    Time++;
    double in = stringToDouble(income) + stringToDouble(std::string(inAll));
    strcpy(inAll, doubleToString(in).c_str());
    double out = stringToDouble(expense) + stringToDouble(std::string(outAll));
    strcpy(outAll, doubleToString(out).c_str());
    add.time = Time;
    strcpy(add.income, inAll);
    strcpy(add.expense, outAll);
    file.seekp(0, std::ios::end);
    file.write(reinterpret_cast<char*>(&add),sizeInf);
}

void FinanceRecord::printTime(int time){
    if (time > Time) throw BasicException();
    Information print;
    file.seekg(time * sizeInf);
    file.read(reinterpret_cast<char*>(&print),sizeInf);
    std::cout << "+ " << print.income << " - " << print.expense << '\n';
}

void FinanceRecord::printAll(){
    for (int i = 0; i < Time; ++i) {
        Information print;
        file.seekg(i * sizeInf);
        file.read(reinterpret_cast<char*>(&print),sizeInf);
        std::cout << "+ " << print.income << " - " << print.expense << '\n';
    }
}




TradeRecord::TradeRecord(){
    tradeRecord.open("fileTradeFile",std::fstream::in);
    if (!tradeRecord) {
        tradeRecord.open("fileTradeFile",std::fstream::out);
    }
    tradeRecord.close();
    tradeRecord.open("fileTradeFile");
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

