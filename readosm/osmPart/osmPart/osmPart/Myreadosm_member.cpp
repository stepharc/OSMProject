#include "Myreadosm_member.h"



Myreadosm_member::Myreadosm_member()
{
}


Myreadosm_member::~Myreadosm_member()
{
}

// Member_type
Member_type Myreadosm_member::getMember_type(void)
{
	return member_type;
}

void Myreadosm_member::setMember_type(Member_type member_type)
{
	this->member_type = member_type;
}

// Id
std::string Myreadosm_member::getId(void)
{
	return id;
}

void Myreadosm_member::setId(std::string id)
{
	this->id = id;
}

// Role
std::string Myreadosm_member::getRole(void)
{
	return role;
}

void Myreadosm_member::setRole(std::string role)
{
	this->role = role;
}