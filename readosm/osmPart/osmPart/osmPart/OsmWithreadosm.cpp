#define _CRT_SECURE_NO_WARNINGS

#include "OsmWithreadosm.h"

#include "Nodereadosm.h"
#include "Wayreadosm.h"
#include "Relationreadosm.h"

#include "Myreadosm_tag.h"
#include "Myreadosm_member.h"

#include <cstdio>
#include "readosm.h"
#include <iostream>
#include <cstring>

#include <vector>
#include <string>
#include <map>

// pour la lecture des fichier non osm
#include <fstream>

// "node" qui sera intégré a l'objet

/*L'idée est d'ouvrir une et une seule fois le fichier et de modifier des objet stocker dans des variables*/

// il y a tellement de donnéess qu'il faut stocker sa sur plusieurs vecteur
static std::vector<Nodereadosm> myNodes;
static std::vector<Wayreadosm> myWays;
static std::vector<Relationreadosm> myRelations;


// le filtre

// Sera extrait du fichier osm, tt les nodes ayant ce tags est cette value
std::string nodesKey;
std::string nodesValue;

// Sera extrait du fichier osm, tt les ways ayant ce tags est cette value
std::string waysKey;
std::string waysValue;

// Sera extrait du fichier osm, tt les relations ayant ce tags est cette value
std::string relationsKey;
std::string relationsValue;

	
// Fonction pour écrire les information d'un node dans un fichier

