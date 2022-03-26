:: MakeFile
ECHO OFF

:: Get .o filepaths lib
set olib=
for /f %%f in ('dir /b lib') do (
call set "olib=%%olib%% "lib\%%f\%%f.o""
)

IF %1==main GOTO main
IF %1==all GOTO all
IF %1==lib GOTO lib
IF %1==s GOTO single

GOTO end

:main
ECHO Compile main
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" -c "main.cpp" -o main.o -std=c++11 -m32 -I"..\..\Dev-Cpp\MinGW64\include" -I"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" 
::-L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 "..\..\Dev-Cpp\lib\libwsock32.a" -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 "..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib\libbgi.a" 

:: Make executeable
ECHO Create executeable
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" main.o %olib% -o "bin\SomeProg.exe" -std=c++11 -m32 -I"..\..\Dev-Cpp\MinGW64\include" -I"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" -L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 "..\..\Dev-Cpp\lib\libwsock32.a" -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 "..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib\libbgi.a" 
ECHO Ready
GOTO end

:lib
for /f %%f in ('dir /b lib') do (
echo Compile %%f
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" -c "lib\%%f\%%f.cpp" -fpermissive -o "lib\%%f\%%f.o" -std=c++11 -m32 -I"..\..\Dev-Cpp\MinGW64\include" -I"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" 
::-L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 
)
GOTO end

:single
echo Compile %2
"..\..\Dev-Cpp\MinGW64\bin\g++.exe" -c "lib\%2\%2.cpp" -fpermissive -o "lib\%2\%2.o" -std=c++11 -m32 -I"..\..\Dev-Cpp\MinGW64\include" -I"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include" -I"..\..\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include\C++" 
::-L"..\..\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib32" -static-libgcc -m32 

:end