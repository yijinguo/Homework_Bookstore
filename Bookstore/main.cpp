#include "parser.h"
#include <fstream>

AccountInf Account::accountLog = {0,"0","0","0"};
bool Account::haveSelect = false;
Vector Account::staffAll("fileStaffDataBase");
Stack Account::accountInStack;

using namespace std;
int main(){
    Statement stmt;
    string cmd;
    while (getline(cin, cmd)) {
        try {
            execute(stmt,cmd);
        } catch (BlankLineException &skip) {
            continue;
        } catch (OutException &out) {
            return 0;
        } catch (BasicException &ex) {
            cout << "Invalid\n";
        }
    }
}