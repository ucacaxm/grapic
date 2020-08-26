# GrAPiC 

http://licence-info.univ-lyon1.fr/grapic


## Introduction

GrAPiC is a small, easy-to-use framework to add graphical functionalities to your C/C++ code. 
Its main goal is to provide easy graphical functions to be used in introduction courses of algorithm and programming in C/C++. 
It is based on SDL2 but to use it you need to know only few very simples functions. 
The Grapic archive comes with the needed SDL part, you do not have to install SDL. Just unzip and enjoy ! 



## Note for developpers and to produce the webpages
Compiling from the github (which is not the standard way)

* On Linux you need to install:
- g++
- doxygen
- zip
- lib-sdl2-dev lib-sdl2 sd2_image... sdl2_ttf...

1. sudo apt install g++ premake4 zip doxygen libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev
2. make premake
3. make
(4. make zip or make web): to create the zip files in doc/download or to sync everything to the webpage (need passwd).



* On windows
1. Copy extern/mingw-cb20/bin/*.dll on bin/*
2. run premake.bat
3. open build/windows/grapic.workspace

