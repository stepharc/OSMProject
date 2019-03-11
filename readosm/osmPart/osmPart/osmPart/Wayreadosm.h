#pragma once

#include <string>
#include <vector>

#include "readosm.h"
#include "Myreadosm_tag.h"

class Wayreadosm
{
public:
	Wayreadosm();
	~Wayreadosm();

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

	int getNode_ref_count(void);
	void setNode_ref_count(int);

	std::vector<std::string> getNode_refs(void);
	void setNode_refs(std::vector<std::string>);
	void appendNode_refs(std::string);

	int getTag_count(void);
	void setTag_count(int);

	std::vector<Myreadosm_tag> getTags(void);
	void setTags(std::vector<Myreadosm_tag>);
	void appendTags(Myreadosm_tag);

	void affiche(void);

private:
	std::string id = "-1"; /**< WAY-ID (expected to be a unique value) */
	int version = -1; /**< object version */
	std::string changeset = "-1"; /**< ChangeSet ID */
	std::string user = "-1"; /**< name of the User defining this WAY */
	int uid = -1; /**< corresponding numeric UserID */
	std::string timestamp = "-1"; /**< when this WAY was defined */
	int node_ref_count = -1; /**< number of referenced NODE-IDs (may be zero) */
	std::vector<std::string> node_refs; /**< array of NODE-IDs (may be NULL) */
	int tag_count = -1; /**< number of associated TAGs (may be zero) */
	std::vector<Myreadosm_tag> tags; /**< array of TAG objects (may be NULL) */
};

