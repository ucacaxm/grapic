
del /S /Q build

.\script\premake4.exe --os=windows gmake
.\script\premake4.exe --os=windows codeblocks

.\script\premake4.exe --os=linux gmake
.\script\premake4.exe --os=linux codeblocks

.\script\premake4.exe --os=macosx gmake
.\script\premake4.exe --os=macosx xcode4

pause
