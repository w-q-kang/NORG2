#ifndef N2ENTRY_H
#define N2ENTRY_H
#include <string>

using namespace std;

class N2Entry
{
    public:
        N2Entry();
        virtual ~N2Entry();
        void init(string cmd, int ppos);
        string getCmd();
        void setPos(int p);
        int getPos();
    protected:

    private:
        string commands;
        int pos;
};

#endif // N2ENTRY_H
