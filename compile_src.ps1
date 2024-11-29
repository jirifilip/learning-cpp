if (-not (test-path "out")) { 
	mkdir out
}

g++ -I include -c src/lib.cpp -o out/lib.a
g++ -I include -L out src/main.cpp -l:lib.a -o out/main.exe
