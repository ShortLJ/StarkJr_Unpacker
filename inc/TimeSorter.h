
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

		TimeSorter(char *);
		~TimeSorter();

		int ReadAndFillQ();

		priority_queue<Sig> q_sig[Nsid][Nmid][Nch];
		uint32_t Pop(uint8_t sid, uint8_t mid, uint8_t ch);
		bool Empty(uint8_t sid, uint8_t mid, uint8_t ch);
		void Push(Sig sig);
		uint32_t Size(uint8_t sid, uint8_t mid, uint8_t ch);
		Sig Top(uint8_t sid, uint8_t mid, uint8_t ch);
		virtual void Clear();
		void PrintTopAll();
		void PrintTop(uint8_t sid, uint8_t mid, uint8_t ch);
		void PrintTopAndPop(uint8_t sid, uint8_t mid, uint8_t ch);
		void Print(uint8_t sid, uint8_t mid, uint8_t ch);
		void PrintSize();

		bool checker(uint8_t sid, uint8_t mid, uint8_t ch);


	private:
		FILE *fp;
		int file_size;
		int data_read;
		uint8_t data[8192];
		uint8_t data_length;
		Sig sig_tmp;



};








#endif // __TIMESORTER__


