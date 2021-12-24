#ifndef UNTITLED3_FILEDATA_H
#define UNTITLED3_FILEDATA_H

#define MAXSIZE 400
#define MINSIZE 200

#include <fstream>
#include <cstring>
#include <algorithm>
#include "../error.h"


//todo 输出<<的重载；
//todo 强制类型转换不成立(?)
//todo modify需要有两个变量

/*template<class T>//文件数据结构类接口 T可为Books SumIncome Account OperaRecord中任意一个
class FileDataString {

private:

    struct Information {
        char index[30];
        T value;
        void Initialize(std::string _index, T _value){
            strcpy(index, _index.c_str());
            value = _value;
        }
        bool operator<(const Information &b) const {
            if (strcmp(index,b.index) < 0) return true;
            return false;
        }
        bool operator<=(const Information &b) const {
            if (strcmp(index,b.index) <= 0) return true;
            return false;
        }
        bool operator>(const Information &b) const {
            if (strcmp(index, b.index) > 0) return true;
            return false;
        }
        bool operator>=(const Information &b) const {
            if (strcmp(index,b.index) >= 0) return true;
            return false;
        }
        bool operator==(const Information &b) const {
            if (strcmp(index, b.index) == 0) return true;
            return false;
        }
        bool operator!=(const Information &b) const {
            if (strcmp(index, b.index) != 0) return true;
            return false;
        }
    };

    struct Node {
        int num;
        int next;
        Information array[MAXSIZE];

        void Initialize(std::string index, T value){
            num = 1;
            next = -1;
            Information initial;
            initial.Initialize(index, value);
            array[0] = initial;
        }

    };
    int head = -1;//用于记录头节点在文件中的位置
    std::fstream file;
    std::string fileName;
    long sizeofNod = sizeof(Node);

public:

    FileDataString() = default;

    ~FileDataString(){
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&head), sizeof(int));
        file.close();
    }

    //初始化该文件:创建文件
    void initialise(std::string FN){
        fileName = FN;
        file.open(fileName,std::fstream::in);
        if (!file) {
            file.open(fileName,std::fstream::out);
            file.write(reinterpret_cast<char*>(&head), sizeof(int));
        } else {
            file.seekg(0);
            file.read(reinterpret_cast<char*>(&head), sizeof(int));
        }
        file.close();
        file.open(fileName);
    }

    //添加某数据
    void addInfo(std::string index, T info){
        Information newInsert;
        newInsert.Initialize(index,info);
        if (head == -1) {
            file.seekp(0,std::ios::end);
            long long newHead = file.tellp();
            head = (newHead - sizeof(int)) / sizeofNod;
            Node add;
            add.Initialize(index,info);
            file.write(reinterpret_cast<char*>(&add),sizeofNod);
            return;
        }
        Node nodNow;
        int nodLoc;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            file.seekg(nodNow.next * sizeofNod + sizeof(int));
            file.read(reinterpret_cast<char*>(&nodNow),sizeofNod);
            if (nodNow.num == 0) continue;
            if (newInsert > nodNow.array[nodNow.num - 1] && nodNow.next != -1) continue;
            int next = std::upper_bound(nodNow.array, nodNow.array + nodNow.num,info) - nodNow.array;
            if (newInsert == nodNow.array[next]) throw BasicException();
            for (int i = nodNow.num; i > next; --i) {
                nodNow.array[i] = nodNow.array[i - 1];
            }
            nodNow.array[next] = newInsert;
            nodNow.num++;
            if (nodNow.num != MAXSIZE) {
                file.seekp(nodLoc * sizeofNod + sizeof(int));
                file.write(reinterpret_cast<char*>(&nodNow),sizeofNod);
                return;
            }
            Split(nodLoc,nodNow);
            return;
        }
    }
    //分块
    void Split(int nodLoc,Node &nodNow){
        Node nodNew;
        nodNew.num = nodNow.num = MAXSIZE / 2;
        nodNew.next = nodNow.next;
        for (int i = MAXSIZE / 2; i < MAXSIZE; ++i) {
            nodNew.array[i - MAXSIZE / 2] = nodNow.array[i];
        }
        file.seekp(0,std::ios::end);
        long long _newLoc = file.tellp();
        int newLoc = (_newLoc - sizeof(int)) / sizeofNod;
        file.write(reinterpret_cast<char*>(nodNew),sizeofNod);
        nodNow.next = newLoc;
        file.seekp(nodLoc * sizeofNod + sizeof(int));
        file.write(reinterpret_cast<char*>(&nodNow),sizeofNod);
    }

    //删除某数据(只关心索引string)
    void deleteInfo(std::string index){
        if (head == -1) throw BasicException();
        Information deleteInf;
        deleteInf.index = index;
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            file.seekg(nodNow.next * sizeofNod + sizeof(int));
            file.read(reinterpret_cast<char*>(&nodNow),sizeofNod);
            if (nodNow.num == 0) continue;
            if (deleteInf < nodNow.array[0]) throw BasicException();
            if (deleteInf > nodNow.array[nodNow.num - 1]) continue;
            //可能在该块内
            if (nodNow.num == 1) {
                if (nodLoc == head && nodNow.next == -1) {
                    head = -1;
                    return;
                }
                if (nodLoc == head) {
                    head = nodNow.next;
                    return;
                }
                nodNow.num--;
                file.seekp(nodLoc * sizeofNod + sizeof(int));
                file.write(reinterpret_cast<char*>(&nodNow),sizeofNod);
                return;
            }
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, deleteInf) - nodNow.array;
            if (n >= nodNow.num || nodNow.array[n] != deleteInf) throw BasicException();
            for (int i = n; i < nodNow.num - 1; ++i) {
                nodNow.array[i] = nodNow.array[i + 1];
            }
            nodNow.num--;
            file.seekp(nodLoc * sizeofNod + sizeof(int));
            file.write(reinterpret_cast<char*>(&nodNow),sizeofNod);
            return;
        }
    }

    //返回某位置的数据
    T readInfo(std::string index) {
        if (head == -1) throw BasicException();
        Information findInf;
        findInf.index = index;
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            file.seekg(nodNow.next * sizeofNod + sizeof(int));
            file.read(reinterpret_cast<char*>(&nodNow),sizeofNod);
            if (nodNow.num == 0) continue;
            if (findInf < nodNow.array[0]) throw BasicException();
            if (findInf > nodNow.array[nodNow.num - 1]) continue;
            //可能在该块内
            if (nodNow.num == 1) {
                return nodNow.array[0];
            }
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, findInf) - nodNow.array;
            if (n >= nodNow.num || nodNow.array[n] != findInf) throw BasicException();
            return nodNow.array[n];
        }
    }

    //修改某数据
    void modifyInfo(std::string oldIndex, std::string newIndex, T &info) {//原index,新index,新数据info
        try {
            if (oldIndex != newIndex) {
                deleteInfo(oldIndex);
                addInfo(newIndex, info);
            } else {
                if (head == -1) throw BasicException();
                Information findInf;
                findInf.index = oldIndex;
                int nodLoc;
                Node nodNow;
                nodNow.next = head;
                while (nodNow.next != -1) {
                    nodLoc = nodNow.next;
                    file.seekg(nodNow.next * sizeofNod + sizeof(int));
                    file.read(reinterpret_cast<char *>(&nodNow), sizeofNod);
                    if (nodNow.num == 0) continue;
                    if (findInf < nodNow.array[0]) throw BasicException();
                    if (findInf > nodNow.array[nodNow.num - 1]) continue;
                    //可能在该块内
                    if (nodNow.num == 1) {
                        nodNow.array[0] = info;
                    } else {
                        int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, findInf) - nodNow.array;
                        if (n >= nodNow.num || nodNow.array[n] != findInf) throw BasicException();
                        nodNow.array[n] = info;
                    }
                    file.seekp(nodLoc * sizeofNod + sizeof(int));
                    file.write(reinterpret_cast<char *>(&nodNow), sizeofNod);
                }
            }
        } catch (BasicException &ex) {
            throw BasicException();
        }
    }

    //返回某数据的上一位数据
    //T frontInfo(const T info);
    //查找某索引的数据是否存在
    bool findInfo(const T info);
    //打印数据全体
    void printAllInfo();
    //打印符合demandInfo要求的内容
    void printDemand(T demandInfo);
    //获得该数据的位置（依靠index文件类）
    //int getIndex(const T info);
    //合并块
    //void Merge();
    //删除某个块
    //void deleteBlock();
};*/

