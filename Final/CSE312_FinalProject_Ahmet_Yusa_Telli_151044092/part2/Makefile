CC = g++
CFLAGS = -c -std=c++11
VALG = valgrind -v

all:main sortArrays

main: main.o sort.o page_replacement.o page_table.o
	$(CC) main.o sort.o page_replacement.o page_table.o -o sortArrays -pthread -std=c++11

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

sort.o: sort.cpp
	$(CC) $(CFLAGS) sort.cpp

page_replacement.o: page_replacement.cpp
	$(CC) $(CFLAGS) page_replacement.cpp

page_table.o: page_table.cpp
	$(CC) $(CFLAGS) page_table.cpp

write.o: write.cpp
	$(CC) $(CFLAGS) write.cpp

read.o: read.cpp
	$(CC) $(CFLAGS) read.cpp

list.o: list.cpp
	$(CC) $(CFLAGS) list.cpp

del.o: del.cpp
	$(CC) $(CFLAGS) del.cpp

dumpe2fs.o: dumpe2fs.cpp
	$(CC) $(CFLAGS) dumpe2fs.cpp


clean:
	rm -rf *o sortArrays