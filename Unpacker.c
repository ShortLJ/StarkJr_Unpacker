#include <stdio.h>
//#include "TString.h"



void print_usage()
{
		fprintf(stdout,"opt explain\n");
}





int main(int argc, char *argv[])
{
	char *inputfilename;



	if(argc<2)
	{
		print_usage();
		return -1;
	}
	for (int i=1; i<argc; i++)
	{
		if(strcmp(argv[i],"-i")==0 && argv[i+1])
		{
			inputfilename = argv[++i];
		}
		else if(strcmp(argv[i],"-h")==0)
		{
			print_usage();
			return 0;
		}
		else
		{
			fprintf(stderr,"invalid opt\n");
			print_usage();
		}
	}



	fprintf(stdout, "Input File: %s\n",inputfilename);
	return 0;

}

