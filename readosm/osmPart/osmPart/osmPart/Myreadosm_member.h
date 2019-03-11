#pragma once

#include <string>

enum Member_type { Node, Way, Relation };

class Myreadosm_member
{
public:
	Myreadosm_member();
	~Myreadosm_member();

	Member_type getMember_type(void);
	void setMember_type(Member_type);

	std::string getId(void);
	void setId(std::string);

	std::string getRole(void);
	void setRole(std::string);

private:
	Member_type member_type; /**< can be one of: READOSM_MEMBER_NODE, READOSM_MEMBER_WAY or READOSM_MEMBER_RELATION */
	std::string id; /**< ID-value identifying the referenced object */
	std::string role; /**< intended role for this reference */
};

