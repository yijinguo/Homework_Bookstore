#ifndef UNTITLED3_FILEDATA_H
#define UNTITLED3_FILEDATA_H
#define MAXSIZE 400


#include <fstream>
#include <cstring>
#include <algorithm>
#include <sstream>
#include "../error.h"
#include <vector>

//用这个文件保存地址，起到索引作用
template<class T>
class FileData {
private:

    struct Information {
        char index[30];
        int Loc;

        void Initialize(std::string &_index, int _Loc) {
            strcpy(index, _index.c_str());
            Loc = _Loc;
        }

        bool operator<(const Information &b) const {
            if (strcmp(index, b.index) < 0) return true;
            return false;
        }

        bool operator>(const Information &b) const {
            if (strcmp(index, b.index) > 0) return true;
            return false;
        }

        bool operator<=(const Information &b) const {
            if (strcmp(index, b.index) <= 0) return true;
            return true;
        }

        bool operator>=(const Information &b) const {
            if (strcmp(index, b.index) >= 0) return true;
            return false;
        }

        bool operator==(const Information &b) {
            if (strcmp(index, b.index) == 0) return true;
            return false;
        }

        bool operator!=(const Information &b) {
            if (strcmp(index, b.index) != 0) return true;
            return false;
        }
    };

    struct Node {
        int num;
        int next;
        Information array[MAXSIZE];

        void Initialized(std::string index, int loc) {
            num = 1;
            next = -1;
            Information initial;
            initial.Initialize(index, loc);
            array[0] = initial;
        }
    };

    int head = -1;
    int sizeNod = sizeof(Node);
    int sizeT = sizeof(T);
    std::fstream dataFile, file;


public:

    FileData() = default;

    void initialize(std::string dataFN,std::string FN){
        dataFile.open(dataFN,std::ostream::in);
        if (!dataFile) {
            dataFile.open(dataFN,std::ostream::out);
            dataFile.write(reinterpret_cast<char*>(&head), sizeof(int));
            file.open(FN,std::ostream::out);
            file.close();
        } else {
            dataFile.seekg(0);
            dataFile.read(reinterpret_cast<char*>(&head), sizeof(int));
        }
        dataFile.close();
        dataFile.open(dataFN);
        file.open(FN);
    }

    ~FileData() {
        dataFile.seekp(0);
        dataFile.write(reinterpret_cast<char *>(&head), sizeof(int));
        dataFile.close();
        file.close();
    }

