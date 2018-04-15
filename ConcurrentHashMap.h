#include "ListaAtomica.hpp"
#include <utility>
#include <iostream>
#include <semaphore.h>
#include <mutex>

using namespace std;
class ConcurrentHashMap {
	private:

		sem_t* semaforosAddAndInt[26];
		mutex max_mutex;


	public:
		Lista<pair<string,int> > tabla[26];

		ConcurrentHashMap();
		
		void addAndInc(string key);
		
		bool member(string key);
		
		pair<string, int> maximum(unsigned int nt);

		typedef struct {
			int intervalo;
			int filaInicial;
			pair<string, int> * max;
			Lista<pair<string,int> >* list;
		} threadArguments;

	
};
