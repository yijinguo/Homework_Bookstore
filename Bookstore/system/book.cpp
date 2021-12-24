#include "books.h"

//todo keyword,book-name,author不含英文双引号，该问题没有判断

Books::Books(){
    BookDataStore.initialize("fileAllBooksData","fileAllBooks");
}

void Books::show(std::string cmd) {
    try {
        if (Account::accountLog.priority < 1) throw BasicException();
        int index = 0;
        BooksInf demandInfo;
        while (cmd[index] == ' ') { index++; }
        if (cmd[index] == '\0') {
            //输出所有图书
            BookDataStore.printAllInfo();
        } else if (cmd[index] == '-') {

            int tmp = index;
            while (cmd[index] != '=' && cmd[index] != '\0') { index++; }
            if (cmd[index] == '\0') throw BasicException();
            char word[index - tmp];
            for (int i = 0; i < index - tmp; ++i) {
                word[i] = cmd[tmp + i];//word:-ISBN等
            }
            index++;
            tmp = index;
            if (cmd[index] == '\0' || cmd[index] == ' ') throw BasicException();
            while (cmd[index] != ' ' && cmd[index] != '\0') { index++; }
            char demand[index - tmp];
            for (int i = 0; i < index - tmp; ++i) {
                demand[i] = cmd[tmp + i];
            }
            while (cmd[index] == ' ') { index++; }
            if (cmd[index] != '\0') throw BasicException();
            //以上处理指令，word表示要求的信息类型，demand表示具体信息
            std::string _word = std::string(word);
            std::string _demand = std::string(demand);
            defineShowDemand(demandInfo, _word, _demand);//以下将要求的信息放入demandInfo
            while (cmd[index] == ' ') { index++; }
            if (cmd[index] != '-') throw BasicException();
            //现在得到了demandInfo，遍历文件输出所有符合的内容
            BookDataStore.printDemand(demandInfo);
        } else {
            throw BasicException();
        }
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::buy(const std::string &isbn, const int _quantity,Diary &diarySystem) {
    try {
        if (Account::accountLog.priority < 1) throw BasicException();
        BooksInf modify = BookDataStore.readInfo(isbn);
        if (modify.quantity < _quantity) throw BasicException();
        modify.quantity -= _quantity;
        BookDataStore.modifyInfo(isbn,modify);
        std::string buyCost = doubleToString(_quantity * stringToDouble(modify.price));
        std::string _user_id = std::string(Account::accountLog.userID);
        std::string _book_name = bookSelect.bookName;
        diarySystem.buyBook(_user_id,isbn,_book_name,_quantity,buyCost);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::select(const std::string isbn){
    try {
        if (Account::accountLog.priority < 3) throw BasicException();
        if (!Account::haveSelect) Account::selectTrue();
        bookSelect = BookDataStore.findInfo(isbn);
    } catch (CreateException &ex) {
        BooksInf newCreate;
        strcpy(newCreate.ISBN,isbn.c_str());
        BookDataStore.addInfo(isbn,newCreate);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::modify(std::string cmd) {
    try {
        if (Account::accountLog.priority < 3 || !Account::haveSelect) throw BasicException();
        if (cmd.empty()) throw BasicException();
        int index = 0;
        std::string OldIndex = std::string(bookSelect.ISBN);
        while (cmd[index] != '\0') {
            while (cmd[index] == ' ') { index++; }
            if (cmd[index] != '-') throw BasicException();
            int tmp = index;
            while (cmd[index] != '=' && cmd[index] != '\0') {index++;}
            if (cmd[index] == '\0') throw BasicException();
            char word[index - tmp];
            for (int i = 0; i < index - tmp; ++i) {
                word[i] = cmd[tmp + i];//word:-ISBN等
            }
            index++;
            if (cmd[index] == '\0') throw BasicException();
            tmp = index;
            while (cmd[index] != '\0' && cmd[index] != ' ') {index++;}
            char demand[index - tmp];
            for (int i = 0; i < index - tmp; ++i) {
                demand[i] = cmd[tmp + i];
            }
            std::string _word = std::string(word);
            std::string _demand = std::string(demand);
            //以上处理指令，word表示要求的信息类型，demand表示具体信息
            defineDemand(bookSelect, _word, _demand);//以下将要求的信息放入demandInfo
            if (cmd[index] == '\0') break;
        }
        //现在得到了demandInfo，进行修改
        if (strcmp(bookSelect.ISBN,OldIndex.c_str()) != 0) {
            BookDataStore.modifyIndex(OldIndex,bookSelect.ISBN,bookSelect);
        } else {
            BookDataStore.modifyInfo(OldIndex,bookSelect);
        }
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::import(int _quantity, const std::string _total_cost, Diary &diarySystem){
    try {
        if (Account::accountLog.priority < 3 || !Account::haveSelect) throw BasicException();
        bookSelect.quantity += _quantity;
        if (!checkDouble(_total_cost)) throw BasicException();
        double _totalCost = stringToDouble(_total_cost);
        double oldTotal = stringToDouble(bookSelect.totalCost);
        strcpy(bookSelect.totalCost, doubleToString(oldTotal + _totalCost).c_str());
        std::string _price = doubleToString(_totalCost / _quantity);
        strcpy(bookSelect.price, _price.c_str());
        BookDataStore.modifyInfo(bookSelect.ISBN,bookSelect);
        //    void importBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,std::string cost);
        std::string _user_id = std::string(Account::accountLog.userID);
        std::string isbn = std::string(bookSelect.ISBN);
        std::string _book_name = std::string(bookSelect.bookName);
        diarySystem.importBook(_user_id,isbn,_book_name,_quantity,_total_cost);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

//以下为补充函数
//只针对show
void Books::defineShowDemand(BooksInf &demandInfo, std::string word, std::string demand){
    if (word == "-ISBN") {
        strcpy(demandInfo.ISBN,demand.c_str());
    } else if (word == "-name") {
        strcpy(demandInfo.bookName,demand.c_str());
    } else if (word == "-author") {
        strcpy(demandInfo.author,demand.c_str());
    } else if (word == "-keyword") {
        int i = 0;
        while (demand[i] != '\0') {
            i++;
            if (demand[i] == '|') throw BasicException();
        }
        strcpy(demandInfo.keyword,demand.c_str());
    } else {
        throw BasicException();
    }
}

//只针对modify
void Books::defineDemand(BooksInf &demandInfo,std::string word, std::string demand) {
    if (word == "-ISBN") {
        strcpy(demandInfo.ISBN,demand.c_str());
    }
    if (word == "-name") {
        strcpy(demandInfo.bookName,demand.c_str());
    }
    if (word == "-author") {
        strcpy(demandInfo.author,demand.c_str());
    }
    if (word == "-keyword") {
        int i = 0;
        while (demand[i] != '\0') {
            i++;
            if (demand[i] == '|' || demand[i] == ' ') throw BasicException();
        }
        strcpy(demandInfo.keyword,demand.c_str());
    }
    if (word == "-price") {
        if (!checkDouble(demand)) throw BasicException();
        strcpy(demandInfo.price,demand.c_str());
    } else {
        throw BasicException();
    }
}

//检查一个字符串是否符合小数点后两位的要求
bool Books::checkDouble(std::string money){
    for (int i = 0; i < money.length() - 3; ++i){
        if (!(money[i] >= '0' && money[i] <= '9')) return false;
    }
    if (money[money.length() - 3] != '.') return false;
    for (int i = money.length() - 2; i <= money.length() - 1; ++i) {
        if (!(money[i] >= '0' && money[i] <= '9')) return false;
    }
    return true;
}