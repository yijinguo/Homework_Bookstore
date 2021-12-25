#include "parser.h"

AccountInf Account::accountLog = {0,"0","0","0"};
int Account::staffNum = 0;
bool Account::haveSelect = false;
std::vector<std::string> Account::staffAll;

using namespace std;
int main(){
    //freopen("100.txt","r",stdin);
    //freopen("gyj.out","a",stdout);
    Statement stmt;
    while (true) {
        try {
            string cmd;
            getline(cin, cmd);
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