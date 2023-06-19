Main:
	g++ -o Main Main.o Disk.o

Main.o: Main.cpp
	g++ -c Main.cpp

Disk.o: Disk.cpp
	g++ -c Disk.cpp
