LINK.o := g++
CXXFLAGS=-std=c++0x
test: test.o servparse.o
clean:
	rm -f servparse.o servparse test.o
