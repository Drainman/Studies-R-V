#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <set>

#define DELIMITER ","
#define FOS_FILE_CSV "datas/fos.csv"
#define DBLP_FILE_CSV "datas/dblp.csv"
#define MINIMUM_OCCURRENCE_LIMITATION 10


vector<string> parseFOSFromDBLP(string str_fos);
string extractFosFromString(string one_fos);
void connect_all(vector<string> v_to_connect,map<string,node> * MAP_node_origin);

map<string,int> parseFOS(){

	map<string,int> MAP_theme;

	ifstream infile(FOS_FILE_CSV);
	string line;
	int cptLine = 0;
	int ignored_theme = 0;

	while(getline(infile,line)){

		if(cptLine != 0){
			size_t pos = 0;
			string token;
			while((pos = line.find(DELIMITER)) != std::string::npos){
				token = line.substr(0,pos);
				string str_cpt = line.substr(token.length()+1,line.length()-1);
				int cpt = stoi(str_cpt);
				//CONTROLE DE LA VALEUR POUR EVINCER LES THEMES AVEC UNE OCCURENCE FAIBLE
				if(cpt > 10 )
					MAP_theme[token] = cpt;
				else
					ignored_theme++;

				line.erase(0,pos + 1);
			}
		}
		cptLine++;

	}

	string str_limitation_theme = "Limitation des champs en f(x) de leur occurence : " + to_string(MINIMUM_OCCURRENCE_LIMITATION);
	string str_ignored_theme = "Nombre de champs ignorée : " + to_string(ignored_theme);
	string str_ok_theme = "Nombre de champs prises en compte : "+ to_string(MAP_theme.size());
	logger("INFO",str_limitation_theme);
	logger("INFO",str_ignored_theme);
	logger("INFO",str_ok_theme);
	return MAP_theme;
}


map<string,node> parseDBLP(map<string,node> * MAP_node_origin,int line_limit){

	map<string,node> MAP_node;

	ifstream infile(DBLP_FILE_CSV);
	string line;
	int cptLine = 0;
	while(getline(infile,line) && cptLine < line_limit){

		if(cptLine != 0){
			size_t pos = 0;
			string token,title, authors, year, n_citation, fos;

			int cptParse = 0;
			while((pos = line.find(DELIMITER)) != std::string::npos){
				cptParse++;

				//ID
				if(cptParse == 1 )
					token = line.substr(0,pos);

				//TITLE
				else if(cptParse == 2)
					title = line.substr(0,pos);

				//AUTHORS
				else if(cptParse == 3){
					authors = line.substr(0,pos);
				}

				//YEAR
				else if(cptParse == 4){
					year = line.substr(0,pos);
				}
				//n_citation
				else if(cptParse == 5){
					n_citation = line.substr(0,pos);
					line.erase(0,pos + 1);
					fos = line.substr(0,line.length() - 1);
				}

				line.erase(0,pos + 1);
			}


			vector<string> v_to_connect;
			v_to_connect= parseFOSFromDBLP(fos);
			connect_all(v_to_connect,MAP_node_origin);


		}
		cptLine++;

	}

	return MAP_node;

}

vector<string> parseFOSFromDBLP(string str_fos){
	size_t pos;
	string one_random_fos;
	str_fos.erase(0,1);

	vector<string> v_fos;

	while((pos = str_fos.find(';')) != std::string::npos){
		one_random_fos = str_fos.substr(0,pos);
		string a_fos =  extractFosFromString(one_random_fos);
		//Filtrage des theme non voulus
		if(a_fos != "None")
			v_fos.push_back(a_fos);
		//cout << a_fos << endl;
		str_fos.erase(0,pos + 1);
	}

	return v_fos;
}

string extractFosFromString(string one_fos){

	size_t pos;
	string the_fos="None";

	while((pos = one_fos.find(':')) != std::string::npos){
		the_fos = one_fos.substr(0,pos);
		one_fos.erase(0,pos + 1);
	}

	return the_fos;
}


void connect_all(vector<string> v_to_connect,map<string,node> *  MAP_node_origin){

	int cpt=0;
	for(auto el : v_to_connect){
		vector<string> withouCurrent = v_to_connect;
		withouCurrent.erase(withouCurrent.begin()+cpt);

		for(auto el_s : withouCurrent)
			connect( & (* MAP_node_origin)[el_s] , & (* MAP_node_origin)[el] );

		cpt++;
	}
}
