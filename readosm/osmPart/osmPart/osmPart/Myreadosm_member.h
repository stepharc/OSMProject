#pragma once

/*!
 * \file Myreadosm_member.h
 * \brief class representing a member of a Relation
 * \author Ludovic
 */

#include <string>

 /**
  * \enum Member_type
  * \brief To know if the member is a Node, Way or Relation.
  */
enum Member_type { 
	Node, /*!< The member is a node. */
	Way, /*!< The member is a way. */
	Relation /*!< The member is a relation. */
};


/*! \class Myreadosm_member
* \brief class representing a member of a Relation
*/
class Myreadosm_member
{
public:
	/*!
	*  \brief Constructor
	*
	*  Constructor of the class Myreadosm_member.
	*/
	Myreadosm_member();

	/*!
	*  \brief Destructor 
	*
	*  Destructor of the class Myreadosm_member.
	*/
	~Myreadosm_member();

	/*!
	*  \brief Get the type of the member (Node, Way or Relation).
	*
	*  \return The type of the member (Node, Way or Relation).
	*/
	Member_type getMember_type(void);
	
	/*!
	*  \brief Set the type of the member (Node, Way or Relation).
	*
	*  \param The type of the member (Node, Way or Relation).
	*/
	void setMember_type(Member_type);

	/*!
	*  \brief Get member id
	*
	*  \return Member id
	*/
	std::string getId(void);
	
	/*!
	*  \brief Set the id of the member
	*
	*  \param Member id
	*/
	void setId(std::string);

	/*!
	*  \brief Get the member's role
	*
	*  \return The role of the member
	*/
	std::string getRole(void);
	
	/*!
	*  \brief Set the role of the member
	*
	*  \param The role of the member
	*/
	void setRole(std::string);

private:
	Member_type member_type; /*!< can be one of: READOSM_MEMBER_NODE, READOSM_MEMBER_WAY or READOSM_MEMBER_RELATION */
	std::string id; /*!< ID-value identifying the referenced object */
	std::string role; /*!< intended role for this reference */
};

