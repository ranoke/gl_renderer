@echo off
pushd %~dp0\..\
call proc\premake5.exe clean
popd
PAUSE