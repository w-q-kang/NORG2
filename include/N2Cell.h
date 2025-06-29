/*
Copyright 2025 W. Q. Kang

This file is part of the NORG2 Interpreter.

    The NORG2 Interpreter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The NORG2 Interpreter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the NORG Interpreter.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef N2CELL_H
#define N2CELL_H
#include <string>

using namespace std;

class N2Cell
{
    public:
        N2Cell();
        virtual ~N2Cell();
        // value
        long long int getInt();
        void setInt(long long int i);
        void add(long long int i);
        void subtract(long long int i);
        void mult(long long int i);
        bool divide(long long int i);
        void minus();
        void setMin(long long int i);
        void setMax(long long int i);
        // compare
        bool compEq(long long int i);
        bool compGe(long long int i);
        // string value
        //void clearReg();
        string getStr();
        void setStr(string s);
        void appendStr(string s);
        void prependStr(string s);
        // exec register
        string getExec();
        void setExec(string s);
    protected:

    private:
        long long int ival;
        string sval;
        string exec;
};

#endif // N2CELL_H
