#pragma once

/*!
 * \file Nodereadosm.h
 * \brief class representing a Node
 * \author Ludovic
 */

#include <string>
#include <vector>

#include "readosm.h"
#include "Myreadosm_tag.h"
#include "Point.h"


 /*! \class Nodereadosm
 * \brief class representing a Node
 */
class Nodereadosm
{
public:
	/*!
	*  \brief Constructor
	*
	*  Constructor of the class Nodereadosm.
	*/
	Nodereadosm();

	/*!
	*  \brief Destructor
	*
	*  Destructor of the class Nodereadosm.
	*/
	~Nodereadosm();

	/*!
	*  \brief Get the Node id
	*
	*  \return The Node id
	*/
	std::string getId(void);
	
	/*!
	*  \brief Set the Node id
	*
	*  \param The Node id
	*/
	void setId(std::string);


	/*!
	*  \brief Get the latitude of the Node
	*
	*  \return The latitude of the Node
	*/
	double getLatitude(void);
	
	/*!
	*  \brief Set the latitude of the Node
	*
	*  \param The latitude of the Node
	*/
	void setLatitude(double);
	
	/*!
	*  \brief Get the longitude of the Node
	*
	*  \return The longitude of the Node
	*/
	double getLongitude(void);
	
	/*!
	*  \brief Set the longitude of the Node
	*
	*  \param  The longitude of the Node
	*/
	void setLongitude(double);
	
	/*!
	*  \brief Get the version of the Node
	*
	*  \return The version of the Node
	*/
	int getVersion(void);
	
	/*!
	*  \brief Set the version of the Node
	*
	*  \param The version of the Node
	*/
	void setVersion(int);
	
	/*!
	*  \brief Get the changeset of the Node
	*
	*  \return The changeset of the Node
	*/
	std::string getChangeset(void);
	
	/*!
	*  \brief Set the changeset of the Node
	*
	*  \param The changeset of the Node
	*/
	void setChangeset(std::string);
	
	/*!
	*  \brief Get the user of the Node
	*
	*  \return The user of the Node
	*/
	std::string getUser(void);
	
	/*!
	*  \brief Set the user of the Node
	*
	*  \return The user of the Node
	*/
	void setUser(std::string);
	
	/*!
	*  \brief Get the uid of the Node
	*
	*  \return The uid of the Node
	*/
	int getUid(void);
	
	/*!
	*  \brief Set the uid of the Node
	*
	*  \return The uid of the Node
	*/
	void setUid(int);

	/*!
	*  \brief Get the timestamp of the Node
	*
	*  \return The timestamp of the Node
	*/
	std::string getTimestamp(void);
	
	/*!
	*  \brief Set the timestamp of the Node
	*
	*  \return The timestamp of the Node
	*/
	void setTimestamp(std::string);
	
	/*!
	*  \brief Get the tag_count of the Node
	*
	*  \return The tag_count of the Node
	*/
	int getTag_count(void);
	
	/*!
	*  \brief Set the tag_count of the Node
	*
	*  \return The tag_count of the Node
	*/
	void setTag_count(int);
	
	/*!
	*  \brief Get the tags of the Node
	*
	*  \return The tags of the Node
	*/
	std::vector<Myreadosm_tag> getTags(void);
	
	/*!
	*  \brief Set the tags of the Node
	*
	*  \return The tags of the Node
	*/
	void setTags(std::vector<Myreadosm_tag>);

	/*!
	*  \brief Append a tag to the list of tags of the Node
	*
	*  \param The tag to add to the Node
	*/
	void appendTags(Myreadosm_tag);

	/*!
	*  \brief Display node informations on standard output
	*
	*/
	void affiche(void);

	/*!
	*  \brief Get the "ele" of the node (the elevation in meters)
	*
	*  \return The "ele" of the node (the elevation in meters)
	*/
	double getEle(void);
	
	/*!
	*  \brief Set the "ele" of the node (the elevation in meters)
	*
	*  \return The "ele" of the node (the elevation in meters)
	*/
	void setEle(double);

	/*!
	*  \brief Get the pos of the node
	*
	*  \return The pos of the node
	*/
	Point getPos(void);
	
	/*!
	*  \brief Set the pos of the node
	*
	*  \return The pos of the node
	*/
	void setPos(Point);

	/*!
	*  \brief Set pos and the ele with the tag which informations already exist
	*
	*/
	void setPosEle(void);


private:
	std::string id = "-1"; /*!< NODE-ID (expected to be a unique value) */
	double latitude = -1; /*!< geographic latitude */
	double longitude = -1;	/*!< geographic longitude */
	int version = -1; /*!< object version */
	std::string changeset ="-1"; /*!< ChangeSet ID */
	std::string user; /*!< name of the User defining this NODE */
	int uid = -1; /*!< corresponding numeric UserID */
	std::string timestamp = "-1"; /*!< when this NODE was defined */
	int tag_count = -1; /*!< number of associated TAGs (may be zero) */
	std::vector<Myreadosm_tag> tags; /*!< array of TAG objects (may be NULL) */

	// 
	double ele; /*!< ele of point, if the "ele" is equal to 0 is because the ele is not defined in the osm file */
	Point pos; /*!< pos of poinr, it depend of the latitude, longitude and ele */

};

