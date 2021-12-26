
#include <set>
#include "books.h"


Books::Books(){
    BookDataStore.initialize("fileAllBooksData","fileAllBooks");
    BookNameStore.initialize("fileBooksNameData");
    BookAuthorStore.initialize("fileBooksAuthorData");
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
            char word[index - tmp + 1];
            for (int i = 0; i < index - tmp; ++i) {word[i] = cmd[tmp + i];}//word:-ISBN等
            word[index - tmp] = '\0';
            index++;
            tmp = index;
            if (cmd[index] == '\0' || cmd[index] == ' ') throw BasicException();
            while (cmd[index] != ' ' && cmd[index] != '\0') { index++; }
            char demand[index - tmp + 1];
            for (int i = 0; i < index - tmp; ++i) {demand[i] = cmd[tmp + i];}
            demand[index - tmp] = '\0';
            while (cmd[index] == ' ') { index++; }
            if (cmd[index] != '\0') throw BasicException();
            //以上处理指令，word表示要求的信息类型，demand表示具体信息
            std::string _word = std::string(word);
            std::string _demand = std::string(demand);
            defineShowDemand(demandInfo, _word, _demand);
        } else {
            throw BasicException();
        }
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::buy(const std::string &isbn, const int _quantity, Diary &diarySystem) {
    try {
        if (Account::accountLog.priority < 1) throw BasicException();
        BooksInf modify = BookDataStore.readInfo(isbn);
        if (modify.quantity < _quantity) throw BasicException();
        modify.quantity -= _quantity;
        BookDataStore.modifyInfo(isbn,modify);
        double re = _quantity * modify.price;
        std::cout << std::fixed << std::setprecision(2) << re << '\n';
        std::string _user_id = std::string(Account::accountLog.userID);
        std::string _book_name = bookSelect.bookName;
        diarySystem.buyBook(_user_id,isbn,_book_name,_quantity,re);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::select(const std::string isbn){
    try {
        if (Account::accountLog.priority < 3) throw BasicException();
        if (!Account::haveSelect) Account::selectTrue();
        bookSelect = BookDataStore.findInfo(isbn);
        Account::accountInStack.modifyBook(isbn);
    } catch (CreateException &ex) {
        BooksInf newCreate;
        strcpy(newCreate.ISBN,isbn.c_str());
        BookDataStore.addInfo(isbn,newCreate);
        strcpy(bookSelect.ISBN,isbn.c_str());
        strcpy(bookSelect.bookName, "\0");
        strcpy(bookSelect.author,"\0");
        strcpy(bookSelect.keyword, "\0");
        bookSelect.totalCost = 0;
        bookSelect.price = 0;
        bookSelect.quantity = 0;
        Account::accountInStack.modifyBook(isbn);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::modify(std::string cmd) {
    try {
        if (Account::accountLog.priority < 3 || !Account::haveSelect) throw BasicException();
        if (cmd[0] == '\0') throw BasicException();
        int index = 0;
        std::string OldIndex = std::string(bookSelect.ISBN);
        std::string OldBookName = std::string(bookSelect.bookName);
        std::string OldAuthor = std::string(bookSelect.author);
        while (cmd[index] != '\0') {
            while (cmd[index] == ' ') { index++; }
            if (cmd[index] != '-') throw BasicException();
            int tmp = index;
            while (cmd[index] != '=' && cmd[index] != '\0') { index++; }
            if (cmd[index] == '\0') throw BasicException();
            char word[index - tmp + 1];
            for (int i = 0; i < index - tmp; ++i) { word[i] = cmd[tmp + i]; }
            word[index - tmp] = '\0';
            index++;
            if (cmd[index] == '\0') throw BasicException();
            tmp = index;
            while (cmd[index] != '\0' && cmd[index] != ' ') { index++; }
            char demand[index - tmp + 1];
            for (int i = 0; i < index - tmp; ++i) { demand[i] = cmd[tmp + i]; }
            demand[index - tmp] = '\0';
            std::string _word = std::string(word);
            std::string _demand = std::string(demand);
            //以上处理指令，word表示要求的信息类型，demand表示具体信息
            defineDemand(bookSelect, _word, _demand);//以下将要求的信息放入demandInfo
            if (cmd[index] == '\0') break;
        }
        //现在得到了demandInfo，进行修改
        if (strcmp(bookSelect.ISBN, OldIndex.c_str()) != 0) {//isbn被改
            BookDataStore.modifyIndex(OldIndex, bookSelect.ISBN, bookSelect);
        } else {//isbn未被改
            BookDataStore.modifyInfo(OldIndex, bookSelect);
        }
        if (OldBookName[0] == '\0') {
            if (bookSelect.bookName[0] != '\0') {
                BookNameStore.addInfo(bookSelect.bookName, bookSelect.ISBN);
            }
        } else {
            if (strcmp(bookSelect.bookName, OldBookName.c_str()) != 0 ||
                strcmp(bookSelect.ISBN, OldIndex.c_str()) != 0) {
                BookNameStore.modifyIndex(OldBookName, bookSelect.bookName, OldIndex, bookSelect.ISBN);
            }
        }
        if (OldAuthor[0] == '\0') {
            if ( bookSelect.author[0] != '\0') {
                BookAuthorStore.addInfo(bookSelect.author, bookSelect.ISBN);
            }
        } else {
            if (strcmp(bookSelect.author, OldAuthor.c_str()) != 0 ||
                strcmp(bookSelect.ISBN, OldIndex.c_str()) != 0) {
                BookAuthorStore.modifyIndex(OldAuthor, bookSelect.author, OldIndex, bookSelect.ISBN);
            }
        }
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Books::import(int _quantity, const std::string _total_cost, Diary &diarySystem){
    try {
        if (Account::accountLog.priority < 3 || !Account::haveSelect) throw BasicException();
        bookSelect.quantity += _quantity;
        bookSelect.totalCost += checkDouble(_total_cost);
        BookDataStore.modifyInfo(bookSelect.ISBN,bookSelect);
        std::string _user_id = std::string(Account::accountLog.userID);
        std::string isbn = std::string(bookSelect.ISBN);
        std::string _book_name = std::string(bookSelect.bookName);
        diarySystem.importBook(_user_id,isbn,_book_name,_quantity, stringToDouble(_total_cost));
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

//以下为补充函数
//只针对show
void Books::defineShowDemand(BooksInf &demandInfo, std::string word, std::string demand) {
    try {
        if (word == "-ISBN") {
            BookDataStore.printIndex(demand);
        } else if (word == "-name") {
            if (demand[0] != '"' || demand[demand.length() - 1] != '"') throw DealException();
            for (int i = 0; i < demand.length() - 2; ++i) { demand[i] = demand[i + 1]; }
            demand[demand.length() - 2] = '\0';
            BookNameStore.printIndex(BookDataStore,demand);
        } else if (word == "-author") {
            if (demand[0] != '"' || demand[demand.length() - 1] != '"') throw DealException();
            for (int i = 0; i < demand.length() - 2; ++i) { demand[i] = demand[i + 1]; }
            demand[demand.length() - 2] = '\0';
            BookAuthorStore.printIndex(BookDataStore,demand);
        } else if (word == "-keyword") {
            if (demand[0] != '"' || demand[demand.length() - 1] != '"') throw DealException();
            for (int i = 0; i < demand.length() - 2; ++i) {demand[i] = demand[i + 1];}
            demand[demand.length() - 2] = '\0';
            int i = 0;
            while (demand[i] != '\0') {
                i++;
                if (demand[i] == '|') throw DealException();
            }
            strcpy(demandInfo.keyword, demand.c_str());
            BookDataStore.printDemand(demandInfo);
        } else {
            throw DealException();
        }
    } catch (BasicException &ex) {
        std::cout << '\n';
    } catch (DealException &ex) {
        throw BasicException();
    }
}

//只针对modify
void Books::defineDemand(BooksInf &demandInfo,std::string word, std::string demand) {
    try {
        if (word == "-ISBN") {
            strcpy(demandInfo.ISBN, demand.c_str());
        } else if (word == "-name") {
            if (demand[0] != '"' || demand[demand.length() - 1] != '"') throw BasicException();
            for (int i = 0; i < demand.length() - 2; ++i) {
                demand[i] = demand[i + 1];
            }
            demand[demand.length() - 2] = '\0';
            strcpy(demandInfo.bookName, demand.c_str());
        } else if (word == "-author") {
            if (demand[0] != '"' || demand[demand.length() - 1] != '"') throw BasicException();
            for (int i = 0; i < demand.length() - 2; ++i) {
                demand[i] = demand[i + 1];
            }
            demand[demand.length() - 2] = '\0';
            strcpy(demandInfo.author, demand.c_str());
        } else if (word == "-keyword") {
            if (demand[0] != '"' || demand[demand.length() - 1] != '"') throw BasicException();
            for (int i = 0; i < demand.length() - 2; ++i) {
                demand[i] = demand[i + 1];
            }
            demand[demand.length() - 2] = '\0';
            checkKeyword(demand);
            strcpy(demandInfo.keyword, demand.c_str());
        } else if (word == "-price") {
            demandInfo.price = checkDouble(demand);
        } else {
            throw BasicException();
        }
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

//检查一个字符串是否是浮点数，如果是就修改为double
double Books::checkDouble(std::string money){
    bool haveDot = false;
    for (int i = 0; i < money.length(); ++i) {
        if (money[i] == '.') {
            if (haveDot) throw BasicException();
            haveDot = true;
        } else {
            if (!(money[i] >= '0' && money[i] <= '9')) throw BasicException();
        }
    }
    return stringToDouble(money);
}

bool Books::checkKeyword(std::string keyword) {
    //[keyword] 包含重复信息段则操作失败
    if (keyword.length() > 60) return false;
    int index = 0;
    std::set<std::string> words;
    while (keyword[index] != '\0') {
        int tmp = index;
        while (keyword[index] != '|') {
            if (keyword[index] == '"' || keyword[index] == ' ') return false;
            index++;
        }
        char word[index - tmp];
        for (int i = 0; i < index - tmp; ++i) {
            word[i] = keyword[tmp + i];
        }
        std::string s = std::string(word);
        if (words.count(s) != 0) return false;
        words.insert(s);
        index++;
    }
    return true;
}

