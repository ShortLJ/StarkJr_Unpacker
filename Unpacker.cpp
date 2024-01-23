#include<unistd.h>


#include <queue>
#include <cstring>

#include <stdio.h>
#include <stdint.h>

#include "Sig.h"
#include "TimeSorter.h"



//#define Nsid 2
//#define Nmid 10
//#define Nch 32



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
	FILE *fp;
	int file_size;
	int data_read;
	uint64_t evt_processed = 0;
 
 	uint8_t data[8192];
	uint8_t data_length;
	Sig sig_tmp;


	TimeSorter *timesorter = new TimeSorter();
	//std::priority_queue<Sig*> q_sig[Nsid][Nmid][Nch];

	//bool enabled[Nsid][Nmid][Nch];





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

	fp = fopen(inputfilename,"rb");
	if (fp==NULL)
	{
		fprintf(stderr,"No file named \"%s\"\n",inputfilename);
		return -2;
	}
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	fprintf(stdout, "Opened innput File: \"%s\" (%d Bytes)\n",inputfilename, file_size);
	if (file_size<0)
	{
		fprintf(stderr, "file size is larger than 2GB\n");
		return -3;
	}
	fseek(fp, 0, SEEK_SET);
	data_read = 0;

	while (data_read < file_size)
	{
		//fprintf(stdout, "data_reading after = %d/%d\n", data_read, file_size);
		data_read += fread(data, 1, 32, fp);

		data_length = data[0] & 0x00FF;
		if (data_length != 32) 
		{
			fprintf(stderr, "data_length %u!=32\n data_read += fread(data, 1, 8160, fp);\n",data_length);
			data_read += fread(data, 1, 8160, fp);
		}      
		else
		{
			sig_tmp = Sig(data);
		}

		//sig_tmp.Print();

		fprintf(stdout, "data_read to= %d/%d\t", data_read, file_size);
//		fprintf(stdout,"\n");

		timesorter->Push(sig_tmp);
		//timesorter->PrintTopAll();
		//timesorter->PrintTop(sig_tmp.sid, sig_tmp.mid, sig_tmp.ch);
		//timesorter->Print(sig_tmp.sid, sig_tmp.mid, sig_tmp.ch);


		evt_processed++;


		fprintf(stdout,"evt_processed %lu\n", evt_processed);
		//fflush(stdout);

		fprintf(stdout,"\n");
		//usleep(5000);


		//if((sig_tmp.sid==0 && sig_tmp.mid==7)) continue;
		//if((sig_tmp.sid==0 && sig_tmp.mid==1)) continue;
		//break;
	}


	timesorter->Print(0,7,0);






	fclose(fp);
	return 0;

}

