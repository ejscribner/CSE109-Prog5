prog5: Node.o ListFile.o prog5.o
	g++ -g -Wall -Werror -o prog5 Node.o ListFile.o prog5.o

prog5.o: prog5.cpp
	g++ -g -Wall -Werror -c prog5.cpp

ListFile.o: ListFile.cpp ListFile.h
	g++ -g -Wall -Werror -c ListFile.cpp

Node.o: Node.cpp Node.h
	g++ -g -Wall -Werror -c Node.cpp

clean:
	rm *.o

.PHONY: clean *.o
