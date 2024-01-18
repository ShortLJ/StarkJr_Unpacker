#include <cstring>

#include <stdio.h>
#include <stdint.h>

#include "Sig.h"

void print_usage()
{
		fprintf(stdout,"opt explain\n");
}

int main(int argc, char *argv[])
{
	char *inputfilename;
	FILE *fp;
	int file_size;
	int data_read;
 
 	uint16_t data[4096];
	uint16_t data_length;
	Sig sig_tmp;


	if (argc<2)
	{
		print_usage();
		return -1;
	}
	for (int i=1; i<argc; i++)
	{
		if (strcmp(argv[i],"-i")==0 && argv[i+1])
		{
			inputfilename = argv[++i];
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
		}
	}

	fp = fopen(inputfilename,"rb");
	if (fp==NULL)
	{
		fprintf(stderr,"No file named \"%s\"\n",inputfilename);
		return -1;
	}
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	if (file_size<0)
	{
		fprintf(stderr, "file size is larger than 2GB\n");
		return -2;
	}
	fseek(fp, 0, SEEK_SET);

	fprintf(stdout, "Opened innput File: \"%s\"\n",inputfilename);

	data_read = 0;

	while (data_read < file_size)
	{
		fprintf(stdout, "data_reading after = %d/%d\n", data_read, file_size);
		data_read += 2* fread(data, 2, 16, fp);

		data_length = data[0] & 0x00FF;
		if (data_length != 32) 
		{
			fflush(stdout);
			fflush(stderr);
			fprintf(stderr, "data_length %u!=32\n data_read += 2*fread(data, 2, 4080, fp);\n",data_length);
			data_read += 2* fread(data, 2, 8160/2, fp);
		}      
		else
		{
			sig_tmp = Sig(data);
		}

		fprintf(stdout,"sid %u mid %u ch %u\t",sig_tmp.sid, sig_tmp.mid, sig_tmp.ch);
		fprintf(stdout,"tcb_trigger_number %u local_trigger_number %u\t", sig_tmp.tcb_trigger_number, sig_tmp.local_trigger_number);
		fprintf(stdout,"tcb_trigger_time %u local_gate_time %u\t", sig_tmp.tcb_trigger_time, sig_tmp.local_gate_time);


		fprintf(stdout,"\n");
		fprintf(stdout, "data_read to= %d/%d\n", data_read, file_size);
		fprintf(stdout,"\n");
	}








	fclose(fp);
	return 0;

}

