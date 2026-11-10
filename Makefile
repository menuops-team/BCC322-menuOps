CFLAGS = -Wall -std=c++17 -Iinclude -Isrc

SRC = $(wildcard src/*.cpp)
TEST_SRC = $(filter-out src/main.cpp,$(SRC))

UNIT_SRC = $(wildcard test/unit/*.cpp)
FUNC_SRC = $(wildcard test/func/*.cpp)

build:
	g++ $(SRC) $(CFLAGS) -o bin/app

run: build
	./bin/app

build_unit:
	g++ $(UNIT_SRC) $(TEST_SRC) $(CFLAGS) -o bin/unit_testes

run_unit: build_unit
	./bin/unit_testes

build_func:
	g++ $(FUNC_SRC) $(TEST_SRC) $(CFLAGS) -o bin/func_testes

run_func: build_func
	./bin/func_testes

build_o:
	mkdir -p bin
	g++ $(CFLAGS) -c $(SRC)
	mv *.o bin/

build_a: build_o
	ar rcs bin/app.a bin/*.o

build_so: build_o
	g++ -shared bin/*.o -o bin/app.so

build_dll: build_o
	g++ -shared bin/*.o -o bin/app.dll

build_dll_app: build_dll
	g++ $(SRC) $(CFLAGS) -o bin/app.exe

clean:
	rm -f bin/*.o bin/*.a bin/*.so bin/*.dll bin/*.exe bin/app bin/unit_testes bin/func_testes *.o