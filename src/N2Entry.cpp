#include "N2Entry.h"

using namespace std;

N2Entry::N2Entry()
{
    //ctor
}

N2Entry::~N2Entry()
{
    //dtor
}

void N2Entry::init(string cmd, int ppos) {
    commands = cmd;
    pos = ppos;

}
string N2Entry::getCmd() {
    return commands;
}

void N2Entry::setPos(int p) {
    pos = p;
}

int N2Entry::getPos() {
    return pos;
}