    void addInfo(std::string index, T value) {
        file.seekp(0,std::ios::end);
        int loc = file.tellp() / sizeT;
        Information newInsert;
        newInsert.Initialize(index, loc);
        if (head == -1) {
            dataFile.seekp(0,std::ios::end);
            long long newHead = dataFile.tellp();
            head = (newHead - sizeof(int)) / sizeNod;
            Node headNod;
            headNod.Initialized(index, loc);
            dataFile.write(reinterpret_cast<char *>(&headNod), sizeNod);
            file.write(reinterpret_cast<char*>(&value), sizeT);
            return;
        }
        Node nodNow;
        int nodLoc;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (newInsert > nodNow.array[nodNow.num - 1] && nodNow.next != -1) continue;
            int next = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, newInsert) - nodNow.array;
            if (next < nodNow.num) {
                if (strcmp(nodNow.array[next].index, newInsert.index) == 0) throw BasicException();
                for (int i = nodNow.num; i >= next + 1; --i) {
                    nodNow.array[i] = nodNow.array[i - 1];
                }
            }
            nodNow.array[next] = newInsert;
            nodNow.num++;
            file.write(reinterpret_cast<char*>(&value), sizeT);
            if (nodNow.num != MAXSIZE) {
                dataFile.seekp(nodLoc * sizeNod + sizeof(int));
                dataFile.write(reinterpret_cast<char *>(&nodNow), sizeNod);
                return;
            }
            //分块
            splitBlock(nodLoc,nodNow);
            return;
        }
    }

    void splitBlock(int nodLoc,Node &nodNow){
        Node nodNew;
        nodNew.num = nodNow.num = MAXSIZE / 2;
        nodNew.next = nodNow.next;
        for (int i = MAXSIZE / 2; i < MAXSIZE; ++i) {
            nodNew.array[i - MAXSIZE / 2] = nodNow.array[i];
        }
        dataFile.seekp(0,std::ios::end);
        long long _newLoc = dataFile.tellp();
        int newLoc = (_newLoc - sizeof(int)) / sizeNod;
        dataFile.write(reinterpret_cast<char*>(&nodNew),sizeNod);
        nodNow.next = newLoc;
        dataFile.seekp(nodLoc * sizeNod + sizeof(int));
        dataFile.write(reinterpret_cast<char*>(&nodNow),sizeNod);
    }

    void deleteInfo(std::string index) {
        if (head == -1) throw BasicException();
        Information deleteInf;
        strcpy(deleteInf.index, index.c_str());
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (deleteInf < nodNow.array[0]) throw BasicException();
            if (deleteInf > nodNow.array[nodNow.num - 1]) continue;
            //可能在该节点内
            if (nodNow.num == 1) {
                //删节点;采用置空策略
                nodNow.num--;
                if (nodLoc == head && nodNow.next == -1) {
                    head = -1;
                    return;
                }
                if (nodLoc == head) {
                    head = nodNow.next;
                    return;
                }
                dataFile.seekp(nodLoc * sizeNod + sizeof(int));
                dataFile.write(reinterpret_cast<char *>(&nodNow), sizeNod);
                return;
            }
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, deleteInf) - nodNow.array;
            if (nodNow.array[n] != deleteInf) throw BasicException();
            for (int j = n; j < nodNow.num - 1; ++j) {
                nodNow.array[j] = nodNow.array[j + 1];
            }
            nodNow.num--;
            dataFile.seekp(nodLoc * sizeNod + sizeof(int));
            dataFile.write(reinterpret_cast<char *>(&nodNow), sizeNod);
            return;
        }
        throw BasicException();
    }

    //就地修改
    void modifyInfo(std::string index, T modify){
        if (head == -1) throw BasicException();
        Information modifyInf;
        strcpy(modifyInf.index, index.c_str());
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (modifyInf < nodNow.array[0]) throw BasicException();
            if (modifyInf > nodNow.array[nodNow.num - 1]) continue;
            //可能在该节点内
            if (nodNow.num == 1) {
                //删节点;采用置空策略
                file.seekp(nodNow.array[0].Loc * sizeT);
                file.write(reinterpret_cast<char*>(&modify),sizeT);
                return;
            }
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, modifyInf) - nodNow.array;
            if (nodNow.array[n] != modifyInf) throw BasicException();
            file.seekp(nodNow.array[n].Loc * sizeT);
            file.write(reinterpret_cast<char*>(&modify),sizeT);
            return;
        }
    }

    //元素的index被修改
    void modifyIndex(std::string oldIndex, std::string newIndex, T modify){
        deleteInfo(oldIndex);
        addInfo(newIndex,modify);
    }

    //根据索引返回对应值
    T readInfo(std::string index){
        T result;
        if (head == -1) throw BasicException();
        Information readInf;
        strcpy(readInf.index, index.c_str());
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (readInf < nodNow.array[0]) throw BasicException();
            if (readInf > nodNow.array[nodNow.num - 1]) continue;
            //可能在该节点内
            if (nodNow.num == 1) {
                file.seekg(nodNow.array[0].Loc * sizeT);
                file.read(reinterpret_cast<char*>(&result),sizeT);
                return result;
            }
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, readInf) - nodNow.array;
            if (nodNow.array[n] != readInf) throw BasicException();
            file.seekg(nodNow.array[n].Loc * sizeT);
            file.read(reinterpret_cast<char*>(&result),sizeT);
            return result;
        }
        throw BasicException();
    }

    //尝试寻找索引对应值，找不到抛出异常CreateException
    T findInfo(std::string index){
        T result;
        if (head == -1) throw CreateException();
        Information readInf;
        strcpy(readInf.index, index.c_str());
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (readInf < nodNow.array[0]) throw CreateException();
            if (readInf > nodNow.array[nodNow.num - 1]) continue;
            //可能在该节点内
            if (nodNow.num == 1) {
                file.seekg(nodNow.array[0].Loc * sizeT);
                file.read(reinterpret_cast<char*>(&result),sizeT);
                return result;
            }
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, readInf) - nodNow.array;
            if (nodNow.array[n] != readInf) throw CreateException();
            file.seekg(nodNow.array[n].Loc * sizeT);
            file.read(reinterpret_cast<char*>(&result),sizeT);
            return result;
        }
        throw CreateException();
    }

    void printIndex(std::string index) {
        T result;
        if (head == -1) throw BasicException();
        Information readInf;
        strcpy(readInf.index, index.c_str());
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (readInf < nodNow.array[0]) throw BasicException();
            if (readInf > nodNow.array[nodNow.num - 1]) continue;
            //可能在该节点内
            if (nodNow.num == 1) {
                file.seekg(nodNow.array[0].Loc * sizeT);
                file.read(reinterpret_cast<char*>(&result),sizeT);
                std::cout << result;
                return;
            }
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, readInf) - nodNow.array;
            if (nodNow.array[n] != readInf) throw BasicException();
            file.seekg(nodNow.array[n].Loc * sizeT);
            file.read(reinterpret_cast<char*>(&result),sizeT);
            std::cout << result;
            return;
        }
        throw BasicException();
    }

    //索引字典序输出所有图书
    void printAllInfo(){
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            for (int i = 0; i < nodNow.num; ++i) {
                T value;
                file.seekg(nodNow.array[i].Loc * sizeT);
                file.read(reinterpret_cast<char*>(&value),sizeT);
                std::cout << value;
            }
        }
    }
    //输出符合要求的图书
    void printDemand(T demand){
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        bool have = false;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            for (int i = 0; i < nodNow.num; ++i) {
                T value;
                file.seekg(nodNow.array[i].Loc * sizeT);
                file.read(reinterpret_cast<char*>(&value),sizeT);
                if (value - demand) {
                    have = true;
                    std::cout << value;
                }
            }
        }
        if (!have) std::cout << '\n';
    }
};

