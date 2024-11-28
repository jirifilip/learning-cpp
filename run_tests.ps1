if (test-path "out") { 
	remove-item -r out
}
mkdir out

$googletest_dir = "build\dependencies\gtest\$(Get-ChildItem build/dependencies/gtest)"

g++ `
	-Wall -Wextra -Werror `
	-I $googletest_dir\googletest\include `
	-L $googletest_dir\build\lib `
	tests/test.cpp `
	-o out/test.exe `
	-l gtest `

out/test.exe