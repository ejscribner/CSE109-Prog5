prog5: Node.o ListFile.o prog5.o
	g++ -g -Wall -Werror -o prog4 Node.o ListFile.o prog5.o

ListFile.o: ListFile.cpp
	g++ -g -Wall -Werror -c ListFile.cpp

Node.o: Node.cpp
	g++ -g -Wall -Werror -c Node.cpp

clean:
	rm *.o prog5