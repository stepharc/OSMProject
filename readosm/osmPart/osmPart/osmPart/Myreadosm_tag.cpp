#include "Myreadosm_tag.h"



Myreadosm_tag::Myreadosm_tag()
{
}


Myreadosm_tag::~Myreadosm_tag()
{
}

std::string Myreadosm_tag::getKey(void)
{
	return this->key;
}

void Myreadosm_tag::setKey(std::string key)
{
	this->key = key;
}

std::string Myreadosm_tag::getValue(void)
{
	return this->value;
}

void Myreadosm_tag::setValue(std::string value)
{
	this->value = value;
}