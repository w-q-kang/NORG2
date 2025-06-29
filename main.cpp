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
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include "N2Basics.h"
#include "N2Cell.h"
#include "N2Area.h"

using namespace std;


int main(int argc, char *argv[])
{
    N2Basics b;
    bool printcmd = false;
    bool printstack = false;
    bool iasa = false;
    string workDirectory = "";
    string filename = "";
    string fileonly = "";
    string basefile = "";
    int retcode = 0;

    for (int i = 1; i < argc; ++i) {
        string sargv = argv[i];
        if (sargv == "-p") {
            printcmd = true;
        } else if (sargv == "-s") {
            printstack = true;
        } else if (sargv == "-a") {
            iasa = true;
        }else {
            filename = sargv;
            workDirectory = b.getDir(sargv);
            fileonly = b.getFileOnly(sargv);
            basefile = b.getBase(fileonly);

        }
    }
    if (printcmd) {cout << "workDir: " << workDirectory << endl;}
    if (printcmd) {cout << "file only: " << fileonly << endl;}
    if (printcmd) {cout << "base file: " << basefile << endl;}

    string content = b.readFile(filename);
    string dimension = b.getXInt(content, 0);
    int length = dimension.size();

    string dim1;
    string dim2;
    tie(dim1,dim2) = b.splitFirst(dimension, 'x');
    if (printcmd) {cout << "dimension= " << dim1 << "x" << dim2 << endl;}
    int icols = stoi(dim1);
    int irows = icols;

    if (dim1.size() == 0) {
        cout << "dimension not set" << endl;
        exit(EXIT_FAILURE);
    }

    if (dim2.length() > 0) {
        irows = stoi(dim2);
    }
    content.erase(0,length+1);
    if (printcmd) {cout << "dim: " << icols << "x" << irows << endl;}
    N2Area a = N2Area(icols, irows);

    retcode = a.interpreter(content, printcmd, printstack, iasa, workDirectory, basefile);

    if (retcode == -1) {
        exit(EXIT_FAILURE);
    } else if (retcode == 1) {
        exit(EXIT_SUCCESS);
    }

    return 0;
}


