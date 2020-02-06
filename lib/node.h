#include <string>
#include <vector>
#include <iostream>
#include <deque>

using namespace std;


class node{

	int weight;
	string label;


	public:
		vector<node *> connections;
		vector<string> str_connections;

		node(int w=0,string l="None",vector<node*> vn = {}){
			weight = w; //Le nombre d'articles auquel il est lié (x)
			label=l;
			connections = vn;
			str_connections = {}; //size -> Nombre de connections avec d'autres objets (z plus il a des connexion plus sphere grandes)
		}

		bool alreadyConnect(string label){
			//bool find = false;
			for(auto l : str_connections)
				if(l == label)
					return true;
			return false;
		}

		void connectTo(node * n){
			string pass_node_label = n->label;
			if( !alreadyConnect(pass_node_label) ){
				connections.push_back(n);
				str_connections.push_back(n->label);
			}
		}

		string showConnection(){
			string str_connection = "";
			for(node * element : connections){
				str_connection += "\t* "+ element->label + "\n";
			}
			return str_connection;
		}

		string toString(){
			string this_str = "> [" + label + "] - (weight : "+to_string(weight)+") - (connexions "+to_string(str_connections.size())+")\n";
			this_str += this->showConnection();
			return this_str;
		}
};


void connect(node * n1,node * n2){
	n1->connectTo(n2);
	n2->connectTo(n1);
}


map<string,node> convert_to_node_tree(map<string,int> MAP_str){

	map<string,node>  MAP_convert;

	for(map<string,int>::iterator it = MAP_str.begin(); it != MAP_str.end(); ++it) {
		node tmp(MAP_str[it->first],it->first);
		MAP_convert[it->first] = tmp;
	}

	return MAP_convert;
}
