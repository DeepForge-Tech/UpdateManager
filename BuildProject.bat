@echo off
if exist .\build (
    rem file exists
) else (
    mkdir build
)
echo -- Building library Zipper
cd src
if exist .\src\zipper (
    rmdir .\src\zipper /s /q
)
git clone --recursive https://github.com/sebastiandev/zipper.git
mkdir lib
cd zipper
mkdir build
cmake  .
cmake --build .
rename "libstaticZipper.a" "libZipper.a"
Xcopy *.a  ..\lib\  /Y
cd ..
rmdir .\zipper /s /q
echo - Zipper successfully builded
cd ..
echo -- Building UpdateManager
g++ -o .\build\UpdateManager.exe .\src\UpdateManager.cpp .\src\resource.res -I ..\..\include -I .\src\include\ -L ..\..\lib\  -L .\src\lib\ -static -static-libgcc -static-libstdc++ -lurlmon -lcurl -lsqlite3 -lws2_32 -lwinmm -ljsoncpp -lZipper -lz -std=c++20 -w
echo -- Build of UpdateManager finished