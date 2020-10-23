CFLAGS = -O0 -g
CC = g++

line: main.o raster.o
	$(CC) $(CFLAGS) -o line.out main.o raster.o
	./line.out

main.o: Main.cpp
	$(CC) $(CFLAGS) -c Main.cpp

raster.o: Raster.cpp
	$(CC) $(CFLAGS) -c Raster.cpp

clean: 
	rm -f core *.o
	clear