target: clean filecompress.o
	g++ -o target filecompress.o

filecompress.o: filecompress.cpp
	g++ -c filecompress.cpp

clean:
	rm -f target filecompress.o