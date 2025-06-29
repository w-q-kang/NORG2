#include <iostream>
#include <cstdio>
#include "N2Area.h"
#include "N2Entry.h"
#include <stack>

using namespace std;

N2Area::N2Area(int pcols, int prows)
{
    //ctor
    cols = pcols;
    rows = prows;
    cmax = cols - 1;
    rmax = rows - 1;
    area.resize(cols,vector<N2Cell>(rows));
    bpx[0] = 0; bpy[0] = 0;
    bpx[1] = cmax; bpy[1] = 0;
    bpx[2] = 0; bpy[2]= rmax;
    bpy[3] = cmax; bpy[3] = rmax;
    printcmd = false;
}

N2Area::~N2Area()
{
    //dtor
}

int N2Area::cmdLength(string s, int pos) {
    int i = 1;
    if (parcmds.find(s[pos])!=string::npos) {
        return 2;
    } else if (partwocmds.find(s[pos])!=string::npos) {
        return 3;
    } else if (parthreecmds.find(s[pos])!=string::npos) {
        return 4;
    } else if (parsplitcmds.find(s[pos])!=string::npos) {
        char c1 = s[pos];
        char c2 = s[pos+1];
        i = cmdLen(c1,c2);
        return i;
    } else if (parn1cmds.find(s[pos])!=string::npos) {
        while (i+pos < (int) s.size()) {
           if (s[i+pos] == '.') {break;}
           i++;
        }
        return i;
    } else if (parn2cmds.find(s[pos])!=string::npos) {
        while (i+pos < (int) s.size()) {
           if (s[i+pos] == ';') {break;}
           i++;
        }
        return i;
    }

    return 1;

}

// to be used for split cmds only
int N2Area::cmdLen(char c1, char c2) {
    int result = 2;
    if (c1 == '$') {
        if ((c2 == '=') || (c2 == '<') || (c2 == '>')) {
            result = 3;
        }
    } else if (c1 == '?') {
        if (c2 == '%') {
            result = 3;
        }
    }

    return result;
}

long long int N2Area::readInt() {
    string help = "";
    long long int lhelp = 0;

    if (!fs1.is_open()) {
        fs1.open(finfile,ios::in);
        if (printcmd) {cout << "in:"  << finfile << endl;}
    }

    if (fs1.is_open()) {
        getline(fs1, help);
        if (fs1.eof()) {
            fs1.close();
            givr[3] = 1;
            if (printcmd) {cout << finfile << " read."  << endl;}
        } else {
            lhelp = stoll(help);
        }
    }

    return lhelp;
}

string N2Area::readStr() {
    string help = "";

    if (!fs1.is_open()) {
        fs1.open(finfile,ios::in);
        if (printcmd) {cout << "in:"  << finfile << endl;}
    }

    if (fs1.is_open()) {
        getline(fs1, help);

        if (fs1.eof()) {
            fs1.close();
            givr[3] = 1;
            if (printcmd) {cout << finfile << " eof."  << endl;}
        }
    } else {
        cout << "can't open " << finfile << endl;
    }

    return help;
}

void N2Area::moveCursor(char direction, int& cx, int& cy) {
    if (direction == 'c') {
        // no moves, position remains the same
    } else if (direction == 'd') {
       // down
       cy++;
       if (cy>rmax) {
           cy=0;
       }
    } else if (direction == 'l') {
       // left
       cx--;
       if (cx<0) {
            cx=cmax;
       }
    } else if (direction == 'r') {
      // right
      cx++;
      if (cx > cmax) {
            cx=0;
      };
    } else if (direction == 'u') {
      // up
      cy--;
      if (cy < 0) {
            cy=rmax;
      };
    } else if (direction == 'D') {
       // down
       cy=cy+3;
       if (cy>rmax) {
           cy=cy-rmax-1;
       }
    } else if (direction == 'L') {
       // left
       cx=cx-3;
       if (cx<0) {
            cx=cmax+1+cx;
       }
    } else if (direction == 'R') {
      // right
      cx=cx+3;
      if (cx > cmax) {
            cx=cx-cmax-1;
      };
    } else if (direction == 'U') {
      // up
      cy=cy-3;
      if (cy < 0) {
            cy=rmax+1+cy;
      };
    } else {
        cout << "direction " << direction << " unknown to function 'moveCursor'" << endl;
    }
}

