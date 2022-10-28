
@echo off

SET CompilerFlags=/nologo /Z7 /Od /Ob0 /GS- /Oi /fp:fast /FC -wd4311 -wd4312
REM SET CompilerFlags=/nologo /O2 /Oi /fp:fast /FC /GS- -wd4311 -wd4312
SET LinkerFlags=/incremental:no

IF NOT EXIST build mkdir build

pushd build

cl.exe %CompilerFlags% ../code/main.cpp /link %LinkerFlags%

popd
