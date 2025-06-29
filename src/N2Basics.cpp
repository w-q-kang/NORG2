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
#include "N2Basics.h"
#include <iostream>

using namespace std;

N2Basics::N2Basics()
{
    //ctor
}

N2Basics::~N2Basics()
{
    //dtor
}

string N2Basics::getDir(string s) {
    int i;
    if (s.find_last_of('\\') != string::npos) {
       i = s.find_last_of('\\');
       return s.substr(0,i);
    } else if (s.find_last_of('/') != string::npos) {
       i = s.find_last_of('/');
       return s.substr(0,i);
    } else {
        return s;
    }
}

string N2Basics::getFileOnly(string s) {
    int i = 0;
    if (s.find_last_of('\\') != string::npos) {
       i = s.find_last_of('\\');
       return s.substr(i+1);
    } else if (s.find_last_of('/') != string::npos) {
       i = s.find_last_of('/');
       return s.substr(i+1);
    } else {
       return s;
    }
}

string N2Basics::getBase(string s) {
    if (s.find_last_of('.') != string::npos) {
       int i = s.find_last_of('.');
       return s.substr(0,i);
    } else {
        return s;
    }
}

string N2Basics::readFile(string name) {
   fstream fs;
   string content = "";
   fs.open(name,ios::in); //open a file to perform read operation using file object
   if (fs.is_open()){   //checking whether the file is open
      string tp;
      while(getline(fs, tp)){ //read data from file object and put it into string.
         if ((tp.size() > 0)) {
            content += tp;
         }
      }
      fs.close(); //close the file object.
   }
   return content;
}

string N2Basics::getAFile(string dir) {
    string result = "";
    for (const auto & file : filesystem::directory_iterator(dir)) {
        string path = file.path().string();
        int i = path.size()-4;
        string ext = path.substr(i);
        if ((ext == ".nin") || (ext == ".NIN")) {result = path; break;}
     }
     return result;
}

void N2Basics::deleteFile(string name) {
    int v = remove(name.c_str()); // 0=ok
    if (v != 0) {exit(EXIT_FAILURE);}
}

int N2Basics::digitToInt(char c) {
    return xToInt(c, "0123456789");
}

int N2Basics::xToInt(char c, string s) {
    size_t found = s.find(c);
    if (found!=string::npos) {
        return (int) found;
    } else {
        return -1;
    }
}

string N2Basics::intToStr(long long int i) {
    return to_string(i);
}

void N2Basics::checkDirection(char c) {
    string s = "lrudLRUD";
    size_t found = s.find(c);
    if (found==string::npos) {
        cout << "wrong direction: " << c << endl;
        exit(EXIT_FAILURE);
    }
}

void N2Basics::checkDirectionX(char c) {
    string s = "clrudLRUD";
    size_t found = s.find(c);
    if (found==string::npos) {
        cout << "wrong direction: " << c << endl;
        exit(EXIT_FAILURE);
    }
}

bool N2Basics::isDigit(char c) {
    return (c=='0') || (c=='1') || (c=='2') || (c=='3') || (c=='4') || (c=='5') || (c=='6') || (c=='7') || (c=='8') || (c=='9');
}

bool N2Basics::isNumber(string s) {
    for (int i = 0; i < (int) s.size(); ++i) {
        if (!isDigit(s[i])) {return false;}
    }
    return true;
}

string N2Basics::getXInt(string s, int pos) {
    string result = "";
    int i = 0;
    bool foundx = false;
    while (i+pos < (int) s.size()) {
        if (isDigit(s[i+pos])) {
            result += s[i+pos];
        } else if ((s[i+pos] == 'x') && ! foundx){
            result += s[i+pos];
            foundx = true;
        } else {
            break;
        }
        i++;
    }

    return result;
}

tuple<string, string> N2Basics::splitFirst(string name, char c1) {
    tuple<string, string> result;
    auto pos = name.find(c1);
    if (pos == string::npos) {
        result = make_tuple(name, "");
    } else if (pos == 0) {
        result = make_tuple("", name);
    } else {
        result = make_tuple(name.substr(0,pos), name.substr(pos+1));
    }

    return result;
}

string N2Basics::head(string text, int len) {
    if (len >= (int) text.size()) {
        return text;
    } else {
        return text.substr(0,len);
    }
}

string N2Basics::tail(string text, int pos) {
    if (pos >= (int) text.size()) {
        return "";
    } else {
        return text.substr(pos);
    }
}
