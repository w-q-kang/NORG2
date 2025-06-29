if not exist "bin\" mkdir bin
if not exist "obj\" mkdir obj
if not exist "obj\src\" mkdir obj\src
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c main.cpp -o obj\main.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c src\N2Area.cpp -o obj\src\N2Area.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c src\N2Basics.cpp -o obj\src\N2Basics.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c src\N2Cell.cpp -o obj\src\N2Cell.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c src\N2Entry.cpp -o obj\src\N2Entry.o
g++.exe  -o bin\NORG2.exe obj\main.o obj\src\N2Area.o obj\src\N2Basics.o obj\src\N2Cell.o   obj\src\N2Entry.o -Wl,--stack,25000000 