// Cette fonction ouvre, écrit puis ferme le fichier
void oNode(std::string fic, Nodereadosm myNode)
{
	// Ouverture du fichier, le fichier est créé s'il n'existe pas,
		// mais son contenu n'est pas supprimé s'il existe
	std::ofstream NodeFile(fic, std::ios::out | std::ios::app);

	if (NodeFile)
	{

		NodeFile << myNode.getId() << std::endl;

		NodeFile << myNode.getLatitude() << std::endl;

		NodeFile << myNode.getLongitude() << std::endl;

		NodeFile << myNode.getVersion() << std::endl;

		NodeFile << myNode.getChangeset() << std::endl;

		NodeFile << myNode.getUser() << std::endl;

		NodeFile << myNode.getUid() << std::endl;

		NodeFile << myNode.getTimestamp() << std::endl;

		NodeFile << myNode.getTag_count() << std::endl;

		for (Myreadosm_tag tag : myNode.getTags())
		{
			NodeFile << tag.getKey() << std::endl;
			NodeFile << tag.getValue() << std::endl;
		}

		NodeFile.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}

// Cette fonction renvoie l'ensble des clés de "myNode" présent dans le fichier "arbo"
// Avec leurs valeure correct associée (ex: < < "waterway", "stream" >, < "building" >, etc.>
std::vector< std::vector<std::string>> iArboKey(std::string arbo, Nodereadosm myNode)
{
	// Ligne actuelle dans la lecture du fichier
	std::string ligne;

	// Nom d'une clé valide pour une catégorie
	std::string keyArbo;

	// Nom d'une valeur valide pour une catégorie
	std::string valueArbo;

	// liste des tag valide présent dans le node, contient
	// la liste des clés valide présente dans le node
	// la liste des valeures valide présente dans le node
	// de la forme < <key, value>, <key, value>, <key, value> >
	// si un des deux champs n'est pas indiqué, alors le node n'ont possède pas
	std::vector< std::vector<std::string>> tagsOK;

	// un tag temporaire, qui sera par la suite rajouté à tagsOK
	std::vector<std::string> tagOK;

	// si le flag vaut true, une valeure valide a était troue
	//bool flag = false;

	std::ifstream fichier(arbo, std::ios::in);

	if (fichier)
	{

		// parcourir chaque ligne du fichier arbo
		while (getline(fichier, ligne))
		{
			// Si la ligne contient "Node"
			//    : ligne.find("Node") != std::string::npos
			// et qu'elle ne contient pas "\t"
			//    : ligne.find("\t") == std::string::npos
			// alors la ligne courrante indique le nom d'une clé pour un node
			if ((ligne.find("Node") != std::string::npos)
				&& (ligne.find("\t") == std::string::npos))
			{
				// On prend la catégorie de la clé
				// Dans le fichier arbo, une clé est carractérisé
				// par le premier mot de la phrase suivi de " [" 
				keyArbo = ligne.substr(0, ligne.find(" [") - 1);

				// On analyse les tags du node donné
				for (Myreadosm_tag tag : myNode.getTags())
					// Si le tag du node est valide
					if (tag.getKey() == keyArbo)
					{
						// Alors on stock cette clé dans la liste des clé valide
						tagOK.push_back(tag.getKey());

						// On cherche les valeure asscié
						getline(fichier, ligne);

						// Si la ligne contient "\t[Node]"
						if (ligne.find("\t[Node]") != std::string::npos)
							while (true)
							{
								getline(fichier, ligne);
								if (ligne.find("\t\t") != std::string::npos)
								{
									// On stocke la valeur
									valueArbo = ligne.substr(ligne.find("\t\t") + std::string("\t\t").size());

									// Si le tag du node est valide
									if (tag.getValue() == valueArbo)
										// Alors on stock ce tag dans la liste des tags valide
										tagOK.push_back(tag.getValue());

									tagsOK.push_back(tagOK);

									// réinitialisation de tagOK
									tagOK.clear();
									tagOK.push_back(tag.getKey());
								}
								else
									break;
							}
					}

			}
		}

		fichier.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

	return tagsOK;
}

// Cette fonction insert un node dans les fichier adéquat en fonction de sa catégorie

static int
insertNode(const void *user_data, const readosm_node * node)
{

	char buf[128];
	int i;
	const readosm_tag *tag;

	Nodereadosm myNode;
	Myreadosm_tag myTag;

	std::vector< std::vector<std::string> > tagsOK;

	std::string fic;

	if (user_data != NULL)
		user_data = NULL;	/* silencing stupid compiler warnings */

	// Sauvegarde du node en RAM via l'objet Nodereadosm

#if defined(_WIN32) || defined(__MINGW32__)
	/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
	sprintf(buf, "%I64d", node->id);
#else
	sprintf(buf, "%lld", node->id);
#endif

	myNode.setId(buf);

	if (node->latitude != READOSM_UNDEFINED)
		myNode.setLatitude(node->latitude);

	if (node->longitude != READOSM_UNDEFINED)
		myNode.setLongitude(node->longitude);

	if (node->version != READOSM_UNDEFINED)
		myNode.setVersion(node->version);

	if (node->changeset != READOSM_UNDEFINED)
	{
#if defined(_WIN32) || defined(__MINGW32__)
		/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
		sprintf(buf, "%I64d", node->changeset);
#else
		sprintf(buf, "%lld", node->changeset);
#endif
		myNode.setChangeset(buf);
	}

	if (node->user != NULL)
		myNode.setUser(node->user);

	if (node->uid != READOSM_UNDEFINED)
		myNode.setUid(node->uid);

	if (node->timestamp != NULL)
		myNode.setTimestamp(node->timestamp);

	myNode.setTag_count(node->tag_count);

	if (node->tag_count > 0)
		for (i = 0; i < node->tag_count; i++)
		{
			tag = node->tags + i;
			myTag.setKey(tag->key);
			myTag.setValue(tag->value);
			myNode.appendTags(myTag);
		}

	// Stockage du node dans la bonne catégorie en mémoire masse

	// Stocke, pour commencer le node courrant dans le fichier des nodes
	fic = ".\\..\\..\\..\\Arbo\\Node.txt";
	oNode(fic, myNode);

	//puis, si sa catégorie est rensigné on stocke dans le fichier plus précis

	// On test la clé pour commencer
	tagsOK = iArboKey(".\\..\\..\\..\\arbo.txt", myNode);
	for (std::vector<std::string> tagOK : tagsOK)
	{
		if (tagOK.size() == 1)
		{
			fic = ".\\..\\..\\..\\Arbo\\Node:" + tagOK[0] + ".txt";
			oNode(fic, myNode);
		}
		if (tagOK.size() == 2)
		{
			fic = ".\\..\\..\\..\\Arbo\\Node:" + tagOK[0] + ":" + tagOK[1] + ".txt";
			oNode(fic, myNode);
		}
	}


	return READOSM_OK;
}

// Constructeur / déstructeur

OsmWithreadosm::OsmWithreadosm()
{
	this->fileOsm = "..\\..\\..\\var.osm";



}


OsmWithreadosm::~OsmWithreadosm()
{
}


static int
extractNode(const void *user_data, const readosm_node * node)
{

	char buf[128];
	int i;
	const readosm_tag *tag;

	Nodereadosm myNode;
	Myreadosm_tag myTag;

	if (user_data != NULL)
		user_data = NULL;	/* silencing stupid compiler warnings */

	// si le flag vaut "true" alors le node courrant est ce que veut l'utilisateur
	bool flag = false;

	myNode.setTag_count(node->tag_count);

	if (node->tag_count > 0)
		for (i = 0; i < node->tag_count; i++)
		{
			tag = node->tags + i;
			myTag.setKey(tag->key);
			myTag.setValue(tag->value);
			myNode.appendTags(myTag);
			// la key puis la value est bon
			if (myTag.getKey() == ::nodesKey || ::nodesKey.size() == 0)
			{
				if (myTag.getValue() == ::nodesValue || ::nodesValue.size() == 0)
				{
					flag = true;
				}
			}
		}

	if (flag == false)
		return READOSM_OK;

#if defined(_WIN32) || defined(__MINGW32__)
	/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
	sprintf(buf, "%I64d", node->id);
#else
	sprintf(buf, "%lld", node->id);
#endif

	myNode.setId(buf);

	if (node->latitude != READOSM_UNDEFINED)
		myNode.setLatitude(node->latitude);

	if (node->longitude != READOSM_UNDEFINED)
		myNode.setLongitude(node->longitude);

	if (node->version != READOSM_UNDEFINED)
		myNode.setVersion(node->version);

	if (node->changeset != READOSM_UNDEFINED)
	{
#if defined(_WIN32) || defined(__MINGW32__)
		/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
		sprintf(buf, "%I64d", node->changeset);
#else
		sprintf(buf, "%lld", node->changeset);
#endif
		myNode.setChangeset(buf);
	}

	if (node->user != NULL)
		myNode.setUser(node->user);

	if (node->uid != READOSM_UNDEFINED)
		myNode.setUid(node->uid);

	if (node->timestamp != NULL)
		myNode.setTimestamp(node->timestamp);

	myNode.setPosEle();

	::myNodes.push_back(myNode);

	return READOSM_OK;
}

static int
extractWay(const void *user_data, const readosm_way * way)
{
	char buf[128];
	int i;
	const readosm_tag *tag;

	Wayreadosm myWay;
	Myreadosm_tag myTag;


	if (user_data != NULL)
		user_data = NULL;	/* silencing stupid compiler warnings */

	// si le flag vaut "true" alors le way courrant est ce que veut l'utilisateur
	bool flag = false;

	myWay.setTag_count(way->tag_count);

	if (way->tag_count > 0)
		for (i = 0; i < way->tag_count; i++)
		{
			tag = way->tags + i;
			myTag.setKey(tag->key);
			myTag.setValue(tag->value);
			myWay.appendTags(myTag);
			// la key puis la value est bon
			if (myTag.getKey() == ::waysKey || ::waysKey.size() == 0)
			{
				if (myTag.getValue() == ::waysValue || ::waysValue.size() == 0)
				{
					flag = true;
				}
			}
		}

	if (flag == false)
		return READOSM_OK;


#if defined(_WIN32) || defined(__MINGW32__)
	/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
	sprintf(buf, "%I64d", way->id);
#else
	sprintf(buf, "%lld", way->id);
#endif
	myWay.setId(buf);

	if (way->version != READOSM_UNDEFINED)
		myWay.setVersion(way->version);
	if (way->changeset != READOSM_UNDEFINED)
	{
#if defined(_WIN32) || defined(__MINGW32__)
		/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
		sprintf(buf, "%I64d", way->changeset);
#else
		sprintf(buf, "%lld", way->changeset);
#endif
		myWay.setChangeset(buf);
	}

	if (way->user != NULL)
		myWay.setUser(way->user);
	if (way->uid != READOSM_UNDEFINED)
		myWay.setUid(way->uid);
	if (way->timestamp != NULL)
		myWay.setTimestamp(way->timestamp);

	myWay.setNode_ref_count(way->node_ref_count);

	if (way->node_ref_count > 0)
		for (i = 0; i < way->node_ref_count; i++)
		{
			/* we'll now print each <nd ref> for this way */
#if defined(_WIN32) || defined(__MINGW32__)
		/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
			sprintf(buf, "%I64d", *(way->node_refs + i));
#else
			sprintf(buf, "%lld", *(way->node_refs + i));
#endif
			myWay.appendNode_refs(buf);
		}

	::myWays.push_back(myWay);

	return READOSM_OK;
}

static int
extractRelation(const void *user_data, const readosm_relation * relation)
{
	char buf[128];
	int i;
	const readosm_member *member;
	const readosm_tag *tag;

	Relationreadosm myRelation;
	Myreadosm_tag myTag;
	Myreadosm_member myMember;

	if (user_data != NULL)
		user_data = NULL;	/* silencing stupid compiler warnings */

	// si le flag vaut "true" alors la relation courrante est ce que veut l'utilisateur
	bool flag = false;

	myRelation.setTag_count(relation->tag_count);

	if (relation->tag_count > 0)
		for (i = 0; i < relation->tag_count; i++)
		{
			tag = relation->tags + i;
			myTag.setKey(tag->key);
			myTag.setValue(tag->value);
			myRelation.appendTags(myTag);
			// la key puis la value est bon
			if (myTag.getKey() == ::relationsKey || ::relationsKey.size() == 0)
			{
				if (myTag.getValue() == ::relationsValue || ::relationsValue.size() == 0)
				{
					flag = true;
				}
			}
		}

	if (flag == false)
		return READOSM_OK;

#if defined(_WIN32) || defined(__MINGW32__)
	/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
	sprintf(buf, "%I64d", relation->id);
#else
	sprintf(buf, "%lld", relation->id);
#endif
	myRelation.setId(buf);

	if (relation->version != READOSM_UNDEFINED)
		myRelation.setVersion(relation->version);
	if (relation->changeset != READOSM_UNDEFINED)
	{
#if defined(_WIN32) || defined(__MINGW32__)
		/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
		sprintf(buf, "%I64d", relation->changeset);
#else
		sprintf(buf, "%lld", relation->changeset);
#endif
		myRelation.setChangeset(buf);
	}

	if (relation->user != NULL)
		myRelation.setUser(relation->user);
	if (relation->uid != READOSM_UNDEFINED)
		myRelation.setUid(relation->uid);
	if (relation->timestamp != NULL)
		myRelation.setTimestamp(relation->timestamp);

	myRelation.setMember_count(relation->member_count);

	if (relation->member_count > 0)
		for (i = 0; i < relation->member_count; i++)
		{
			/* we'll now print each <member> for this way */
			member = relation->members + i;
#if defined(_WIN32) || defined(__MINGW32__)
			/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
			sprintf(buf, "%I64d", member->id);
#else
			sprintf(buf, "%lld", member->id);
#endif
			/* any <member> may be of "node", "way" or "relation" type */
			switch (member->member_type)
			{
			case READOSM_MEMBER_NODE:
				myMember.setMember_type(Node);
				break;
			case READOSM_MEMBER_WAY:
				myMember.setMember_type(Way);
				break;
			case READOSM_MEMBER_RELATION:
				myMember.setMember_type(Relation);
				break;
			default:
				break;
			};
			myMember.setId(buf);
			if (member->role != NULL)
				myMember.setRole(member->role);

			myRelation.appendMembers(myMember);

		}

	::myRelations.push_back(myRelation);

	return READOSM_OK;
}

void OsmWithreadosm::initWithCat(std::vector<std::string> nodes, std::vector<std::string> ways, std::vector<std::string> relations)
{
	::nodesKey = nodes[0];
	::nodesValue = nodes[1];

	::waysKey = ways[0];
	::waysValue = ways[1];

	::relationsKey = relations[0];
	::relationsValue = relations[1];


	const void *osmHandle;
	int ret;

	// STEP #1: opening the OSM file
	ret = readosm_open(this->fileOsm.c_str(), &osmHandle);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "OPEN error: %d\n", ret);
		readosm_close(osmHandle);
	}

	// STEP #2: parsing the OSM file, and extract interesing information
	ret =
		readosm_parse(osmHandle, (const void *)0, extractNode, extractWay,
			extractRelation);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "PARSE error: %d\n", ret);
		readosm_close(osmHandle);
	}

	// STEP #3: closing the OSM file
	readosm_close(osmHandle);

	// Stockage des valeurs dans l'objet
	this->nodes = ::myNodes;
	this->ways = ::myWays;
	this->relations = ::myRelations;
}

