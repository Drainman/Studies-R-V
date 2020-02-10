#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <set>
#include "lib/logger.h"
#include "lib/node.h"
#include "lib/parse.h"
#include "lib/jsonstruct.h"

#define DELIMITER ","
#define LIMITED_WEIGHT 10000
#define LINE_LIMITATION 10000// 10000

using namespace std;

map<string,int> MAP_theme;

int main(int argc, char** argv){

	typedef function<bool(pair<string,int>,pair<string,int>)> Comparator;
	Comparator cmpt = [](pair<string,int> const & a,pair<string,int> const & b){
		return a.second > b.second;
	};

	logger("INFO","Récupération des données FOS.");
	MAP_theme = parseFOS();

	logger("INFO","Conversion en map de noeud.");
	map<string,node> MAP_nodes = convert_to_node_tree(MAP_theme);


	logger("INFO","Récupération des données de DBLP.");
	parseDBLP(&MAP_nodes,LINE_LIMITATION);
	string str_limit = "Données récupérées sur la base de " + to_string(LINE_LIMITATION) + " lignes.";
	logger("INFO",str_limit);

	logger("INFO","Création du fichier JSON correspondant.");
	createJSONFromMap(MAP_nodes);

	// print_seperator();
	// string str_test = "Test pour le thème : ";
	// string to_test = "level of detail";
	// str_test += to_test;
	// logger("TEST",str_test);
	// cout << MAP_nodes[to_test].toString() << endl;


}
