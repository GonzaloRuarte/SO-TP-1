#include <iostream>
#include "ConcurrentHashMap.h"

using namespace std;

int main(void) {
	ConcurrentHashMap h;
	int i,j;

	cout<<"introdusca la cantidad de palabras: "<<endl;
	cin>>j;
	cout<<"introdusca las palabras"<<endl;
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

	return 0;
}
