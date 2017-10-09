Ce que j'ai testé avec cmake :


* sous windows
cd .../Grapic
mkdir build-cmake
cmake -G "MSYS Makefile" ../scripts/cmake
make

et les .exe se trouvent dans le répertoire build-cmake

* Sous windows avec CLion
il faut changer la toolchain pour lui indiquer de prendre la version de g++ de codeblocks dans les paramètres.
