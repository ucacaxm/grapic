
:: del /S /Q build

cd ..
.\script\premake5.exe vs2022

mkdir bin
copy extern\visual2022\bin\*.dll bin


pause
