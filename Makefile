Main: Main.o Disk.o FileSystem.o
	g++ -w -o Main Main.o Disk.o FileSystem.o

Main.o: Main.cpp
	g++ -w -c Main.cpp

Disk.o: Disk.cpp
	g++ -w -c Disk.cpp
	
FileSystem.o: FileSystem.cpp
	g++ -w -c FileSystem.cpp
