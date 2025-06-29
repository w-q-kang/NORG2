# NORG2

## Introduction
Two years after having introduced NORG I feel that it is time for a revision (or reinvention) of a small language and I move on to NORG2. Note that NORG2 draws some ideas and methods from the original NORG but is in no way compatible with it so you will have to rewrite NORG programs in order to execute them with NORG2. See the documentation in pdf format. 

While - in a certain sense - NORG2 is less esoteric than the original NORG language it still tries to be concise. It wasn't designed with having code golfing in mind but moves a little bit in that direction.

Be aware that (contrary to NORG) NORG2 programs live in a plane (rectangular area).

I provide an interpreter written in C++ complete with source code.


## Build

You should compile the source code with C++11 or later (I prefer C++17). To avoid crashes on programs, increase the stack size with the corresponding linker option, e.g. `-Wl,--stack,25000000` (g++) or `-Wl,-stack_size -Wl,0x1000000` (clang++, hexadecimal). 

### Windows
Run the build script:

```batch
make.bat
```
After successfully compiling you should be all set. Try executing a sample program like so:

```batch
./bin/NORG2.exe samples/hello.norg2
```


### macOS/Linux
This requires `clang++` to be installed. Run the build script:

```shell
./make.sh
```

After successfully compiling you should be all set. Try executing a sample program like so:

```shell
bin/norg2 samples/hello.norg
```


## Motivation
Though in principle it is possible to write simple programs using NORG there are many obstacles - besides legibility - and restrictions that make it very difficult if you want to go beyond simple programming examples. It takes a long time until simple programs are going to work...

NORG2 preserves some basic ideas of the NORG language but tries to be easier to use and comprehend (Though commands are usually still one-letter-words, I tried to choose them in a way that may be easier to memorize). In the process of (re)designing it I slowly moved away from some of the restrictions and 'principles' of NORG in order to get a better usable language that still (partly) retains the look-and-feel of NORG. The version provided now may be changed and/or extended in the future - depending on the amount of usage that I am aware of.
