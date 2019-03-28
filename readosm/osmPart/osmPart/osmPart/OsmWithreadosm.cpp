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

// for reading non osm files
#include <fstream>


/* The idea is to open the file once and only once and to modify objects store in variables */

static std::vector<Nodereadosm> myNodes;
static std::vector<Wayreadosm> myWays;
static std::vector<Relationreadosm> myRelations;


// for filtering with tags

// Will be extracted from the osm file, all the nodes having this tag and this value
std::string nodesKey;
std::string nodesValue;

// Will be extracted from the osm file, all the ways having this tag and this value
std::string waysKey;
std::string waysValue;

// Will be extracted from the osm file, all the relation having this tag and this value
std::string relationsKey;
std::string relationsValue;

	
// Functions for writing node information to a file

// These three functions are for the nodes

// This function opens, writes and closes the file
void oNode(std::string fic, Nodereadosm myNode)
{
	// Opening the file, the file is created if it does not exist,
	// but its content is not deleted if it exists
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
		std::cerr << "Impossible d'ouvrir le fichier ! > " + fic << std::endl;
}

// This function returns the set of keys of "myNode" present in the file "arbo"
// With their associated correct value (ex: <<"waterway", "stream">, <"building">, etc.>
std::vector< std::vector<std::string>> iArboNode(std::string arbo, Nodereadosm myNode)
{
	// Current line in the file reading
	std::string ligne;

	// Name of a valid key for a category
	std::string keyArbo;

	// Name of a valid value for a category
	std::string valueArbo;

	// list of valid tags present in the node, contains
	// the valid key list present in the node
	// the list of valid values present in the node
	// of the form <<key, value>, <key, value>, <key, value>>
	// if one of the two fields is not indicated, then the node does not have
	std::vector< std::vector<std::string>> tagsOK;

	// a temporary tag, which will be added to tagsOK
	std::vector<std::string> tagOK;

	// if the flag is true, a valid value was found (retired)
	// bool flag = false;

	std::ifstream fichier(arbo, std::ios::in);

	if (fichier)
	{

		// browse each line of the arbo file
		while (getline(fichier, ligne))
		{
			// If the line contains "Node"
			//    : ligne.find("Node") != std::string::npos
			// and it does not contain "\ t"
			//    : ligne.find("\t") == std::string::npos
			// then the current line indicates the name of a key for a node
			if ((ligne.find("Node") != std::string::npos)
				&& (ligne.find("\t") == std::string::npos))
			{
				// We take the category of the key
				// In the arbo file, a key is characterized
				// by the first word of the sentence followed by "["
				keyArbo = ligne.substr(0, ligne.find(" ["));

				// std::cout << keyArbo << std::endl;

				// We analyze the tags of the given node
				for (Myreadosm_tag tag : myNode.getTags())
					// If the tag of the node is valid
					if (tag.getKey() == keyArbo)
					{
						// So we store this key in the valid key list
						tagOK.push_back(tag.getKey());

						tagsOK.push_back(tagOK);

						// We look for the associated value
						getline(fichier, ligne);

						// If the line contains "\t[Node]"
						if (ligne.find("\t[Node]") != std::string::npos)
							while (true)
							{
								getline(fichier, ligne);
								if (ligne.find("\t\t") != std::string::npos)
								{
									// We store the value
									valueArbo = ligne.substr(ligne.find("\t\t") + std::string("\t\t").size());

									// If the tag of the node is valid
									if (tag.getValue() == valueArbo)
										// So we store tag in the valid tag list
										tagOK.push_back(tag.getValue());

									tagsOK.push_back(tagOK);

									// reset of "tagOK"
									tagOK.clear();
									tagOK.push_back(tag.getKey());
								}
								else
									break;
							}
					}

			}
			tagOK.clear();
		}

		fichier.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier ! > " + arbo << std::endl;

	return tagsOK;
}

// This function inserts a node in the appropriate file according to its category
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

	// Backing up the node in RAM via the Node Read Osm object

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

	// Storage of the node in the correct category in mass memory

	// Stores, to start the current "node" in the node file
	fic = ".\\..\\..\\..\\Arbo\\Node.txt";
	oNode(fic, myNode);

	// then, if its category is filled in, it is stored in the file more precisely

	// We test the key to start
	tagsOK = iArboNode(".\\..\\..\\..\\arbo.txt", myNode);
	for (std::vector<std::string> tagOK : tagsOK)
	{
		std::cout << "size" << tagOK.size() << std::endl;
		std::cout << "1" << tagOK[0] << std::endl;
		std::cout << "2" << tagOK[1] << std::endl;
		if (tagOK.size() == 1)
		{
			fic = ".\\..\\..\\..\\Arbo\\Node[]" + tagOK[0] + ".txt";
			oNode(fic, myNode);
		}
		if (tagOK.size() == 2)
		{
			fic = ".\\..\\..\\..\\Arbo\\Node[]" + tagOK[0] + "[]" + tagOK[1] + ".txt";
			oNode(fic, myNode);
		}
	}
	return READOSM_OK;
}

