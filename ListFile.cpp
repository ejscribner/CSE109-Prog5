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
#include <iostream>

using namespace std;

//constructor
ListFile_t::ListFile_t()
{
	this->size = 0;
	this->head = NULL;
}

//copy constructor
ListFile_t::ListFile_t(const ListFile_t& src)
{
	Node_t* current = src.head;
	while (current != NULL)
	{
		insert(current->getName(), current->getData(), current->getNodeSize());
		current = (*current).getNext();
	}
}

ListFile_t& ListFile_t::operator=(const ListFile_t& rhs)
{
	clear();
	Node_t* current = rhs.head;
	while (current != NULL)
	{
		insert(current->getName(), current->getData(), current->getNodeSize());
		current = (*current).getNext();
	}
	return *this;
}

//destructor
ListFile_t::~ListFile_t()
{
	while (this->head != NULL)
	{
		Node_t* next = this->head->getNext();
		free(this->head);
		this->head = next;
	}
	this->size = 0;
	this->head = NULL;
}

//added to cut down on code for reading
int safeRead(int fd, unsigned char* buffer, int toRead)
{
	ssize_t readResult = 0;
	ssize_t haveRead = 0;
	while ((readResult = read(fd, buffer + haveRead, toRead)) != -1)
	{
		haveRead += readResult;
		toRead -= readResult;
		if (toRead == 0)
		{
			break;
		}
	}
	return haveRead;
}

//added to cut down on code for reading
int safeWrite(int fd, const unsigned char* buffer, int toWrite)
{
	ssize_t writeResult = 0;
	ssize_t haveWritten = 0;
	while ((writeResult = write(fd, buffer + haveWritten, toWrite)) != -1)
	{
		haveWritten += writeResult;
		toWrite -= writeResult;
		if (toWrite == 0)
		{
			break;
		}
	}

	return haveWritten;
}


int ListFile_t::readFile(const string& filename)
{
	ListFile_t* temp = new ListFile_t();
	size_t retval;
	retval = temp->appendFromFile(filename);
	if (retval < 0)
	{
		return -1;
	}

	(*this) = *temp;
	return 0;
}

ssize_t ListFile_t::appendFromFile(const string& filename)
{
	int fd = open(filename.c_str(), O_RDONLY);
	if (fd == -1)
	{
		return -1;
	}

	size_t numInserted = 0;
	size_t numElements = 0;
	ssize_t toRead = 8;
	unsigned char* buffer = (unsigned char*) &numElements;

	ssize_t readResult = 0;
	readResult = safeRead(fd, buffer, toRead);
	if (readResult < toRead)
	{
		return -1;
	}

	ListFile_t* temp = new ListFile_t(*this);
	for (size_t i = 0; i < numElements; i++)
	{
		size_t nameLength = 0;
		size_t dataLength = 0;
		char* name = NULL;
		void* data = NULL;

		toRead = 8;
		buffer = (unsigned char*) &nameLength;
		readResult = safeRead(fd, buffer, toRead);
		if (readResult < toRead)
		{
			close(fd);
			delete temp;
			return -1;
		}


		toRead = 8;
		buffer = (unsigned char*) &dataLength;
		readResult = safeRead(fd, buffer, toRead);

		if (readResult < toRead)
		{
			close(fd);
			delete temp;
			return -1;
		}

		toRead = nameLength;
		name = (char*) malloc((nameLength + 1) * sizeof(char));
		buffer = (unsigned char*) name;

		readResult = safeRead(fd, buffer, toRead);

		if (readResult < toRead)
		{
			delete temp;
			free(name);
			close(fd);
			return -1;
		}
		name[nameLength] = '\0';

		toRead = dataLength;
		data = (char*) malloc(dataLength * sizeof(char));
		buffer = (unsigned char*) data;

		readResult = safeRead(fd, buffer, toRead);

		if (readResult < toRead)
		{
			delete temp;
			free(name);
			free(data);
			close(fd);
			return -1;
		}

		temp->insert(name, data, dataLength);
		free(name);
		free(data);
	}

	Node_t* current = temp->head;
	while (current != NULL)
	{
		int retVal = this->insert(current->getName(), current->getData(), current->getNodeSize());
		if (retVal != 0)
		{
			numInserted++;
		}
		current = current->getNext();
	}
	delete temp;
	close(fd);
	return numInserted;
}

