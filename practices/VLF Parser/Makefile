CFLAGS = -O0 -g
CC = g++

vlf: raster.o parserobj.o main.o
	$(CC) $(CFLAGS) -o parser.out headers/main.o headers/raster.o headers/parserobj.o

main.o: Main.cpp
	$(CC) $(CFLAGS) -o headers/main.o -c Main.cpp

raster.o: headers/Raster.cpp
	$(CC) $(CFLAGS) -o headers/raster.o -c headers/Raster.cpp

parserobj.o: headers/ParserObj.cpp
	$(CC) $(CFLAGS) -o headers/parserobj.o -c headers/ParserObj.cpp

clean: 
	rm -f core headers/*.o parser.out
	clear