#ifndef UNTITLED3_ACCOUNT_H
#define UNTITLED3_ACCOUNT_H

#include "../FileDataBase/FileData.h"


struct AccountInf {
    int priority;
    char userID[31];
    char password[31];
    char userName[31];

    AccountInf operator=(const AccountInf &b){
        priority = b.priority;
        strcpy(userID,b.userID);
        strcpy(password,b.password);
        strcpy(userName,b.userName);
        return *this;
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

    FileData<AccountInf> accountDataStore;

public:

    static Stack accountInStack;//登录栈应该只保ID，从栈中提取时再从DataStore中提取
    static Vector staffAll;//全体员工:ID索引
    static bool haveSelect;//记录是否有图书被选中
    static AccountInf accountLog;//只记录最新登陆，进行操作的用户

    static void initialize(){
        accountLog.priority = 0;
        strcpy(accountLog.userID,"0");
        strcpy(accountLog.password,"0");
        strcpy(accountLog.userName,"0");
    }
    static void setLog(AccountInf &a){
        accountLog = a;
    }

    Account();
    //登录{0}
    void su(const std::string &_user_id, const std::string &_password);
    //登出{1}
    std::string logout();
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
