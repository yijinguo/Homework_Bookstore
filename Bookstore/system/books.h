#ifndef UNTITLED3_BOOKS_H
#define UNTITLED3_BOOKS_H

#include "diary.h"
#include <utility>


struct BooksInf {
    char ISBN[21] = "\0";
    char bookName[61] = "\0";
    char author[61] = "\0";
    char keyword[61] = "\0";//记录整段keyword
    int quantity = 0;//库存
    double price = 0;
    double totalCost = 0;//销售总金额

    friend std::ostream &operator<<(std::ostream &os, const BooksInf &b) {
        os << b.ISBN << '\t' << b.bookName << '\t' << b.author << '\t' << b.keyword << '\t' << std::fixed << std::setprecision(2) << b.price << '\t'
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
            while (a.keyword[index] != '|' && a.keyword[index] != ' ' && a.keyword[index] != '\0') {index++;}
            char word[index - tmp + 1];
            for (int i = 0; i < index - tmp; ++i) {
                word[i] = a.keyword[i + tmp];
            }
            word[index - tmp] = '\0';
            if (strcmp(word,b.keyword) == 0) return true;
            while (a.keyword[index] == ' ' || a.keyword[index] == '|') {index++;}
        }
        return false;
    }

};



class Books {
private:

    FileData<BooksInf> BookDataStore;
    DoubleIndex<BooksInf> BookNameStore;
    DoubleIndex<BooksInf> BookAuthorStore;
    //DoubleIndex<BooksInf> BookKeywordStore;

public:

    BooksInf bookSelect;

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
    bool checkKeyword(std::string cmd);
    double checkDouble(std::string money);
    //string-to-double
    static double stringToDouble(std::string demand) {
        int i = 0;
        double result = 0;
        int place = 1;
        while (demand[i] != '.' && demand[i] != '\0') {i++;}
        for (int j = 1; j <= i; ++j) {
            result += (demand[i - j] - '0') * place;
            place *= 10;
        }
        if (demand[i] == '.') {
            double p = 0.1;
            for (int j = i + 1; j < demand.length(); ++j) {
                result += (demand[j] - '0') * p;
                p *= 0.1;
            }
        }
        return result;
    }

};

#endif //UNTITLED3_BOOKS_H


//<<fixed<<setprecision(2)<<
