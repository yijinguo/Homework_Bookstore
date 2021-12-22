#ifndef UNTITLED3_FILEDATA_H
#define UNTITLED3_FILEDATA_H

#define MAXSIZE 400

#include <fstream>
#include <cstring>
#include <algorithm>
#include "error.h"

template<class T>//文件数据结构类接口 T可为Books SumIncome Account OperaRecord中任意一个
class FileData {

private:

    struct Node {
        int num;
        int next;
        T array[MAXSIZE];
    };
    int head = -1;//用于记录头节点在文件中的位置
    std::fstream file;
    std::string fileName;
    int sizeofNod = sizeof(Node);

public:

    FileData() = default;
    FileData(const std::string _fileName){
        fileName = _fileName;
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
    };

    ~FileData(){
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
    void addInfo(T &info){
        if (head == -1) {
            Node add;
            add.num = 1;
            add.next = -1;
            add.array[0] = info;
            head = sizeof(int);
            file.seekp(sizeof(int));
            file.write(reinterpret_cast<char*>(&add),sizeofNod);
            return;
        }
        Node nodNow;
        int nodLoc;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            file.seekg(nodNow.next * sizeofNod + sizeof(int));
            file.read(reinterpret_cast<char*>(&nodNow),sizeofNod);
            if (nodNow.num == 0) continue;
            if (info > nodNow.array[nodNow.num - 1] && nodNow.next != -1) continue;
            int next = std::upper_bound(nodNow.array, nodNow.array + nodNow.num,info) - nodNow.array;
            if (info == nodNow.array[next]) throw BasicException();
            for (int i = nodNow.num; i > next; --i) {
                nodNow.array[i] = nodNow.array[i - 1];
            }
            nodNow.array[next] = info;
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
        file.seekp(nodLoc);
        file.write(reinterpret_cast<char*>(&nodNow),sizeofNod);
    }

    //删除某数据(只关心索引string)
    void deleteInfo(T &info){
        if (head == -1) throw BasicException();
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            file.seekg(nodNow.next * sizeofNod + sizeof(int));
            file.read(reinterpret_cast<char*>(&nodNow),sizeofNod);
            if (nodNow.num == 0) continue;
            if (info < nodNow.array[0]) throw BasicException();
            if (info > nodNow.array[nodNow.num - 1]) continue;
            //可能在该块内
            if (nodNow == 1) {
                if (nodLoc == head && nodNow.next == -1) {
                    head = -1;
                    return;
                }
                nodNow.num--;
                file.seekp(nodLoc * sizeofNod + sizeof(int));
                file.write(reinterpret_cast<char*>(&nodNow),sizeofNod);
                return;
            }
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, info) - nodNow.array;
            if (n >= nodNow.num || nodNow.array[n] != info) throw BasicException();
            for (int i = n; i < nodNow.num - 1; ++i) {
                nodNow.array[i] = nodNow.array[i + 1];
            }
            nodNow.num--;
            file.seekp(nodLoc * sizeofNod + sizeof(int));
            file.write(reinterpret_cast<char*>(&nodNow),sizeofNod);
            return;
        }
    }

    //修改某数据
    void modifyInfo(T &info) {
        if (head == -1) throw BasicException();
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            file.seekg(nodNow.next * sizeofNod + sizeof(int));
            file.read(reinterpret_cast<char *>(&nodNow), sizeofNod);
            if (nodNow.num == 0) continue;
            if (info < nodNow.array[0]) throw BasicException();
            if (info > nodNow.array[nodNow.num - 1]) continue;
            //可能在该块内
            if (nodNow == 1) {
                file.seekp(nodLoc * sizeofNod + sizeof(int));
                file.write(reinterpret_cast<char *>(&info), sizeofNod);
                return;
            }
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, info) - nodNow.array;
            if (n >= nodNow.num || nodNow.array[n] != info) throw BasicException();
            file.seekp(nodLoc * sizeofNod + sizeof(int));
            file.write(reinterpret_cast<char *>(&info), sizeofNod);
            return;
        }
    }

    //返回某位置的数据(只关心索引string)
    T readInfo(T &info){
        if (head == -1) throw BasicException();
        int nodLoc;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            nodLoc = nodNow.next;
            file.seekg(nodNow.next * sizeofNod + sizeof(int));
            file.read(reinterpret_cast<char *>(&nodNow), sizeofNod);
            if (nodNow.num == 0) continue;
            if (info < nodNow.array[0]) throw BasicException();
            if (info > nodNow.array[nodNow.num - 1]) continue;
            //可能在该块内
            int n = std::lower_bound(nodNow.array, nodNow.array + nodNow.num, info) - nodNow.array;
            if (n >= nodNow.num || nodNow.array[n] != info) throw BasicException();
            return nodNow.array[n];
        }
    }

    //返回某数据的上一位数据
    T frontInfo(const T info);
    //查找某索引的数据是否存在
    bool findInfo(const T info);
    //打印数据全体
    void printAllInfo(){
        Node nodLoc;
        nodLoc.next = head;
        while (nodLoc.next != -1) {
            file.seekp(nodLoc.next);
            file.read(reinterpret_cast<char*>(&nodLoc),sizeofNod);
            std::cout << nodLoc;
        }
    }
    //打印符合demandInfo要求的内容
    void printDemand(T demandInfo){
        Node nodLoc;
        nodLoc.next = head;
        while (nodLoc.next != -1) {
            file.seekp(nodLoc.next);
            file.read(reinterpret_cast<char*>(&nodLoc),sizeofNod);
            for (int i = 0; i < nodLoc.num; ++i) {
                if (nodLoc.array[i] - demandInfo) std::cout << nodLoc;
            }
        }
    }
    //获得该数据的位置（依靠index文件类）
    //int getIndex(const T info);
    //合并块
    //void Merge();
    //删除某个块
    //void deleteBlock();
};

#endif //UNTITLED3_FILEDATA_H