//用这个文件保存地址，起到索引作用
template<class T>
class FileData {
private:

    struct Information {
        char index[30];
        int Loc;

        void Initialize(std::string _index, int _Loc) {
            strcpy(index, _index.c_str());
            Loc = _Loc;
        }

        bool operator<(const Information &b) const {
            if (strcmp(index, b.index) < 0) return true;
            return true;
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

        void Initialized(std::string index, T value) {
            num = 1;
            next = -1;
            Information initial;
            initial.Initialize(index, value);
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
            int next = std::upper_bound(nodNow.array, nodNow.array + nodNow.num, newInsert) - nodNow.array;
            if (nodNow.array[next].index == newInsert.index) throw BasicException();
            for (int i = nodNow.num; i >= next + 1; --i) {
                nodNow.array[i] = nodNow.array[i - 1];
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
        deleteInf.index = index;
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
    }

    void modifyInfo(std::string index, T modify);

    void modifyIndex(std::string oldIndex, std::string newIndex, T modify);
    //根据索引返回对应值
    T readInfo(std::string index);
    //尝试寻找索引对应值，找不到抛出异常CreatException
    T findInfo(std::string index);
    //索引字典序输出所有图书
    void printAllInfo();
    //输出符合要求的图书
    void printDemand(T demand);
};

//用文件实现一个登录栈
class Stack {
private:

    struct Information {
        int front = -1;
        char index[30] = "0";
    };

    long nowLoc = -1;
    long lastLoc = -1;
    std::fstream stackLog;
    long sizeT = sizeof(Information);

public:

    Stack(){
        stackLog.open("fileStackLog");
    }

    ~Stack() {
        stackLog.close();
    }

    //将一个元素放入栈
    void push(std::string content){
        Information newPush;
        newPush.front = nowLoc;
        strcpy(newPush.index,content.c_str());
        lastLoc = nowLoc;
        stackLog.seekp(0,std::ios::end);
        nowLoc = stackLog.tellp() / sizeT;
        stackLog.write(reinterpret_cast<char*>(&newPush),sizeT);
    }

    //将一个元素拿出栈,返回当前栈内最末元素
    std::string pop(){
        Information last;
        nowLoc = lastLoc;
        stackLog.seekg(lastLoc * sizeT);
        stackLog.read(reinterpret_cast<char*>(&last),sizeT);
        lastLoc = last.front;
        return std::string(last.index);
    }

    //判断是否为空栈
    bool judgeNull() const {
        if (nowLoc == -1) return true;
        return false;
    }

    //判断某索引是否在栈内
    bool InStack(std::string index) {
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
};

//这是日志系统的操作全记录
class DiaryRecord{
private:
    std::fstream diaryRecord;
public:
    DiaryRecord();
    ~DiaryRecord();
    void write(int &priority,std::string &name, std::string &content);

};

//这是日志系统的交易记录
template<class T>
class FinanceRecord{
private:
    struct Information{
        int time;
        int Loc;
    };
    struct Node{
        int num;
        int next;
        Information array[MINSIZE];
    };

    //考虑到这个文件只需要在末尾添加记录，所以需记录尾节点的位置
    int head = -1;
    int rear = -1;
    int sizeNod = sizeof(Node);
    int sizeT = sizeof(T);
    std::fstream dataFile, file;

public:

    FinanceRecord();

    ~FinanceRecord();
    //写入记录
    void addRecord(int time,std::string income, std::string expense);
    //返回int处记录,直接输出
    void printTime(int time);

};

#endif //UNTITLED3_FILEDATA_H
