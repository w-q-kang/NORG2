if [ ! -d "bin" ]; then mkdir "bin"; fi
if [ ! -d "obj/src" ]; then mkdir -p "obj/src"; fi
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c main.cpp -o obj/main.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c src/N2Area.cpp -o obj/src/N2Area.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c src/N2Basics.cpp -o obj/src/N2Basics.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c src/N2Cell.cpp -o obj/src/N2Cell.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c src/N2Entry.cpp -o obj/src/N2Entry.o
clang++ -o bin/norg obj/main.o obj/src/N2Area.o  obj/src/N2Basics.o obj/src/N2Cell.o  obj/src/N2Entry.o -Wl,-stack_size -Wl,0x1000000 