template<class T>
class DoubleIndex{
private:
    struct Information {
        char index[31];
        char value[31];

        void Initialize(std::string &_index, std::string &_value) {
            strcpy(index, _index.c_str());
            strcpy(value,_value.c_str());
        }

        bool operator<(const Information &b) const {
            if (strcmp(index, b.index) < 0) return true;
            if (strcmp(index, b.index) > 0) return false;
            if (strcmp(value,b.value) < 0) return true;
            return false;
        }

        bool operator>(const Information &b) const {
            if (strcmp(index, b.index) > 0) return true;
            if (strcmp(index, b.index) < 0) return false;
            if (strcmp(value,b.value) > 0) return true;
            return false;
        }

        bool operator<=(const Information &b) const {
            if (strcmp(index, b.index) < 0) return true;
            if (strcmp(index, b.index) > 0) return false;
            if (strcmp(value,b.value) <= 0) return true;
            return false;
        }

        bool operator>=(const Information &b) const {
            if (strcmp(index, b.index) > 0) return true;
            if (strcmp(index, b.index) < 0) return false;
            if (strcmp(value,b.value) >= 0) return true;
            return false;
        }

        bool operator==(const Information &b) {
            if (strcmp(index, b.index) != 0) return false;
            if (strcmp(value,b.value) != 0) return false;
            return true;
        }

