#include "parser.h"
#include <fstream>
AccountInf Account::accountLog = {0,"0","0","0"};
int Account::staffNum = 0;
bool Account::haveSelect = false;
std::vector<std::string> Account::staffAll;


using namespace std;
int main(){
    //freopen("testcase5.txt","r",stdin);
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