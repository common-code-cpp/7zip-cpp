@echo off
if not exist ".\build" (
    mkdir .\build
)
cd .\build

::arm64
for /r %%d in (*) do @if /i not "%%~xd"==".lib" del "%%d"
cmake -G "Visual Studio 17 2022" -A ARM64 ../
cmake --build ../build --config Release

for /r %%d in (*) do @if /i not "%%~xd"==".lib" del "%%d"
cmake -G "Visual Studio 17 2022" -A ARM64 ../
cmake --build ../build --config Debug

::x64
for /r %%d in (*) do @if /i not "%%~xd"==".lib" del "%%d"
cmake -G "Visual Studio 17 2022" -A x64 ../
cmake --build ../build --config Release

for /r %%d in (*) do @if /i not "%%~xd"==".lib" del "%%d"
cmake -G "Visual Studio 17 2022" -A x64 ../
cmake --build ../build --config Debug

cd ..
