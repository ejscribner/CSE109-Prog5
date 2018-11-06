/*
CSE 109: Fall 2018
Elliot Scribner
ejs320
This program is a singly linked list supporting in-order insertion by name and holding raw data
Program #5
*/

#include "Node.h"

using namespace std;

//constructor
Node_t::Node_t(const string& name, void* data, size_t size, Node_t* next)
{
	//initialise values
	this->name = name;
	this->size = size;
	this->next = next;

	//loop through for data
	if (data != NULL)
	{
		this->data = malloc(size);
		for (size_t i = 0; i < size; i++)
		{
			((char*) this->data)[i] = ((char*) data)[i];
		}
	}
	else
	{
		//dont malloc
		this->data = NULL;
	}
}

//destructor
Node_t::~Node_t()
{
	free(this->data);
	this->data = NULL;
}

//accessors
string Node_t::getName() const
{
	string name = string(this->name);
	return name;
}

void* Node_t::getData() const
{
	return this->data;
}

Node_t* Node_t::getNext() const
{
	return this->next;
}

size_t Node_t::getNodeSize() const
{
	return this->size;
}

//mutators
void Node_t::setName(string name)
{
	this->name = name;
}

void Node_t::setData(void* data, size_t size)
{
	free(this->data);
	this->size = size;
	this->data = NULL;
	if (data == NULL)
	{
		return;
	}
	this->data = malloc(size);
	for (size_t i = 0; i < size; i++)
	{
		((char*) this->data)[i] = ((char*) data)[i];
	}

}

void Node_t::setNext(Node_t* next)
{
	this->next = next;
}

