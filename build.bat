rmdir /S /Q build_X64
rmdir /S /Q build_X86
rmdir /S /Q output

mkdir build_X64
cd build_X64
del CMakeCache.txt
cmake .. -G "Visual Studio 16 2019" -A x64 -D_PLATFORM=x64
cmake --build . --config Debug
cmake --build . --config Release
cd ..

mkdir output\x64\Debug
copy .\build_x64\Debug\*  .\output\x64\Debug\
mkdir output\x64\Release
copy .\build_x64\Release\*  .\output\x64\Release\


mkdir build_X86
cd build_X86
del CMakeCache.txt
cmake .. -G "Visual Studio 16 2019" -A Win32  -D_PLATFORM=x86
cmake --build . --config Debug
cmake --build . --config Release
cd ..

mkdir output\x86\Debug
copy .\build_x86\Debug\*  .\output\x86\Debug\
mkdir output\x86\Release
copy .\build_x86\Release\*  .\output\x86\Release\

pause
