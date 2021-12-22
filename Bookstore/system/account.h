#ifndef UNTITLED3_ACCOUNT_H
#define UNTITLED3_ACCOUNT_H

#include "FileData.h"
#include <vector>

struct AccountInf {
    int priority;
    char userID[30];
    char password[30];
    char userName[30];
    bool operator<(const AccountInf &b){
        if (strcmp(userID,b.userID) < 0) return true;
        return false;
    }
    bool operator>(const AccountInf &b){
        if (strcmp(userID,b.userID) > 0) return true;
        return false;
    }
    friend bool operator>(const AccountInf &a,AccountInf &b){
        if (strcmp(a.userID,b.userID) >= 0) return true;
        return false;
    }
    friend bool operator<(const AccountInf &a,AccountInf &b){
        if (strcmp(a.userID,b.userID) <= 0) return true;
        return false;
    }
    bool operator==(const AccountInf &b) {
        if (strcmp(userID,b.userID) == 0) return true;
        return false;
    }
    friend bool operator==(const AccountInf &a, AccountInf &b) {
        if (strcmp(a.userID, b.userID) == 0) return true;
        return false;
    }
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

    FileData<AccountInf> accountInStack;
    FileData<AccountInf> accountDataStore;

public:

    std::vector<AccountInf> staffAll;//全体员工
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
    void passwd(const std::string &_user_id, const std::string &old_password,const std::string &new_password);
    //创建用户{3}
    void Useradd(const std::string &_user_id, const std::string &_password, const int &priority, const std::string &_user_name);
    //删除用户{7}
    void deleteAccount(const std::string &_user_id);

};
#endif //UNTITLED3_ACCOUNT_H
