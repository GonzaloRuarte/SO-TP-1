#include <iostream>
#include "ConcurrentHashMap.h"
#include <fstream>
#include <list>
#include "rdtsc.h"

using namespace std;


void test2() {
	ConcurrentHashMap* h = ConcurrentHashMap::count_words("corpus");
	ConcurrentHashMap::printConcurrentHashMap(h);
	delete h;
}

void test3() {
	//TODO: el pthread_create da seg fault, todavia no se por que
	list<string> l = {"corpus-0", "corpus-1", "corpus-2", "corpus-3", "corpus-4"};
	ConcurrentHashMap* h = ConcurrentHashMap::count_words(l);
	ConcurrentHashMap::printConcurrentHashMap(h);
	delete h;
}

void test4() {
	//TODO: este count words no se esta linkeando

	list<string> l = {"corpus-0", "corpus-1", "corpus-2", "corpus-3", "corpus-4"};
	ConcurrentHashMap* h = ConcurrentHashMap::count_words(1, l);
	ConcurrentHashMap::printConcurrentHashMap(h);
	delete h;
}

void test5() {

}

void testsCountWords(int cantArch, int cantThreads,string salida ){
		ofstream txt(salida);

	list<string> l ;
	for(int i=0;i<cantArch;i++){
		l.push_front("Corpuses/corpus" + to_string(i));
	}
	unsigned long delta=1111111110;
	for(int i=0;i<100;i++){
		unsigned long start, end;
		RDTSC_START(start);
		ConcurrentHashMap* h = ConcurrentHashMap::count_words(l);
		RDTSC_STOP(end);
		delete h;
		//delta += end - start;
		delta = (end - start) < delta ? (end - start) : delta;
		cout<<"vuelta "<<i<<" del 1-1"<<endl;
	}
	txt<<"El count_words de 1 thread - 1 archivo tomo tiempo min: "<<(delta)<<endl;
	cout<<"ahora pasamos al siguiente pero primero probamos que onda..."<<endl;
	ConcurrentHashMap* h = ConcurrentHashMap::count_words(1,l);
	cout<<"ahora si probamos posta"<<endl;

	delta=1111111110;

	for(int j=1;j<cantThreads;j++){
		cout<<"iteracion de "<<j<<"threads"<<endl;
		delta=1111111110;
		for(int i=0;i<100;i++){
			unsigned long start, end;
			RDTSC_START(start);
			ConcurrentHashMap* h = ConcurrentHashMap::count_words(j,l);
			RDTSC_STOP(end);
			delete h;
			//delta += end - start;
			delta = (end - start) < delta ? (end - start) : delta;
			cout<<"vuelta "<<i<<" del "<<j<<" threads"<<endl;
		}
		txt<<"El count_words de "<<j<<" threads tomo tiempo min: "<<(delta)<<endl;
	}

	txt.close();
}


void test1() {
	testsCountWords(40,30,"test count_words 40 archivos 30threads");
}

void testsMaximum(int cantArch, int cantThreads,string salida ){
		ofstream txt(salida);

	list<string> l ;
	for(int i=0;i<cantArch;i++){
		l.push_front("Corpuses/corpus" + to_string(i));
	}
	unsigned long delta=1111111110;
	delta=1111111110;



	for(int j=1;j<cantThreads+1;j++){
		cout<<"iteracion de "<<j<<"threads maximum6"<<endl;
		delta=1111111110;
		for(int i=0;i<100;i++){
			unsigned long start, end;
			RDTSC_START(start);
			ConcurrentHashMap::maximum6(j,cantArch,l);
			RDTSC_STOP(end);			//delta += end - start;
			delta = (end - start) < delta ? (end - start) : delta;
			cout<<"vuelta "<<i<<" del "<<j<<" threads maximum6"<<endl;
		}
		txt<<"El maximum6 de "<<j<<" threads tomo tiempo min: "<<(delta)<<endl;
	}


	for(int j=1;j<cantThreads+1;j++){
		cout<<"iteracion de "<<j<<"threads maximum5"<<endl;
		delta=1111111110;
		for(int i=0;i<100;i++){
			unsigned long start, end;
			RDTSC_START(start);
			ConcurrentHashMap::maximum5(j,cantArch,l);
			RDTSC_STOP(end);
			//delta += end - start;
			delta = (end - start) < delta ? (end - start) : delta;
			cout<<"vuelta "<<i<<" del "<<j<<" threads maximum5"<<endl;
		}
		txt<<"El maximum5 de "<<j<<" threads tomo tiempo min: "<<(delta)<<endl;
	}

	delta=1111111110;



	txt.close();
}

void test6() {
	testsMaximum(40,30,"test maximum 40 archivos y 30 threads");
}

int main(int argc, char * argv[]) {

	if (argc == 2) {
		ConcurrentHashMap* h;
		char test = *argv[1];
		
		if (test == '1') {
			test1();
		}

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
		if (test == '6') {	
			test6();
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