// These three functions are for the ways

// This function opens, writes and closes the file
void oWay(std::string fic, Wayreadosm myWay)
{
	// Opening the file, the file is created if it does not exist,
	// but its content is not deleted if it exists
	std::ofstream wayFile(fic, std::ios::out | std::ios::app);

	if (wayFile)
	{

		wayFile << myWay.getId() << std::endl;

		wayFile << myWay.getVersion() << std::endl;

		wayFile << myWay.getChangeset() << std::endl;

		wayFile << myWay.getUser() << std::endl;

		wayFile << myWay.getUid() << std::endl;

		wayFile << myWay.getTimestamp() << std::endl;

		wayFile << myWay.getNode_ref_count() << std::endl;

		for (std::string node : myWay.getNode_refs())
		{
			wayFile << node << std::endl;
		}

		wayFile << myWay.getTag_count() << std::endl;

		for (Myreadosm_tag tag : myWay.getTags())
		{
			wayFile << tag.getKey() << std::endl;
			wayFile << tag.getValue() << std::endl;
		}

		wayFile.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}

// This function returns all the keys of "myWay" present in the file "arbo"
// With their associated correct value (ex: <<"waterway", "stream">, <"building">, etc.>
std::vector< std::vector<std::string>> iArboWay(std::string arbo, Wayreadosm myWay)
{
	// Current line in the file reading
	std::string ligne;

	// Name of a valid key for a category
	std::string keyArbo;

	// Name of a valid value for a category
	std::string valueArbo;

	// list of valid tags present in the node, contains
	// the valid key list present in the node
	// the list of valid values present in the node
	// of the form <<key, value>, <key, value>, <key, value>>
	// if one of the two fields is not indicated, then the node does not have
	std::vector< std::vector<std::string>> tagsOK;

	// a temporary tag, which will be added to tagsOK
	std::vector<std::string> tagOK;

	// if the flag is true, a valid value has been drilled (removed)
	// bool flag = false;

	std::ifstream fichier(arbo, std::ios::in);

	if (fichier)
	{

		// browse each line of the arbo file
		while (getline(fichier, ligne))
		{
			// If the line contains "Way"
			//    : ligne.find("Way") != std::string::npos
			// and it does not contain "\t"
			//    : ligne.find("\t") == std::string::npos
			// then the current line indicates the name of a key for a way
			if ((ligne.find("Way") != std::string::npos)
				&& (ligne.find("\t") == std::string::npos))
			{
				// We take the category of the key
				// In the arbo file, a key is characterized
				// by the first word of the sentence followed by "["
				keyArbo = ligne.substr(0, ligne.find(" ["));

				// We analyze the tags of the given way
				for (Myreadosm_tag tag : myWay.getTags())
					// If the way tag is valid
					if (tag.getKey() == keyArbo)
					{
						// So we store this key in the valid key list
						tagOK.push_back(tag.getKey());

						tagsOK.push_back(tagOK);

						// We look for the associated value
						// as long as the line does not contain "\t[Way]"
						while (ligne.find("\t[Way]") == std::string::npos)
							getline(fichier, ligne);

						while (true)
						{
							getline(fichier, ligne);
							if (ligne.find("\t\t") != std::string::npos)
							{
								// We store the value
								valueArbo = ligne.substr(ligne.find("\t\t") + std::string("\t\t").size());

								// If the tag of the node is valid
								if (tag.getValue() == valueArbo)
									// So we store tag in the valid tag list
									tagOK.push_back(tag.getValue());

								tagsOK.push_back(tagOK);

								// reset of "tagOK"
								tagOK.clear();
								tagOK.push_back(tag.getKey());
							}
							else
								break;
						}
					}

			}
			tagOK.clear();
		}

		fichier.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

	return tagsOK;
}

// This function inserts a way in the appropriate file according to its category
static int
insertWay(const void *user_data, const readosm_way * way)
{
	char buf[128];
	int i;
	const readosm_tag *tag;

	Wayreadosm myWay;
	Myreadosm_tag myTag;

	std::vector< std::vector<std::string> > tagsOK;

	std::string fic;


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

	// Storage of the node in the correct category in mass memory

	// Stores, to start the current node in the node file
	fic = ".\\..\\..\\..\\Arbo\\Way.txt";
	oWay(fic, myWay);

	// then, if its category is filled in, it is stored in the file more precisely


	// We test the key to start
	tagsOK = iArboWay(".\\..\\..\\..\\arbo.txt", myWay);
	for (std::vector<std::string> tagOK : tagsOK)
	{
		if (tagOK.size() == 1)
		{
			fic = ".\\..\\..\\..\\Arbo\\Way[]" + tagOK[0] + ".txt";
			oWay(fic, myWay);
		}
		if (tagOK.size() == 2)
		{
			fic = ".\\..\\..\\..\\Arbo\\Way[]" + tagOK[0] + "[]" + tagOK[1] + ".txt";
			oWay(fic, myWay);
		}
	}

	return READOSM_OK;
}

// These three functions are for Relation

// This function opens, writes and closes the file
void oRelation(std::string fic, Relationreadosm myRelation)
{
	// Opening the file, the file is created if it does not exist,
	// but its content is not deleted if it exists
	std::ofstream relationFile(fic, std::ios::out | std::ios::app);

	if (relationFile)
	{

		relationFile << myRelation.getId() << std::endl;

		relationFile << myRelation.getVersion() << std::endl;

		relationFile << myRelation.getChangeset() << std::endl;

		relationFile << myRelation.getUser() << std::endl;

		relationFile << myRelation.getUid() << std::endl;

		relationFile << myRelation.getTimestamp() << std::endl;

		relationFile << myRelation.getMember_count() << std::endl;

		for (Myreadosm_member member : myRelation.getMembers())
		{
			relationFile << member.getMember_type() << std::endl;
			relationFile << member.getId() << std::endl;
			relationFile << member.getRole() << std::endl;
		}

		relationFile << myRelation.getTag_count() << std::endl;

		for (Myreadosm_tag tag : myRelation.getTags())
		{
			relationFile << tag.getKey() << std::endl;
			relationFile << tag.getValue() << std::endl;
		}

		relationFile.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}

// This function returns the set of keys of "myRelation" present in the file "arbo"
// With their associated correct value (ex: <<"waterway", "stream">, <"building">, etc.>
std::vector< std::vector<std::string>> iArboRelation(std::string arbo, Relationreadosm myRelation)
{
	// Current line in the file reading
	std::string ligne;

	// Name of a valid key for a category
	std::string keyArbo;

	// Name of a valid value for a category
	std::string valueArbo;

	// list of valid tags present in the node, contains
	// the valid key list present in the node
	// the list of valid values present in the node
	// of the form <<key, value>, <key, value>, <key, value>>
	// if one of the two fields is not indicated, then the node does not have
	std::vector< std::vector<std::string>> tagsOK;

	// a temporary tag, which will be added to tagsOK
	std::vector<std::string> tagOK;

	// if the flag is true, a valid value has been found (removed)
	// bool flag = false;

	std::ifstream fichier(arbo, std::ios::in);

	if (fichier)
	{

		// browse each line of the arbo file
		while (getline(fichier, ligne))
		{
			// If the line contains "Relation"
			//    : ligne.find("Relation") != std::string::npos
			// and it does not contain "\t"
			//    : ligne.find("\t") == std::string::npos
			// then the current line indicates the name of a key for a "Relation"
			if ((ligne.find("Relation") != std::string::npos)
				&& (ligne.find("\t") == std::string::npos))
			{
				// We take the category of the key
				// In the arbo file, a key is characterized
				// by the first word of the sentence followed by "["
				keyArbo = ligne.substr(0, ligne.find(" ["));

				// We analyze the tags of the given relation
				for (Myreadosm_tag tag : myRelation.getTags())
					// If the relation tag is valid
					if (tag.getKey() == keyArbo)
					{
						// So we store this key in the valid key list
						tagOK.push_back(tag.getKey());

						tagsOK.push_back(tagOK);

						// We look for the associated value
						// while the line does not contain "\t[Relation]"
						while (ligne.find("\t[Relation]") == std::string::npos)
							getline(fichier, ligne);

						while (true)
						{
							getline(fichier, ligne);
							if (ligne.find("\t\t") != std::string::npos)
							{
								// We store the value
								valueArbo = ligne.substr(ligne.find("\t\t") + std::string("\t\t").size());

								// If the tag of the node is valid
								if (tag.getValue() == valueArbo)
									// So we store tag in the valid tag list
									tagOK.push_back(tag.getValue());

								tagsOK.push_back(tagOK);

								// reset of "tagOK"
								tagOK.clear();
								tagOK.push_back(tag.getKey());
							}
							else
								break;
						}
					}

			}
			tagOK.clear();
		}

		fichier.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

	return tagsOK;
}

// This function inserts a relation in the appropriate file according to its category

static int
insertRelation(const void *user_data, const readosm_relation * relation)
{
	char buf[128];
	int i;
	const readosm_member *member;
	const readosm_tag *tag;

	Relationreadosm myRelation;
	Myreadosm_tag myTag;
	Myreadosm_member myMember;

	std::vector< std::vector<std::string> > tagsOK;

	std::string fic;


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

			myRelation.appendMembers(myMember);

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

	// Storage of the node in the correct category in mass memory

	// Stores, to start the current no in the node file
	fic = ".\\..\\..\\..\\Arbo\\Relation.txt";
	oRelation(fic, myRelation);

	// Then, if its category is renamed, it is stored in the file more precisely

	// We test the key to start
	tagsOK = iArboRelation(".\\..\\..\\..\\arbo.txt", myRelation);
	for (std::vector<std::string> tagOK : tagsOK)
	{
		if (tagOK.size() == 1)
		{
			fic = ".\\..\\..\\..\\Arbo\\Relation[]" + tagOK[0] + ".txt";
			oRelation(fic, myRelation);
		}
		if (tagOK.size() == 2)
		{
			fic = ".\\..\\..\\..\\Arbo\\Relation[]" + tagOK[0] + "[]" + tagOK[1] + ".txt";
			oRelation(fic, myRelation);
		}
	}

	return READOSM_OK;
}

// Constructor / Destructor

OsmWithreadosm::OsmWithreadosm()
{
	this->fileOsm = "..\\..\\..\\var.osm";
}


void OsmWithreadosm::classify(void)
{
	std::ifstream file(".\\..\\..\\..\\Arbo\\Node.txt", std::ios::in);

	if (file)
		file.close();
	else
	{
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
			readosm_parse(osmHandle, (const void *)0, insertNode, insertWay,
				insertRelation);
		if (ret != READOSM_OK)
		{
			fprintf(stderr, "PARSE error: %d\n", ret);
			readosm_close(osmHandle);
		}

		// STEP #3: closing the OSM file
		readosm_close(osmHandle);
	}
}

OsmWithreadosm::~OsmWithreadosm()
{
}

void iNode(std::string fic)
{
	// open the file
	std::ifstream NodeFile(fic, std::ios::in);

	std::string ligne;

	Nodereadosm myNode;

	Myreadosm_tag tag;

	if (NodeFile)
	{

		getline(NodeFile, ligne);
		myNode.setId(ligne);

		getline(NodeFile, ligne);
		myNode.setLatitude( atof( ligne.c_str() ) );

		getline(NodeFile, ligne);
		myNode.setLongitude(atof(ligne.c_str()));

		getline(NodeFile, ligne);
		myNode.setVersion(atoi(ligne.c_str()));

		getline(NodeFile, ligne);
		myNode.setChangeset(ligne);

		getline(NodeFile, ligne);
		myNode.setUser(ligne);

		getline(NodeFile, ligne);
		myNode.setUid( atoi( ligne.c_str() ));

		getline(NodeFile, ligne);
		myNode.setTimestamp(ligne);

		getline(NodeFile, ligne);
		myNode.setTag_count(atoi(ligne.c_str()));

		for (int i =0; i < myNode.getTag_count(); i++)
		{
			getline(NodeFile, ligne);
			tag.setKey(ligne);

			getline(NodeFile, ligne);
			tag.setValue(ligne);

			myNode.appendTags(tag);
		}

		myNode.setPosEle();

		NodeFile.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier ! > " + fic << std::endl;

	::myNodes.push_back(myNode);
}

void iWay(std::string fic)
{
	// open the file
	std::ifstream WayFile(fic, std::ios::in);

	std::string ligne;

	Wayreadosm myWay;

	Myreadosm_tag tag;

	if (WayFile)
	{

		getline(WayFile, ligne);
		myWay.setId(ligne);

		getline(WayFile, ligne);
		myWay.setVersion(atoi(ligne.c_str()));

		getline(WayFile, ligne);
		myWay.setChangeset(ligne);

		getline(WayFile, ligne);
		myWay.setUser(ligne);

		getline(WayFile, ligne);
		myWay.setUid(atoi(ligne.c_str()));

		getline(WayFile, ligne);
		myWay.setTimestamp(ligne);

		getline(WayFile, ligne);
		myWay.setNode_ref_count(atoi(ligne.c_str()));

		for (int i = 0; i < myWay.getNode_ref_count(); i++)
		{
			getline(WayFile, ligne);
			myWay.appendNode_refs(ligne);
		}

		getline(WayFile, ligne);
		myWay.setTag_count(atoi(ligne.c_str()));

		for (int i = 0; i < myWay.getTag_count(); i++)
		{
			getline(WayFile, ligne);
			tag.setKey(ligne);

			getline(WayFile, ligne);
			tag.setValue(ligne);

			myWay.appendTags(tag);
		}

		WayFile.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier ! > " + fic << std::endl;

	::myWays.push_back(myWay);
}

void iRelation(std::string fic)
{
	// open the file
	std::ifstream RelationFile(fic, std::ios::in);

	std::string ligne;

	Relationreadosm myRelation;

	Myreadosm_tag tag;

	Myreadosm_member member;

	if (RelationFile)
	{

		getline(RelationFile, ligne);
		myRelation.setId(ligne);

		getline(RelationFile, ligne);
		myRelation.setVersion(atoi(ligne.c_str()));

		getline(RelationFile, ligne);
		myRelation.setChangeset(ligne);

		getline(RelationFile, ligne);
		myRelation.setUser(ligne);

		getline(RelationFile, ligne);
		myRelation.setUid(atoi(ligne.c_str()));

		getline(RelationFile, ligne);
		myRelation.setTimestamp(ligne);

		getline(RelationFile, ligne);
		myRelation.setMember_count(atoi(ligne.c_str()));

		for (int i = 0; i < myRelation.getMember_count(); i++)
		{
			getline(RelationFile, ligne);
			
			if (ligne == "Node")
				member.setMember_type(Node);
			else if (ligne == "Way")
				member.setMember_type(Way);
			else if (ligne == "Relation")
				member.setMember_type(Relation);

			getline(RelationFile, ligne);
			member.setId(ligne);

			getline(RelationFile, ligne);
			member.setRole(ligne);

			myRelation.appendMembers(member);
		}

		getline(RelationFile, ligne);
		myRelation.setTag_count(atoi(ligne.c_str()));

		for (int i = 0; i < myRelation.getTag_count(); i++)
		{
			getline(RelationFile, ligne);
			tag.setKey(ligne);

			getline(RelationFile, ligne);
			tag.setValue(ligne);

			myRelation.appendTags(tag);
		}

		RelationFile.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier ! > " + fic << std::endl;

	::myRelations.push_back(myRelation);
}


void OsmWithreadosm::initWithCat(std::vector<std::string> nodes, std::vector<std::string> ways, std::vector<std::string> relations)
{
	std::string fic;

	// if no key has been specified
	if (nodes[0] != "")
		fic = ".\\..\\..\\..\\Arbo\\Node.txt";
	// if we specify a key but no value
	else if (nodes[1] != "")
		fic = ".\\..\\..\\..\\Arbo\\Node[]" + nodes[0] + ".txt";
	// if we cut a key and a value
	else
		fic = ".\\..\\..\\..\\Arbo\\Node[]" + nodes[0] + "[]" + nodes[1] + ".txt";
	iNode(fic);

	// if no key has been specified
	if(ways[0] != "")
		fic = ".\\..\\..\\..\\Arbo\\Way.txt";
	// if we specify a key but no value
	else if (ways[1] != "")
		fic = ".\\..\\..\\..\\Arbo\\Way[]" + ways[0] + ".txt";
	// if we cut a key and a value
	else
		fic = ".\\..\\..\\..\\Arbo\\Way[]" + ways[0] + "[]" + ways[1] +  ".txt";
	iWay(fic);

	// if no key has been specified
	if (relations[0] != "")
		fic = ".\\..\\..\\..\\Arbo\\Relation.txt";
	// if we specify a key but no value
	else if (relations[1] != "")
		fic = ".\\..\\..\\..\\Arbo\\Relation[]" + relations[0] + ".txt";
	// if we cut a key and a value
	else
		fic = ".\\..\\..\\..\\Arbo\\Relation[]" + relations[0] + "[]" + relations[1] + ".txt";
	iRelation(fic);

	// Storing values in the object
	this->nodes = ::myNodes;
	this->ways = ::myWays;
	this->relations = ::myRelations;
}



void OsmWithreadosm::deleteFiles(void)
{
	system("del ..\\..\\..\\Arbo\\*.txt");
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

	// if the flag is "true" then the current node is what the user wants
	bool flag = false;

	myNode.setTag_count(node->tag_count);

	if (node->tag_count > 0)
		for (i = 0; i < node->tag_count; i++)
		{
			tag = node->tags + i;
			myTag.setKey(tag->key);
			myTag.setValue(tag->value);
			myNode.appendTags(myTag);
			// the key then the value is good
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

	// if the flag is "true" then the current way is what the user wants
	bool flag = false;

	myWay.setTag_count(way->tag_count);

	if (way->tag_count > 0)
		for (i = 0; i < way->tag_count; i++)
		{
			tag = way->tags + i;
			myTag.setKey(tag->key);
			myTag.setValue(tag->value);
			myWay.appendTags(myTag);
			// the key then the value is good
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


void OsmWithreadosm::initWithTags(std::vector<std::string> nodes, std::vector<std::string> ways, std::vector<std::string> relations)
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

	// Storing values in the object
	this->nodes = ::myNodes;
	this->ways = ::myWays;
	this->relations = ::myRelations;
}



void OsmWithreadosm::changeFile(std::string fileOsm)
{
	this->fileOsm = fileOsm;
}

// tree
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


// get everything
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