void OsmWithreadosm::changeFile(std::string fileOsm)
{
	this->fileOsm = fileOsm;
}

// Arboressence
/*
Arbo OsmWithreadosm::getArbo()
{

}
*/

static Nodereadosm myNodeWithId;
std::string id;

static int
extractNodeWithId(const void *user_data, const readosm_node * node)
{

	char buf[128];
	int i;
	const readosm_tag *tag;

	Nodereadosm myNode;
	Myreadosm_tag myTag;

	if (user_data != NULL)
		user_data = NULL;	/* silencing stupid compiler warnings */

#if defined(_WIN32) || defined(__MINGW32__)
	/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
	sprintf(buf, "%I64d", node->id);
#else
	sprintf(buf, "%lld", node->id);
#endif

	myNode.setId(buf);

	if (node->latitude != READOSM_UNDEFINED)
		myNode.setLatitude(node->latitude);

	if (node->longitude != READOSM_UNDEFINED)
		myNode.setLongitude(node->longitude);

	if (node->version != READOSM_UNDEFINED)
		myNode.setVersion(node->version);

	if (node->changeset != READOSM_UNDEFINED)
	{
#if defined(_WIN32) || defined(__MINGW32__)
		/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
		sprintf(buf, "%I64d", node->changeset);
#else
		sprintf(buf, "%lld", node->changeset);
#endif
		myNode.setChangeset(buf);
	}

	if (node->user != NULL)
		myNode.setUser(node->user);

	if (node->uid != READOSM_UNDEFINED)
		myNode.setUid(node->uid);

	if (node->timestamp != NULL)
		myNode.setTimestamp(node->timestamp);

	myNode.setTag_count(node->tag_count);

	if (node->tag_count > 0)
		for (i = 0; i < node->tag_count; i++)
		{
			tag = node->tags + i;
			myTag.setKey(tag->key);
			myTag.setValue(tag->value);
			myNode.appendTags(myTag);
		}

	myNode.setPosEle();

	if(myNode.getId() == ::id)
		::myNodeWithId = myNode;

	return READOSM_OK;
}

