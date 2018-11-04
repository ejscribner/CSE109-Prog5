#include "Node.h"
#include "ListFile.h"

int main(int argc, char **argv)
{
	ListFile_t myList;
//	void* data = (void*)"testing";

//	void* data = malloc(10);
//	void* data2 = malloc(10);

//	myList.removeByName("testing");
//	myList.insert(string("A"), data, 10); //strlen + 1
//	myList.insert(string("B"), data2, 10);

	myList.readFile("inFile.txt");

	return 0;
}