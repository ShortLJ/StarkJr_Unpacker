


#include "TimeSorter.h"

#ifndef __HITBUILDER__
#define __HITBUILDER__

#define Ndet 6
#define Npad 4
#define Nstr 8

uint8_t sid_junc[Ndet] = {0,0,0,0,0,0};
uint8_t mid_junc[Ndet] = {1,1,3,3,5,5};
uint8_t ch_juncU[Ndet][Nstr] ={
	{ 0, 2, 4, 6, 8,10,12,14},
	{16,18,20,22,24,26,28,30},
	{ 0, 2, 4, 6, 8,10,12,14},
	{16,18,20,22,24,26,28,30},
	{ 0, 2, 4, 6, 8,10,12,14},
	{16,18,20,22,24,26,28,30}
};
uint8_t ch_juncD[Ndet][Nstr] ={
	{ 1, 3, 5, 7, 9,11,13,15},
	{17,19,21,23,25,27,29,31},
	{ 1, 3, 5, 7, 9,11,13,15},
	{17,19,21,23,25,27,29,31},
	{ 1, 3, 5, 7, 9,11,13,15},
	{17,19,21,23,25,27,29,31}
};

uint8_t sid_ohmic[Ndet] = {0,0,0,0,0,0};
uint8_t mid_ohmic[Ndet] = {7,7,7,7,7,7};
uint8_t ch_ohmic[Ndet][Npad] ={
	{ 0, 1, 2, 3},
	{ 4, 5, 6, 7},
	{ 8, 9,10,11},
	{16,17,18,19},
	{20,21,22,23},
	{24,25,26,27}	
};






using namespace std;

class HitBuilder
{
	public:
		HitBuilder(TimeSorter*);
		~HitBuilder();

		void SetTimeWindow(int64_t tw){timewindow=tw;}
		void Func();


	private:
		TimeSorter *timesorter;

		uint64_t GetMinLGT();
		void Clear();
		void Push_Back(uint8_t isid, uint8_t imid, uint8_t ich);
		uint32_t Size();
		int FindSigWithLGT(int idet, uint64_t lgt);
		void PrintBuilt(int idet);
		bool AllEmpty(int idet);
		bool AllEmpty();



		int idet, ipad, istr;
		//bool b_first;
		uint64_t minlgt;
		uint64_t prevlgt;
		//int64_t dlgt;
		vector<uint8_t> v_hit_sid, v_hit_mid, v_hit_ch;

		
		int64_t timewindow = 0;





};
#endif // __HITBUILDER__



HitBuilder::HitBuilder(TimeSorter *timesorter)
	: timesorter(timesorter)
{
	//b_first=1;
	minlgt=0;
}

HitBuilder::~HitBuilder()
{	}


void HitBuilder::Func()
{
	while(AllEmpty())
	{
		minlgt = GetMinLGT();

		for (idet=0; idet<Ndet; idet++)
		{
			Clear();
			if(FindSigWithLGT(idet, minlgt)>0)
			{
				PrintBuilt(idet);
			}
		}
	}
}


