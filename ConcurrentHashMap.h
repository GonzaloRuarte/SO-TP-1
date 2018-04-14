#include "ListaAtomica.hpp"
#include <utility>
#include <iostream>
#include <semaphore.h>

using namespace std;
class ConcurrentHashMap {
	private:
		Lista<pair<string,int> > table[26];

		sem_t* semaforosAddAndInt[26];

		typedef struct {
			int intervalo;
			int filaInicial;
			pair<string, int> * max;
		} threadArguments;
		
		void procesarFila(void *mod);

	public:
		ConcurrentHashMap();
		
		void addAndInc(string key);
		
		bool member(string key);
		
		pair<string, int> maximum(unsigned int nt);


	
};