        bool operator!=(const Information &b) {
            if (strcmp(index, b.index) != 0) return true;
            if (strcpy(value,b.value) != 0) return true;
            return false;
        }
    };

    struct Node {
        int num;
        int next;
        Information array[MAXSIZE];

        void Initialized(std::string index, std::string value) {
            num = 1;
            next = -1;
            Information initial;
            initial.Initialize(index,value);
            array[0] = initial;
        }
    };

    int head = -1;
    int sizeNod = sizeof(Node);
    std::fstream dataFile;

public:

    DoubleIndex() = default;

    void initialize(std::string fileName){
        dataFile.open(fileName,std::ostream::in);
        if (!dataFile) {
            dataFile.open(fileName,std::ostream::out);
            dataFile.write(reinterpret_cast<char*>(&head), sizeof(int));
        } else {
            dataFile.seekg(0);
            dataFile.read(reinterpret_cast<char*>(&head), sizeof(int));
        }
        dataFile.close();
        dataFile.open(fileName);
    }

    ~DoubleIndex() {
        dataFile.seekp(0);
        dataFile.write(reinterpret_cast<char *>(&head), sizeof(int));
        dataFile.close();
    }

    void addInfo(std::string index, std::string value) {
        Information newInsert;
        newInsert.Initialize(index, value);
        if (head == -1) {
            dataFile.seekp(0,std::ios::end);
            long long newHead = dataFile.tellp();
            head = (newHead - sizeof(int)) / sizeNod;
            Node headNod;
            headNod.Initialized(index, value);
            dataFile.write(reinterpret_cast<char *>(&headNod), sizeNod);
            return;
        }
        Node nodNow;
        int nodLoc;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (newInsert > nodNow.array[nodNow.num - 1] && nodNow.next != -1) continue;
            //nodNow.array[nodNow.num] = newInsert;
            int next = std::upper_bound(nodNow.array, nodNow.array + nodNow.num, newInsert) - nodNow.array;
            for (int i = nodNow.num; i >= next + 1; --i) {
                nodNow.array[i] = nodNow.array[i - 1];
            }
            nodNow.array[next] = newInsert;
            nodNow.num++;
            if (nodNow.num != MAXSIZE) {
                dataFile.seekp(nodLoc * sizeNod + sizeof(int));
                dataFile.write(reinterpret_cast<char *>(&nodNow), sizeNod);
                return;
            }
            //分块
            splitBlock(nodLoc,nodNow);
            return;
        }
    }

    void splitBlock(int nodLoc,Node &nodNow){
        Node nodNew;
        nodNew.num = nodNow.num = MAXSIZE / 2;
        nodNew.next = nodNow.next;
        for (int i = MAXSIZE / 2; i < MAXSIZE; ++i) {
            nodNew.array[i - MAXSIZE / 2] = nodNow.array[i];
        }
        dataFile.seekp(0,std::ios::end);
        long long _newLoc = dataFile.tellp();
        int newLoc = (_newLoc - sizeof(int)) / sizeNod;
        dataFile.write(reinterpret_cast<char*>(&nodNew),sizeNod);
        nodNow.next = newLoc;
        dataFile.seekp(nodLoc * sizeNod + sizeof(int));
        dataFile.write(reinterpret_cast<char*>(&nodNow),sizeNod);
    }

    void deleteInfo(std::string index,std::string value) {
        if (head == -1) throw BasicException();
        Information deleteInf;
        deleteInf.Initialize(index, value);
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (deleteInf < nodNow.array[0]) throw BasicException();
            if (deleteInf > nodNow.array[nodNow.num - 1]) continue;
            //可能在该节点内
            if (nodNow.num == 1) {
                //删节点;采用置空策略
                if (nodLoc == head && nodNow.next == -1) {
                    head = -1;
                    return;
                }
                if (nodLoc == head) {
                    head = nodNow.next;
                    return;
                }
                nodNow.num--;
                dataFile.seekp(nodLoc * sizeNod + sizeof(int));
                dataFile.write(reinterpret_cast<char *>(&nodNow), sizeNod);
                return;
            }
            int n = std::lower_bound(nodNow.array,nodNow.array + nodNow.num,deleteInf) - nodNow.array;
            if (nodNow.array[n] == deleteInf) {
                for (int j = n; j < nodNow.num - 1; ++j) {
                    nodNow.array[j] = nodNow.array[j + 1];
                }
                nodNow.num--;
                dataFile.seekp(nodLoc * sizeNod + sizeof(int));
                dataFile.write(reinterpret_cast<char *>(&nodNow), sizeNod);
                return;
            } else {
                throw BasicException();
            }
        }
    }

    //元素的index被修改
    void modifyIndex(std::string oldIndex, std::string newIndex, std::string oldValue,std::string newValue){
        deleteInfo(oldIndex, oldValue);
        addInfo(newIndex,newValue);
    }

    //打印某索引
    void printIndex(FileData<T> &system, std::string &index){
        if (head == -1) throw BasicException();
        std::vector<std::string> allFind;
        bool haveFind = false;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (strcmp(nodNow.array[0].index,index.c_str()) > 0) break;
            if (strcmp(nodNow.array[nodNow.num - 1].index,index.c_str()) < 0) continue;
            //可能在本节点内
            bool findEnd = false;
            for (int i = 0; i < nodNow.num; ++i) {
                if (strcmp(nodNow.array[i].index,index.c_str()) > 0) {
                    findEnd = true;
                    break;
                }
                if (strcmp(nodNow.array[i].index,index.c_str()) < 0) continue;
                haveFind = true;
                allFind.emplace_back(nodNow.array[i].value);
            }
            if (findEnd) break;
        }
        if (!haveFind) throw BasicException();
        auto it = allFind.begin();
        while (it != allFind.end()) {
            system.printIndex(*it);
            it++;
        }
    }

};




