if (-not (test-path "out")) { 
	mkdir out
}

$googletest_dir = "build\dependencies\gtest\$(Get-ChildItem build/dependencies/gtest)"

g++ `
	-Wall -Wextra -Werror `
	-I include `
	-I $googletest_dir\googletest\include `
	-I $googletest_dir\googlemock\include `
	-L $googletest_dir\build\lib `
	-L out `
	tests/test.cpp `
	-o out/test.exe `
	-l gtest `
	-l gmock `
	-l gtest_main `
	-l:lib.a
	
if ($?) {
	out/test.exe
}