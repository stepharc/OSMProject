#pragma once

/*!
 * \file Myreadosm_tag.h
 * \brief class representing a tag
 * \author Ludovic
 */

#include <string>

 /*! \class Myreadosm_tag
 * \brief class representing a tag
 */
class Myreadosm_tag
{
public:
	/*!
	*  \brief Constructor
	*
	*  Constructor of the class Myreadosm_tag.
	*/
	Myreadosm_tag();

	/*!
	*  \brief Destructor
	*
	*  Destructor of the class Myreadosm_tag.
	*/
	~Myreadosm_tag();

	/*!
	*  \brief Get the key of the tag
	*
	*  \return The key of the tag
	*/
	std::string getKey(void);

	/*!
	*  \brief Set the key of the tag
	*
	*  \param The key of the tag
	*/
	void setKey(std::string);

	/*!
	*  \brief Get the value of the tag
	*
	*  \return The value of the tag
	*/
	std::string getValue(void);
	
	/*!
	*  \brief Set the value of the tag
	*
	*  \param The value of the tag
	*/
	void setValue(std::string);

private:
	std::string key; /*!< the KEY */
	std::string value; /*!< the VALUE */
};

