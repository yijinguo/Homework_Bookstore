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
    char price[14] = "0.00";
    char totalCost[14] = "0.00";//销售总金额

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
    BooksInf bookSelect;
    FileData<BooksInf> BookDataStore;
    DoubleIndex<BooksInf> BookNameStore;
    DoubleIndex<BooksInf> BookAuthorStore;
    //DoubleIndex<BooksInf> BookKeywordStore;

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
    bool checkKeyword(std::string cmd);
    std::string checkDouble(std::string money);
    //string-to-double;小数点后仅两位
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
            i++;
            int num[3];
            int j = 0;
            for (; j < 3; ++j) {
                if (demand[i + j] == '\0') break;
                num[j] = demand[i + j] - '0';
            }
            if (j == 1) {
                result += num[0] * 0.1;
            } else if (j == 2) {
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
        std::string str;
        str = os.str();
        int dot = -1;
        for (int i = 0; i < str.length(); ++i) {
            if (str[i] == '.') {
                dot = i;
                break;
            }
        }
        if (dot == -1) {
            char result[str.length()];
            for (int i = 0; i < str.length(); ++i) {
                result[i] = str[i];
            }
            result[str.length()] = '.';
            result[str.length() + 1] = '0';
            result[str.length() + 2] = '0';
            result[str.length() + 3] = '\0';
            std::string s = std::string(result);
            return s;
        }
        if (dot == str.length() - 2) {
            char result[str.length() + 2];
            for (int i = 0; i < str.length(); ++i) {result[i] = str[i];}
            result[str.length()] = '0';
            result[str.length() + 1] = '\0';
            std::string s = std::string(result);
            return s;
        }
        if (dot == str.length() - 3) {return str;}
        if (str[dot + 3] >= 5) {
            demand += 0.01;
            std::ostringstream _os;
            _os << demand;
            std::string _str;
            _str = _os.str();
            char result[dot + 5];
            int i = 0;
            while (_str[i] != '.') {
                result[i] = _str[i];
                i++;
            }
            result[i] = '.';
            result[i + 1] = _str[i + 1];
            result[i + 2] = _str[i + 2];
            result[i + 3] = '\0';
            std::string s = std::string(result);
            return s;
        } else {
            char result[dot + 4];
            for (int i = 0; i <= dot + 2; ++i) {result[i] = str[i];}
            result[dot + 3] = '\0';
            std::string s = std::string (result);
            return s;
        }
    }

};

#endif //UNTITLED3_BOOKS_H
