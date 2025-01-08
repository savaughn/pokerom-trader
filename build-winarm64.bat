rm -r build/
cmake -B build -G "Visual Studio 17 2022" -A ARM64 .
cmake --build build