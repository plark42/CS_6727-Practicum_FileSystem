Main: Main.o Disk.o FileSystem.o
	g++ -o Main Main.o Disk.o FileSystem.o

Main.o: Main.cpp
	g++ -c Main.cpp

Disk.o: Disk.cpp
	g++ -c Disk.cpp
	
FileSystem.o: FileSystem.cpp
	g++ -c FileSystem.cpp
