#include "FileData.h"

template<class T>
FileDataString<T>::FileDataString() = default;
template<class T>
FileDataString<T>::~FileDataString(){
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