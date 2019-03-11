#include "Wayreadosm.h"
#include "readosm.h"

#include <iostream>
#include <string>
#include <vector>


Wayreadosm::Wayreadosm()
{
}


Wayreadosm::~Wayreadosm()
{
}

// id
std::string Wayreadosm::getId(void)
{
	return id;
}

void Wayreadosm::setId(std::string id)
{
	this->id = id;
}


// version
int Wayreadosm::getVersion(void)
{
	return version;
}

void Wayreadosm::setVersion(int version)
{
	this->version = version;
}

// changeset
std::string Wayreadosm::getChangeset(void)
{
	return changeset;
}

void Wayreadosm::setChangeset(std::string changeset)
{
	this->changeset = changeset;
}


// user
std::string Wayreadosm::getUser(void)
{
	return user;
}

void Wayreadosm::setUser(std::string user)
{
	this->user = user;
}


// uid
int Wayreadosm::getUid(void)
{
	return uid;
}

void Wayreadosm::setUid(int uid)
{
	this->uid = uid;
}

// timestamp
std::string Wayreadosm::getTimestamp(void)
{
	return timestamp;
}

void Wayreadosm::setTimestamp(std::string timestamp)
{
	this->timestamp = timestamp;
}

// node_ref_count
int Wayreadosm::getNode_ref_count(void)
{
	return this->node_ref_count;
}

void Wayreadosm::setNode_ref_count(int node_ref_count)
{
	this->node_ref_count = node_ref_count;
}

// Node_refs
std::vector<std::string> Wayreadosm::getNode_refs(void)
{
	return node_refs;
}

void Wayreadosm::setNode_refs(std::vector<std::string> node_refs)
{
	this->node_refs = node_refs;
}

void Wayreadosm::appendNode_refs(std::string node_ref)
{
	this->node_refs.push_back(node_ref);
}

// tag_count
int Wayreadosm::getTag_count(void)
{
	return tag_count;
}

void Wayreadosm::setTag_count(int tag_count)
{
	this->tag_count = tag_count;
}

// tag
std::vector<Myreadosm_tag> Wayreadosm::getTags(void)
{
	return tags;
}

void Wayreadosm::setTags(std::vector<Myreadosm_tag> tags)
{
	this->tags = tags;
}

void Wayreadosm::appendTags(Myreadosm_tag tag)
{
	this->tags.push_back(tag);
}

void Wayreadosm::affiche(void)
{
	std::cout << "Way \"" << this->id << "\" : " << std::endl;
	std::cout << "version : \t" << this->version << std::endl;
	std::cout << "changeset : \t" << this->changeset << std::endl;
	std::cout << "user : \t" << this->user << std::endl;
	std::cout << "uid : \t" << this->uid << std::endl;
	std::cout << "timestamp : \t" << this->timestamp << std::endl;

	std::cout << "node_ref_count : " << this->node_ref_count << std::endl;
	for (std::string node_ref : node_refs)
		std::cout << node_ref << std::endl;

	std::cout << "tags : " << this->tag_count << std::endl;
	for (Myreadosm_tag tag : tags)
		std::cout << "{ " << tag.getKey() << " : " << tag.getValue() << " }" << std::endl;
}

