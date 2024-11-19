CC=g++

all: files

files: files.cpp 
	$(CC) -g -o files files.cpp

clean:
	rm files files.exe
