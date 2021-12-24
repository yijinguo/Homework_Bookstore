#ifndef UNTITLED3_BOOKS_H
#define UNTITLED3_BOOKS_H

#include "diary.h"
#include <utility>
#include <sstream>

struct BooksInf {
    char ISBN[20] = "\0";
    char bookName[60] = "\0";
    char author[60] = "\0";
    char keyword[60] = "\0";//记录整段keyword
    int quantity = 0;//库存
    char price[13] = "0.00";
    char totalCost[13] = "0.00";//销售总金额

    friend std::ostream &operator<<(std::ostream &os, const BooksInf &b) {
        os << b.ISBN << '\t' << b.bookName << '\t' << b.author << '\t' << b.keyword << '\t' << b.price << '\t'
           << b.quantity << '\n';
        return os;
    }

    //专门为show而写
    friend bool operator-(BooksInf &a,BooksInf &b) {
        if (b.ISBN[0] != '\0' && strcmp(a.ISBN,b.ISBN) != 0) {return false;}
        if (b.bookName[0] != '\0' && strcmp(a.bookName, b.bookName) != 0) {return false;}
        if (b.author[0] != '\0' && strcmp(a.author,b.author) != 0) {return false;}
        if (b.keyword[0] == '\0') return true;
        int index = 0;
        while (a.keyword[index] != '\0') {
            int tmp = index;
            while (a.keyword[index] != '|' && a.keyword[index] != ' ') {index++;}
            char word[index - tmp];
            for (int i = 0; i < index - tmp; ++i) {
                word[i] = a.keyword[i + tmp];
            }
            if (strcmp(word,b.keyword) == 0) return true;
            while (a.keyword[index] == ' ' || a.keyword[index] == '|') {index++;}
        }
        return false;
    }

};

class Books {
private:
    BooksInf bookSelect;
    FileData<BooksInf> BookDataStore;
    //FileData<BooksInf> BooksName;
    //FileData<BooksInf> BooksAuthor;
public:
    Books();

    //以ISBN字典升序依次输出满足要求的图书信息{1}
    void show(std::string cmd);

    //购买指定数量的图书{1}
    void buy(const std::string &isbn, const int _quantity,Diary &diarySystem);

    //以当前账户图书选中{3}
    void select(const std::string isbn);

    //修改图书信息{3}
    void modify(std::string cmd);

    //图书进货
    void import(int _quantity, const std::string _total_cost, Diary &diarySystem);

    //以下为自补函数
    //用于处理show和modify的多条信息
    void defineShowDemand(BooksInf &demandInfo, std::string word, std::string demand);
    void defineDemand(BooksInf &demandInfo,std::string word, std::string demand);
    bool checkDouble(std::string money);
    //string-to-double;小数点后仅两位
    static double stringToDouble(std::string demand) {
        int i = 0;
        double result = 0;
        while (demand[i] != '.' && demand[i] != '\0') {i++;}
        for (int j = i; j >= 1; --j) {
            result += (demand[i - j] - '0') * (i - j) *10;
        }
        if (demand[i] == '.') {
            i++;
            int num[3];
            int j = 0;
            for (; j < 3; ++j) {
                if (demand[i + j] == '\0') break;
                num[j] = demand[i + j] - '0';
            }
            if (j == 0) {
                result += num[0] * 0.1;
            } else if (j == 1) {
                result += num[0] * 0.1 + num[1] * 0.01;
            } else {
                result += num[0] * 0.1 + num[1] * 0.01;
                if (num[2] >= 5) {result += 0.01;}
            }
        }
        return result;
    }
    //double-to-string
    static std::string doubleToString(double demand){
        std::ostringstream os;
        os << demand;
        std::string result;
        result = os.str();
        return result;
    }
};

#endif //UNTITLED3_BOOKS_H
