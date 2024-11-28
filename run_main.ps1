if (test-path "out") { 
	remove-item -r out
}
mkdir out

g++ src/main.cpp -o out/main.exe
out/main.exe
