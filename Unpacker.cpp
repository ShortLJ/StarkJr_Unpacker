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


	/*int idet=0; int ipad=1; int istr=3;

	int sid_junc[Ndet] = {0,0,0,0,0,0};
	int mid_junc[Ndet] = {1,1,3,5,5,7};
	int ch_juncU[Ndet][Nstr] ={
		{ 0, 2, 4, 6, 8,10,12,14},
		{16,18,20,22,24,26,28,30},
		{ 0, 2, 4, 6, 8,10,12,14},
		{ 0, 2, 4, 6, 8,10,12,14},
		{16,18,20,22,24,26,28,30},
		{ 0, 2, 4, 6, 8,10,12,14}	};
	int ch_juncD[Ndet][Nstr]; 
	for (idet=0; idet<Ndet; idet++) for (istr=0; istr<Nstr; istr++) ch_juncD[idet][istr] = ch_juncU[idet][istr]+1;

	int sid_ohmic[Ndet] = {0,0,0,0,0,0};
	int mid_ohmic[Ndet] = {3,3,3,7,7,7};
	int ch_ohmic[Ndet][Npad] ={
		{16,17,18,19},
		{20,21,22,23},
		{24,25,26,27},
		{16,17,18,19},
		{20,21,22,23},
		{24,25,26,27}	};
	*/
	
	int idet=0; int ipad=1; int istr=3;

	uint8_t sid_junc[Ndet] = {0,0,0,0,0,0};
	uint8_t mid_junc[Ndet] = {1,1,3,3,5,5};
	uint8_t ch_juncU[Ndet][Nstr] ={
		{ 0, 2, 4, 6, 8,10,12,14},
		{16,18,20,22,24,26,28,30},
		{ 0, 2, 4, 6, 8,10,12,14},
		{16,18,20,22,24,26,28,30},
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
	bool allempty=1;
	bool fff; uint64_t minttt; bool fir;
	Sig ssss[3];
	while(allempty)
	{
		fff=1;
		for (int ipad=0; ipad<Npad; ipad++)	for (int istr=0; istr<Nstr; istr++)
		{
			ssss[0] = timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]) ;
			ssss[1] = timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]) ;
			ssss[2] = timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]) ;

			if(ssss[0].local_gate_time==ssss[1].local_gate_time && ssss[0].local_gate_time==ssss[2].local_gate_time) 
			{
				fprintf(stdout,"//// Hit built ////\n");
				timesorter.PrintTopAndPop(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]);
				timesorter.PrintTopAndPop(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]);
				timesorter.PrintTopAndPop(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]);
				fff=0;
			}
		}
		if(fff)
		{
			fir=1;
			for (int ipad=0; ipad<Npad; ipad++)	
			{
				if(fir)
				{
					minttt=timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time; fir=0;
				}
				if(minttt>timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time) 
					minttt=timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time;
			}
			for (int istr=0; istr<Nstr; istr++)
			{
				if(minttt>timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][ipad]).local_gate_time) 
					minttt=timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][ipad]).local_gate_time;
				if(minttt>timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][ipad]).local_gate_time) 
					minttt=timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][ipad]).local_gate_time;
			}


			fprintf(stdout,"//// Sig Dumped ////\n");
			for (int ipad=0; ipad<Npad; ipad++)	
			{
				if(minttt==timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time)
				{
					timesorter.PrintTopAndPop(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]);
				}
			}
			for (int istr=0; istr<Nstr; istr++)
			{
				if(minttt==timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][ipad]).local_gate_time) 
				{
					timesorter.PrintTopAndPop(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]);
				}
				if(minttt==timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][ipad]).local_gate_time) 
				{
					timesorter.PrintTopAndPop(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]);
				}
			}


		}



		if (timesorter.Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad])) break;
		if (timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr])) break;
		if (timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr])) break;

	}

	/*idet=1; 
	//for (int idet=0; idet<Ndet; idet++) 
	{
	fprintf(stdout,"\n");
		fprintf(stdout,"\n");
		for (int ipad=0; ipad<Npad; ipad++)
		{
			if (!timesorter.Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]))
			{
				timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).Print();
				timesorter.Pop(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]);
			}
		}
		fprintf(stdout,"\n");
		for (int istr=0; istr<Nstr; istr++)
		{
			if (!timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]))
			{
				timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).Print();
				timesorter.Pop(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]);
			}
			if (!timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]))
			{
				timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).Print();
				timesorter.Pop(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]);
			}
		}
		fprintf(stdout,"\n");
		for (int ipad=0; ipad<Npad; ipad++)
		{
			if (!timesorter.Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]))
			{
				timesorter.Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).Print();
				timesorter.Pop(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]);
			}
		}
		fprintf(stdout,"\n");
		for (int istr=0; istr<Nstr; istr++)
		{
			if (!timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]))
			{
				timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).Print();
				timesorter.Pop(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]);
			}
			if (!timesorter.Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]))
			{
				timesorter.Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).Print();
				timesorter.Pop(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]);
			}
		}
	}
	*/







	return 0;

}

