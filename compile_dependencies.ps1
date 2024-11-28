if (test-path "build") { 
	remove-item -r build
}
mkdir -p build/dependencies

$current_loc = $(pwd)

Invoke-WebRequest https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip -OutFile build/dependencies/gtest.zip

Expand-Archive build/dependencies/gtest.zip build/dependencies/gtest

set-location .\build\dependencies\gtest\
set-location $(Get-ChildItem)

cmake -S . -B build -G "MinGW Makefiles" -D CMAKE_CXX_COMPILER=g++ -D CMAKE_C_COMPILER=gcc
cmake --build build --config Debug

set-location $current_loc
