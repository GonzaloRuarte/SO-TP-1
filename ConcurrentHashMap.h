#include "ListaAtomica.hpp"
#include <utility>
#include <iostream>
#include <semaphore.h>

using namespace std;
class ConcurrentHashMap {
	private:
		Lista<pair<string,int> > table[26];

		sem_t* semaforosAddAndInt[26];

		sem_t* semaforoIsWriting;
	public:
		ConcurrentHashMap();
		
		void addAndInc(string key);
		
		bool member(string key);
		
		pair<string, int> maximum(unsigned int nt);
	
};
