#include <stdio.h>

#include "readosm.h"

#include <iostream>
#include <vector>

#include <string.h>

#include "OsmWithreadosm.h"
#include "Nodereadosm.h"
#include "Wayreadosm.h"
#include "Relationreadosm.h"

int main(int argc, char *argv[])
{
	
	// for node we would like get
	//std::vector<std::string> nodes;

	// his key
	//nodes.push_back("aerialway");
	// his value
	//nodes.push_back("");

	// for way we would like get
	//std::vector<std::string> ways;

	// his key
	//ways.push_back("aerialway");
	// his value
	//ways.push_back("cable_car");

	// for relation we would like get
	//std::vector<std::string> relations;

	// his key
	//relations.push_back("");
	// his value
	//relations.push_back("");

	// We open the default file osm and réorganize the object, for optimize the tie after
	OsmWithreadosm dataOsm;
	//dataOsm.initWithCat(nodes, ways, relations);

	//std::vector<Nodereadosm> Ns;

	// We get all the nodes
	//Ns = dataOsm.getNodes();

	//for(Nodereadosm N : Ns)
		//N.affiche();

	//std::cout << "----------------" << std::endl;

	//std::vector<Wayreadosm> Ws;

	// We get all the ways
	//Ws = dataOsm.getWays();

	//for (Wayreadosm W : Ws)
		//W.affiche();

	//std::cout << "----------------" << std::endl;

	//std::vector<Relationreadosm> Rs;

	// We get all the ways
	//Rs = dataOsm.getRelations();

	//for (Relationreadosm R : Rs)
		//R.affiche();

	// We select a node with his id
	//Nodereadosm Nid;
	// like this
	//(dataOsm.getNodeWithId("14468319")).affiche();
	
	//std::cout << "----------------" << std::endl;

	// or like this
	//std::string S;
	//S = "14468319";
	//(dataOsm.getNodeWithId(S)).affiche();

	std::cout << "end ?";
	std::cin.get();

	return 0;

}
