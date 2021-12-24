#include "statement.h"

AccountInf Account::accountLog = {0,"0","0","0"};
int Account::staffNum = 0;
bool Account::haveSelect = false;
std::vector<std::string> Account::staffAll;

Statement::Statement() = default;
Statement::~Statement() noexcept  = default;
//顺序解析命令语句，得到单词
Statement::Statement(std::string &cmd):cmdLine(cmd){}
void Statement::separateCmd(std::string &cmd){
    words.clear();
    int index = 0;
    while (cmd[index] != '\0') {
        while (cmd[index] == ' ') { ++index; }
        if (cmd[index] == '\0') {
            words.emplace_back("");
            return;
        }
        int wordHead = index;
        while (cmd[index] != ' ' && cmd[index] != '\0') { ++index; }
        char word[index - wordHead + 1];
        for (int i = 0; i < index - wordHead; ++i) {word[i] = cmd[i + wordHead];}
        word[index - wordHead] = '\0';
        std::string separateWord = std::string(word);
        wordNum++;
        words.push_back(separateWord);
    }
}

Quit::Quit(std::string &cmd) : Statement::Statement(cmd){}
void Quit::execute(){
    throw OutException();
}

Exit::Exit(std::string &cmd) : Statement::Statement(cmd){}
void Exit::execute() {
    throw OutException();
}

/*检查仅含有数字，字母，下划线*/
bool wordExam(std::string word) {
    if (word.length() >= 30) throw BasicException();
    for (int i = 0; i < word.length(); ++i) {
        char s = word[i];
        if (!(s == '_' || s >= '0' && s <= '9' || s >= 'a' && s <= 'z' || s >= 'A' && s <= 'Z')) return false;
    }
    return true;
}

Su::Su(std::string &cmd,std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void Su::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum > 2 || wordNum == 0) throw BasicException();
        if (!wordExam(words[0])) throw BasicException();
        if (!wordExam(words[1])) throw BasicException();
        accountSystem.su(words[0], words[1]);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Logout::Logout(std::string &cmd,std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void Logout::execute() {
    try {
        if (!cmdLine.empty()) throw BasicException();
        accountSystem.logout();
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Register::Register(std::string &cmd,std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void Register::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 3) throw BasicException();
        if (!wordExam(words[0])) throw BasicException();
        if (!wordExam(words[1])) throw BasicException();
        if (words[2].length() > 30) throw BasicException();
        accountSystem.Register(words[0],words[1],words[2]);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Passwd::Passwd(std::string &cmd, std::string &line) {
    diaryLine = line;
    cmdLine = cmd;
}
void Passwd::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 2 && wordNum != 3) throw BasicException();
        if (!wordExam(words[0])) throw BasicException();
        if (!wordExam(words[1])) throw BasicException();
        if (wordNum == 2) {
            accountSystem.passwd(words[0],"",words[1]);
        } else {
            if (!wordExam(words[2])) throw BasicException();
            accountSystem.passwd(words[0],words[1],words[2]);
        }
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Useradd::Useradd(std::string &cmd, std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void Useradd::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 4) throw BasicException();
        if (!wordExam(words[0])) throw BasicException();
        if (!wordExam(words[1])) throw BasicException();
        if (words[2] != "0" && words[2] != "1" && words[2] != "3") throw BasicException();
        if (words[3].length() > 30) throw BasicException();
        int _priority = words[2][0] - '0';
        accountSystem.Useradd(words[0],words[1],_priority,words[3]);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Delete::Delete(std::string &cmd, std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void Delete::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 1) throw BasicException();
        if (!wordExam(words[0])) throw BasicException();
        accountSystem.deleteAccount(words[0]);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Show::Show(std::string &cmd, std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void Show::execute() {
    try {
        bookSystem.show(cmdLine);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Buy::Buy(std::string &cmd, std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void Buy::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 2) throw BasicException();
        if (words[0].length() > 20) throw BasicException();
        if (words[1].length() > 10) throw BasicException();
        int _quantity = 0, l = words[1].length(), place = 1;
        for (int i = 1; i <= l; ++i) {
            if (!(words[1][i] >= '0' && words[1][i] <= '9')) throw BasicException();
            _quantity += place * words[1][l - i];
            place *= 10;
        }
        bookSystem.buy(words[0],_quantity,diarySystem);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Select::Select(std::string &cmd, std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void Select::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 1) throw BasicException();
        if (words[0].length() > 20) throw BasicException();
        bookSystem.select(words[0]);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Modify::Modify(std::string &cmd, std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void Modify::execute() {
    try {
        bookSystem.modify(cmdLine);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

Import::Import(std::string &cmd, std::string &line) {
    diaryLine = line;
    cmdLine = cmd;
}
void Import::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 2) throw BasicException();
        if (words[0].length() > 10) throw BasicException();
        if (words[1].length() > 13) throw BasicException();
        int _quantity = 0, l = words[0].length(),place = 1;
        for (int i = 1; i <= l; ++i) {
            if (!(words[0][i] >= '0' && words[0][i] <= '9')) throw BasicException();
            _quantity += place * words[0][l - i];
            place *= 10;
        }
        for (int i = 0; i < words[1].length(); ++i) {
            if (!(words[1][i] == '.' || words[1][i] >= '0' && words[1][i] <= '9')) throw BasicException();
        }
        bookSystem.import(_quantity, words[1],diarySystem);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);

    } catch (BasicException &ex) {
        throw BasicException();
    }
}



//以下注意检查cmd是否只有一个指令词，不为空抛出异常
ReportMyself::ReportMyself(std::string &cmd,std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void ReportMyself::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 1) throw BasicException();
        if (Account::accountLog.priority != 3 && Account::accountLog.priority != 7) throw BasicException();
        std::string ID = std::string(Account::accountLog.userID);
        diarySystem.reportMyself(ID);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.reportMyself(name);
        diarySystem.write(priority, name, diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

ShowFinance::ShowFinance(std::string &cmd,std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}

void ShowFinance::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum == 1) {
            diarySystem.showFinance(-1);
        } else if (wordNum == 2) {
            if (words[1].length() > 10) throw BasicException();
            int time = 0, place = 1;
            for (int i = 0; i < words[1].length(); ++i) {
                if (!(words[1][i] >= '0' && words[1][i] <= '9')) throw BasicException();
                time += place * (words[1][i]- '0');
                place *= 10;
            }
            diarySystem.showFinance(time);
        } else {
            throw BasicException();
        }
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name, diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


ReportFinance::ReportFinance(std::string &cmd, std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void ReportFinance::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 1) throw BasicException();
        diarySystem.reportFinance();
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name, diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

ReportEmployee::ReportEmployee(std::string &cmd,std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void ReportEmployee::execute() {
    try {
        separateCmd(cmdLine);
        if (wordNum != 1) throw BasicException();
        diarySystem.reportEmployee();
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name, diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}

log::log(std::string &cmd, std::string &line){
    diaryLine = line;
    cmdLine = cmd;
}
void log::execute() {
    try {
        if (!cmdLine.empty()) throw BasicException();
        diarySystem.LogRecord();
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name, diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}