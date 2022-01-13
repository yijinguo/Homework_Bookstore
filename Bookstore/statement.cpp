#include "statement.h"

Statement::Statement() = default;
Statement::~Statement() noexcept  = default;
//顺序解析命令语句，得到单词
void Statement::define(std::string &cmd,std::string &line) {
    words.clear();
    wordNum = 0;
    cmdLine = cmd;
    diaryLine = line;
}

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


void Statement::Quit (){
    throw OutException();
}

void Statement::Exit () {
    throw OutException();
}

/*检查仅含有数字，字母，下划线*/
bool wordExam(std::string word) {
    if (word.length() > 30) throw BasicException();
    for (int i = 0; i < word.length(); ++i) {
        char s = word[i];
        if (!(s == '_' || s >= '0' && s <= '9' || s >= 'a' && s <= 'z' || s >= 'A' && s <= 'Z')) return false;
    }
    return true;
}

/*检测仅含可见字符*/
bool wordVisual(std::string word){
    for (int i = 0; i < word.length(); ++i) {
        if (!(word[i] > 32 && word[i] < 127)) return false;
    }
    return true;
}

void Statement::Su () {
    try {
        separateCmd(cmdLine);
        if (wordNum > 2 || wordNum == 0) throw BasicException();
        if (!wordExam(words[0])) throw BasicException();
        std::string second;
        if (wordNum == 2) {
            if (!wordExam(words[1])) throw BasicException();
            second = words[1];
        } else {
            second = "";
        }
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
        accountSystem.su(words[0], second);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


void Statement::Logout () {
    try {
        if (cmdLine[0] != '\0') throw BasicException();
        std::string s = accountSystem.logout();
        if (s[0] != '\0') {
            //Account::selectTrue();
            Account::haveSelect = true;
            bookSystem.select(s);
        }
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


void Statement::Register () {
    try {
        separateCmd(cmdLine);
        if (wordNum != 3) throw BasicException();
        if (!wordExam(words[0])) throw BasicException();
        if (!wordExam(words[1])) throw BasicException();
        if (words[2].length() > 30 || !wordVisual(words[2])) throw BasicException();
        accountSystem.Register(words[0],words[1],words[2]);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


void Statement::Passwd () {
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


void Statement::Useradd () {
    try {
        separateCmd(cmdLine);
        if (wordNum != 4) throw BasicException();
        if (!wordExam(words[0])) throw BasicException();
        if (!wordExam(words[1])) throw BasicException();
        if (words[2] != "0" && words[2] != "1" && words[2] != "3") throw BasicException();
        if (words[3].length() > 30 || !wordVisual(words[3])) throw BasicException();
        int _priority = words[2][0] - '0';
        accountSystem.Useradd(words[0],words[1],_priority,words[3]);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


void Statement::Delete () {
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


void Statement::Show () {
    try {
        bookSystem.show(cmdLine);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


void Statement::Buy () {
    try {
        separateCmd(cmdLine);
        if (wordNum != 2) throw BasicException();
        if (words[0].length() > 20) throw BasicException();
        if (words[1].length() > 10) throw BasicException();
        int _quantity = 0, l = words[1].length(), place = 1;
        for (int i = 1; i <= l; ++i) {
            if (!(words[1][l - i] >= '0' && words[1][l - i] <= '9')) throw BasicException();
            _quantity += place * (words[1][l - i] - '0');
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


void Statement::Select () {
    try {
        separateCmd(cmdLine);
        if (wordNum != 1) throw BasicException();
        if (words[0].length() > 20 || !wordVisual(words[0])) throw BasicException();
        bookSystem.select(words[0]);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


void Statement::Modify () {
    try {
        bookSystem.modify(cmdLine);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name,diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


void Statement::Import () {
    try {
        separateCmd(cmdLine);
        if (wordNum != 2) throw BasicException();
        if (words[0].length() > 10) throw BasicException();
        if (words[1].length() > 13) throw BasicException();
        int _quantity = 0, l = words[0].length() - 1, place = 1;
        for (int i = 0; i <= l; ++i) {
            if (!(words[0][l - i] >= '0' && words[0][l - i] <= '9')) throw BasicException();
            _quantity += place * (words[0][l - i] - '0');
            place *= 10;
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

void Statement::ReportMyself () {
    try {
        separateCmd(cmdLine);
        if (wordNum != 0) throw BasicException();
        if (Account::accountLog.priority != 3 && Account::accountLog.priority != 7) throw BasicException();
        std::string ID = std::string(Account::accountLog.userID);
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name, diaryLine);
        diarySystem.reportMyself(ID);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}



void Statement::ShowFinance () {
    try {
        separateCmd(cmdLine);
        if (wordNum == 1) {
            diarySystem.showFinance(-1);
        } else if (wordNum == 2) {
            if (words[1].length() > 10) throw BasicException();
            int time = 0, place = 1;
            for (int i = words[1].length() - 1; i >= 0; --i) {
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


void Statement::ReportFinance () {
    try {
        separateCmd(cmdLine);
        if (wordNum != 0) throw BasicException();
        diarySystem.reportFinance();
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name, diaryLine);
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


void Statement::ReportEmployee () {
    try {
        separateCmd(cmdLine);
        if (wordNum != 0) throw BasicException();
        int priority = Account::accountLog.priority;
        std::string name = std::string(Account::accountLog.userID);
        diarySystem.write(priority, name, diaryLine);
        diarySystem.reportEmployee();
    } catch (BasicException &ex) {
        throw BasicException();
    }
}


void Statement::log () {
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
