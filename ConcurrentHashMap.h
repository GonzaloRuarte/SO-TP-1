#include "ListaAtomica.hpp"

using namespace std
class ConcurrentHashMap {
	private:
		Lista<pair<string,int> > hashMap[26];
	public:
		ConcurrentHashMap();
		
		void addAndInc(string,key);
		
		bool member(string, key);
		
		pair<string, int> maximum(unsigned int nt);
	
};
