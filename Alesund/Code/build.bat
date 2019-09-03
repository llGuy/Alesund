@echo off

set CC=cl
set DB=devenv
set CFLAGS=-Zi /EHsc /std:c++latest /DEBUG:FULL

set DEF=/DGLM_ENABLE_EXPERIMENTAL /DUNITY_BUILD /DSTB_IMAGE_IMPLEMENTATION /D_MBCS

set GLM_INC_DIR=/I C:/dependencies/

set EXE_NAME=Alesund.exe

set LIBS=winmm.lib user32.lib User32.lib Gdi32.lib Shell32.lib kernel32.lib gdi32.lib msvcrt.lib msvcmrt.lib opengl32.lib

set BIN=../x64/Debug/

If "%1" == "compile" goto :compile
If "%1" == "debug" goto :debug
If "%1" == "clean" goto :clean
If "%1" == "run" goto :run
If "%1" == "help" goto :help

set PROJECT_DIR=C:/gamedev/Alesund/
set SRC=..\..\Code\win32_core.cpp
set INC=%GLM_INC_DIR% %PROJECT_DIR%Alesund/Dependencies/

:compile
pushd ..\x64\Debug

%CC% %CFLAGS% %DEF% /I ..\..\Dependencies\ ..\..\Code\win32_core.cpp /Fe%EXE_NAME% %LIBS%

popd
goto :eof

:debug
%DB% %BIN%
goto :eof

:clean
rm *.exe *.obj *.ilk *.pdb
goto :eof

:run
%BIN%
goto :eof

:help
echo To build application, enter into command line: build.bat compile
echo To debug application, enter into command line: build.bat debug
echo To run application, enter into command line: build.bat run
echo To clean application, enter into command line: build.bat clean

:eof

