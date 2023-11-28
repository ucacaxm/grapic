
:: del /S /Q build

cd ..
.\script\premake5.exe vs2023

copy extern\visual2023\bin\*.dll bin


pause
