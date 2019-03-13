#pragma once

/*!
 * \file OsmWithreadosm.h
 * \brief OSM File Parser
 * \author Ludovic
 */

#include "Nodereadosm.h"
#include "Wayreadosm.h"
#include "Relationreadosm.h"

#include <vector>
#include <string>

typedef std::vector<std::vector<std::string> > Arbo;

/*! \class OsmWithreadosm
* \brief class representing the osm parser
*
*  The class allows to parse an osm file without worrying about the opening or the closing of this one,
*  a default osm file is given, but the user can always change it.
*  The parsing is done either on the category or on the position (latitude and longitude)
*  The user can obtain a complete list of the objects [Nodes, Ways or Relation] obtained during the parsing,
*  via an id or via a more detailed search.
*/
class OsmWithreadosm
{
public:

	/*!
	 *  \brief Constructor
	 *
	 *  Constructor of the class OsmWithreadosm, this one allows to give a file by default
	 */
	OsmWithreadosm();

	/*!
	*  \brief Destructor
	*
	*  Destructor of the class OsmWithreadosm
	*/
	~OsmWithreadosm();

	// Donne une arboressence
	//Arbo getArbo();

	/*!
	 *  \brief Parse and store in RAM according to the given category.
	 *
	 *  Method that parses the osm file and stores the objects according to their category,
	 *  category given by the user via std :: vector <std :: string> where the first field of the vector
	 *  is the key of the category, and the second its value, the accepted keys and values are given in the file "arbo.txt"
	 *  or on the official OSM (wiki) site on this page  "https://wiki.openstreetmap.org/wiki/Map_Features".
	 *  To perform a keyless or valueless search, it is necessary to put the character string empty for one of the two parameters
	 *  <"", "something"> look for the "something" value associated with any key, for example.
	 *
	 *  \param Nodes : search category for Nodes.
	 *  \param Ways : search category for Way.
	 *  \param Relations : search category for Relation.
	 */
	void initWithCat(std::vector<std::string>, std::vector<std::string>, std::vector<std::string>);

	//Init with the latitude and the longitude
	//void initWithll(double, double);

	/*!
	 *  \brief Allows you to change osm files to parser. Do not forget to parse it with initWithCat.
	 *
	 *  \param fileOsm : OSM file
	 */
	void changeFile(std::string);

	/*!
	 *  \brief Get a Node via his id
	 *
	 *  Method that allows to obtain a node via the given id,
	 *  if the bool is "true", the code will look for the object having the id in the osm file,
	 *  if it does not find it in the vector (the one in variable menbre).
	 *  The bool is false by default.
	 *
	 *  \param id : id of the Node.
	 *  \param flag : flag described above.
	 *  \return the node wanted by the user if it exists,
	 *  otherwise it returns the blank node.
	 */
	Nodereadosm getNodeWithId(std::string, bool);
	
	/*!
	 *  \brief Get a Way via his id
	 *
	 *  Method that allows to obtain a Way via the given id,
	 *  if the bool is "true", the code will look for the object having the id in the osm file,
	 *  if it does not find it in the vector (the one in variable menbre).
	 *  The bool is false by default.
	 *
	 *  \param id : id of the Way.
	 *  \param flag : flag described above.
	 *  \return the Way wanted by the user if it exists,
	 *  otherwise it returns the blank Way.
	 */
	Wayreadosm getWayWithId(std::string, bool);

	/*!
	 *  \brief Get a Relation via his id
	 *
	 *  Method that allows to obtain a Relation via the given id,
	 *  if the bool is "true", the code will look for the object having the id in the osm file,
	 *  if it does not find it in the vector (the one in variable menbre).
	 *  The bool is false by default.
	 *
	 *  \param id : id of the Relation.
	 *  \param flag : flag described above.
	 *  \return the Relation wanted by the user if it exists,
	 *  otherwise it returns the blank Relation.
	 */
	Relationreadosm getRelationWithId(std::string, bool);

	/*!
	 *  \brief Get all the Nodes stored in RAM
	 *
	 *  \return The Nodes stored in RAM
	 */
	std::vector<Nodereadosm> getNodes(void);
	
	/*!
	 *  \brief Get all the Ways stored in RAM
	 *
	 *  \return the Ways stored in RAM
	 */
	std::vector <Wayreadosm> getWays(void);
	
	/*!
	 *  \brief Get all the Relations stored in RAM
	 *
	 *  \return The Relations stored in RAM
	 */
	std::vector <Relationreadosm> getRelations(void);

	// Méthodes pour obtenir des objets du fichier qui répondent à TOUTES les conditions
	//std::vector<Nodereadosm> getNodesWithSpecAnd(Id);
	//std::vector<Wayreadosm> getWaysWithSpecAnd(Id);
	//std::vector<Relationreadosm> getRelationsWithSpecAnd(Id);

	// Méthodes pour obtenir des objets du fichier qui répondent à UNE OU PLUSIEURS des conditions
	//std::vector<Nodereadosm> getNodesWithSpecOr(Id);
	//std::vector<Wayreadosm> getWaysWithSpecOr(Id);
	//std::vector<Relationreadosm> getRelationsWithSpecOr(Id);

private:

	Arbo arbo;

	std::vector<Nodereadosm> nodes; /*!< Nodes stored during the parse phase*/
	std::vector<Wayreadosm> ways; /*!< Ways stored during the parse phase*/
	std::vector<Relationreadosm> relations; /*!< Relations stored during the parse phase*/

	std::string fileOsm; /*!< OSM file to parser*/
};

