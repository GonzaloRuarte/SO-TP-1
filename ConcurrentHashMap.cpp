#include "ConcurrentHashMap.h"

#include <pthread.h>

ConcurrentHashMap::ConcurrentHashMap() {
	for(uint i = 0; i < 26; i++){
		sem_init(semaforosAddAndInt[i],0,1);
	}
	sem_init(semaforoIsWriting,0,1);
}

void ConcurrentHashMap::addAndInc(string key) {
	int index = (key[0]-(int)'a'); //le restamos 'a' para que empiece de 0

	sem_wait(semaforosAddAndInt[index]);
	Lista<pair<string,int> >::Iterador it = table[index].CrearIt();
	while(it.HaySiguiente() && it.Siguiente().first!=key){
		it.Avanzar();
	}

	if(it.HaySiguiente() && it.Siguiente().first==key){
		it.Siguiente().second++;
	}else{
		table[index].push_front(make_pair(key,1));
	}
	sem_post(semaforosAddAndInt[index]);

}

bool ConcurrentHashMap::member(string key) {
	int index = (key[0]-(int)'a'); //le restamos 'a' para que empiece de 0


///// Copy paste de la teorica de SWMR
	Lista<pair<string,int> >::Iterador it = table[index].CrearIt();
	while(it.HaySiguiente() && it.Siguiente().first!=key){
		it.Avanzar();
	}
	bool member;
	if(it.HaySiguiente() && it.Siguiente().first==key){
		member=true; 
	}else{
		member=false;
	}
///// Copy paste de la teorica de SWMR

	return member;
}
/*
void procesarFila(void *mod){
	pair <pair <int,int*>,int> par = *((pair <pair <int,int*>,int> *) mod); //notar que casteo el long como puntero a int
	int* max = mod.first.second;
	for (int i = mod.second; i < 26; i+=par.first){
		//recorrer lista, bla bla y poner *max = lo que corresponda;
		
	}
	pthread_exit(NULL);
}*/

pair<string, int> ConcurrentHashMap::maximum(unsigned int nt){
	/*pthread_t thread[nt];
	int tid;
	int cantFilas = 26 div nt; //intervalo, por ej si nt es 4, el thread 2 procesa la fila 2, la 6, la 10, etc.
	int retValues[nt]; //arreglo con los valores de retorno de cada thread
	par< <int,long>, int > args[nt]; //arreglo con los argumentos que le voy a pasar a cada thread, el intervalo, la fila inicial y un puntero al arreglo retValues, uso long porque las direcciones son de 64 bits
	for (tid = 0; tid < nt; ++tid) { //creo nt threads
		argAPasar[tid].first.first = cantFilas; //intervalo
		argAPasar[tid].second = nt; //fila inicial
		argAPasar[tid].first.second = &(retValues[tid]); //puntero
		pthread_create(&thread[tid], NULL, procesarFila, &argAPasar[tid]);
	}

	for (tid = 0; tid < nt; ++tid){
		pthread_join(thread[tid], NULL);
	}*/

}
