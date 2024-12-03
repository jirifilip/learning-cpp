if (-not (test-path "out")) { 
	mkdir out
}

g++ -I include -c src/lib.cpp -o out/lib.a -std=c++20
g++ -I include -L out src/main.cpp -l:lib.a -std=c++20 -o out/main.exe
