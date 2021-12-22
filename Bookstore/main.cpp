#include "parser.h"

using namespace std;
int main(){
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