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

:main
ECHO Compile main
"..\..\C++\Dev-Cpp\MinGW64\bin\g++.exe" -c "main.cpp" -o main.o -std=c++11 -m32 -I..\..\C++\Dev-Cpp\MinGW64\include" -I"..\..\C++\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\C++\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\C++\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\C++\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 "..\..\C++\Dev-Cpp\lib\libwsock32.a"  

:: Make executeable
ECHO Create executeable
"..\..\C++\Dev-Cpp\MinGW64\bin\g++.exe" main.o %olib% %ofilepaths% -o "bin\SomeProg.exe" -std=c++11 -m32 -I"..\..\C++\Dev-Cpp\MinGW64\include" -I"..\..\C++\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\C++\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\C++\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\C++\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 "..\..\C++\Dev-Cpp\lib\libwsock32.a" 
ECHO Ready
GOTO end

:all
for /f %%f in ('dir /b src') do (
echo Compile %%f
"..\..\C++\Dev-Cpp\MinGW64\bin\g++.exe" -c "src\%%f\%%f.cpp" -fpermissive -o "src\%%f\%%f.o" -std=c++11 -m32 -I..\..\C++\Dev-Cpp\MinGW64\include" -I"..\..\C++\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\C++\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\C++\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\C++\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 
)
GOTO end

:lib
for /f %%f in ('dir /b lib') do (
echo Compile %%f
"..\..\C++\Dev-Cpp\MinGW64\bin\g++.exe" -c "lib\%%f\%%f.cpp" -fpermissive -o "lib\%%f\%%f.o" -std=c++11 -m32 -I..\..\C++\Dev-Cpp\MinGW64\include" -I"..\..\C++\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\C++\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\C++\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\C++\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 
)
GOTO end

:end