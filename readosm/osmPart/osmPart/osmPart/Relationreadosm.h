#pragma once

#include <string>
#include <vector>

#include "readosm.h"
#include "Myreadosm_tag.h"
#include "Myreadosm_member.h"

class Relationreadosm
{
public:
	Relationreadosm();
	~Relationreadosm();

	std::string getId(void);
	void setId(std::string);

	int getVersion(void);
	void setVersion(int);

	std::string getChangeset(void);
	void setChangeset(std::string);

	std::string getUser(void);
	void setUser(std::string);

	int getUid(void);
	void setUid(int);

	std::string getTimestamp(void);
	void setTimestamp(std::string);

	int getMember_count(void);
	void setMember_count(int);

	std::vector<Myreadosm_member> getMembers(void);
	void setMembers(std::vector<Myreadosm_member>);
	void appendMembers(Myreadosm_member);

	int getTag_count(void);
	void setTag_count(int);

	std::vector<Myreadosm_tag> getTags(void);
	void setTags(std::vector<Myreadosm_tag>);
	void appendTags(Myreadosm_tag);

	void affiche(void);

private:
	std::string id; /**< RELATION-ID (expected to be a unique value) */
	int version; /**< object version */
	std::string changeset; /**< ChangeSet ID */
	std::string user; /**< name of the User defining this RELATION */
	int uid; /**< corresponding numeric UserID */
	std::string timestamp; /**< when this RELATION was defined */
	int member_count;	/**< number of associated MEMBERs (may be zero) */
	std::vector <Myreadosm_member> members; /**< array of MEMBER objects (may be NULL) */
	int tag_count; /**< number of associated TAGs (may be zero) */
	std::vector<Myreadosm_tag> tags; /**< array of TAG objects (may be NULL) */
};

