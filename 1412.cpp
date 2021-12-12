#include <iostream>
#include <fstream>
#include <map>
#include <set>
using namespace std;

template<class Key,class Value>
class database {

    //块链用于保存索引对应的值在值文件中的读入位置
    //只有在维护值时才打开值文件;只有在析构和构造时才打开索引文件

private:

    struct datalist{
        map<Key,set<Value>> location;
        Key begin,end;//记录本节点数组中索引值范围
        int num = 0;//本节点已存储索引条目,即location大小
        datalist *front;
        datalist *next;
    };
    datalist *head;
    datalist *rear;
    int eachLength = 0;//每个节点数组长度,维护2√all;
    int all = 0;//已储存总条目数
    fstream fileKey;//储存索引的文件(索引值,在值文件的位置)
    fstream fileValue;//储存值的文件

public:

    string fileKeyName = "debugKey";
    string fileValueName = "debugValue";

    database() {
        head = new datalist;
        rear = head;
        head->front = nullptr;
        rear->next = nullptr;
        fileKey.open(fileKeyName,fstream::in);
        fileValue.open(fileValueName,fstream::in);
        if (!fileKey) {
            fileKey.open(fileKeyName,fstream::out);
            fileValue.open(fileValueName,fstream::out);
            fileKey.close();
            fileValue.close();
            return;
        }
        fileValue.close();
        //fileKey和fileValue都已存在
        //以下读取文件内信息,构建块链
        fileKey >> eachLength;
        datalist *it = rear;
        it->num = 0;
        while (fileKey.peek() != EOF) {
            Key index;
            long place;
            fileKey >> index >> place;
            if (it->location.count(index) != 0) {
                it->location[index].insert(place);
            } else {
                if (rear == head) {
                    it = new datalist;
                    head->next = it;
                    it->front = head;
                    rear = it;
                    rear->next = nullptr;
                }
                all++;
                if (it->location.empty()) {
                    it->begin = index;
                }
                if (it->num < eachLength / 2) {
                    it->location[index].insert(place);
                    it->end = index;
                    it->num++;
                } else {
                    it = new datalist;
                    it->location[index].insert(place);
                    it->begin = it->end = index;
                    it->num = 1;
                    it->front = rear;
                    rear->next = it;
                    rear = it;
                    rear->next = nullptr;
                }
            }
        }
        fileKey.close();
    }

    ~database() {
        //把链表信息写入索引文件,首先写eachLength,然后把每组索引写入文件
        fileKey.open(fileKeyName,fstream::out);
        fileKey << eachLength;
        datalist *it = head->next;
        while (it != nullptr) {
            auto iter = it->location.begin();
            while (iter != it->location.end()) {
                for (Value x : iter->second) {
                    fileKey << ' ' << iter->first;
                    fileKey << ' '<< x ;
                }
                iter->second.clear();
                iter++;
            }
            it->location.clear();
            it = it->next;
        }
        fileKey.close();
        //析构链表
        datalist *tmp = rear;
        while (tmp != head) {
            rear = rear->front;
            delete tmp;
            tmp = rear;
        }
        delete head;
    }

