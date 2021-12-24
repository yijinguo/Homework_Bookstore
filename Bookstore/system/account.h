#ifndef UNTITLED3_ACCOUNT_H
#define UNTITLED3_ACCOUNT_H

#include "../FileDataBase/FileData.h"
#include <vector>

struct AccountInf {
    int priority;
    char userID[30];
    char password[30];
    char userName[30];

    /*AccountInf &operator=(const AccountInf &b) {
        if (*this == b) return *this;
        priority = b.priority;
        strcpy(userID,b.userID);
        strcpy(password,b.password);
        strcpy(userName,b.userName);
        return *this;
    }

    bool operator==(const AccountInf &b) const {
        if (priority != b.priority) return false;
        if (strcmp(userID,b.userID) != 0) return false;
        if (strcmp(password,b.password) != 0) return false;
        if (strcmp(userName,b.userName) != 0) return false;
        return true;
    }

    friend std::ostream &operator<<(std::ostream &os, const AccountInf &a) {
        os << a.priority << " " << a.userID << " " << a.password << " " << a.userName << '\n';
        return os;
    }*/

    void Initialize(){
        strcpy(userID,"root");
        strcpy(password,"sjtu");
        priority = 7;
    }
    void original(){
        priority = 0;
        userID[0] = '\0';
        password[0] = '\0';
        userName[0] = '\0';
    }

};

class Account {

private:

    Stack accountInStack;//登录栈应该只保ID，从栈中提取时再从DataStore中提取
    FileData<AccountInf> accountDataStore;

public:

    std::vector<std::string> staffAll;//全体员工:ID索引
    int staffNum = 0;//记录员工人数
    static bool haveSelect;//记录是否有图书被选中
    static AccountInf accountLog;//只记录最新登陆，进行操作的用户

    static void setLog(AccountInf &a){accountLog = a;}
    static void selectTrue(){haveSelect = true;}
    static void selectFalse(){haveSelect = false;}

    Account();
    //登录{0}
    void su(const std::string &_user_id, const std::string &_password);
    //登出{1}
    void logout();
    //注册{0}
    void Register(const std::string &_user_id, const std::string &_password, const std::string &_user_name);
    //修改密码{1}
    //注意如果该账户为目前登录账户记得修改accountLog
    void passwd(const std::string &_user_id, const std::string &old_password,const std::string &new_password);
    //创建用户{3}
    void Useradd(const std::string &_user_id, const std::string &_password, const int &priority, const std::string &_user_name);
    //删除用户{7}
    void deleteAccount(const std::string &_user_id);

};
#endif //UNTITLED3_ACCOUNT_H
