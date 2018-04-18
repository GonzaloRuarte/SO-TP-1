#include <iostream>
#include "ConcurrentHashMap.h"
#include <fstream>
#include <list>

using namespace std;

void test2() {
	ConcurrentHashMap* h = ConcurrentHashMap::count_words("corpus");
	ConcurrentHashMap::printConcurrentHashMap(h);
	delete h;
}

void test3() {
	//TODO: el pthread_create da seg fault, todavia no se por que
	list<string> l = {"corpus", "corpus", "corpus", "corpus", "corpus"};
	ConcurrentHashMap* h = ConcurrentHashMap::count_words(l);
	ConcurrentHashMap::printConcurrentHashMap(h);
	delete h;
}

void test4() {
	//TODO: este count words no se esta linkeando

	//list<string> l = {"corpus-0", "corpus-1", "corpus-2", "corpus-3", "corpus-4"};
	//ConcurrentHashMap* h = ConcurrentHashMap::count_words(3, l);
	//ConcurrentHashMap::printConcurrentHashMap(h);
	//delete h;
}

void test5() {

}


int main(int argc, char * argv[]) {

	if (argc == 2) {
		ConcurrentHashMap* h;
		char test = *argv[1];
		
		if (test == '2') {
			test2();
		}

		if (test == '3') {
			test3();
		}

		if (test == '4') {
			test4();
		}

		if (test == '5') {	
			test5();
		}
	} else if (argc == 1) {
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
		cout << h.maximum(5).first << endl;
		cout << h.maximum(5).second << endl;
	}
	return 0;
}
