
#include <set>
#include "books.h"


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
            defineShowDemand(demandInfo, _word, _demand);//以下将要求的信息放入demandInfo
            //现在得到了demandInfo，遍历文件输出所有符合的内容
            BookDataStore.printDemand(demandInfo);
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
        std::string buyCost = doubleToString(_quantity * stringToDouble(modify.price));
        std::cout << buyCost << '\n';
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
        strcpy(bookSelect.ISBN,isbn.c_str());
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
        while (cmd[index] != '\0') {
            while (cmd[index] == ' ') { index++; }
            if (cmd[index] != '-') throw BasicException();
            int tmp = index;
            while (cmd[index] != '=' && cmd[index] != '\0') {index++;}
            if (cmd[index] == '\0') throw BasicException();
            char word[index - tmp + 1];
            for (int i = 0; i < index - tmp; ++i) {
                word[i] = cmd[tmp + i];//word:-ISBN等
            }
            word[index - tmp] = '\0';
            index++;
            if (cmd[index] == '\0') throw BasicException();
            tmp = index;
            while (cmd[index] != '\0' && cmd[index] != ' ') {index++;}
            char demand[index - tmp + 1];
            for (int i = 0; i < index - tmp; ++i) {
                demand[i] = cmd[tmp + i];
            }
            demand[index - tmp] = '\0';
            std::string _word = std::string(word);
            std::string _demand = std::string(demand);
            //以上处理指令，word表示要求的信息类型，demand表示具体信息
            defineDemand(bookSelect, _word, _demand);//以下将要求的信息放入demandInfo
            if (cmd[index] == '\0') break;
        }
        //现在得到了demandInfo，进行修改
        if (OldIndex.empty()) {
            BookDataStore.addInfo(bookSelect.ISBN,bookSelect);
        } else {
            if (strcmp(bookSelect.ISBN, OldIndex.c_str()) != 0) {
                BookDataStore.modifyIndex(OldIndex, bookSelect.ISBN, bookSelect);
            } else {
                BookDataStore.modifyInfo(OldIndex, bookSelect);
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
        std::string total_cost = checkDouble(_total_cost);
        double _totalCost = stringToDouble(total_cost);
        double oldTotal = stringToDouble(bookSelect.totalCost);
        strcpy(bookSelect.totalCost, doubleToString(oldTotal + _totalCost).c_str());
        BookDataStore.modifyInfo(bookSelect.ISBN,bookSelect);
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
        if (demand[0] != '"' || demand[demand.length() - 1] != '"') throw BasicException();
        for (int i = 0; i < demand.length() - 2; ++i) {
            demand[i] = demand[i + 1];
        }
        demand[demand.length() - 2] = '\0';
        strcpy(demandInfo.bookName,demand.c_str());
    } else if (word == "-author") {
        if (demand[0] != '"' || demand[demand.length() - 1] != '"') throw BasicException();
        for (int i = 0; i < demand.length() - 2; ++i) {
            demand[i] = demand[i + 1];
        }
        demand[demand.length() - 2] = '\0';
        strcpy(demandInfo.author,demand.c_str());
    } else if (word == "-keyword") {
        if (demand[0] != '"' || demand[demand.length() - 1] != '"') throw BasicException();
        for (int i = 0; i < demand.length() - 2; ++i) {
            demand[i] = demand[i + 1];
        }
        demand[demand.length() - 2] = '\0';
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
            std::string s = checkDouble(demand);
            strcpy(demandInfo.price, s.c_str());
        } else {
            throw BasicException();
        }
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

//检查一个字符串是否是浮点数，并修改为小数点后两位
std::string Books::checkDouble(std::string money){
    int dot = -1;
    for (int i = 0; i < money.length(); ++i){
        if (money[i] == '.') {
            if (dot != -1) throw BasicException();
            dot = i;
            continue;
        }
        if (!(money[i] >= '0' && money[i] <= '9')) throw BasicException();
    }
    if (dot == -1) {
        char result[money.length() + 4];
        for (int i = 0; i < money.length(); ++i) {result[i] = money[i];}
        result[money.length()] = '.';
        result[money.length() + 1] = '0';
        result[money.length() + 2] = '0';
        result[money.length() + 3] = '\0';
        std::string s = std::string(result);
        return s;
    } else {
        if (dot == money.length() - 1) throw BasicException();
        if (dot == money.length() - 2) {
            char result[money.length() + 2];
            for (int j = 0; j < money.length(); ++j) {result[j] = money[j];}
            result[money.length()] = '0';
            result[money.length() + 1] = '\0';
            std::string s = std::string(result);
            return s;
        }
        if (dot == money.length() - 3) {return money;}
        //小数点后位数超过2位，四舍五入
        //todo
        //暂时采用舍去策略
        char result[dot + 4];
        for (int j = 0; j <= dot + 2; ++j) {
            result[j] = money[j];
        }
        result[dot + 3] = '\0';
        std::string s = std::string(result);
        return s;
    }

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

