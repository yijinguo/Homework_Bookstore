#include "FileData.h"

Stack::Stack() = default;

Stack::~Stack() {
    vecStack.clear();
}

void Stack::push(std::string content){
    account a;
    a.accountName = content;
    vecStack.push_back(a);
}

account Stack::pop() {
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
        if (it->accountName == index) return true;
        it++;
    }
    return false;
}

void Stack::erase(std::string index){
    auto it = vecStack.begin();
    while (it != vecStack.end()) {
        if (it->accountName == index) {
            vecStack.erase(it);
            return;
        }
        it++;
    }
}

bool Stack::selectBook(){
    account a = vecStack.back();
    if (a.bookISBN == "\0") return false;
    return true;
}

void Stack::modifyBook(const std::string &ISBN){
    account a = vecStack.back();
    a.bookISBN = ISBN;
    vecStack.pop_back();
    vecStack.push_back(a);
}

void Stack::modifyIndex(std::string &oldIndex, std::string newIndex){
    int i = 0;
    auto it = vecStack.begin();
    while (it != vecStack.end()) {
        if (it->bookISBN == oldIndex) {
            it->bookISBN = newIndex;
            vecStack[i] = *it;
        }
        it++;
        i++;
    }
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

std::string takeFirstWord(std::string &cmd){
    int index = 0;
    int firstWordIndex;
    std::string firstWord;
    while (cmd[index] == ' ') {++index;}
    if (cmd[index] != '\0') {
        firstWordIndex = index;
        while (cmd[index] != ' ' && cmd[index] != '\0') {++index;}
        char word[index - firstWordIndex + 1];
        for (int i = 0; i < index - firstWordIndex; ++i) {word[i] = cmd[i + firstWordIndex];}
        word[index - firstWordIndex] = '\0';
        std::string result = std::string(word);
        while (cmd[index] == ' ') {++index;}
        int l = cmd.length();
        if (cmd[index] == '\0') {
            cmd = "\0";
            return result;
        }
        for (int i = index; i < l; ++i) cmd[i - index] = cmd[i];
        cmd[l - index] = '\0';
        return result;
    } else {
        throw BlankLineException();
    }
}
//生成员工操作,注意生成最后需要多输出一行'\n'作为划分
void DiaryRecord::returnIndex(std::string index, int p) {
    int _priority = 0;
    char nameAndContent[1050];
    staffRecord.seekp(0,std::ios::end);
    staffRecord << index << "'s record" <<'\n';
    diaryRecord.close();
    diaryRecord.open("fileDiaryRecord",std::fstream::in);
    int operator_num = 0;
    while (diaryRecord >> _priority) {
        diaryRecord.getline(nameAndContent, 1050, '\n');
        if (_priority != p) continue;
        std::string content = std::string(nameAndContent);
        std::string name = takeFirstWord(content);
        if (name != index) continue;
        char transfer[1024];
        strcpy(transfer,content.c_str());
        std::string _content = std::string(transfer);
        staffRecord << _content << '\n';
        operator_num++;
    }
    if (p == 7) {
        staffRecord << "This is my life." << '\n';
    } else {
        if (operator_num == 0) {
            staffRecord << "Maybe "<< index << " has a day off today." << '\n';
        } else if (operator_num <= 30) {
            staffRecord << index << " is working hard." << '\n';
        } else {
            staffRecord << index << " is so busy now." << '\n';
        }
    }
    staffRecord << '\n';
}

void DiaryRecord::clear(){
    staffRecord.close();
    staffRecord.open("fileStaffRecord",std::fstream::out);
    staffRecord.close();
    staffRecord.open("fileStaffRecord");
}



FinanceRecord::FinanceRecord(){
    Information initial;
    initial.time = 0;
    initial.income = 0;
    initial.expense = 0;
    file.open("fileFinanceRecord",std::fstream::in);
    if (!file) {
        Time = 0;
        inAll = 0;
        outAll = 0;
        file.open("fileFinanceRecord",std::fstream::out);
        file.write(reinterpret_cast<char*>(&initial),sizeInf);
        file.close();
    } else {
        file.read(reinterpret_cast<char*>(&initial), sizeInf);
        file.close();
        Time = initial.time;
        inAll = initial.income;
        outAll = initial.expense;
    }
    file.open("fileFinanceRecord");
}

FinanceRecord::~FinanceRecord(){
    Information last;
    last.time = Time;
    last.income = inAll;
    last.expense = outAll;
    file.seekp(0);
    file.write(reinterpret_cast<char*>(&last),sizeInf);
    file.close();
}

void FinanceRecord::addRecord(double income, double expense){
    Information add;
    Time++;
    add.time = Time;
    add.income = income;
    add.expense = expense;
    inAll += add.income;
    outAll += add.expense;
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
        incomeAll += tmp.income;
        expenseAll += tmp.expense;
    }
    std::cout << "+ " << std::fixed << std::setprecision(2) << incomeAll << " - " << std::fixed << std::setprecision(2) << expenseAll << '\n';
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
    tradeRecord.open("fileTradeRecord",std::fstream::app);
}

TradeRecord::~TradeRecord(){
    tradeRecord.close();
}

void TradeRecord::buyBook(std::string _user_id, std::string isbn,std::string _book_name,int _quantity,double cost){
    tradeRecord << _user_id << " buy " << isbn << " " << _book_name << " " << _quantity << " " << std::fixed << std::setprecision(2) << cost << '\n';
}

void TradeRecord::importBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,double cost){
    tradeRecord << _user_id << " import " << isbn << " " << _book_name << " " << _quantity << " " << std::fixed << std::setprecision(2) << cost << '\n';
}

void TradeRecord::writeTotal(FinanceRecord &a){
    tradeRecord << "The boss wants to know the sales now\n";
    tradeRecord << "Trade times: " << a.Time << "\nTotal income: " << a.inAll << "\nTotal cost: " << a.outAll << "\nTotal profit: " << a.inAll - a.outAll << '\n';
}

