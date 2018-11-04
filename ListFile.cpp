/*
CSE 109: Fall 2018
Elliot Scribner
ejs320
This program is a singly linked list supporting in-order insertion by name and holding raw data
Program #5
*/

#include "ListFile.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

using namespace std;

//constructor
ListFile_t::ListFile_t() {
	this->size = 0;
	this->head = NULL;
}

ListFile_t::ListFile_t(const ListFile_t& src) {
	Node_t* current = src.head; //might not work?, maybe loop on src.getSize() and get the element data?
	//could just do src[0]?
	while(current != NULL) {
		insert(current->getName(), current->getData(), current->getNodeSize());
//		current = current->getNext();
		current = (*current).getNext();
	}
}

ListFile_t& ListFile_t::operator=(const ListFile_t& rhs) {
	//whenever uses operator= set two lists equal to eachother
	clear();
	Node_t* current = rhs.head; //might not work
	while(current != NULL) {
		insert(current->getName(), current->getData(), current->getNodeSize());
//		current = current->getNext();
		current = (*current).getNext();
	}
	return *this;
}


ListFile_t::~ListFile_t() {
	while(this->head != NULL) {
		Node_t* next = this->head->getNext();
		free(this->head);
		this->head = next;
	}
	this->size = 0;
}

int ListFile_t::readFile(const string& filename) {

	int fd = open(filename.c_str(), O_RDONLY);
	if (fd == -1) {
		return -1;
	}

	size_t numElements = 0;

	ssize_t toRead = 8;
	ssize_t haveRead = 0;
	unsigned char* buffer = (unsigned char*)&numElements;

	ssize_t readResult = 0;
	while((readResult = read(fd, buffer + haveRead, toRead)) != -1)
	{
		if(readResult == 0)
		{
			//???
		}
		haveRead += readResult;
		toRead -= readResult;
		if(toRead == 0)
		{
			break;
		}
	}
	if (toRead!=0) {
		return -1;
	}
	ListFile_t* temp = new ListFile_t();
//	ListFile_t temp;
	for(size_t i = 0; i < numElements; i++) {
		size_t nameLength = 0;
		size_t dataLength = 0;
		char* name = NULL;
		void* data = NULL;

		toRead = 8;
		haveRead = 0;
		buffer = (unsigned char*)&nameLength;

		while((readResult = read(fd, buffer + haveRead, toRead)) != -1)
		{
			if(readResult == 0)
			{
				//return -1 or exit??
			}
			haveRead += readResult;
			toRead -= readResult;
			if(toRead == 0)
			{
				break;
			}
		}
		if (toRead!=0) {
			delete temp;
			return -1;
		}


		toRead = 8;
		haveRead = 0;
		buffer = (unsigned char*)&dataLength;

		while((readResult = read(fd, buffer + haveRead, toRead)) != -1)
		{
			if(readResult == 0)
			{
				//return -1 or exit??
			}
			haveRead += readResult;
			toRead -= readResult;
			if(toRead == 0)
			{
				break;
			}
		}
		if (toRead!=0) {
			delete temp;
			return -1;
		}

		toRead = nameLength;
		haveRead = 0;
		name = (char*)malloc(nameLength * sizeof(char));
		buffer = (unsigned char*)name;

		while((readResult = read(fd, buffer + haveRead, toRead)) != -1)
		{
			if(readResult == 0)
			{
				//return -1 or exit??
			}
			haveRead += readResult;
			toRead -= readResult;
			if(toRead == 0)
			{
				break;
			}
		}
		if (toRead!=0) {
			delete temp;
			return -1;
		}

		toRead = dataLength;
		haveRead = 0;
		data = (char*)malloc(dataLength * sizeof(char));
		buffer = (unsigned char*)data;

		while((readResult = read(fd, buffer + haveRead, toRead)) != -1)
		{
			if(readResult == 0)
			{
				//return -1 or exit??
			}
			haveRead += readResult;
			toRead -= readResult;
			if(toRead == 0)
			{
				break;
			}
		}
		if (toRead!=0) {
			delete temp;
			return -1;
		}

		temp->insert(name, data, dataLength);

	}

	this->~ListFile_t(); //should call destructor but not actually delete

//	*this = temp; //expecting const ListFile_t& but getting ListFile_t*, issue with copy constructor
	//loop through temp and insert namd and data for each node
	for(size_t i = 0; i < temp->getSize(); i++) {
		this->insert(temp->getElementName(i), temp->getElementData(i), temp->getElementSize(i));
	}
	delete temp;
	return 0;
}

