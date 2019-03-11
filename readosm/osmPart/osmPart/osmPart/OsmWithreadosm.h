#pragma once

#include "Nodereadosm.h"
#include "Wayreadosm.h"
#include "Relationreadosm.h"

#include <vector>
#include <string>

typedef std::vector<std::vector<std::string> > Arbo;

class OsmWithreadosm

{

public:
	OsmWithreadosm();
	~OsmWithreadosm();

	// Donne une arboressence
	//Arbo getArbo();

	//Init the rechearche argument
	void initWithCat(std::vector<std::string>, std::vector<std::string>, std::vector<std::string>);

	// for change the osm file
	void changeFile(std::string);

	// Méthodes pour obtenir un objet via l'id
	// si flag vaut "true" alors l'objet ayant l'id voulu
	// sera cherché dans le fichier osm s'il n'est pas présent en ram
	Nodereadosm getNodeWithId(std::string, bool);
	Wayreadosm getWayWithId(std::string, bool);
	Relationreadosm getRelationWithId(std::string id, bool);

	// Méthodes pour obtenir tout les objets du fichier, chargé en ram
	std::vector<Nodereadosm> getNodes(void);
	std::vector <Wayreadosm> getWays(void);
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

	// Ceci sont des vecteur de vecteur parce que les fichier osm sont trop volumineux
	std::vector<Nodereadosm> nodes;
	std::vector<Wayreadosm> ways;
	std::vector<Relationreadosm> relations;

	std::string fileOsm;
};

