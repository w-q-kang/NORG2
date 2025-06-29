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
#ifndef N2BASICS_H
#define N2BASICS_H
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

class N2Basics
{
    public:
        N2Basics();
        virtual ~N2Basics();
        string getDir(string s);
        string getFileOnly(string s);
        string getBase(string s);
        string readFile(string name);
        string getAFile(string dir);
        void deleteFile(string name);
        int charToInt(char c);
        int digitToInt(char c);
        int xToInt(char c, string s);
        string intToStr(long long int i);
        void checkDirection(char c);
        void checkDirectionX(char c);
        bool isDigit(char c);
        bool isNumber(string s);
        string getXInt(string s, int pos);
        tuple<string, string> splitFirst(string name, char c1);
        string head(string text, int len);
        string tail(string text, int pos);
    protected:
     private:

};

#endif // N2BASICS_H