static int
extractNoNodes(const void *user_data, const readosm_node * node)
{
	return READOSM_OK;
}

static int
extractNoWays(const void *user_data, const readosm_way * way)
{
	return READOSM_OK;
}

static int
extractNoRelations(const void *user_data, const readosm_relation * relation)
{
	return READOSM_OK;
}

// id
Nodereadosm OsmWithreadosm::getNodeWithId(std::string id, bool flag = false)
{
	::id = id;
	std::string N;
	for (Nodereadosm node : this->nodes)
	{
			N = node.getId();
			if (id == N)
				return node;
	}

	if (flag == false)
	{
		Nodereadosm nodeErr;
		return nodeErr;
	}

	const void *osmHandle;
	int ret;

	// STEP #1: opening the OSM file
	ret = readosm_open(this->fileOsm.c_str(), &osmHandle);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "OPEN error: %d\n", ret);
		readosm_close(osmHandle);
	}

	// STEP #2: parsing the OSM file, and extract interesing information
	ret =
		readosm_parse(osmHandle, (const void *)0, extractNodeWithId, extractNoWays,
			extractNoRelations);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "PARSE error: %d\n", ret);
		readosm_close(osmHandle);
	}

	// STEP #3: closing the OSM file
	readosm_close(osmHandle);

	return ::myNodeWithId;
}

