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
#include "N2Cell.h"

using namespace std;

N2Cell::N2Cell()
{
    //ctor
    ival = 0;
    sval = "";
    exec = "";
}

N2Cell::~N2Cell()
{
    //dtor
}

long long int N2Cell::getInt() {
    return ival;
}

void N2Cell::setInt(long long int i) {
     ival = i;
}

void N2Cell::setMin(long long int i) {
     if (i < ival) {ival = i;}
}

void N2Cell::setMax(long long int i) {
     if (i > ival) {ival = i;}
}


void N2Cell::add(long long int i) {
     ival += i;
}

void N2Cell::subtract(long long int i) {
     ival -= i;
}

void N2Cell::minus() {
     ival = -ival;
}

void N2Cell::mult(long long int i) {
     ival *= i;
}

bool N2Cell::divide(long long int i) {
     if (i==0) {return true;}
     ival /= i;
     return false;
}


bool N2Cell::compEq(long long int i) {
    int comp = 0;
    if (ival==i) {
        comp = 1;
    } else {
        comp = -1;
    }
    return comp==1;
}

bool N2Cell::compGe(long long int i) {
    int comp = 0;
    if (ival>=i) {
       comp = 1;
    } else {
       comp = -1;
    }
    return comp==1;
}

string N2Cell::getStr() {
    return sval;
}

void N2Cell::setStr(string s) {
    sval=s;
}

void N2Cell::appendStr(string s) {
    sval=sval+s;
}

void N2Cell::prependStr(string s) {
    sval=s+sval;
}

string N2Cell::getExec() {
    return exec;
}

void N2Cell::setExec(string s) {
    exec=s;
}
