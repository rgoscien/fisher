all:
	cc -lstdc++ -std=c++11 -Wall *.cpp -o fisher
install:
	cp fisher /usr/local/bin/
clean:
	rm fisher