static Wayreadosm myWayWithId;

static int
extractWayWithId(const void *user_data, const readosm_way * way)
{
	char buf[128];
	int i;
	const readosm_tag *tag;

	Wayreadosm myWay;
	Myreadosm_tag myTag;


	if (user_data != NULL)
		user_data = NULL;	/* silencing stupid compiler warnings */

#if defined(_WIN32) || defined(__MINGW32__)
	/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
	sprintf(buf, "%I64d", way->id);
#else
	sprintf(buf, "%lld", way->id);
#endif
	myWay.setId(buf);

	if (way->version != READOSM_UNDEFINED)
		myWay.setVersion(way->version);
	if (way->changeset != READOSM_UNDEFINED)
	{
#if defined(_WIN32) || defined(__MINGW32__)
		/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
		sprintf(buf, "%I64d", way->changeset);
#else
		sprintf(buf, "%lld", way->changeset);
#endif
		myWay.setChangeset(buf);
	}

	if (way->user != NULL)
		myWay.setUser(way->user);
	if (way->uid != READOSM_UNDEFINED)
		myWay.setUid(way->uid);
	if (way->timestamp != NULL)
		myWay.setTimestamp(way->timestamp);

	myWay.setNode_ref_count(way->node_ref_count);

	if (way->node_ref_count > 0)
		for (i = 0; i < way->node_ref_count; i++)
		{
			/* we'll now print each <nd ref> for this way */
#if defined(_WIN32) || defined(__MINGW32__)
		/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
			sprintf(buf, "%I64d", *(way->node_refs + i));
#else
			sprintf(buf, "%lld", *(way->node_refs + i));
#endif
			myWay.appendNode_refs(buf);
		}

	myWay.setTag_count(way->tag_count);

	if (way->tag_count > 0)
		for (i = 0; i < way->tag_count; i++)
		{
			tag = way->tags + i;
			myTag.setKey(tag->key);
			myTag.setValue(tag->value);
			myWay.appendTags(myTag);
		}

	if (myWay.getId() == ::id)
		::myWayWithId = myWay;

	return READOSM_OK;
}


