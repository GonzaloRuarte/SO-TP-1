#include "ConcurrentHashMap.h"
#include <pthread.h>

ConcurrentHashMap::ConcurrentHashMap() {
	for(uint i = 0; i < 26; i++){
		semaforosAddAndInt[i] = new sem_t();
		sem_init(semaforosAddAndInt[i],0,1);
	}
}



void ConcurrentHashMap::addAndInc(string key) {
	int index = (key[0]-(int)'a'); //le restamos 'a' para que empiece de 0

	max_mutex.lock();
	sem_wait(semaforosAddAndInt[index]);
	//SECCION CRITICA
	Lista<pair<string,int> >::Iterador it = tabla[index].CrearIt();
	while(it.HaySiguiente() && it.Siguiente().first!=key){
		it.Avanzar();
	}

	if(it.HaySiguiente() && it.Siguiente().first==key){
		it.Siguiente().second++;
	}else{
		tabla[index].push_front(make_pair(key,1));
	}
	//SECCION CRITICA
	sem_post(semaforosAddAndInt[index]);
	max_mutex.unlock();

}

bool ConcurrentHashMap::member(string key) {
	int index = (key[0]-(int)'a'); //le restamos 'a' para que empiece de 0

	sem_wait(semaforosAddAndInt[index]);
	//SECCION CRITICA
	Lista<pair<string,int> >::Iterador it = tabla[index].CrearIt();
	while(it.HaySiguiente() && it.Siguiente().first!=key){
		it.Avanzar();
	}
	bool member = it.HaySiguiente();
	//SECCION CRITICA
	sem_post(semaforosAddAndInt[index]);

	return member;
}

pair<string, int>  max_pair(pair<string, int>  a, pair<string, int>  b ){
	return a.second>b.second?a:b;
}


void * procesarFila(void *mod) {
	ConcurrentHashMap::threadArguments * args = ((ConcurrentHashMap::threadArguments *)mod);
	pair<string, int> * max = args->max;
	max->second=0;
	for (int i = args->filaInicial; i < 26 ; i+=args->intervalo){

			//SECCION CRITICA
			Lista<pair<string,int> >::Iterador it = args->list->CrearIt();
			while(it.HaySiguiente()){
				*max=max_pair(*max,it.Siguiente());

				it.Avanzar();
			}
			//SECCION CRITICA
		
	}
	pthread_exit(NULL);
}


pair<string, int> ConcurrentHashMap::maximum(unsigned int nt){
	pthread_t thread[nt];
	int tid;
	max_mutex.lock();
	int cantFilas =  div(26,nt).quot; //intervalo, por ej si nt es 4, el thread 2 procesa la fila 2, la 6, la 10, etc.
	pair<string, int> retValues[nt]; //arreglo con los valores de retorno de cada thread
	ConcurrentHashMap::threadArguments argAPasar[nt]; //arreglo con los argumentos que le voy a pasar a cada thread, el intervalo, la fila inicial y un puntero al arreglo retValues, uso long porque las direcciones son de 64 bits
	for (tid = 0; tid < nt; ++tid) { //creo nt threads
		argAPasar[tid].intervalo = cantFilas; //intervalo
		argAPasar[tid].filaInicial = nt*cantFilas; //fila inicial
		argAPasar[tid].max = &(retValues[tid]); //puntero al maximo
		argAPasar[tid].list = &(tabla[tid]); //puntero a la lista

		pthread_create(&thread[tid], NULL, procesarFila, &argAPasar[tid]);
	}

	for (tid = 0; tid < nt; ++tid){
		pthread_join(thread[tid], NULL);
	}
	
	max_mutex.unlock();
	pair<string, int> max =retValues[0];
	for (tid = 1; tid < nt; ++tid){
		max=max_pair(max,retValues[tid]);
	}

	return max;

}

