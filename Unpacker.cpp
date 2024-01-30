#include<unistd.h>


#include <queue>
#include <cstring>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//#include "TInterpreter.h"
#include "TFile.h"
#include "TTree.h"

#include "Sig.h"
#include "TimeSorter.h"
#include "HitBuilder.h"
//#include "WriteTree.h"


void print_usage()
{
	fprintf(stdout,"StarkJr_Unpacker \\\n");
	fprintf(stdout,"--input,-i <file.dat>\\\n");
	fprintf(stdout,"--map,-m <file.txt>\\\n");
	fprintf(stdout,"--timewindow,-tw <timewindow> ## 8 ns for NKfadc\\\n");
}

int main(int argc, char *argv[])
{
	char *inputfilename;
	char *mapfilename;
 	int64_t timewindow = 0;



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
		else if ((strcmp(argv[i],"--timewindow")==0 || strcmp(argv[i],"-tw")==0) && (argv[i+1]))
		{
			timewindow = atoll(argv[++i]);
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
//	memo for sid mapping parser
// int sid[4] = {2,4,6,8};
// int iii[128]
// for(int i=0; i<4; i++){ iii[sid[i]]=i; }
//



	TimeSorter timesorter(inputfilename);
	timesorter.ReadAndFillQ();

	timesorter.PrintSize();

	HitBuilder hitbuilder(&timesorter);
	hitbuilder.SetTimeWindow(timewindow);

	//gInterpreter->GenerateDictionary("vector<Hit>", "Hit.h");

	TFile *file = new TFile("output.root","recreate");
	TTree *tree = new TTree("nkfadc","nkfadc");
	//Hit hits[Ndet];
	//tree->Branch("hits[Ndet]", hits );
	vector<Hit> hits;
	tree->Branch("vector<Hit>", &hits );


	while(!hitbuilder.AllEmpty())
	{
		hitbuilder.Build();
		//hitbuilder.PrintBuilt();
		for (int idet=0; idet<Ndet; idet++)
		{
			//hits[idet] = hitbuilder.GetHit(idet);
			hits.clear();
			hits.push_back(hitbuilder.GetHit(idet));
		}
		tree->Fill();
	}

	tree->Write();
	file->Close();

	return 0;

}

