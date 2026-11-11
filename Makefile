CFLAGS = -Wall -std=c++17 -Iinclude -Isrc

build:
	g++ src/*.cpp $(CFLAGS) -o bin/app

run: build
	./bin/app

build_unit:
	mkdir -p bin
	g++ test/unit/*.cpp src/*.cpp $(CFLAGS) -o bin/unit_testes

run_unit: build_unit
	./bin/unit_testes

build_func:
	mkdir -p bin
	g++ test/func/*.cpp src/*.cpp $(CFLAGS) -o bin/func_testes

run_func: build_func
	./bin/func_testes

build_o:
	mkdir -p bin
	g++ $(CFLAGS) -c src/*.cpp
	mv *.o bin/

build_a: build_o
	ar rcs bin/app.a bin/*.o

build_so: build_o
	g++ -shared bin/*.o -o bin/app.so

build_dll: build_o
	g++ -shared bin/*.o -o bin/app.dll

build_dll_app: build_dll
	g++ src/*.cpp $(CFLAGS) -o bin/app.exe

clean:
	rm -f bin/*.o bin/*.a bin/*.so bin/*.dll bin/*.exe bin/app bin/unit_testes bin/func_testes *.o
