@echo off

SET Location=%cd:~0,-5%

SUBST /D T:
SUBST T: %Location%

pushd T:\

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
start gvim.exe