    void Insert(Key index, Value value){
        //将信息写入两个文件
        fileValue.open(fileValueName,fstream::app);
        long place = fileValue.tellp();
        fileValue << value << ' ';
        fileValue.close();
        //遍历链表，找到插入位置
        if (head == rear) {//块链为空，直接增加节点
            datalist *tmp = new datalist;
            tmp->location[index].insert(place);
            tmp->num = 1;
            tmp->begin = tmp->end = index;
            tmp->front = rear;
            rear->next = tmp;
            rear = tmp;
            rear->next = nullptr;
            eachLength = 2;
            all = 1;
        } else {//先找到对应位置，再考虑插入方法
            datalist *it = head->next;
            bool haveInsert = false;
            while (it->next != nullptr) {
                if (index >= it->begin && index <= it->end) {
                    //在it节点范围内，放入it节点
                    haveInsert = true;
                    if (it->location.count(index) != 0) {
                        it->location[index].insert(place);
                    } else {
                        all++;
                        if (eachLength*eachLength < 4 * all) eachLength+=2;
                        if (it->num < eachLength) {
                            it->location[index].insert(place);
                        } else {//分裂节点
                            datalist *tmp = new datalist;
                            tmp->end = it->end;
                            tmp->location[index].insert(place);
                            auto iter = it->location.begin();
                            int iterNum = 1;
                            while (iterNum < it->num / 2) {
                                iterNum++;
                                iter++;
                            }
                            //iterNum = it->num / 2
                            it->end = iter->first;
                            iter++;
                            iterNum++;
                            tmp->begin = iter->first;
                            while (iterNum <= it->num) {
                                tmp->location[iter->first] = iter->second;
                                it->location.erase(iter->first);
                                iter++;
                                iterNum++;
                            }
                            it->num = it->num / 2;
                            tmp->num = it->num + 1;
                            //以上完成对it节点的分裂；以下完成对新增节点的维护
                            it->next->front = tmp;
                            tmp->next = it->next;
                            it->next = tmp;
                            tmp->front = it;
                        }
                    }
                    break;
                } else if (index > it->end && index < it->next->begin) {
                    haveInsert = true;
                    all++;
                    if (eachLength*eachLength < 4 * all) eachLength += 2;
                    if (it->num < eachLength) {
                        it->location[index].insert(place);
                        it->end = index;
                        it->num++;
                    } else if (it->next->num < eachLength) {
                        it->next->location[index].insert(place);
                        it->next->begin = index;
                        it->next->num++;
                    } else {
                        datalist *tmp = new datalist;
                        tmp->location[index].insert(place);
                        tmp->begin = tmp->end = index;
                        tmp->num = 1;
                        it->next->front = tmp;
                        tmp->next = it->next;
                        it->next = tmp;
                        tmp->front = it;
                    }
                    break;
                } else {
                    it = it->next;
                }
            }
            //it为链表最后一个节点
            if (!haveInsert) {
                if (it->location.count(index) != 0) {
                    it->location[index].insert(place);
                } else {
                    all++;
                    if (eachLength*eachLength < 4 * all) eachLength += 2;
                    if (it->num < eachLength) {
                        it->location[index].insert(place);
                        it->num++;
                        if (index < it->begin) {
                            it->begin = index;
                        } else if (index > it->end) {
                            it->end = index;
                        }
                    } else {//分裂节点
                        datalist *tmp = new datalist;
                        if (index > it->end) {
                            tmp->end = index;
                        } else {
                            tmp->end = it->end;
                        }
                        tmp->location[index].insert(place);
                        auto iter = it->location.begin();
                        int iterNum = 1;
                        while (iterNum < it->num / 2) {
                            iterNum++;
                            iter++;
                        }
                        //iterNum = it->num / 2
                        it->end = iter->first;
                        iter++;
                        iterNum++;
                        if (index < iter->first) {
                            tmp->begin = index;
                        } else {
                            tmp->begin = iter->first;
                        }
                        while (iterNum <= it->num) {
                            tmp->location[iter->first] = iter->second;
                            it->location.erase(iter->first);
                            iter++;
                            iterNum++;
                        }
                        it->num = it->num / 2;
                        tmp->num = it->num + 1;
                        it->next = tmp;
                        tmp->front = it;
                        tmp->next = nullptr;
                        rear = tmp;
                    }
                }
            }
        }
    }

    void Delete(Key index, Value value){
        //遍历链表，找到相应元素
        datalist *it = head->next;
        bool haveFind = false;
        while (it != nullptr) {
            if (it->location.count(index) == 0) {
                it = it->next;
            } else {
                haveFind = true;
                break;
            }
        }
        if (haveFind) {
            set<Value> del = it->location[index];
            fileValue.open(fileValueName);
            auto iter = del.begin();
            while (iter != del.end()) {
                fileValue.seekg(*iter);
                Value u;
                fileValue >> u;
                if (u == value) {
                    del.erase(*iter);
                    break;
                }
            }
            fileValue.close();
            if (del.empty()) {
                it->num--;
                all--;
                if ((eachLength - 1) * (eachLength - 1) > 4 * all) eachLength -= 2;
                if (it->num == 0) {
                    if (it->next == nullptr) {
                        it->front->next = nullptr;
                    } else {
                        it->front->next = it->next;
                        it->next->front = it->front;
                    }
                    delete it;
                } else {
                    if (index == it->begin) {
                        auto iter1 = (++it->location.begin());
                        it->begin = iter1->first;
                    } else if (index == it->end) {
                        auto iter1 = (++it->location.rbegin());
                        it->end = iter1->first;
                    }
                    it->location.erase(index);
                }
            } else {
                it->location[index] = del;
            }
        }
    }

    void Find(Key index){
        //遍历链表，找到相应元素
        datalist *it = head->next;
        bool haveFind = false;
        while (it != nullptr) {
            if (it->location.count(index) == 0) {
                it = it->next;
            } else {
                haveFind = true;
                break;
            }
        }
        if (!haveFind) {
            cout << "null" << '\n';
        } else {
            set<Value> plus = it->location[index];
            auto iter = plus.begin();
            fileValue.open(fileValueName);
            set<Value> findOut;
            while (iter != plus.end()) {
                fileValue.seekg(*iter);
                Value value;
                fileValue >> value;
                findOut.insert(value);
                iter++;
            }
            fileValue.close();
            for (Value x:findOut) {
                cout << x << " ";
            }
            cout << '\n';
        }
    }

};

int main(){
    freopen("gyj.in","r",stdin);
    int n;
    cin >> n;
    string command;
    database<string,int> content;
    for (int i = 1; i <= n; ++i) {
        cin >> command;
        if (command == "insert") {
            string index;
            int value;
            cin >> index >> value;
            content.Insert(index,value);
        } else if (command == "delete") {
            string index;
            int value;
            cin >> index >> value;
            content.Delete(index,value);
        } else if (command == "find") {
            string index;
            cin >> index;
            content.Find(index);
        }
    }
    return 0;
}