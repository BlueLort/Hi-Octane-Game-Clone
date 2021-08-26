@echo off
pushd %~dp0\..\
call Scripts\premake5.exe vs2019
xcopy GXSystem\Deps\lib\SDL2-2.0.16\Windows\x64\SDL2.dll bin\windows-x86_64-Debug\GXSystem\ /i
xcopy GXSystem\Deps\lib\SDL2-2.0.16\Windows\x64\SDL2.dll bin\windows-x86_64-Release\GXSystem\ /i
popd
PAUSE