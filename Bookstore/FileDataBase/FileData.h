#ifndef UNTITLED3_FILEDATA_H
#define UNTITLED3_FILEDATA_H

#define MAXSIZE 400

#include <fstream>
#include <cstring>
#include <algorithm>
#include "error.h"

template<class T>//文件数据结构类接口
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

    FileData();
    FileData(const std::string _fileName);

    ~FileData();

    //初始化该文件:创建文件
    void initialise(std::string FN);

    //添加某数据
    void addInfo(T &info);
    //分块
    void Split(int nodLoc,Node &nodNow);

    //删除某数据(只关心索引string)
    void deleteInfo(T &info);

    //修改某数据
    void modifyInfo(T &info);

    //返回某位置的数据(只关心索引string)
    T readInfo(T &info);

    //返回某数据的上一位数据
    T frontInfo(const T info);
    //查找某索引的数据是否存在
    bool findInfo(const T info);
    //打印数据全体
    void printAllInfo();
    //打印符合demandInfo要求的内容
    void printDemand(T demandInfo);
    //获得该数据的位置（依靠index文件类）
    int getIndex(const T info);
    //合并块
    void Merge();
    //删除某个块
    void deleteBlock();
};

#endif //UNTITLED3_FILEDATA_H
