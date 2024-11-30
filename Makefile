OUT=octal

all: linux win

.PHONY: all clean win

linux: src/main.cpp src/lib/*.cpp
	mkdir -p build/linux
	g++ $^ -o build/linux/$(OUT)

win: src/main.cpp src/lib/*.cpp
	mkdir -p build/win
	x86_64-w64-mingw32-g++-win32 $^ -static -o build/win/$(OUT).exe

clean:
	rm -rf build