#include <iostream>
#include "ConcurrentHashMap.h"
#include <fstream>

using namespace std;

ConcurrentHashMap* count_words(string arch){
	ConcurrentHashMap* h = new ConcurrentHashMap();
	ifstream entrada(arch);
	string word;

	while(entrada >> word){
		h->addAndInc(word);
	}

	return h;
}


int main(int argc, char * argv[]) {

	if (argc == 2) {
		string file = argv[1];
		ConcurrentHashMap* h = count_words(file);
		for (uint i = 0; i < 26; i++) {
			for (auto it = h->tabla[i].CrearIt(); it.HaySiguiente(); it.Avanzar()) {
				auto t = it.Siguiente();
				cout << t.first << " " << t.second << endl;
			}
		}
		delete h;
	} else if (argc ==1) {
		ConcurrentHashMap h;
		int i,j;

		cout<<"introduzca la cantidad de palabras: "<<endl;
		cin>>j;
		cout<<"introduzca las palabras"<<endl;
		for(int p=0;p<j;p++){
			string s;
			cin>>s;
			h.addAndInc(s);
		}
		for (i = 0; i < 26; i++) {
			for (auto it = h.tabla[i].CrearIt(); it.HaySiguiente(); it.Avanzar()) {
				auto t = it.Siguiente();
				cout << t.first << " " << t.second << endl;
			}
		}
	}
	return 0;
}
