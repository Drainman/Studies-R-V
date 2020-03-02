#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#define OUTPUT_FILE_PATH "outputs/output.json"


// TODO : Créations d'une structure JSON à partir de map
void createJSONFromMap(map<string,node> MAP_node);
string createNodeField(string field,string value);
string createNodeFromMAP(node n);
string createListConnectionFromMAP(vector<string> arr_str_connections);
void writeFile(string toWrite);


void createJSONFromMap(map<string,node> MAP_node){

	//Parcours de la map
	string str_JSON_node = "{\"nodes\":[";

	map<string, node>::iterator last_el = MAP_node.end();
	--last_el;

	for(map<string, node>::iterator it=MAP_node.begin(); it!=MAP_node.end(); ++it){
		node cpt_node = it->second;
		//Filtrage des noeuds vides
		if(cpt_node.label != "None")
		{
			str_JSON_node += createNodeFromMAP(cpt_node);
			if(it != last_el)
				str_JSON_node += ",\n";
		}


	}

	str_JSON_node += "]}";
	//cout << str_JSON_node << endl;
	writeFile(str_JSON_node);

}


string createNodeField(string field,string value){
	string str_return = "\"" + field + "\":\"" + value +"\"";
	return str_return;
}

string createNodeFromMAP(node n){

	string str_return = "{";
	str_return  += createNodeField("label",n.label) + ",";
	str_return += createNodeField("weight",to_string(n.weight)) + ",";
	str_return += createNodeField("nb_connections",to_string(n.connections.size())) + ",";
	str_return += createListConnectionFromMAP(n.str_connections);
	str_return += "}";
	return str_return;

}


string createListConnectionFromMAP(vector<string> arr_str_connections){

	vector<string>::iterator last_el = arr_str_connections.end();
	--last_el;
	string str_list = "\"list_connections\":[";

	for(vector<string>::iterator it=arr_str_connections.begin(); it!=arr_str_connections.end(); ++it){
		str_list += "\"" + *it + "\"";
		if(it != last_el)
			str_list += ",";

	}

	str_list += "]";
	return str_list;
}


void writeFile(string toWrite){
	logger("INFO","Ecriture du fichier JSON.");
	fstream out(OUTPUT_FILE_PATH,ofstream::out);
	out << toWrite;
	out.close();
	string str_end_write = "Ecriture du fichier terminée > ";
	str_end_write += OUTPUT_FILE_PATH;
	logger("INFO",str_end_write);
}
