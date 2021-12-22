#include "account.h"

Account::Account() {
    accountInStack.initialise("fileInStack");
    accountDataStore.initialise("fileAllAccount");
    AccountInf shopkeeper;
    shopkeeper.Initialize();
    accountDataStore.addInfo(shopkeeper);
    accountLog.original();
    accountInStack.addInfo(accountLog);
    selectFalse();
}

void Account::su(const std::string &_user_id, const std::string &_password){
    try {
        AccountInf index;
        strcpy(index.userID, _user_id.c_str());
        AccountInf accountSu = accountDataStore.readInfo(index);
        if (accountLog.priority <= accountSu.priority) {
            if (strcmp(accountSu.password, _password.c_str()) != 0) throw BasicException();
        }
        accountInStack.addInfo(accountSu);
        setLog(accountSu);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Account::logout(){
    try{
        if (accountLog.priority < 1) throw BasicException();
        AccountInf another = accountInStack.frontInfo(accountLog);
        setLog(another);
        accountInStack.deleteInfo(accountLog);
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
        accountDataStore.addInfo(newRegister);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Account::passwd(const std::string &_user_id, const std::string &old_password,const std::string &new_password){
    try {
        if (accountLog.priority < 1) throw BasicException();
        AccountInf modify;
        strcpy(modify.userID, _user_id.c_str());
        modify = accountDataStore.readInfo(modify);
        if (accountLog.priority != 7) {
            if (modify.password != old_password) throw BasicException();
        }
        strcpy(modify.password, new_password.c_str());
        accountDataStore.modifyInfo(modify);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Account::Useradd(const std::string &_user_id, const std::string &_password, const int &priority, const std::string &_user_name){
    try {
        if (accountLog.priority < 3 || accountLog.priority <= priority) throw BasicException();
        AccountInf newCreate;
        newCreate.priority = priority;
        strcpy(newCreate.userID,_user_id.c_str());
        strcpy(newCreate.password,_password.c_str());
        strcpy(newCreate.userName,_user_name.c_str());
        accountDataStore.addInfo(newCreate);
        if (priority == 3) {
            staffNum++;
            staffAll.push_back(newCreate);
        }
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

void Account::deleteAccount(const std::string &_user_id){
    try {
        if (accountLog.priority < 7) throw BasicException();
        AccountInf deleteIt;
        strcpy(deleteIt.userID,_user_id.c_str());
        if (accountInStack.findInfo(deleteIt)) throw BasicException();
        accountDataStore.deleteInfo(deleteIt);
    } catch (BasicException  &ex) {
        throw BasicException();
    }
}