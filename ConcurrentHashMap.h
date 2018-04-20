#include "ListaAtomica.hpp"
#include <utility>
#include <iostream>
#include <semaphore.h>
#include <mutex>
#include <list>
#include <fstream>
#include <vector>

using namespace std;
class ConcurrentHashMap {
	private:

		sem_t* semaforosAddAndInt[26];
		mutex max_mutex;

		sem_t* addInc;
		sem_t* maximumSem;
		sem_t* fair;

		uint maxCount;
		uint addCount;

		atomic_int archivoADesencolar;

	public:
		Lista<pair<string,int> > tabla[26];

		ConcurrentHashMap();

		void addAndInc(string key);
		void addAndIncN(string key, int n);
		
		bool member(string key);
		
		pair<string, int> maximum(unsigned int nt);

		static ConcurrentHashMap* count_words(string arch);
		static ConcurrentHashMap* count_words(list<string> archs);
		static ConcurrentHashMap* count_words(unsigned int n, list<string> archs);

		static void printConcurrentHashMap(ConcurrentHashMap* h);

		static pair<string, unsigned int> maximum5(unsigned int p_archivos, unsigned int p_maximos, list<string> archs);
		static pair<string, unsigned int> maximum6(unsigned int p_archivos, unsigned int p_maximos, list<string> archs);

		typedef struct {
			int intervalo;
			int filaInicial;
			pair<string, int> * max;
			Lista<pair<string,int> >*  dirTabla;
		} threadArguments;

	
};
