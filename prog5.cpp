#include "Node.h"
#include "ListFile.h"
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char **argv)
{
	ListFile_t* myList = new ListFile_t();
//	void* data = (void*)"testing";

	void* data = malloc(10);

	memset(data, 0, 10);
	myList->insert(string("B"), data, 10);
	myList->insert(string("D"), data, 10);
	myList->insert(string("E"), data, 10);
	myList->insert(string("A"), data, 10); //strlen + 1
	myList->insert(string("F"), data, 10);
	myList->insert(string("G"), data, 10);
	myList->insert(string("C"), data, 10);
	myList->insert(string("H"), data, 10);
	myList->insert(string("M"), data, 10);
	myList->insert(string("J"), data, 10);
	myList->insert(string("I"), data, 10);
	myList->insert(string("K"), data, 10);
	myList->insert(string("N"), data, 10);
	myList->insert(string("O"), data, 10);
	myList->insert(string("P"), data, 10);
	myList->insert(string("L"), data, 10);
	myList->insert(string("Y"), data, 10);
	myList->insert(string("R"), data, 10);
	myList->insert(string("S"), data, 10);
	myList->insert(string("T"), data, 10);
	myList->insert(string("Q"), data, 10);
	myList->insert(string("U"), data, 10);
	myList->insert(string("V"), data, 10);
	myList->insert(string("W"), data, 10);
	myList->insert(string("X"), data, 10);
	myList->insert(string("Z"), data, 10);
//	myList->removeByName("A");
//	myList->removeByName("B");
//	myList->removeByName("C");
	myList->removeByName("D");
	myList->removeByName("E");
	myList->removeByName("F");
	myList->removeByName("G");
	myList->removeByName("H");
	myList->removeByName("I");
	myList->removeByName("J");
	myList->removeByName("K");
	myList->removeByName("L");
	myList->removeByName("M");
	myList->removeByName("N");
	myList->removeByName("O");
	myList->removeByName("P");
	myList->removeByName("Q");
	myList->removeByName("R");
	myList->removeByName("S");
	myList->removeByName("T");
	myList->removeByName("U");
	myList->removeByName("V");
	myList->removeByName("W");
	myList->removeByName("X");
	myList->removeByName("Y");
	myList->removeByName("Z");

	//cerr << "name at 2 is: " << (*myList)[20].getName() << endl;

	/*int result = myList.saveToFile("out.txt");
	cout << "Result is: " << result << endl;
*/
	size_t result = myList->appendFromFile("genFile.txt");
	cerr << "Result is: "<< result<< endl;

//	for(size_t i = 0; i < myList->getSize(); i++) {
//		cout << "Node: " << myList->getElementName(i) << endl;
//	}

	cout << "List Size = " << myList->getSize() << endl;
	cout << "Name = " << myList->getElementName(1) << endl;
	cout << "Size = " << myList->getElementSize(1) << endl;
	cout << "Data = " << myList->getElementData(1) << endl;
	cout << "Exists = " << myList->exists("f") << endl;
	cout << "Count = " << myList->count(data, 10) << endl;

	myList->clear();

	for(size_t i = 0; i < myList->getSize(); i++) {
		cout << "Node: " << myList->getElementName(i) << endl;
	}

	myList->saveToFile("ourFile.txt");
	free(data);
	data = NULL;

	delete myList;
	return 0;
}