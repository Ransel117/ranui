@echo off

IF EXIST "main.exe" del main.exe
IF EXIST "main.pdb" del main.pdb
IF EXIST "main.ilk" del main.ilk

SET CC=clang
SET CFLAGS=-Wall -Wpedantic -ggdb -std=c11 -Wl,-nodefaultlib:libcmt
SET CDEFINES=
SET CINCLUDES=
SET CFILES=main.c
SET CLIBS=-ldbghelp -lonecore -lsynchronization -lmsvcrt -luser32

SET EXEC=main.exe

echo "--------COMPILING--------"
echo "%CC% %CFLAGS% %CDEFINES% %CINCLUDES% %CFILES% -o %EXEC% %CLIBS%"
%CC% %CFLAGS% %CDEFINES% %CINCLUDES% %CFILES% -o %EXEC% %CLIBS%

echo "---------RUNNING---------"
echo ".\%EXEC%"
.\%EXEC%
