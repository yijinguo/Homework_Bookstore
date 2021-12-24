#include "parser.h"

using namespace std;
int main(){
    //freopen("testcase1.txt","r",stdin);
    //freopen("gyj.out","w",stdout);
    while (true) {
        try {
            string cmd;
            getline(cin, cmd);
            parser cmdP;
            Statement *stmt = cmdP.parserWord(cmd);
            stmt->execute();
            delete stmt;
        } catch (BlankLineException &skip) {
            continue;
        } catch (OutException &out) {
            return 0;
        } catch (BasicException &ex) {
            cout << "Invalid\n";
        }
    }
}