Wayreadosm OsmWithreadosm::getWayWithId(std::string id, bool flag = false)
{
	::id = id;
	std::string W;
	for (Wayreadosm way : ways)
	{
		W = way.getId();
		if (id == W)
			return way;
	}

	if (flag == false)
	{
		Wayreadosm wayErr;
		return wayErr;
	}

	const void *osmHandle;
	int ret;

	// STEP #1: opening the OSM file
	ret = readosm_open(this->fileOsm.c_str(), &osmHandle);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "OPEN error: %d\n", ret);
		readosm_close(osmHandle);
	}

	// STEP #2: parsing the OSM file, and extract interesing information
	ret =
		readosm_parse(osmHandle, (const void *)0, extractNoNodes, extractWayWithId,
			extractNoRelations);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "PARSE error: %d\n", ret);
		readosm_close(osmHandle);
	}

	// STEP #3: closing the OSM file
	readosm_close(osmHandle);

	return ::myWayWithId;
	
}

static Relationreadosm myRelationWithId;

static int
extractRelationWithId(const void *user_data, const readosm_relation * relation)
{
	char buf[128];
	int i;
	const readosm_member *member;
	const readosm_tag *tag;

	Relationreadosm myRelation;
	Myreadosm_tag myTag;
	Myreadosm_member myMember;

	if (user_data != NULL)
		user_data = NULL;	/* silencing stupid compiler warnings */

#if defined(_WIN32) || defined(__MINGW32__)
	/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
	sprintf(buf, "%I64d", relation->id);
#else
	sprintf(buf, "%lld", relation->id);
#endif
	myRelation.setId(buf);

	if (relation->version != READOSM_UNDEFINED)
		myRelation.setVersion(relation->version);
	if (relation->changeset != READOSM_UNDEFINED)
	{
#if defined(_WIN32) || defined(__MINGW32__)
		/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
		sprintf(buf, "%I64d", relation->changeset);
#else
		sprintf(buf, "%lld", relation->changeset);
#endif
		myRelation.setChangeset(buf);
	}

	if (relation->user != NULL)
		myRelation.setUser(relation->user);
	if (relation->uid != READOSM_UNDEFINED)
		myRelation.setUid(relation->uid);
	if (relation->timestamp != NULL)
		myRelation.setTimestamp(relation->timestamp);

	myRelation.setMember_count(relation->member_count);

	if (relation->member_count > 0)
		for (i = 0; i < relation->member_count; i++)
		{
			/* we'll now print each <member> for this way */
			member = relation->members + i;
#if defined(_WIN32) || defined(__MINGW32__)
			/* CAVEAT - M$ runtime doesn't supports %lld for 64 bits */
			sprintf(buf, "%I64d", member->id);
#else
			sprintf(buf, "%lld", member->id);
#endif
			/* any <member> may be of "node", "way" or "relation" type */
			switch (member->member_type)
			{
			case READOSM_MEMBER_NODE:
				myMember.setMember_type(Node);
				break;
			case READOSM_MEMBER_WAY:
				myMember.setMember_type(Way);
				break;
			case READOSM_MEMBER_RELATION:
				myMember.setMember_type(Relation);
				break;
			default:
				break;
			};
			myMember.setId(buf);
			if (member->role != NULL)
				myMember.setRole(member->role);
		}

	myRelation.setTag_count(relation->tag_count);

	if (relation->tag_count > 0)
		for (i = 0; i < relation->tag_count; i++)
		{
			tag = relation->tags + i;
			myTag.setKey(tag->key);
			myTag.setValue(tag->value);
			myRelation.appendTags(myTag);
		}

	if (myRelation.getId() == ::id)
		::myRelationWithId = myRelation;

	return READOSM_OK;
}



