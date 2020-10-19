
del /S /Q build

.\script\premake4.exe --os=windows --file=premake4.lua --cb-version=cb17 codeblocks 

copy extern\mingw-cb20\bin\*.dll bin

:: .\script\premake4.exe --os=linux gmake
:: .\script\premake4.exe --os=linux codeblocks
:: .\script\premake4.exe --os=macosx gmake
:: .\script\premake4.exe --os=macosx xcode4

pause
