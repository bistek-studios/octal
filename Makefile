all: octal

.PHONY: all clean

octal: octal.cpp
	g++ $< -o $@

clean:
	rm -f octal