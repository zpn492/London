:: MakeFile
ECHO OFF

:: Get .o filepaths src
set ofilepaths=
for /f %%f in ('dir /b src') do (
call set "ofilepaths=%%ofilepaths%% "src\%%f\%%f.o""
)

:: Get .o filepaths lib
set olib=
for /f %%f in ('dir /b lib') do (
call set "olib=%%olib%% "lib\%%f\%%f.o""
)

IF %1==main GOTO main
IF %1==all GOTO all
IF %1==lib GOTO lib
IF %1==w GOTO worker
IF %1==m GOTO market

:main
ECHO Compile main
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" -c "mainWin.cpp" -o main.o -std=c++11 -m32 -I..\..\Dev-Cpp\MinGW64\include" -I"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 "..\..\Dev-Cpp\lib\libwsock32.a" "..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32\libgdi32.a" 

:: Make executeable
ECHO Create executeable
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" main.o %olib% %ofilepaths% -o "bin\SomeProg.exe" -std=c++11  -m32 -I"..\..\Dev-Cpp\MinGW64\include" -I"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 "..\..\Dev-Cpp\lib\libwsock32.a" "..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32\libgdi32.a"
ECHO Ready
GOTO end

:all
for /f %%f in ('dir /b src') do (
echo Compile %%f
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" -c "src\%%f\%%f.cpp" -fpermissive -o "src\%%f\%%f.o" -std=c++11 -m32 -I..\..\C++\Dev-Cpp\MinGW64\include" -I"..\..\C++\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 
)
GOTO end

:lib
for /f %%f in ('dir /b lib') do (
echo Compile %%f
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" -c "lib\%%f\%%f.cpp" -o "lib\%%f\%%f.o" -std=c++11 -m32 -I..\..\Dev-Cpp\MinGW64\include" -I"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32  "..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32\libgdi32.a"
)
GOTO end

:worker
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" -c "lib\worker\worker.cpp" -o "lib\worker\worker.o" -std=c++11 -m32 -I..\..\Dev-Cpp\MinGW64\include" -I"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32  "..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32\libgdi32.a"
GOTO end

:market
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" -c "lib\market\market.cpp" -o "lib\market\market.o" -std=c++11 -m32 -I..\..\Dev-Cpp\MinGW64\include" -I"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32  "..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32\libgdi32.a"
GOTO end

:end