//for append, same as read but loop through temp at the end and insert each one
//never delete list
ssize_t ListFile_t::appendFromFile(const string& filename) {
	return 1;
}

int ListFile_t::saveToFile(const string& filename) const {
	//calling open on a file that doesnt exist creates a new file

	return 1;
}

size_t ListFile_t::getSize() const {
	return this->size;
}

size_t ListFile_t::getElementSize(size_t index) const {
	Node_t* current = this->head->getNext();
	for(size_t i = 0; i < index; i++) {
		current = this->head->getNext();
	}
	return current->getNodeSize();
}

void* ListFile_t::getElementData(size_t index) const {
	Node_t* current = this->head->getNext();
	for(size_t i = 0; i < index; i++) {
		current = this->head->getNext();
	}
	return current->getData();
}

string ListFile_t::getElementName(size_t index) const {
	Node_t* current = this->head->getNext();
	for(size_t i = 0; i < index; i++) {
		current = this->head->getNext();
	}
	return current->getName();
}

const Node_t& ListFile_t::operator[](size_t index) const {
	Node_t* currNode = this->head;
	for(size_t i = 0; i < this->getSize(); i++) {
		if(i == index) {
			return (*currNode);
		}
		currNode = currNode->getNext();
	}
	return(*currNode);
}

Node_t& ListFile_t::operator[](size_t index) {
	Node_t* currNode = this->head;
	for(size_t i = 0; i < this->getSize(); i++) {
		if(i == index) {
			return (*currNode);
		}
		currNode = currNode->getNext();
	}
	return(*currNode);
}

void ListFile_t::clear() {
	Node_t* currNode = this->head;
	while(currNode != NULL) {
		Node_t* temp = currNode;
		delete currNode;
		currNode = temp->getNext();
	}
	this->head = NULL;
	this->size = 0;
}

bool ListFile_t::exists(const string& name) const {
	Node_t* current = this->head;
	while(current != NULL) {
		if(name == current->getName()) {
			return true;
		}
		current = current->getNext();
	}
	return false;
}

size_t ListFile_t::count(void* data, size_t size) const {
	size_t numMatches = 0;
	Node_t* current = this->head;
	for(size_t i = 0; i < size; i++) {
		if(current->getData() == data) {
			numMatches++;
		}
		current = current->getNext();
	}
	return numMatches;
}

int ListFile_t::removeByName(const string& name) {
	Node_t* currNode = this->head;
	Node_t* nextNode = currNode->getNext();
	if(currNode->getName() == name) {
		this->head = nextNode;
		delete currNode;
		return 1;
	}
	while(currNode != NULL) {
		if(currNode->getName() == name) {
			currNode->setNext(nextNode->getNext());
			delete nextNode;
			return 1;
		}
		currNode = nextNode;
		nextNode = currNode->getNext();
	}
	return 0;
}

int ListFile_t::insert(const string& name, void* data, size_t size) {
	//check if list is empty, if its empty updates size
	if(this->getSize() == 0) {
		Node_t* inserted = new Node_t(name, data, size, NULL);
		this->size++;
		inserted->setNext(NULL);
		return 1;
	}
	//strcmp = 0 if same
	Node_t* currNode = this->head;
	Node_t* nextNode = this->head->getNext();
	//check special case of first node/adding it to the head
	if(strcmp(currNode->getName().c_str(), name.c_str()) > 0) {
		Node_t* inserted = new Node_t(name, data, size, currNode);
		this->size++;
		inserted->setNext(currNode);
		return 1;
	}

	while(nextNode != NULL) {
		if(strcmp(currNode->getName().c_str(), name.c_str()) == 0) {
			//Same
			return 0;
		} else if (strcmp(currNode->getName().c_str(), name.c_str()) > 0) {
			//first comes before second
			//should check null condition
			Node_t* temp = currNode->getNext();
			Node_t* inserted = new Node_t(name, data, size, temp);
			this->size++;
			currNode->setNext(inserted);
			return 1;
		}
		currNode = nextNode;
		nextNode = currNode->getNext();
	}
	//add node to end
	Node_t* inserted = new Node_t(name, data, size, NULL);
	this->size++;
	currNode->setNext(inserted);
	return 1;
}

void insertInternal(Node_t* data) {

}

