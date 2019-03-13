#pragma once

/*!
 * \file Relationreadosm.h
 * \brief class representing a Relation
 * \author Ludovic
 */

#include <string>
#include <vector>

#include "readosm.h"
#include "Myreadosm_tag.h"
#include "Myreadosm_member.h"

 /*! \class Relationreadosm
 * \brief class representing a Relation
 */
class Relationreadosm
{
public:
	/*!
	*  \brief Constructor
	*
	*  Constructor of the class Relationreadosm.
	*/
	Relationreadosm();

	/*!
	*  \brief Destructor
	*
	*  Destructor of the class Relationreadosm.
	*/
	~Relationreadosm();

	/*!
	*  \brief Get the Relation id
	*
	*  \return The Relation id
	*/
	std::string getId(void);

	/*!
	*  \brief Set the Relation id
	*
	*  \param The Relation id
	*/
	void setId(std::string);

	/*!
	*  \brief Get the version of the Relation
	*
	*  \return The version of the Relation
	*/
	int getVersion(void);

	/*!
	*  \brief Set the version of the Relation
	*
	*  \param The version of the Relation
	*/
	void setVersion(int);

	/*!
	*  \brief Get the changeset of the Relation
	*
	*  \return The changeset of the Relation
	*/
	std::string getChangeset(void);

	/*!
	*  \brief Set the changeset of the Relation
	*
	*  \param The changeset of the Relation
	*/
	void setChangeset(std::string);

	/*!
	*  \brief Get the user of the Relation
	*
	*  \return The user of the Relation
	*/
	std::string getUser(void);

	/*!
	*  \brief Set the user of the Relation
	*
	*  \return The user of the Relation
	*/
	void setUser(std::string);

	/*!
	*  \brief Get the uid of the Relation
	*
	*  \return The uid of the Relation
	*/
	int getUid(void);

	/*!
	*  \brief Set the uid of the Relation
	*
	*  \return The uid of the Relation
	*/
	void setUid(int);

	/*!
	*  \brief Get the timestamp of the Relation
	*
	*  \return The timestamp of the Relation
	*/
	std::string getTimestamp(void);

	/*!
	*  \brief Set the timestamp of the Relation
	*
	*  \return The timestamp of the Relation
	*/
	void setTimestamp(std::string);

	/*!
	*  \brief Get the member_count of the Relation
	*
	*  \return The member_count of the Relation

	*/
	int getMember_count(void);


	/*!
	*  \brief Get the member_count of the Relation
	*
	*  \param The member_count of the Relation
	*/
	void setMember_count(int);

	/*!
	*  \brief Get the Relation members
	*
	*  \return the Relation members
	*/
	std::vector<Myreadosm_member> getMembers(void);
	
	/*!
	*  \brief Set the Relation members
	*
	*  \return the Relation members
	*/
	void setMembers(std::vector<Myreadosm_member>);
	
	/*!
	*  \brief Append a member to the list of members of the Relation
	*
	*  \param The member to add to the Relation
	*/
	void appendMembers(Myreadosm_member);

	/*!
	*  \brief Get the tag_count of the Relation
	*
	*  \return The tag_count of the Relation
	*/
	int getTag_count(void);

	/*!
	*  \brief Set the tag_count of the Relation
	*
	*  \return The tag_count of the Relation
	*/
	void setTag_count(int);

	/*!
	*  \brief Get the tags of the Relation
	*
	*  \return The tags of the Relation
	*/
	std::vector<Myreadosm_tag> getTags(void);

	/*!
	*  \brief Set the tags of the Relation
	*
	*  \return The tags of the Relation
	*/
	void setTags(std::vector<Myreadosm_tag>);

	/*!
	*  \brief Append a tag to the list of tags of the Relation
	*
	*  \param The tag to add to the Relation
	*/
	void appendTags(Myreadosm_tag);

	/*!
	*  \brief Display Relation informations on standard output
	*
	*/
	void affiche(void);

private:
	std::string id; /*!< RELATION-ID (expected to be a unique value) */
	int version; /*!< object version */
	std::string changeset; /*!< ChangeSet ID */
	std::string user; /*!< name of the User defining this RELATION */
	int uid; /*!< corresponding numeric UserID */
	std::string timestamp; /*!< when this RELATION was defined */
	int member_count;	/*!< number of associated MEMBERs (may be zero) */
	std::vector <Myreadosm_member> members; /*!< array of MEMBER objects (may be NULL) */
	int tag_count; /*!< number of associated TAGs (may be zero) */
	std::vector<Myreadosm_tag> tags; /*!< array of TAG objects (may be NULL) */
};

