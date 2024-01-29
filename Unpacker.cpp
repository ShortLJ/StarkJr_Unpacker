#include<unistd.h>


#include <queue>
#include <cstring>

#include <stdio.h>
#include <stdint.h>

#include "Sig.h"
#include "TimeSorter.h"


#define Ndet 6
#define Npad 4
#define Nstr 8


void print_usage()
{
	fprintf(stdout,"StarkJr_Unpacker \\\n");
	fprintf(stdout,"--input,-i <file.dat>\\\n");
	fprintf(stdout,"--map,-m <file.txt>\\\n");
}

int main(int argc, char *argv[])
{
	char *inputfilename;
	char *mapfilename;
 



	if (argc<2)
	{
		print_usage();
		return -1;
	}
	for (int i=1; i<argc; i++)
	{
		if ((strcmp(argv[i],"--input")==0 || strcmp(argv[i],"-i")==0) && (argv[i+1]))
		{
			inputfilename = argv[++i];
		}
		else if ((strcmp(argv[i],"--map")==0 || strcmp(argv[i],"-m")==0) && (argv[i+1]))
		{
			mapfilename = argv[++i];
		}
		else if (strcmp(argv[i],"-h")==0)
		{
			print_usage();
			return 0;
		}
		else
		{
			fprintf(stderr,"invalid opt\n");
			print_usage();
			return -1;
		}
	}



	TimeSorter timesorter(inputfilename);
	timesorter.ReadAndFillQ();

	timesorter.PrintSize();


	int idet; int ipad; int istr;

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
	uint8_t ch_juncD[Ndet][Nstr]; 
	for (idet=0; idet<Ndet; idet++) for (istr=0; istr<Nstr; istr++) ch_juncD[idet][istr] = ch_juncU[idet][istr]+1;

	uint8_t sid_ohmic[Ndet] = {0,0,0,0,0,0};
	uint8_t mid_ohmic[Ndet] = {7,7,7,7,7,7};
	uint8_t ch_ohmic[Ndet][Npad] ={
		{ 0, 1, 2, 3},
		{ 4, 5, 6, 7},
		{ 8, 9,10,11},
		{16,17,18,19},
		{20,21,22,23},
		{24,25,26,27}	};
	
	idet=1;
	bool allempty=0;
	bool fff; 
	uint64_t minlgt; //uint8_t minsid, minmid, minch;
	uint64_t prevlgt = 0; int64_t detlgt; int64_t intpad = INT64_MAX>1;
	bool fir;
	Sig ssss[3];
	while(!allempty)
	{
		fff=1;
		for (int ipad=0; ipad<Npad; ipad++)	for (int istr=0; istr<Nstr; istr++)
		{
			if(timesorter.Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad])) continue;
			if(timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr])) continue;
			if(timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr])) continue;
	
			ssss[0] = timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]) ;
			ssss[1] = timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]) ;
			ssss[2] = timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]) ;

			if(ssss[0].local_gate_time==ssss[1].local_gate_time && ssss[0].local_gate_time==ssss[2].local_gate_time) 
			{
				detlgt = ssss[0].local_gate_time-prevlgt;	prevlgt = ssss[0].local_gate_time;
				fprintf(stdout,"//// Hit built //// detlgt %ld\n", detlgt);
				fprintf(stdout,"\t");
				timesorter.PrintTopAndPop(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]);
				fprintf(stdout,"\t");
				timesorter.PrintTopAndPop(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]);
				fprintf(stdout,"\t");
				timesorter.PrintTopAndPop(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]);
				fff=0;
			}
		}
		if(fff)
		{
			fir=1;
			for (int ipad=0; ipad<Npad; ipad++)	
			{
				if(timesorter.Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad])) continue;
				if(fir) {minlgt=timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time; fir=0; }
				if(minlgt>timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time)
				{
					minlgt=timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time;
				}
			}
			for (int istr=0; istr<Nstr; istr++)
			{
				if(timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr])) continue;
				if(fir) {minlgt=timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).local_gate_time; fir=0; }
				if(minlgt>timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).local_gate_time)
				{
					minlgt=timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).local_gate_time;
				}
			}
			for (int istr=0; istr<Nstr; istr++)
			{
				if(timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr])) continue;
				if(fir) {minlgt=timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).local_gate_time; fir=0; }
				if(minlgt>timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).local_gate_time)
				{
					minlgt=timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).local_gate_time;
				}
			}

			detlgt = minlgt-prevlgt;	prevlgt = minlgt;
			fprintf(stdout,"//// Sig Dumped //// detlgt %ld\n",detlgt);
			for (int ipad=0; ipad<Npad; ipad++)	
			{
				if(timesorter.Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad])) continue;
				if(minlgt==timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time)
				{
					timesorter.PrintTopAndPop(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]);
				}
			}
			for (int istr=0; istr<Nstr; istr++)
			{
				if(timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr])) continue;
				if(minlgt==timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).local_gate_time) 
				{
					timesorter.PrintTopAndPop(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]);
				}
			}
			for (int istr=0; istr<Nstr; istr++)
			{
				if(timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr])) continue;
				if(minlgt==timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).local_gate_time) 
				{
					timesorter.PrintTopAndPop(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]);
				}
			}
		}
		allempty=1;
		for (int ipad=0; ipad<Npad; ipad++)	
		{
			if(!timesorter.Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad])) allempty=0;
		}
		for (int istr=0; istr<Nstr; istr++)
		{
			if(!timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr])) allempty=0;
			if(!timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr])) allempty=0;
		}
		if(allempty) break;
	
	}









	return 0;

}

