@echo off
echo -- Building library Zipper
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
echo -- Building UpdateManager
g++ -o UpdateManager.exe UpdateManager.cpp .\resource.res -I ..\..\include -I .\include\ -L ..\..\lib\  -L .\lib\ -static -static-libgcc -static-libstdc++ -lurlmon -lcurl -lsqlite3 -lws2_32 -lwinmm -ljsoncpp -lZipper -lz -std=c++20 -w
echo -- Build of UpdateManager finished