//用文件实现一个登录栈
class Stack {
private:

    std::vector<std::string> vecStack;


public:

    Stack();

    ~Stack();

    //将一个元素放入栈
    void push(std::string content);

    //将一个元素拿出栈,返回当前栈内最末元素
    std::string pop();

    //判断是否为空栈
    bool judgeNull() const;

    //判断某索引是否在栈内
    bool InStack(std::string index);

};


//这是日志系统的操作全记录
class DiaryRecord{
private:
    std::fstream diaryRecord;//操作全记录
    std::fstream staffRecord;//员工文件
public:
    DiaryRecord();
    ~DiaryRecord();
    void write(int &priority,std::string &name, std::string &content);
    //生成员工操作
    void returnIndex(std::string index);
    //清除生成员工操作的文件
    void clear();
};


//这是日志系统的交易总额记录
class FinanceRecord{
private:
    struct Information{
        int time;
        char income[13];
        char expense[13];
    };

    int sizeInf = sizeof(Information);
    std::fstream file;

public:

    int Time;//目前交易总笔数
    char inAll[13];//目前总收入
    char outAll[13];//目前总支出

    FinanceRecord();

    ~FinanceRecord();
    //写入记录
    void addRecord(std::string income, std::string expense);
    //返回int处记录,直接输出;
    //注意抛出异常
    void printTime(int time);
    //返回全记录
    void printAll();

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


//这是日志系统的交易全记录
class TradeRecord{
private:
    std::fstream tradeRecord;
public:
    TradeRecord();
    ~TradeRecord();
    void buyBook(std::string _user_id, std::string isbn,std::string _book_name,int _quantity,std::string cost);
    void importBook(std::string _user_id,std::string isbn,std::string _book_name,int _quantity,std::string cost);
    //在目前的文件基础上加上一行交易总额记录
    void writeTotal(FinanceRecord &a);
};




#endif //UNTITLED3_FILEDATA_H
