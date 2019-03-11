#include "Relationreadosm.h"
#include "readosm.h"

#include <iostream>
#include <string>
#include <vector>


Relationreadosm::Relationreadosm()
{
}


Relationreadosm::~Relationreadosm()
{
}

// id
std::string Relationreadosm::getId(void)
{
	return id;
}

void Relationreadosm::setId(std::string id)
{
	this->id = id;
}

// version
int Relationreadosm::getVersion(void)
{
	return version;
}

void Relationreadosm::setVersion(int version)
{
	this->version = version;
}

// changeset
std::string Relationreadosm::getChangeset(void)
{
	return changeset;
}

void Relationreadosm::setChangeset(std::string changeset)
{
	this->changeset = changeset;
}


// user
std::string Relationreadosm::getUser(void)
{
	return user;
}

void Relationreadosm::setUser(std::string user)
{
	this->user = user;
}


// uid
int Relationreadosm::getUid(void)
{
	return uid;
}

void Relationreadosm::setUid(int uid)
{
	this->uid = uid;
}

// timestamp
std::string Relationreadosm::getTimestamp(void)
{
	return timestamp;
}

void Relationreadosm::setTimestamp(std::string timestamp)
{
	this->timestamp = timestamp;
}

// member_count
int Relationreadosm::getMember_count(void)
{
	return member_count;
}

void Relationreadosm::setMember_count(int member_count)
{
	this->member_count = member_count;
}

// members
std::vector<Myreadosm_member> Relationreadosm::getMembers(void)
{
	return members;
}

void Relationreadosm::setMembers(std::vector<Myreadosm_member> members)
{
	this->members = members;
}

void Relationreadosm::appendMembers(Myreadosm_member member)
{
	this->members.push_back(member);
}

// tag_count
int Relationreadosm::getTag_count(void)
{
	return tag_count;
}

void Relationreadosm::setTag_count(int tag_count)
{
	this->tag_count = tag_count;
}

// tag
std::vector<Myreadosm_tag> Relationreadosm::getTags(void)
{
	return tags;
}

void Relationreadosm::setTags(std::vector<Myreadosm_tag> tags)
{
	this->tags = tags;
}

void Relationreadosm::appendTags(Myreadosm_tag tag)
{
	this->tags.push_back(tag);
}

void Relationreadosm::affiche(void)
{
	std::cout << "Relation \"" << this->id << "\" : " << std::endl;

	std::cout << "version : \t" << this->version << std::endl;
	std::cout << "changeset : \t" << this->changeset << std::endl;
	std::cout << "user : \t" << this->user << std::endl;
	std::cout << "uid : \t" << this->uid << std::endl;
	std::cout << "timestamp : \t" << this->timestamp << std::endl;

	std::cout << "members : " << this->member_count << std::endl;
	for (Myreadosm_member member : members)
		std::cout << "member_type : " << member.getMember_type() << "\t id : " << member.getId() << "\t role :" << member.getRole() << std::endl;

	std::cout << "tags : " << this->tag_count << std::endl;
	for (Myreadosm_tag tag : tags)
		std::cout << "{ " << tag.getKey() << " : " << tag.getValue() << " }" << std::endl;

}

