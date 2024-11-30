all: octal

.PHONY: all clean

octal: src/main.cpp src/lib/*.cpp
	g++ $^ -o $@

clean:
	rm -f octal