#include "account.h"

Account::Account() {
    initialize();
    accountDataStore.initialize("fileAllAccountData ","fileAllAccount");
    AccountInf shopkeeper;
    shopkeeper.Initialize();
    accountDataStore.addInfo("root",shopkeeper);
    selectFalse();
}

void Account::su(const std::string &_user_id, const std::string &_password){
    try {
        AccountInf accountSu = accountDataStore.readInfo(_user_id);
        if (accountLog.priority <= accountSu.priority) {
            if (strcmp(accountSu.password, _password.c_str()) != 0) throw BasicException();
        }
        accountInStack.push(_user_id);
        setLog(accountSu);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Account::logout(){
    try{
        if (accountLog.priority < 1) throw BasicException();
        if (accountInStack.judgeNull()) {
            accountLog.priority = 0;
            return;
        }
        std::string nowInLog = accountInStack.pop();
        AccountInf another = accountDataStore.readInfo(nowInLog);
        setLog(another);
        selectFalse();
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Account::Register(const std::string &_user_id, const std::string &_password, const std::string &_user_name){
    try {
        AccountInf newRegister;
        newRegister.priority = 1;
        strcpy(newRegister.userID,_user_id.c_str());
        strcpy(newRegister.password,_password.c_str());
        strcpy(newRegister.userName, _user_name.c_str());
        accountDataStore.addInfo(_user_id,newRegister);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Account::passwd(const std::string &_user_id, const std::string &old_password,const std::string &new_password){
    try {
        if (accountLog.priority < 1) throw BasicException();
        AccountInf modify = accountDataStore.readInfo(_user_id);
        if (accountLog.priority != 7) {
            if (modify.password != old_password) throw BasicException();
        }
        strcpy(modify.password, new_password.c_str());
        accountDataStore.modifyInfo(_user_id,modify);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Account::Useradd(const std::string &_user_id, const std::string &_password, const int &priority, const std::string &_user_name){
    try {
        if (accountLog.priority < 3 || accountLog.priority <= priority) throw BasicException();
        if (priority != 1 && priority != 3 && priority != 7) throw BasicException();
        AccountInf newCreate;
        newCreate.priority = priority;
        strcpy(newCreate.userID,_user_id.c_str());
        strcpy(newCreate.password,_password.c_str());
        strcpy(newCreate.userName,_user_name.c_str());
        accountDataStore.addInfo(_user_id,newCreate);
        if (priority == 3) addStaff(_user_id);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Account::deleteAccount(const std::string &_user_id){
    try {
        if (accountLog.priority != 7) throw BasicException();
        if (accountInStack.InStack(_user_id)) throw BasicException();
        accountDataStore.deleteInfo(_user_id);
    } catch (BasicException  &ex) {
        throw BasicException();
    }
}