void N2Area::closefs() {
    if (fs1.is_open()) {fs1.close();}
    if (fs2.is_open()) {fs2.close();}
}

string N2Area::getStrLrdir(char lrdir, int cx, int cy) {
    string result = "";
    int newcx;
    int newcy;
    if (b.isDigit(lrdir)) {
        result = gsvr[b.digitToInt(lrdir)];
    } else {
        newcx=cx;newcy=cy;
        moveCursor(lrdir, newcx, newcy);
        result = (area[newcx][newcy]).getStr();
    }

    return result;
}

long long int N2Area::getIntLrdir(char lrdir, int cx, int cy) {
    long long int result = 0;
    int newcx;
    int newcy;
    if (b.isDigit(lrdir)) {
        result = givr[b.digitToInt(lrdir)];
    } else {
        newcx=cx;newcy=cy;
        moveCursor(lrdir, newcx, newcy);
        result = (area[newcx][newcy]).getInt();
    }

    return result;
}


int N2Area::interpreter(string cmds, bool printcmd, bool printstack, bool iasa, string workDir, string basefile) {
    int retcode = 0;
    string commands = cmds;
    stack<N2Entry> cstack;
    int cxctr = cols / 2; // center
    int cyctr = rows / 2; // center
    int cx = cxctr; // currentx
    int cy = cyctr; // currenty
    int newcx = 0;
    int newcy = 0;
    int newcx2 = 0;
    int newcy2 = 0;

    int ge = -1; // last ge index
    int q1 = 0;
    int q2 = 0;

    string help = ""; // help
    string help2 = ""; // help
    string help3 = "";
    long long int lhelp = 0;
    long long int lhelp2 = 0;
    long long int fhelp = 0;

    char c1 = '?'; // curr cmd
    char c2 = '?'; // curr cmd par
    char c3 = '?'; // curr cmd par2
    char c4 = '?'; // curr cmd par3
    char c5 = '?'; // extra char for #
    bool flag = false; // multipurpose flag
    bool found;
    finfile = workDir + "\\" + basefile + ".nin";
    foufile = workDir + "\\" + basefile + ".nou";

    if (printcmd) {cout << "initial cmd size: "  << commands.size() << endl;}
    if (printcmd) {cout << "center set to: " << cxctr << ", " << cyctr << endl;}

    bool goon = true;
    bool doJump = false;
    int i = 0;
    int j = 0; //for jump
    N2Entry ne;
    N2Entry ne2;

    ne.init(commands, 0);
    cstack.push(ne);

    while (!cstack.empty()) {
      ne = cstack.top();
      i = ne.getPos();
      commands = ne.getCmd();
      if (printstack) {cout << "stacksize " << cstack.size() << " pos " << i << " cmdlen " << commands.size() << endl;}
      while (i < (int) commands.size()) {

        c1 = commands[i];

        // test option
        if (iasa) {
            if (c1 == 'i') {
                c1 = 'a';
                commands[i] = 'a';
            } else if (c1 == 'I') {
                c1 = 'A';
                commands[i] = 'A';
            }
        }

        c2 = '?';
        if (parcmds.find(c1)!=string::npos) {
            i++;
            if (i >= (int) commands.size()) {
                retcode = -1;
                goon = false;
                ;}
            c2 = commands[i];
            if (printcmd) {cout << "command: "  << c1 << c2 << ": ";}
        } else if (partwocmds.find(c1)!=string::npos) {
            i++;
            if (i >= (int) commands.size()) {retcode = -1; goon = false;}
            c2 = commands[i];
            i++;
            if (i >= (int) commands.size()) {retcode = -1; goon = false;}
            c3 = commands[i];
            if (printcmd) {cout << "command: "  << c1 << c2 << c3 << ": ";}
        } else if (parthreecmds.find(c1)!=string::npos) {
            i++;
            if (i >= (int) commands.size()) {retcode = -1; goon = false;}
            c2 = commands[i];
            i++;
            if (i >= (int) commands.size()) {retcode = -1; goon = false;}
            c3 = commands[i];
            i++;

            if (i >= (int) commands.size()) {retcode = -1; goon = false;}
            c4 = commands[i];
            if (printcmd) {cout << "command: "  << c1 << c2 << c3 << ": ";}
        }else if (parsplitcmds.find(c1)!=string::npos) {
            i++;
            if (i >= (int) commands.size()) {retcode = -1; goon = false;}
            c2 = commands[i];
            if (cmdLen(c1,c2) ==3){
                i++;
                if (i >= (int) commands.size()) {retcode = -1; goon = false;}
                c3 = commands[i];
                if (printcmd) {cout << "command: "  << c1 << c2 << c3 << ": ";}
            } else {
                if (printcmd) {cout << "command: "  << c1 << c2 << ": ";}
            }
        } else if (printcmd){
            cout << "command: "  << c1 << ": ";
        }


        switch(c1) {
        case 'a':
            lhelp = readInt();
            if (printcmd) {cout << " set int cell to: '" << lhelp << "' " << endl;}
            (area[cx][cy]).setInt(lhelp);
            break;
        case 'A':
            help = readStr();
            if (printcmd) {cout << " set str cell to: '" << help << "' " << endl;}
            (area[cx][cy]).setStr(help);
            break;
        case 'b':
            lhelp = b.digitToInt(c2);
            cx = bpx[lhelp];
            cy = bpy[lhelp];
            if (printcmd) {cout << cx << "," << cy << endl;}
            break;
        case 'B':
            lhelp = b.digitToInt(c2);
            bpx[lhelp] = cx;
            bpy[lhelp] = cy;
            break;
        case 'l':
        case 'L':
        case 'r':
        case 'R':
        case 'd':
        case 'D':
        case 'u':
        case 'U':
            // #corrected#
            moveCursor(c1, cx, cy);
            break;
        case 'c':
            q1 = 0;
            if (b.isDigit(commands[i+1])) {
                q1 = b.digitToInt(commands[i+1]);
                i++;
            }
            if (givr[q1]!=1) {
                i++;
                if (i >= (int) commands.size()) {
                    cout << "no argument for cmd c" << endl;
                    retcode = -1; goon = false;
                } else {
                    int p = cmdLength(commands, i);
                    i=i+p-1;
                    if (i >= (int) commands.size()) {
                        cout << "argument for cmd c too long " << p << endl;
                        retcode = -1;  goon = false;
                    }
                }
            }
            break;
        case 'C':
            q1 = 0;
            if (b.isDigit(commands[i+1])) {
                q1 = b.digitToInt(commands[i+1]);
                i++;
            }
            if (givr[q1]==1) {
                i++;
                if (i >= (int) commands.size()) {
                    cout << "no argument for cmd C" << endl;
                    retcode = -1; goon = false;
                }

                int p = cmdLength(commands, i);
                i=i+p-1;
                if (i >= (int) commands.size()) {
                    cout << "argument for cmd C too long " << p << endl;
                    retcode = -1; goon = false;
                }
            }

            break;
        case 'e':
            // #unchanged#
            help = (area[cx][cy]).getExec();

            cstack.pop();
            // replace top element
            ne.init(commands.substr(i+1, string::npos), 0);
            cstack.push(ne);
            if (printstack) {cout << "split stack " << cstack.size() << " at " << i+1 << endl;}
            ne2.init(help, 0);
            cstack.push(ne2);
            commands = help;
            if (printstack) {cout << "stacksize " << cstack.size() << " pos " << i << " cmdlen " << commands.size() << endl;}
            i=-1;
            break;
        case 'E':
            // #newly introduced#
            flag = false;
            i++;
            help = "";
            if (commands[i] == '.') {
                flag = true;
                i++;
            }
            while (i < (int) commands.size()) {
                c2 = commands[i];
                if (c2 == ';') {
                    break;
                } else {
                    help.push_back(c2);
                }
                i++;
            }

            if (flag) {
                ge++;
                ge = ge % 10;
                gecr[ge] = help;
                if (printcmd) {cout << "set gecr[" << ge << "]=" << gecr[ge] << endl;}
            } else {
                (area[cx][cy]).setExec(help);
                if (printcmd) {cout << "set exec: " << (area[cx][cy]).getExec() << endl;}
            }

            break;
        case 'f':
            found = false;
            if ((c3 == 'z') || (c3 == 'n')) {
                fhelp = 0;
            } else {
                fhelp = givr[b.digitToInt(c3)];
            }

            if (c2 == 'd') {
                found = findDown(fhelp, c3 != 'n', b.digitToInt(c4), cx, cy);
            } else {
                found = findRight(fhelp, c3 != 'n', b.digitToInt(c4), cx, cy);
            }
            if (printcmd) {
                if (found) {
                    cout << "(f" << c2 << c3 << c4 <<  ") found " << (area[cx][cy]).getInt() << " at pos " << cx << "," << cy << endl;
                } else {
                    cout << "(f" << c2 << c3 << c4 <<  ") not found" << endl;
                }
            }
            break;
        case 'g':
            // #corrected#
            if (b.isDigit(c2)) {
                (area[cx][cy]).setInt(givr[b.digitToInt(c2)]);
                if (printcmd) {cout << "c.i.v.r set to " << givr[b.digitToInt(c2)] << endl;}
            } else if (c2 == 'i') {
                (area[cx][cy]).setInt(cx);
            } else if (c2 == 'j') {
                (area[cx][cy]).setInt(cy);
            } else {
                b.checkDirection(c2);
                newcx=cx;newcy=cy;
                moveCursor(c2, newcx, newcy);
                (area[cx][cy]).setInt((area[newcx][newcy]).getInt());
                if (printcmd) {cout << cx << "," << cy  << " set to " << (area[cx][cy]).getInt() << endl;}
            }
            break;
        case 'G':
            // #corrected#
             if (b.isDigit(c2)) {
              (area[cx][cy]).setStr(gsvr[b.digitToInt(c2)]);
              if (printcmd) {cout << "c.s.v.r set to " << givr[b.digitToInt(c2)] << endl;}
            } else {
                b.checkDirection(c2);
                newcx=cx;newcy=cy;
                moveCursor(c2, newcx, newcy);
                (area[cx][cy]).setStr((area[newcx][newcy]).getStr());
                if (printcmd) {cout << cx << "," << cy  << " set to " << (area[cx][cy]).getStr() << endl;}
            }
            break;
        case 'h':
            // #newly introduced#
            lhelp = b.digitToInt(c2);
            help = gecr[lhelp];

            cstack.pop();
            // replace top element
            ne.init(commands.substr(i+1, string::npos), 0);
            cstack.push(ne);
            if (printstack) {cout << "split stack " << cstack.size() << " at " << i+1 << endl;}
            ne2.init(help, 0);
            cstack.push(ne2);
            commands = help;
            if (printstack) {cout << "stacksize " << cstack.size() << " pos " << i << " cmdlen " << commands.size() << endl;}
            i=-1;

            break;
        case 'i':
            // #newly introduced#
            cout << "?";
            getline(cin, help);
            if (b.isNumber(help)) {
                (area[cx][cy]).setInt(stoll(help));
            } else {
                (area[cx][cy]).setInt(0);
            }
            break;
        case 'I':
            // #newly introduced#
            cout << "??";
            getline(cin, help);
            (area[cx][cy]).setStr(help);
            break;
        case 'j':
            // #changed
            doJump = true;
            break;
        case 'J':
            // #newly introduced
            b.checkDirection(c2);
            b.checkDirection(c3);
            lhelp = (area[cx][cy]).getInt();
            if (lhelp <= 0) {
              moveCursor(c3, cx, cy);
            } else {
                j = 1;
                while (j <= lhelp) {
                    moveCursor(c2, cx, cy);
                    j++;
                }
            }
            break;
        case 'k':
            // #newly introduced#
            b.checkDirection(c2);
            (area[cx][cy]).subtract(1);
            if ((area[cx][cy]).getInt() <= 0) {
                moveCursor(c2, cx, cy);
            }
            break;
        case 'K':
            // #newly introduced#
            b.checkDirectionX(c2);
            b.checkDirectionX(c3);
            (area[cx][cy]).add(1);
            newcx=cx;newcy=cy;
            moveCursor(c2, newcx, newcy);
            lhelp = (area[newcx][newcy]).getInt();
            if ((area[cx][cy]).getInt() >= lhelp) {
                moveCursor(c3, cx, cy);
            }
            break;
        case 'm':
            // #newly introduced#
            if (b.isDigit(c2)) {
                lrdir = c2;
            } else {
                b.checkDirection(c2);
                lrdir = c2;
            }
            break;
        case 'n':
            // #newly introduced#
            // output newline
            cout << endl;
            break;
        case 'N':
            fs2.open(foufile,ios::app);
            if (!fs2.is_open()) {
                cout << "can't open file" << endl;
                retcode = -1; goon = false;
            } else {
                fs2 << endl;
                fs2.close();
                if (printcmd) {cout << "newline to:"  << foufile << endl;}
            }
            break;

        case 'o':
            // #corrected#
            // output current c.i.c.r
            cout << b.intToStr((area[cx][cy]).getInt());
            break;
        case 'O':
            // #newly introduced#
            // output current c.s.c.r
            cout << (area[cx][cy]).getStr();
            break;
        case 's':
            // #corrected#
            if (b.isDigit(c2)) {
              givr[b.digitToInt(c2)]=(area[cx][cy]).getInt();
              if (printcmd) {cout << "givr[" << b.digitToInt(c2) << "] set to " << givr[b.digitToInt(c2)] << endl;}
            } else if (c2 == 'c') {
                lhelp = (area[cx][cy]).getInt();
                (area[cx][cy]).setStr(to_string(lhelp));
            } else {
                b.checkDirection(c2);
                lhelp = (area[cx][cy]).getInt();
                newcx=cx;newcy=cy;
                moveCursor(c2, newcx, newcy);
                if (printcmd) {cout << newcx << "," << newcy << ": " << (area[newcx][newcy]).getInt();}
                (area[newcx][newcy]).setInt(lhelp);
                if (printcmd) {cout << " -> " << lhelp << endl;}
            }
            break;
        case 'S':
            // #newly introduced#
             if (b.isDigit(c2)) {
              gsvr[b.digitToInt(c2)]=(area[cx][cy]).getStr();
              if (printcmd) {cout << "gsvr[" << b.digitToInt(c2) << "] set to " << gsvr[b.digitToInt(c2)] << endl;}
            } else if (c2 == 'c') {
                help = (area[cx][cy]).getStr();
                (area[cx][cy]).setInt(stoll(help));
            } else {
                b.checkDirection(c2);
                help  = (area[cx][cy]).getStr();
                newcx=cx;newcy=cy;
                moveCursor(c2, newcx, newcy);
                if (printcmd) {cout << newcx << "," << newcy << ": " << (area[newcx][newcy]).getStr();}
                (area[newcx][newcy]).setStr(help);
                if (printcmd) {cout << " -> " << help << endl;}
            }
            break;

        case 't':
            // #newly introduced#
            if (printcmd) {cout << cx << "," << cy << ": " << (area[cx][cy]).getInt();}

            if (c1 == 't') {i++;}
            help = "";
            while (i < (int) commands.size()) {
                c2 = commands[i];
                if (c2 == '.') {
                    break;
                } else {
                    help.push_back(c2);
                }
                i++;
            }
            lhelp = stoll(help);
            (area[cx][cy]).setInt(lhelp);

            if (printcmd) {cout << " -> " << lhelp << endl;}

            break;


        case 'T':
            // #corrected#

            i++;
            help = "";
            while (i < (int) commands.size()) {
                c2 = commands[i];
                if (c2 == '.') {
                    break;
                } else {
                    help.push_back(c2);
                }
                i++;
            }

            (area[cx][cy]).setStr(help);

            if (printcmd) {
                cout << "set string: " << (area[cx][cy]).getStr() << endl;
            }

            break;
        case 'v':
            b.checkDirection(c2);
            newcx=cx;newcy=cy;
            moveCursor(c2, newcx, newcy);
            help = (area[newcx][newcy]).getExec();
            (area[cx][cy]).setExec(help);
            break;
        case 'w':
            fs2.open(foufile,ios::app);
            if (!fs2.is_open()) {
                cout << "can't open file" << endl;
                retcode = -1; goon = false;
            }
            fs2 << (area[cx][cy]).getInt();
            fs2.close();
            if (printcmd) {cout << "write int to:"  << foufile << endl;}
            break;
        case 'W':
            fs2.open(foufile,ios::app);
            if (!fs2.is_open()) {
                cout << "can't open file" << endl;
                retcode = -1; goon = false;
            }
            fs2 << (area[cx][cy]).getStr();
            fs2.close();
            if (printcmd) {cout << "write str to:"  << foufile << endl;}
            break;
        case 'x':
            // #newly introduced#
            b.checkDirectionX(c2);
            b.checkDirectionX(c3);
            newcx=cx;newcy=cy;
            moveCursor(c2, newcx, newcy);
            lhelp = (area[newcx][newcy]).getInt();
            newcx2=cx;newcy2=cy;
            moveCursor(c3, newcx2, newcy2);
            lhelp2 = (area[newcx2][newcy2]).getInt();
            (area[newcx2][newcy2]).setInt(lhelp);
            (area[newcx][newcy]).setInt(lhelp2);
            break;
        case 'X':
            // #newly introduced#
            b.checkDirectionX(c2);
            b.checkDirectionX(c3);
            newcx=cx;newcy=cy;
            moveCursor(c2, newcx, newcy);
            help = (area[newcx][newcy]).getStr();
            newcx2=cx;newcy2=cy;
            moveCursor(c3, newcx2, newcy2);
            help2 = (area[newcx2][newcy2]).getStr();
            (area[newcx2][newcy2]).setStr(help);
            (area[newcx][newcy]).setStr(help2);
            break;
        case 'Z':
            // #corrected#
            retcode = 1; goon = false;
            break;
        case '+':
            // #newly introduced#
            lhelp = getIntLrdir(lrdir, cx, cy);
            (area[cx][cy]).add(lhelp);
            if (printcmd) {cout << "new result is " << (area[cx][cy]).getInt() << endl;}
            break;
        case '-':
            // #newly introduced#
             lhelp = getIntLrdir(lrdir, cx, cy);
            (area[cx][cy]).subtract(lhelp);
            if (printcmd) {cout << "new result is " << (area[cx][cy]).getInt() << endl;}
            break;
        case '*':
            // #newly introduced#
            lhelp = getIntLrdir(lrdir, cx, cy);
            (area[cx][cy]).mult(lhelp);
            if (printcmd) {cout << "new result is " << (area[cx][cy]).getInt() << endl;}
            break;
        case '/':
            // #newly introduced#
            lhelp = getIntLrdir(lrdir, cx, cy);
            (area[cx][cy]).divide(lhelp);
            if (printcmd) {cout << "new result is " << (area[cx][cy]).getInt() << endl;}
            break;
        case '[':
            // #newly introduced#
            lhelp = getIntLrdir(lrdir, cx, cy);
            (area[cx][cy]).setMin(lhelp);
            break;
        case ']':
            // #newly introduced#
             lhelp = getIntLrdir(lrdir, cx, cy);
            (area[cx][cy]).setMax(lhelp);
            break;

        case '%':
            // #newly introduced#
            if (b.isDigit(c2)){
                q1 = b.digitToInt(c2);
                if (b.isDigit(lrdir)) {
                   givr[q1] = (area[cx][cy]).getInt() % givr[b.digitToInt(lrdir)];
                } else {
                    newcx=cx;newcy=cy;
                    moveCursor(lrdir, newcx, newcy);
                    lhelp = (area[newcx][newcy]).getInt();
                    givr[q1] = (area[cx][cy]).getInt() % lhelp;
                }
                if (printcmd) {cout << "set givr[ " << q1 << "]= " << givr[q1] << endl;}
            } else {
                cout << "argument " << c2 << " not recognized for command " << c1 << endl;
                retcode = -1; goon = false;
            }
            break;
        case '=':
            // #newly introduced#
            if (b.isDigit(c2)){
                q1 = b.digitToInt(c2);
                lhelp = getIntLrdir(lrdir, cx, cy);
                if ((area[cx][cy]).getInt() == lhelp) {
                   givr[q1] = 1;
                } else {
                   givr[q1] = 0;
                }
            } else {
                cout << "argument " << c2 << " not recognized for command " << c1 << endl;
                retcode = -1; goon = false;
            }
            break;
        case '<':
            // #newly introduced#
            if (b.isDigit(c2)){
                q1 = b.digitToInt(c2);
                lhelp = getIntLrdir(lrdir, cx, cy);
                if ((area[cx][cy]).getInt() < lhelp) {
                   givr[q1] = 1;
                } else {
                   givr[q1] = 0;
                }
            } else {
                cout << "argument " << c2 << " not recognized for command " << c1 << endl;
                retcode = -1; goon = false;
            }
            break;
        case '>':
            // #newly introduced#
            if (b.isDigit(c2)){
                q1 = b.digitToInt(c2);
                lhelp = getIntLrdir(lrdir, cx, cy);
                if ((area[cx][cy]).getInt() > lhelp) {
                   givr[q1] = 1;
                } else {
                   givr[q1] = 0;
                }
            } else {
                cout << "argument " << c2 << " not recognized for command " << c1 << endl;
                retcode = -1; goon = false;
            }
            break;
        case '!':
            // #newly introduced#
            if (b.isDigit(c2)){
                q1 = b.digitToInt(c2);

                if (givr[q1] == 0) {
                    givr[q1] = 1;
                } else {
                    givr[q1] = 0;
                }
            } else {
                cout << "argument " << c2 << " not recognized for command " << c1 << endl;
                retcode = -1; goon = false;
            }
            break;
        case '&':
            // #newly introduced#
            if (b.isDigit(c2) && b.isDigit(c3)){
                q1 = b.digitToInt(c2);
                q2 = b.digitToInt(c3);
                if ((givr[q1] != 0) && (givr[q2] != 0)) {
                    givr[q1] = 1;
                } else {
                    givr[q1] = 0;
                }
            } else {
                cout << "argument " << c2 << " or " << c3 <<" not recognized for command " << c1 << endl;
                retcode = -1; goon = false;
            }
            break;
        case '|':
             // #newly introduced#
             if (b.isDigit(c2) && b.isDigit(c3)){
                q1 = b.digitToInt(c2);
                q2 = b.digitToInt(c3);
                if ((givr[q1] != 0) || (givr[q2] != 0)) {
                    givr[q1] = 1;
                } else {
                    givr[q1] = 0;
                }
            } else {
                cout << "argument " << c2 << " or " << c3 <<" not recognized for command " << c1 << endl;
                retcode = -1; goon = false;
            }
            break;
        case '$':
            // #newly introduced#
            if (c2 == '<') {
                if (b.isDigit(c3)){
                    q1 = b.digitToInt(c3);
                    help = getStrLrdir(lrdir, cx, cy);
                    if ((area[cx][cy]).getStr() < help) {
                        givr[q1] = 1;
                    } else {
                        givr[q1] = 0;
                    }
                } else {
                    cout << "argument " << c3 << " not recognized for command " << c1 << c2 << endl;
                    retcode = -1; goon = false;
                }
            } else if (c2 == '>') {
                if (b.isDigit(c3)){
                    q1 = b.digitToInt(c3);

                    help = getStrLrdir(lrdir, cx, cy);
                    if ((area[cx][cy]).getStr() > help) {
                        givr[q1] = 1;
                    } else {
                        givr[q1] = 0;
                    }
                } else {
                    cout << "argument " << c3 <<" not recognized for command " << c1 << c2 << endl;
                    retcode = -1; goon = false;
                }
            } else if (c2 == '=') {
                if (b.isDigit(c3)){
                    q1 = b.digitToInt(c3);
                    help = getStrLrdir(lrdir, cx, cy);
                    if ((area[cx][cy]).getStr() == help) {
                        givr[q1] = 1;
                    } else {
                        givr[q1] = 0;
                    }
                } else {
                    cout << "argument " << c3 <<" not recognized for command " << c1 << c2 << endl;
                    retcode = -1; goon = false;
                }
            } else if (c2 == '+') {
                // append
                help = getStrLrdir(lrdir, cx, cy);
                (area[cx][cy]).appendStr(help);
                if (printcmd) {cout << cx << "," << cy << " set to " << (area[cx][cy]).getStr() << endl;}
            } else if (c2 == '&') {
                // prepend
                help = getStrLrdir(lrdir, cx, cy);
                (area[cx][cy]).prependStr(help);
                if (printcmd) {cout << cx << "," << cy << " set to " << (area[cx][cy]).getStr() << endl;}
            } else if (c2 == '.') {
                (area[cx][cy]).appendStr(".");
            } else if (c2 == 'l') {
                help = (area[cx][cy]).getStr();
                (area[cx][cy]).setInt(help.size());
            } else {
                cout << "argument " << c2 << " not recognized for command " << c1 << endl;
                retcode = -1; goon = false;
            }
            break;
        case '?':
            // #newly introduced#
            lhelp = (area[cx][cy]).getInt();
            if (c2 == '-') {
                (area[cx][cy]).setInt(-lhelp);
            } else if (c2 == 's') {
                if (lhelp > 0) {
                    (area[cx][cy]).setInt(1);
                } else if (lhelp < 0) {
                    (area[cx][cy]).setInt(-1);
                }
            } else if (c2 == '%') {
                if (b.isDigit(c3)){
                    q1 = b.digitToInt(c3);
                    lhelp = getIntLrdir(lrdir, cx, cy);
                    lhelp2 = (area[cx][cy]).getInt() % lhelp;
                    if (lhelp2 == 0) {
                        givr[q1] = 1;
                    } else if (lhelp < 0) {
                        givr[q2] = 0;
                    }
                } else {
                    cout << "argument " << c3 << " not recognized for command " << c1 << c2 << endl;
                    retcode = -1; goon = false;
                }

            } else {
                cout << "argument " << c2 << " not recognized for command " << c1 << endl;
                retcode = -1; goon = false;
            }
            break;
        case '#':
            // #newly introduced#
            if (c2 == 'i') {
                b.checkDirectionX(c3);
                help = (area[cx][cy]).getStr();
                if (help.size()==0) {
                    if (printcmd) {cout << "# cursor move" << endl;}
                    // note that head + tail must be explicitly set to empty
                    if (b.isDigit(lrdir)) {
                        gsvr[b.digitToInt(lrdir)] = "";
                    } else {
                        newcx=cx;newcy=cy;
                        moveCursor(lrdir, newcx, newcy);
                        (area[newcx][newcy]).setStr("");
                    }
                    (area[cx][cy]).setStr("");
                    moveCursor(c3, cx, cy);
                } else {
                    lhelp = (area[cx][cy]).getInt();
                    // head
                    help2 = b.head(help, lhelp);
                    if (b.isDigit(lrdir)) {
                        gsvr[b.digitToInt(lrdir)] = help2;
                    } else {
                        newcx=cx;newcy=cy;
                        moveCursor(lrdir, newcx, newcy);
                        (area[newcx][newcy]).setStr(help2);
                    }
                    // tail
                    string help3 = b.tail(help,lhelp);
                    (area[cx][cy]).setStr(help3);
                    if (printcmd) {cout << "# " << lhelp << " head/tail " << help2 << "/" << help3 << endl;}
                }
            } else {
                b.checkDirection(c2);
                b.checkDirection(c3);
                help = (area[cx][cy]).getStr();
                if (help.size()==0) {
                   if (printcmd) {cout << "# cursor move" << endl;}
                    // note that head + tail must be explicitly set to empty
                    (area[cx][cy]).setStr("");
                    if (b.isDigit(lrdir)) {
                        gsvr[b.digitToInt(lrdir)] = "";
                    } else {
                        newcx=cx;newcy=cy;
                        moveCursor(lrdir, newcx, newcy);
                    }
                    moveCursor(c3, cx, cy);
                } else {
                    newcx=cx;newcy=cy;
                    moveCursor(c2, newcx, newcy);
                    help2 = (area[newcx][newcy]).getStr();
                    c5 = help2[0];
                    // head + tail
                    tie(help2, help3) = b.splitFirst(help, c5);
                    if (b.isDigit(lrdir)) {
                        gsvr[b.digitToInt(lrdir)] = help2;
                    } else {
                        newcx=cx;newcy=cy;
                        moveCursor(lrdir, newcx, newcy);
                        (area[newcx][newcy]).setStr(help2);
                    }

                    (area[cx][cy]).setStr(help3);
                    if (printcmd) {cout << "# head/tail= " << help2 << "/" << help3 << endl;}
                }
            }
            break;
        default:
            cout << "command '" << c1 << "' not recognized" << endl;
            retcode = -1; goon = false;
            break;
        }
        i++;
        if (!goon) {break;}
        if (doJump){doJump = false; break;}
      }
      cstack.pop();
      if (!goon) {break;}
    }
    closefs();

    if (printcmd) {cout << "final cmd size: "  << to_string(commands.size()) << endl;}

    return retcode;
}

bool N2Area::findDown(int val, bool testequal, int flag, int& cx, int& cy) {
    int j = cy+1;
    int found = 0;

    while (j <= rmax ) {
        if (testequal && ((area[cx][j]).getInt() == val)) {
           found = 1;
           cy = j;
           break;
        } else  if (!testequal && ((area[cx][j]).getInt() != val)) {
            found = 1;
            cy = j;
            break;
        }
        j++;
    }
    givr[flag] = found;
    return found == 1;
}

bool N2Area::findRight(int val, bool testequal, int flag, int& cx, int& cy) {
    int i = cx+1;
    int found = 0;

    while (i <= cmax ) {
        if (testequal && ((area[i][cy]).getInt() == val)) {
           found = 1;
           cx = i;
           break;
        } else  if (!testequal && ((area[i][cy]).getInt() != val)) {
            found = 1;
            cx = i;
            break;
        }
        i++;
    }
    givr[flag] = found;
    return found == 1;
}
