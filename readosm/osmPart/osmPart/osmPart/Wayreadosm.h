#pragma once

/*!
 * \file Wayreadosm.h
 * \brief class representing a Way
 * \author Ludovic
 */

#include <string>
#include <vector>

#include "readosm.h"
#include "Myreadosm_tag.h"


 /*! \class Wayreadosm
 * \brief class representing a Way
 */
class Wayreadosm
{
public:
	/*!
	*  \brief Constructor
	*
	*  Constructor of the class Wayreadosm.
	*/
	Wayreadosm();

	/*!
	*  \brief Destructor
	*
	*  Destructor of the class Wayreadosm.
	*/
	~Wayreadosm();

	/*!
	*  \brief Get the Way id
	*
	*  \return The Way id
	*/
	std::string getId(void);

	/*!
	*  \brief Set the Way id
	*
	*  \param The Way id
	*/
	void setId(std::string);

	/*!
	*  \brief Get the version of the Way
	*
	*  \return The version of the Way
	*/
	int getVersion(void);

	/*!
	*  \brief Set the version of the Way
	*
	*  \param The version of the Way
	*/
	void setVersion(int);

	/*!
	*  \brief Get the changeset of the Way
	*
	*  \return The changeset of the Way
	*/
	std::string getChangeset(void);

	/*!
	*  \brief Set the changeset of the Way
	*
	*  \param The changeset of the Way
	*/
	void setChangeset(std::string);

	/*!
	*  \brief Get the user of the Way
	*
	*  \return The user of the Way
	*/
	std::string getUser(void);

	/*!
	*  \brief Set the user of the Way
	*
	*  \return The user of the Way
	*/
	void setUser(std::string);

	/*!
	*  \brief Get the uid of the Way
	*
	*  \return The uid of the Way
	*/
	int getUid(void);

	/*!
	*  \brief Set the uid of the Way
	*
	*  \return The uid of the Way
	*/
	void setUid(int);

	/*!
	*  \brief Get the timestamp of the Way
	*
	*  \return The timestamp of the Way
	*/
	std::string getTimestamp(void);

	/*!
	*  \brief Get the node_ref_count of the Way
	*
	*  \return The node_ref_count of the Way
	*/
	int getNode_ref_count(void);

	/*!
	*  \brief Set the node_ref_count of the Way
	*
	*  \return The node_ref_count of the Way
	*/
	void setNode_ref_count(int);

	/*!
	*  \brief Get the node_refs of the Way
	*
	*  \return The node_refs of the Way
	*/
	std::vector<std::string> getNode_refs(void);
	
	/*!
	*  \brief Set the node_refs of the Way
	*
	*  \return The node_refs of the Way
	*/
	void setNode_refs(std::vector<std::string>);
	
	/*!
	*  \brief Append a node_ref to the list of node_refs of the Way
	*
	*  \param The node_ref to add to the Way
	*/
	void appendNode_refs(std::string);

	/*!
	*  \brief Get the tag_count of the Way
	*
	*  \return The tag_count of the Way
	*/
	int getTag_count(void);

	/*!
	*  \brief Set the tag_count of the Way
	*
	*  \return The tag_count of the Way
	*/
	void setTag_count(int);

	/*!
	*  \brief Get the tags of the Way
	*
	*  \return The tags of the Way
	*/
	std::vector<Myreadosm_tag> getTags(void);

	/*!
	*  \brief Set the tags of the Way
	*
	*  \return The tags of the Way
	*/
	void setTags(std::vector<Myreadosm_tag>);

	/*!
	*  \brief Append a tag to the list of tags of the Way
	*
	*  \param The tag to add to the Way
	*/
	void appendTags(Myreadosm_tag);

	/*!
	*  \brief Display Way informations on standard output
	*
	*/
	void affiche(void);

private:
	std::string id = "-1"; /*!< WAY-ID (expected to be a unique value) */
	int version = -1; /*!< object version */
	std::string changeset = "-1"; /*!< ChangeSet ID */
	std::string user = "-1"; /*!< name of the User defining this WAY */
	int uid = -1; /*!< corresponding numeric UserID */
	std::string timestamp = "-1"; /*!< when this WAY was defined */
	int node_ref_count = -1; /*!< number of referenced NODE-IDs (may be zero) */
	std::vector<std::string> node_refs; /*!< array of NODE-IDs (may be NULL) */
	int tag_count = -1; /*!< number of associated TAGs (may be zero) */
	std::vector<Myreadosm_tag> tags; /*!< array of TAG objects (may be NULL) */
};

