#ifndef N2AREA_H
#define N2AREA_H
#include <string>
#include <vector>
#include <fstream>
#include "N2Basics.h"
#include "N2Cell.h"

using namespace std;

class N2Area
{
    public:
        N2Area(int pcols, int prows); // define the area with its size
        virtual ~N2Area();
        int cmdLength(string s, int pos);
        int cmdLen(char c1, char c2);
        void moveCursor(char direction, int& cx, int& cy);
        void closefs();
        long long int readInt();
        string readStr();
        string getStrLrdir(char lrdir, int cx, int cy);
        long long int getIntLrdir(char lrdir, int cx, int cy);
        int interpreter(string cmds, bool printcmd, bool printstack,bool iasa, string workDir, string basefile);
        bool findDown(int val, bool testequal, int flag, int& cx, int& cy);
        bool findRight(int val, bool testequal, int flag, int& cx, int& cy);
    protected:

    private:
        bool printcmd;
        N2Basics b;
        int cols;
        int rows;
        int cmax;
        int rmax;
        vector<vector<N2Cell>> area;
        char lrdir = 'r'; // current left/right setting
        long long int givr[10] = {0,0,0,0,0,0,0,0,0,0};
        string gsvr[10] = {"","","","","","","","","",""};
        string gecr[10] = {"","","","","","","","","",""};
        string noparcmds = "aAdDeiIjlLnNoOrRuUwW:+-*/[]";
        string parcmds = "bBgGhkmsSv=<>!%";
        string partwocmds = "JKxX#&|";
        string parthreecmds = "f";
        string parn1cmds = "tT";
        string parn2cmds = "E";
        string parsplitcmds = "$?";
        fstream fs1;
        fstream fs2;
        string finfile;
        string foufile;
        // predefined points
        int bpx[4];
        int bpy[4];

};

#endif // N2AREA_H
