#include "ConcurrentHashMap.h"
#include <pthread.h>

ConcurrentHashMap::ConcurrentHashMap() {
	for(uint i = 0; i < 26; i++){
		semaforosAddAndInt[i] = new sem_t();
		sem_init(semaforosAddAndInt[i],0,1);
	}
	maximumSem = new sem_t();
	sem_init(maximumSem,0,1);
	addInc = new sem_t();
	sem_init(addInc,0,1);
	fair = new sem_t();
	sem_init(fair,0,1);
	maxCount = 0;
	addCount = 0;
}



void ConcurrentHashMap::addAndInc(string key) {
	int index = (key[0]-(int)'a'); //le restamos 'a' para que empiece de 0

	sem_wait(fair);
	sem_wait(addInc);
	addCount++;
	if(addCount == 1){sem_wait(maximumSem);}
	sem_post(addInc);
	sem_post(fair);

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

	sem_wait(addInc);
	addCount--;
	if(addCount == 0){sem_post(maximumSem);}
	sem_post(addInc);
}

void ConcurrentHashMap::addAndIncN(string key, int n) {
	int index = (key[0]-(int)'a'); //le restamos 'a' para que empiece de 0

	sem_wait(fair);
	sem_wait(addInc);
	addCount++;
	if(addCount == 1){sem_wait(maximumSem);}
	sem_post(addInc);
	sem_post(fair);

	sem_wait(semaforosAddAndInt[index]);
	//SECCION CRITICA
	Lista<pair<string,int> >::Iterador it = tabla[index].CrearIt();
	while(it.HaySiguiente() && it.Siguiente().first!=key){
		it.Avanzar();
	}

	if(it.HaySiguiente() && it.Siguiente().first==key){
		it.Siguiente().second += n;
	}else{
		tabla[index].push_front(make_pair(key,1));
	}
	//SECCION CRITICA
	
	sem_post(semaforosAddAndInt[index]);

	sem_wait(addInc);
	addCount--;
	if(addCount == 0){sem_post(maximumSem);}
	sem_post(addInc);
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
	ConcurrentHashMap::threadArguments* args = ((ConcurrentHashMap::threadArguments *)mod);
	pair<string, int>* max = args->max;
	Lista<pair<string,int> >* dirTabla = args->dirTabla;
	max->second=0;
	for (int i = args->filaInicial; i < 26 ; i+=args->intervalo){

			//SECCION CRITICA
			Lista<pair<string,int> >::Iterador it = args->dirTabla[i].CrearIt();
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
	
	sem_wait(fair);
	sem_wait(maximumSem);
	maxCount++;
	if(maxCount == 1){sem_wait(addInc);}
	sem_post(maximumSem);
	sem_post(fair);


	int cantFilas =  div(26,nt).quot; //intervalo, por ej si nt es 4, el thread 2 procesa la fila 2, la 6, la 10, etc.
	pair<string, int> retValues[nt]; //arreglo con los valores de retorno de cada thread
	ConcurrentHashMap::threadArguments argAPasar[nt]; //arreglo con los argumentos que le voy a pasar a cada thread, el intervalo, la fila inicial y un puntero al arreglo retValues, uso long porque las direcciones son de 64 bits
	for (tid = 0; tid < nt; ++tid) { //creo nt threads
		argAPasar[tid].intervalo = nt; //intervalo
		argAPasar[tid].filaInicial = tid; //fila inicial
		argAPasar[tid].max = &(retValues[tid]); //puntero al maximo
		argAPasar[tid].dirTabla = &(tabla[0]); //puntero a la lista

		pthread_create(&thread[tid], NULL, procesarFila, &argAPasar[tid]);
	}

	for (tid = 0; tid < nt; ++tid){
		pthread_join(thread[tid], NULL);
	}
	
	
	sem_wait(maximumSem);
	maxCount--;
	if(maxCount == 0){sem_post(addInc);}
	sem_post(maximumSem);

	pair<string, int> max =retValues[0];
	for (tid = 1; tid < nt; ++tid){
		max=max_pair(max,retValues[tid]);
	}

	return max;

}


//STRUCTS
typedef struct {
	string archivo;
	ConcurrentHashMap* h;
} sCountWords2;

typedef struct {
	ConcurrentHashMap* h;
	vector<string>* v;
	atomic_int* archivoADesencolar;
} sCountWords3;

typedef struct {
	ConcurrentHashMap* mergedH;
	vector<string>* v;
	atomic_int* archivoADesencolar;
} sCountWords5;

//FUNCIONES AUXILIARES
void count_words_aux(string arch, ConcurrentHashMap* h) {
	ifstream entrada(arch);
	string word;
	while(entrada >> word){
		h->addAndInc(word);
	}
}

void* procesarTextoCount2(void* mod) {
	sCountWords2* args = (sCountWords2*) mod;
	count_words_aux(args->archivo, args->h);
	pthread_exit(NULL);
}

void* procesarTextoCount3(void* mod) {
	sCountWords3* args = (sCountWords3*) mod;
	int index =0;
	while(index==args->v->size()){
		index=args->archivoADesencolar->fetch_add(1);
		count_words_aux(args->v->at(index), args->h);
	}
	pthread_exit(NULL);
}

void* procesarTextoCount5(void* mod) {
	sCountWords5* args = (sCountWords5*) mod;

	int index =0;
	while(index<args->v->size()){
		index=args->archivoADesencolar->fetch_add(1);
		if(index<args->v->size()){
			ConcurrentHashMap* h = count_words(args->archivo);

			for(int i = 0; i<26; ++i) {
				for(auto it = h->tabla[i].CrearIt(); it.HaySiguiente(); it.Avanzar()) {
					args->mergedH->addAndIncN(it.Siguiente().first, it.Siguiente().second);
				}
			}
		}
	}


	pthread_exit(NULL);
}

void ConcurrentHashMap::printConcurrentHashMap(ConcurrentHashMap* h) {
	for (uint i = 0; i < 26; i++) {
		for (auto it = h->tabla[i].CrearIt(); it.HaySiguiente(); it.Avanzar()) {
			auto t = it.Siguiente();
			cout << t.first << " " << t.second << endl;
		}
	}
}


//FUNCIONES
ConcurrentHashMap* ConcurrentHashMap::count_words(string arch) {
	ConcurrentHashMap* h = new ConcurrentHashMap();
	count_words_aux(arch, h);
	return h;
}

ConcurrentHashMap* ConcurrentHashMap::count_words(list<string> archs) {
	ConcurrentHashMap* h = new ConcurrentHashMap();
	int nt = archs.size();
	pthread_t threads[nt];
	int tid = 0;
	sCountWords2 argAPasar[nt];
	for (std::list<string>::iterator it=archs.begin(); it != archs.end(); ++it) {
		argAPasar[tid].archivo = *it;
		argAPasar[tid].h = h;
		pthread_create(&threads[tid], NULL, procesarTextoCount2, &argAPasar[tid]);

		++tid;
	}

	for (tid = 0; tid < nt; ++tid){
		pthread_join(threads[tid], NULL);
	}

	return h;
}

ConcurrentHashMap* ConcurrentHashMap::count_words(unsigned int n, list<string> archs) {
	ConcurrentHashMap* h = new ConcurrentHashMap();
	vector<string> v{make_move_iterator(begin(archs)), make_move_iterator(end(archs))};
	pthread_t threads[n];
	sCountWords3 argAPasar[n];

	for (int tid = 0; tid < n; ++tid) {
		argAPasar[tid].h = h;
		argAPasar[tid].v = &v;
		argAPasar[tid].archivoADesencolar = &(this->archivoADesencolar);
		pthread_create(&threads[tid], NULL, procesarTextoCount3, &argAPasar[tid]);
		
	}

	for (int i = 0; i < n; ++i) {
		pthread_join(threads[i], NULL);
	}

	return h;
}

pair<string, unsigned int> ConcurrentHashMap::maximum5(unsigned int p_archivos, unsigned int p_maximos, list<string> archs) {
	ConcurrentHashMap* mergedH = new ConcurrentHashMap();
	pthread_t threads[p_archivos];
	vector<string> v{make_move_iterator(begin(archs)), make_move_iterator(end(archs))};
	sCountWords3 argAPasar[p_archivos];

	for (int tid = 0; tid < p_archivos; ++tid) {
		argAPasar[tid].h = h;
		argAPasar[tid].v = &v;
		argAPasar[tid].archivoADesencolar = &(this->archivoADesencolar);
		pthread_create(&threads[tid], NULL, procesarTextoCount5, &argAPasar[tid]);
		
	}

	for (int i = 0; i < p_archivos; ++i) {
		pthread_join(threads[i], NULL);
	}

	pair<string, unsigned int> maximo = mergedH->maximum(p_maximos);
	delete mergedH;
	return maximo;
}

pair<string, unsigned int> ConcurrentHashMap::maximum6(unsigned int p_archivos, unsigned int p_maximos, list<string> archs) {
	ConcurrentHashMap* h = ConcurrentHashMap::count_words(p_archivos, archs);
	pair<string, unsigned int> maximo = h->maximum(p_maximos);
	delete h;
	return maximo;
}