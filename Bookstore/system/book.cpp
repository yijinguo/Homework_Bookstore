#include "books.h"

//todo keyword,book-name,author不含英文双引号，该问题没有判断

Books::Books(){
    BookDataStore.initialise("fileAllBooks");
}

void Books::show(std::string cmd) {
    //cmd为除了show以外的语句
    try {
        if (Account::accountLog.priority < 1) throw BasicException();
        int index = 0;
        BooksInf demandInfo;
        while (cmd[index] == ' ') { index++; }
        if (cmd[index] == '\0') {
            //输出所有满足要求的图书
            BookDataStore.printAllInfo();
        } else {
            while (cmd[index] != '\0') {
                int tmp = index;
                while (cmd[index] != '=' && cmd[index] != '\0') { index++; }
                if (cmd[index] == '\0') throw BasicException();
                char word[index - tmp];
                for (int i = 0; i < index - tmp; ++i) {
                    word[i] = cmd[tmp + i];//word:-ISBN等
                }
                index++;
                tmp = index;
                if (cmd[index] == '\0') throw BasicException();
                while (cmd[index] != ' ' && cmd[index] != '\0') { index++; }
                char demand[index - tmp];
                for (int i = 0; i < index - tmp; ++i) {
                    demand[i] = cmd[tmp + i];
                }
                std::string _word = std::string(word);
                std::string _demand = std::string(demand);
                //以上处理指令，word表示要求的信息类型，demand表示具体信息
                defineDemand(demandInfo, _word, _demand);//以下将要求的信息放入demandInfo
                if (cmd[index] == '\0') break;
                while (cmd[index] == ' ') { index++; }
                if (cmd[index] != '-') throw BasicException();
            }
            //现在得到了demandInfo，遍历文件输出所有符合的内容
            BookDataStore.printDemand(demandInfo);
        }
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::buy(const std::string &isbn, const int _quantity,Diary &diarySystem) {
    try {
        if (Account::accountLog.priority < 1) throw BasicException();
        BooksInf modify;
        strcpy(modify.ISBN,isbn.c_str());
        BookDataStore.readInfo(modify);
        if (modify.quantity < _quantity) throw BasicException();
        modify.quantity -= _quantity;
        BookDataStore.modifyInfo(modify);
        std::string s = doubleToString(_quantity * modify.price);
        double buyCost = stringToDouble(s);
        diarySystem.buyBook(buyCost);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::select(const std::string isbn){
    try {
        if (Account::accountLog.priority < 3) throw BasicException();
        BooksInf selectBook;
        strcpy(selectBook.ISBN, isbn.c_str());
        if (BookDataStore.findInfo(selectBook)) {
            bookSelect = BookDataStore.readInfo(selectBook);
        } else {
            bookSelect = selectBook;
            BookDataStore.addInfo(selectBook);
        }
        if (!Account::haveSelect) Account::selectTrue();
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::modify(std::string cmd) {
    try {
        if (Account::accountLog.priority < 3 || !Account::haveSelect) throw BasicException();
        if (!Account::haveSelect) throw BasicException();
        if (cmd.empty()) throw BasicException();
        int index = 0;
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
        //现在得到了demandInfo，遍历文件输出所有符合的内容
        BookDataStore.modifyInfo(bookSelect);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::import(int _quantity, const double _total_cost, Diary &diarySystem){
    try {
        if (Account::accountLog.priority < 3 || !Account::haveSelect) throw BasicException();
        if (!Account::haveSelect) throw BasicException();
        bookSelect.quantity += _quantity;
        std::string s = doubleToString(_total_cost);
        double _totalCost = stringToDouble(s);
        diarySystem.importBook(_totalCost);
        bookSelect.totalCost += _totalCost;
        double _price = _total_cost / _quantity;
        std::string p = doubleToString(_price);
        bookSelect.price = stringToDouble(p);
        BookDataStore.modifyInfo(bookSelect);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

//以下为补充函数
void Books::defineDemand(BooksInf &demandInfo,std::string word, std::string demand) {
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
            if (demand[i] == '|' || demand[i] == ' ') throw BasicException();
        }
        strcpy(demandInfo.keyword,demand.c_str());
    } else if (word == "-price") {
        demandInfo.price = stringToDouble(demand);
    } else {
        throw BasicException();
    }
}
