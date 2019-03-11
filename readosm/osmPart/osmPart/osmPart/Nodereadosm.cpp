#include "Nodereadosm.h"
#include "readosm.h"

#include <iostream>
#include <string>
#include <vector>


Nodereadosm::Nodereadosm()
{

}


Nodereadosm::~Nodereadosm()
{
}


// id
std::string Nodereadosm::getId(void)
{
	return id;
}

void Nodereadosm::setId(std::string id)
{
	this->id = id;
}

// latitude
double Nodereadosm::getLatitude(void)
{
	return latitude;
}

void Nodereadosm::setLatitude(double latitude)
{
	this->latitude = latitude;
}


// longitude
double Nodereadosm::getLongitude(void)
{
	return longitude;
}

void Nodereadosm::setLongitude(double longitude)
{
	this->longitude = longitude;
}


// version
int Nodereadosm::getVersion(void)
{
	return version;
}

void Nodereadosm::setVersion(int version)
{
	this->version = version;
}

// changeset
std::string Nodereadosm::getChangeset(void)
{
	return changeset;
}

 void Nodereadosm::setChangeset(std::string changeset)
{
	this->changeset = changeset;
}


 // user
std::string Nodereadosm::getUser(void)
{
	return user;
}

void Nodereadosm::setUser(std::string user)
{
	this->user = user;
}


// uid
int Nodereadosm::getUid(void)
{
	return uid;
}

void Nodereadosm::setUid(int uid)
{
	this->uid = uid;
}

// timestamp
std::string Nodereadosm::getTimestamp(void)
{
	return timestamp;
}

void Nodereadosm::setTimestamp(std::string timestamp)
{
	this->timestamp = timestamp;
}

// tag_count
int Nodereadosm::getTag_count(void)
{
	return tag_count;
}

void Nodereadosm::setTag_count(int tag_count)
{
	this->tag_count = tag_count;
}

// tag
std::vector<Myreadosm_tag> Nodereadosm::getTags(void)
{
	return tags;
}

void Nodereadosm::setTags(std::vector<Myreadosm_tag> tags)
{
	this->tags = tags;
}

void Nodereadosm::appendTags(Myreadosm_tag tag)
{
	this->tags.push_back(tag);
}

void Nodereadosm::affiche(void)
{
	std::cout << "Node \"" << this->id << "\" : " << std::endl;
	std::cout << "latitude : \t" << this->latitude << std::endl;
	std::cout << "longitude : \t" << this->longitude << std::endl;
	std::cout << "version : \t" << this->version << std::endl;
	std::cout << "changeset : \t" << this->changeset << std::endl;
	std::cout << "user : \t" << this->user << std::endl;
	std::cout << "uid : \t" << this->uid << std::endl;
	std::cout << "timestamp : \t" << this->timestamp << std::endl;
	std::cout << "ele : \t" << this->ele << std::endl;
	std::cout << "Pos : \t" << this->pos.getEle() << std::endl;
	std::cout << "Pos : \t" << this->pos.getLongitude() << std::endl;
	std::cout << "Pos : \t" << this->pos.getLatitude() << std::endl;
	std::cout << "tags : " << this->tag_count << std::endl;
	for(Myreadosm_tag tag : tags)
		std::cout << "{ " << tag.getKey() << " : " << tag.getValue() << " }" << std::endl;

}

double Nodereadosm::getEle(void)
{
	return ele;
}

void Nodereadosm::setEle(double ele)
{
	this->ele = ele;
}

Point Nodereadosm::getPos(void)
{

	return pos;

}

void Nodereadosm::setPos(Point pos)
{
	this->pos = pos;
}

void Nodereadosm::setPosEle(void)
{
	for (Myreadosm_tag tag : tags)
		if (tag.getKey() == "ele")
			this->ele = stod(tag.getValue());

	this->pos.setEle(this->ele);
	this->pos.setLatitude(this->latitude);
	this->pos.setLongitude(this->longitude);
}