#include "parser.h"
#include <fstream>

AccountInf Account::accountLog = {0,"0","0","0"};
bool Account::haveSelect = false;
Vector Account::staffAll("StaffDataBase");
Stack Account::accountInStack;

using namespace std;
int main(){
    //freopen("2.in","r",stdin);
    //freopen("gyj.out","w",stdout);
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