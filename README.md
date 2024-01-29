# GrAPiC 

http://licence-info.univ-lyon1.fr/grapic


## Introduction

GrAPiC is a small, easy-to-use framework to add graphical functionalities to your C/C++ code. 
Its main goal is to provide easy graphical functions to be used in introduction courses of algorithm and programming in C/C++. 
It is based on SDL2 but to use it you need to know only few very simples functions. 
The Grapic archive comes with the needed SDL part, you do not have to install SDL. Just unzip and enjoy ! 



## How to start using it

[See here](https://perso.liris.cnrs.fr/alexandre.meyer/grapic/html/index.html#start])


### Linux

Install: - g++, doxygen, zip, lib-sdl2-dev lib-sdl2 sd2_imag sdl2_ttf

1. `sudo apt install g++ premake5 zip doxygen libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2_ttf-dev`
2. `make premake`
3. `make`


### Windows

The easiest

1. Install CodeBlocks 20
2. run the script `run-premake-cb20.bat` or `run-lifami-cb20.bat`
3. open build/windows-codeblocks/grapic.workspace with Codeblocks



## Change doc of the web page

`make web`: to create doc and sync everything to the webpage (need passwd).
