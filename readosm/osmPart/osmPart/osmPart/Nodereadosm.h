#pragma once

#include <string>
#include <vector>

#include "readosm.h"
#include "Myreadosm_tag.h"
#include "Point.h"

class Nodereadosm
{
public:
	Nodereadosm();
	~Nodereadosm();

	std::string getId(void);
	void setId(std::string);
	
	double getLatitude(void);
	void setLatitude(double);
	
	double getLongitude(void);
	void setLongitude(double);
	
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
	
	int getTag_count(void);
	void setTag_count(int);
	
	std::vector<Myreadosm_tag> getTags(void);
	void setTags(std::vector<Myreadosm_tag>);
	void appendTags(Myreadosm_tag);

	void affiche(void);

	double getEle(void);
	void setEle(double);

	Point getPos(void);
	void setPos(Point);

	// set the pos and the ele with this tag informations already exist
	void setPosEle(void);


private:
	std::string id = "-1"; /**< NODE-ID (expected to be a unique value) */
	double latitude = -1; /**< geographic latitude */
	double longitude = -1;	/**< geographic longitude */
	int version = -1; /**< object version */
	std::string changeset ="-1"; /**< ChangeSet ID */
	std::string user; /**< name of the User defining this NODE */
	int uid = -1; /**< corresponding numeric UserID */
	std::string timestamp = "-1"; /**< when this NODE was defined */
	int tag_count = -1; /**< number of associated TAGs (may be zero) */
	std::vector<Myreadosm_tag> tags; /**< array of TAG objects (may be NULL) */

	// ele of point, if the "ele" is equal to 0 is because the ele is not defined in the osm file
	double ele;
	Point pos;

};