int ListFile_t::saveToFile(const string& filename) const
{
	int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
	if (fd == -1)
	{
		return -1;
	}

	size_t haveWritten = 0;

	ssize_t toWrite = 8;
	const unsigned char* buffer = (unsigned char*) &(this->size);

	ssize_t writeResult = safeWrite(fd, buffer, toWrite);
	if (writeResult < 0)
	{
		close(fd);
		return -1;
	}

	Node_t* currNode = this->head;
	while (currNode != NULL)
	{
		size_t nameLength = currNode->getName().length();
		size_t dataLength = currNode->getNodeSize();
		const char* name = currNode->getName().c_str();
		void* data = currNode->getData();


		toWrite = 8;
		buffer = (unsigned char*) &nameLength;

		writeResult = safeWrite(fd, buffer, toWrite);
		if (writeResult < 0)
		{
			close(fd);
			return -1;
		}

		toWrite = 8;
		buffer = (unsigned char*) &dataLength;

		writeResult = safeWrite(fd, buffer, toWrite);
		if (writeResult < 0)
		{
			close(fd);
			return -1;
		}

		toWrite = nameLength;
		buffer = (unsigned char*) name;

		writeResult = safeWrite(fd, buffer, toWrite);
		if (writeResult < 0)
		{
			close(fd);
			return -1;
		}

		toWrite = dataLength;
		buffer = (unsigned char*) data;

		writeResult = safeWrite(fd, buffer, toWrite);
		if (writeResult < 0)
		{
			close(fd);
			return -1;
		}

		currNode = currNode->getNext();
		haveWritten++;
	}

	if (haveWritten != size)
	{
		fprintf(stderr, "Unexpected number of nodes found. Expected %d, found %d\n", (int) size, (int) haveWritten);
		return -1;
	}

	return 1;
}

size_t ListFile_t::getSize() const
{
	return this->size;
}

size_t ListFile_t::getElementSize(size_t index) const
{
	return (*this)[index].getNodeSize();
}

void* ListFile_t::getElementData(size_t index) const
{
	return (*this)[index].getData();
}

string ListFile_t::getElementName(size_t index) const
{
	return (*this)[index].getName();
}

const Node_t& ListFile_t::operator[](size_t index) const
{
	Node_t* currNode = this->head;
	size_t i = 0;
	while (true)
	{
		if (i == index)
		{
			return (*currNode);
		}
		i++;
		currNode = currNode->getNext();
	}
}

Node_t& ListFile_t::operator[](size_t index)
{
	Node_t* currNode = this->head;
	size_t i = 0;
	while (true)
	{
		if (i == index)
		{
			return (*currNode);
		}
		i++;
		currNode = currNode->getNext();
	}
}

void ListFile_t::clear()
{
	Node_t* currNode = this->head;
	while (currNode != NULL)
	{
		Node_t* temp = currNode->getNext();
		delete currNode;
		currNode = temp;
	}
	this->head = NULL;
	this->size = 0;
}

bool ListFile_t::exists(const string& name) const
{
	Node_t* current = this->head;
	while (current != NULL)
	{
		if (name == current->getName())
		{
			return true;
		}
		current = current->getNext();
	}
	return false;
}

size_t ListFile_t::count(void* data, size_t size) const
{
	size_t numMatches = 0;
	Node_t* current = this->head;
	for (size_t i = 0; i < size; i++)
	{
		if (current->getNodeSize() == size)
		{
			if (memcmp(current->getData(), data, size) == 0)
			{
				numMatches++;
			}
		}
		current = current->getNext();
	}
	return numMatches;
}

int ListFile_t::removeByName(const string& name)
{
	if (this->size <= 0)
	{
		return 0;
	}
	Node_t* currNode = this->head;
	Node_t* nextNode = currNode->getNext();
	if (currNode->getName() == name)
	{
		this->head = nextNode;
		delete currNode;
		this->size--;
		return 1;
	}
	while (nextNode != NULL)
	{
		if (nextNode->getName() == name)
		{
			currNode->setNext(nextNode->getNext());
			delete nextNode;
			this->size--;
			return 1;
		}
		currNode = nextNode;
		nextNode = currNode->getNext();
	}
	return 0;
}

int ListFile_t::insert(const string& name, void* data, size_t size)
{
	if (this->size == 0 || head == NULL)
	{
		Node_t* inserted = new Node_t(name, data, size, NULL);
		this->head = inserted;
		this->size++;
		return 1;
	}
	Node_t* currNode = this->head;
	Node_t* nextNode = this->head->getNext();

	if (strcmp(currNode->getName().c_str(), name.c_str()) > 0)
	{
		Node_t* inserted = new Node_t(name, data, size, currNode);
		this->head = inserted;
		this->size++;
		return 1;
	}

	if (strcmp(currNode->getName().c_str(), name.c_str()) == 0)
	{
		return 0;
	}

	while (nextNode != NULL)
	{
		if (strcmp(nextNode->getName().c_str(), name.c_str()) == 0)
		{
			return 0;
		}
		else if (strcmp(nextNode->getName().c_str(), name.c_str()) > 0)
		{
			Node_t* inserted = new Node_t(name, data, size, nextNode);
			this->size++;
			currNode->setNext(inserted);
			return 1;
		}
		currNode = nextNode;
		nextNode = currNode->getNext();
	}
	Node_t* inserted = new Node_t(name, data, size, NULL);
	this->size++;
	currNode->setNext(inserted);
	return 1;
}

void insertInternal(Node_t* data)
{
}