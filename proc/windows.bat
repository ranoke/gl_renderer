@echo off
pushd %~dp0\..\
call proc\premake5.exe vs2019
popd
PAUSE