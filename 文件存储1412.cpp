#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#define MAXSIZE 400

template<class T>
class database {
private:

    struct Information {
        char index[65];
        T value;

        void Initialize(std::string _index, T _value) {
            strcpy(index, _index.c_str());
            value = _value;
        }

        bool operator<(const Information &b) {
            if (strcmp(index, b.index) < 0) return true;
            if (strcmp(index, b.index) > 0) return false;
            if (value < b.value) return true;
            return false;
        }

        friend bool operator<(const Information &a, const Information &b) {
            if (strcmp(a.index, b.index) < 0) return true;
            if (strcmp(a.index, b.index) > 0) return false;
            if (a.value < b.value) return true;
            return false;
        }

        bool operator>(const Information &b) {
            if (strcmp(index, b.index) > 0) return true;
            if (strcmp(index, b.index) < 0) return false;
            if (value > b.value) return true;
            return false;
        }

        friend bool operator>(const Information &a, const Information &b) {
            if (strcmp(a.index, b.index) > 0) return true;
            if (strcmp(a.index, b.index) < 0) return false;
            if (a.value > b.value) return true;
            return false;
        }

        friend bool operator>=(const Information &a, const Information &b) {
            if (strcmp(a.index, b.index) > 0) return true;
            if (strcmp(a.index, b.index) < 0) return false;
            if (a.index >= b.index) return true;
            return false;
        }

        bool operator==(const Information &b) {
            if (strcmp(index, b.index) == 0 && value == b.value) return true;
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
    std::fstream dataFile;
    std::string fileName = "AFileList";

public:

    database() {
        dataFile.open(fileName, std::fstream::in);
        if (!dataFile) {
            dataFile.open(fileName, std::fstream::out);
            dataFile.write(reinterpret_cast<char *>(&head), sizeof(int));
        } else {
            dataFile.seekg(0);
            dataFile.read(reinterpret_cast<char *>(&head), sizeof(int));
        }
        dataFile.close();
        dataFile.open(fileName);
    }

    ~database() {
        dataFile.seekp(0);
        dataFile.write(reinterpret_cast<char *>(&head), sizeof(int));
        dataFile.close();
    }

    void Insert(std::string index, T value) {
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

    void Delete(std::string index, T value) {
        if (head == -1) return;
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
            if (deleteInf < nodNow.array[0]) {
                return;
            }
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
            //遍历数组
            for (int i = 0; i < nodNow.num; ++i) {
                if (nodNow.array[i] == deleteInf) {
                    for (int j = i; j < nodNow.num - 1; ++j) {
                        nodNow.array[j] = nodNow.array[j + 1];
                    }
                    nodNow.num--;
                    dataFile.seekp(nodLoc * sizeNod + sizeof(int));
                    dataFile.write(reinterpret_cast<char *>(&nodNow), sizeNod);
                    return;
                }
            }
        }
    }

    void Find(std::string index) {
        if (head == -1) {
            std::cout << "null" << '\n';
            return;
        }
        std::vector<T> allFind;
        bool haveFind = false;
        Node nodNow;
        nodNow.next = head;
        while (nodNow.next != -1) {
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            if (nodNow.num == 0) continue;
            if (index < std::string(nodNow.array[0].index)) break;
            if (index > std::string(nodNow.array[nodNow.num - 1].index)) continue;
            //可能在本节点内
            bool findEnd = false;
            for (int i = 0; i < nodNow.num; ++i) {
                if (index < std::string(nodNow.array[i].index)) {
                    findEnd = true;
                    break;
                }
                if (index > std::string(nodNow.array[i].index)) continue;
                haveFind = true;
                allFind.push_back(nodNow.array[i].value);
            }
            if (findEnd) break;
        }
        if (!haveFind) {
            std::cout << "null" << '\n';
            return;
        }
        auto it = allFind.begin();
        while (it != (--allFind.end())) {
            std::cout << *it << ' ';
            it++;
        }
        std::cout << *it << '\n';
    }

    void printList() {
        std::fstream printFile("APrintList.out", std::fstream::out);
        Node nodNow;
        nodNow.next = head;
        dataFile.open(fileName);
        printFile << head << '\n';
        while (nodNow.next != -1) {
            dataFile.seekg(nodNow.next * sizeNod + sizeof(int));
            dataFile.read(reinterpret_cast<char *>(&nodNow), sizeNod);
            printFile << nodNow.num << '\n';
            if (nodNow.num == 0) continue;
            for (int i = 0; i < nodNow.num; ++i) {
                printFile << nodNow.array[i].index << ' ' << nodNow.array[i].value << '\n';
            }
        }
        dataFile.close();
    }

};

using namespace std;
int main() {
    //freopen("5.in","r",stdin);
    //freopen("gyj.out","w",stdout);
    int n;
    cin >> n;
    string command;
    database<int> content;
    for (int i = 1; i <= n; ++i) {
        cin >> command;
        if (command == "insert") {
            string index;
            int value;
            cin >> index >> value;
            content.Insert(index, value);
        } else if (command == "delete") {
            string index;
            int value;
            cin >> index >> value;
            content.Delete(index, value);
        } else if (command == "find") {
            string index;
            cin >> index;
            content.Find(index);
        }
    }
    //以下数行函数调用用于debug
    //content.printList();
    return 0;
}