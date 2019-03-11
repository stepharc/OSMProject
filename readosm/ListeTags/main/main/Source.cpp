
// Rajout de @Ludovic pour ignorer l'erreur sprintf unsafe
#define _CRT_SECURE_NO_WARNINGS

/*
/ test_osm1.c
/
/ libreadosm Sample code
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ ------------------------------------------------------------------------------
/
/ Version: MPL 1.1/GPL 2.0/LGPL 2.1
/
/ The contents of this file are subject to the Mozilla Public License Version
/ 1.1 (the "License"); you may not use this file except in compliance with
/ the License. You may obtain a copy of the License at
/ http://www.mozilla.org/MPL/
/
/ Software distributed under the License is distributed on an "AS IS" basis,
/ WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
/ for the specific language governing rights and limitations under the
/ License.
/
/ The Original Code is the ReadOSM library
/
/ The Initial Developer of the Original Code is Alessandro Furieri
/
/ Portions created by the Initial Developer are Copyright (C) 2012-2017
/ the Initial Developer. All Rights Reserved.
/
/ Contributor(s):
/
/ Alternatively, the contents of this file may be used under the terms of
/ either the GNU General Public License Version 2 or later (the "GPL"), or
/ the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
/ in which case the provisions of the GPL or the LGPL are applicable instead
/ of those above. If you wish to allow use of your version of this file only
/ under the terms of either the GPL or the LGPL, and not to allow others to
/ use your version of this file under the terms of the MPL, indicate your
/ decision by deleting the provisions above and replace them with the notice
/ and other provisions required by the GPL or the LGPL. If you do not delete
/ the provisions above, a recipient may use your version of this file under
/ the terms of any one of the MPL, the GPL or the LGPL.
/
*/

#include <stdio.h>

// rajout de @Ludovic
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "readosm.h"

// <<key,value>,<key,value>,<key,value>,<key,value>, etc.>
std::vector<std::vector<std::string>> tagsNode;
std::vector<std::vector<std::string>> tagsWays;
std::vector<std::vector<std::string>> tagsRelation;

static int
print_node(const void *user_data, const readosm_node * node)
{
	if (user_data != NULL)
		user_data = NULL;	/* silencing stupid compiler warnings */

	if (node->tag_count != 0)
		for (int i = 0; i < node->tag_count; i++)
			tagsNode.push_back( { (node->tags + i)->key, (node->tags + i)->value } );

	return READOSM_OK;
}

static int
print_way(const void *user_data, const readosm_way * way)
{
	if (user_data != NULL)
		user_data = NULL;	/* silencing stupid compiler warnings */

	if (way->tag_count != 0)
		for (int i = 0; i < way->tag_count; i++)
			tagsWays.push_back({ (way->tags + i)->key, (way->tags + i)->value });

	return READOSM_OK;
}

static int
print_relation(const void *user_data, const readosm_relation * relation)
{
		if (user_data != NULL)
			user_data = NULL;	/* silencing stupid compiler warnings */

	if (relation->tag_count != 0)
		for (int i = 0; i < relation->tag_count; i++)
			tagsRelation.push_back({ (relation->tags + i)->key, (relation->tags + i)->value });

	return READOSM_OK;
}

int
main(int argc, char *argv[])
{
	const void *osm_handle;
	int ret;

	if (argc != 2)
	{
		fprintf(stderr, "usage: test_osm1 path-to-OSM-file\n");
		return -1;
	}

	/*
	* STEP #1: opening the OSM file
	* this can indifferently be an OSM XML encoded file (.osm)
	* or an OSM Protocol Buffer encoded file (.pbf)
	* the library will transparently perform any required
	* action in both cases.
	*/
	const char *t = argv[1];
	//const char *t = "..\..\..\test.osm";
	ret = readosm_open(t, &osm_handle);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "OPEN error: %d\n", ret);
		goto stop;
	}

	/*
	* STEP #2: parsing the OSM file
	* this task is unbelievably simple
	*
	* you are simply required to pass the appropriate
	* pointers for callback funtions respectively intended
	* to process Node-objects, Way-objects and Relation-objects
	*
	* the library will then parse the whole input file, calling
	* the appropriate callback handling function for each OSM object
	* found: please see the callback functions implementing code
	* to better understand how it works
	*
	* important notice: in this first example we'll not use at
	* all the USER_DATA pointer. so the second arg will simply
	* be (const void *)0 [i.e. NULL]
	*/
	ret =
		readosm_parse(osm_handle, (const void *)0, print_node, print_way,
			print_relation);
	if (ret != READOSM_OK)
	{
		fprintf(stderr, "PARSE error: %d\n", ret);
		goto stop;
	}

	fprintf(stderr, "Ok, OSM input file successfully parsed\n");

stop:
	/*
	* STEP #3: closing the OSM file
	* this will release any internal memory allocation
	*/
	readosm_close(osm_handle);

	//rajout de @Ludovic, pour stopper le prog a la fin (pour voir ce qui ce passe

	std::cout << "RelationTags" << std::endl;
	for(int i = 0; i < tagsRelation.size(); i++)
		if(tagsRelation[i][0] == "aerialway")
		std::cout << tagsRelation[i][0] << ":" << tagsRelation[i][1] <<  std::endl;

	// suppression des doublon Nodes

	/*std::vector<std::string> keysNodes;
	for (int i = 0; i < tagsNode.size(); i++)
		keysNodes.push_back( tagsNode[i][0] );

	std::sort(keysNodes.begin(), keysNodes.end());
	auto last = std::unique(keysNodes.begin(), keysNodes.end());
	keysNodes.erase(last, keysNodes.end());

	// affichage
	std::cout << "NodeTags" << std::endl;
	for(int i = 0; i < keysNodes.size(); i++)
		std::cout << "\t" << keysNodes[i] << std::endl;*/

	// suppression des doublon Ways

	/*std::vector<std::string> keysWays;
	for (int i = 0; i < tagsWays.size(); i++)
		keysWays.push_back(tagsWays[i][0]);

	std::sort(keysWays.begin(), keysWays.end());
	last = std::unique(keysWays.begin(), keysWays.end());
	keysWays.erase(last, keysWays.end());

	// affichage
	std::cout << "Ways" << std::endl;
	for (int i = 0; i < keysWays.size(); i++)
		std::cout << "\t" << keysWays[i] << std::endl;*/

	// suppression des doublon relation

	/*std::vector<std::string> keysRelation;
	for (int i = 0; i < tagsRelation.size(); i++)
		keysRelation.push_back(tagsRelation[i][0]);

	std::sort(keysRelation.begin(), keysRelation.end());
	last = std::unique(keysRelation.begin(), keysRelation.end());
	keysRelation.erase(last, keysRelation.end());

	// affichage
	std::cout << "Relation" << std::endl;
	for (int i = 0; i < keysRelation.size(); i++)
		std::cout << "\t" << keysRelation[i] << std::endl;
	
	//std::cout << t << strcmp(t,"..\..\..\test.osm") << "..\..\..\test.osm" <<  std::endl;

	*/
	std::cout << "end" << std::endl;
	std::cin.get();

	return 0;
}
