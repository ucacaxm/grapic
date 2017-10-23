
# Grapic et CLion/CMake

##Windows
Voici ce que j'ai testé avec Clion 2017.2.3 et qui marche (Windows).

1. Configurez CLion pour qu'il utilise le g++ de codeblocks (lui donner le chemin vers c:\Program Files (x86)\Codeblocks\Mingw
Il faut changer la toolchain pour lui indiquer de prendre la version de g++ de codeblocks dans les paramètres.

2. Ouvrir le projet Grapic/script/cmake/CMakeFiles.txt avec CLion
3. Copier tous les fichiers Grapic/bin/*.dll dans  Grapic\script\cmake\cmake-build-debug
Le répertoire cmake-build-debug est crée par CLion/Cmake


# Linux

cd .......Grapic/script/cmake
mkdir build-cmake
cmake -G "gmake" ../scripts/cmake
make
