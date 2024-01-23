
#include <queue>

#include "Sig.h"


#define Nsid 2
#define Nmid 8
#define Nch 32




#ifndef __TIMESORTER__
#define __TIMESORTER__




using namespace std;

class TimeSorter
{
	public:
		priority_queue<Sig> q_sig[Nsid][Nmid][Nch];

		TimeSorter();
		~TimeSorter();

		void Pop(uint8_t sid, uint8_t mid, uint8_t ch, bool del);
		bool Empty(uint8_t sid, uint8_t mid, uint8_t ch);
		void Push(Sig sig);
		uint32_t Size(uint8_t sid, uint8_t mid, uint8_t ch);
		Sig Top(uint8_t sid, uint8_t mid, uint8_t ch);
		virtual void Clear();
		void PrintTopAll();
		void PrintTop(uint8_t sid, uint8_t mid, uint8_t ch);
		void Print(uint8_t sid, uint8_t mid, uint8_t ch);

		bool checker(uint8_t sid, uint8_t mid, uint8_t ch);


	private:
		//uint8_t isid;
		//uint8_t imid;
		//uint8_t ich;
		Sig thissig;

};








#endif // __TIMESORTER__