Relationreadosm OsmWithreadosm::getRelationWithId(std::string id, bool flag = false)
{
	::id = id;
	std::string R;
	for (Relationreadosm relation : relations)
	{
		R = relation.getId();
		if (id == R)
			return relation;
	}

	const void *osmHandle;
	int ret;

	// STEP #1: opening the OSM file
	ret = readosm_open(this->fileOsm.c_str(), &osmHandle);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "OPEN error: %d\n", ret);
		readosm_close(osmHandle);
	}

	// STEP #2: parsing the OSM file, and extract interesing information
	ret =
		readosm_parse(osmHandle, (const void *)0, extractNoNodes, extractNoWays,
			extractRelationWithId);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "PARSE error: %d\n", ret);
		readosm_close(osmHandle);
	}

	// STEP #3: closing the OSM file
	readosm_close(osmHandle);

	return ::myRelationWithId;

}


// obtenir tt
std::vector<Nodereadosm> OsmWithreadosm::getNodes(void)
{
	return this->nodes;
}


std::vector <Wayreadosm> OsmWithreadosm::getWays(void)
{
	return this->ways;
}


std::vector <Relationreadosm> OsmWithreadosm::getRelations(void)
{
	return this->relations;
}


// specAnd
/*
std::vector<Nodereadosm> OsmWithreadosm::getNodesWithSpecAnd(Id)
{

}

std::vector<Wayreadosm> OsmWithreadosm::getWaysWithSpecAnd(Id)
{

}

std::vector<Relationreadosm> OsmWithreadosm::getRelationsWithSpecAnd(Id)
{

}
*/

// specOr
/*
std::vector<Nodereadosm> OsmWithreadosm::getNodesWithSpecOr(Id)
{

}
std::vector<Wayreadosm> OsmWithreadosm::getWaysWithSpecOr(Id)
{

}
std::vector<Relationreadosm> OsmWithreadosm::getRelationsWithSpecOr(Id)
{

}
*/