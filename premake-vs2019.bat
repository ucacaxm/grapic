
:: del /S /Q build

.\script\premake5.exe vs2023

copy extern\visual2023\bin\*.dll bin


pause
