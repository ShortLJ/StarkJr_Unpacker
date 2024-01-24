
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "TimeSorter.h"



TimeSorter::TimeSorter(char *inputfilename)
{
	fp = fopen(inputfilename,"rb");
	if (fp==NULL)
	{
		fprintf(stderr,"No file named \"%s\"\n",inputfilename);
		exit(-2);
	}
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	fprintf(stdout, "Opened innput File: \"%s\" (%d Bytes)\n",inputfilename, file_size);
	if (file_size<0)
	{
		fprintf(stderr, "file size is larger than 2GB\n");
		exit(-3);
	}
	fseek(fp, 0, SEEK_SET);
	data_read=0;
}



TimeSorter::~TimeSorter()
{
	fclose(fp);
}

int TimeSorter::ReadAndFillQ()
{
	int evt_processed = 0;
	while (data_read < file_size)
	{
		//fprintf(stdout, "data_reading after = %d/%d\n", data_read, file_size);
		data_read += fread(data, 1, 32, fp);
		data_length = data[0] & 0x00FF;
		if (data_length != 32) 
		{
			fprintf(stderr, "\ndata_length %u!=32\n data_read += fread(data, 1, 8160, fp);\n",data_length);
			data_read += fread(data, 1, 8160, fp);
		}      
		else
		{
			sig_tmp = Sig(data);
		}
		fprintf(stdout, "\rdata_read to= %d/%d\t", data_read, file_size);

		Push(sig_tmp);
		evt_processed++;
		fprintf(stdout,"evt_processed %d", evt_processed);
		fflush(stdout);


	}
	fprintf(stdout,"\n");
	fprintf(stdout,"\n");

	return evt_processed;
}







void TimeSorter::Pop(uint8_t isid, uint8_t imid, uint8_t ich)
{
	checker(isid,imid,ich);
	q_sig[isid][imid][ich].pop();
}

bool TimeSorter::Empty(uint8_t isid, uint8_t imid, uint8_t ich)
{
	checker(isid,imid,ich);
	return q_sig[isid][imid][ich].empty();
}

void TimeSorter::Push(Sig sig)
{
	uint8_t isid = sig.sid; 
	uint8_t imid = sig.mid;
	uint8_t ich = sig.ch;
	checker(isid,imid,ich);
	q_sig[isid][imid][ich].push(sig);
}

uint32_t TimeSorter::Size(uint8_t isid, uint8_t imid, uint8_t ich)
{
	checker(isid,imid,ich);
	return q_sig[isid][imid][ich].size();
}

Sig TimeSorter::Top(uint8_t isid, uint8_t imid, uint8_t ich)
{	
	checker(isid,imid,ich);
	if(Empty(isid,imid,ich)) return Sig();
	return q_sig[isid][imid][ich].top();
}


void TimeSorter::Clear()
{
	uint8_t isid;
	uint8_t imid;
	uint8_t ich;
	
	for(isid=0; isid<Nsid; isid++)	for(imid=0; imid<Nmid; imid++)	for(ich=0; ich<Nch; ich++)
	{
		while(q_sig[isid][imid][ich].size()>0) 
		{
			Pop(isid,imid,ich);
		}
	}
}

void TimeSorter::PrintTop(uint8_t isid, uint8_t imid, uint8_t ich)
{
	checker(isid,imid,ich);
	if(Empty(isid,imid,ich)) return;
	Sig st = q_sig[isid][imid][ich].top();
	st.Print();
}

void TimeSorter::PrintTopAndPop(uint8_t isid, uint8_t imid, uint8_t ich)
{
	PrintTop(isid,imid,ich);
	Pop(isid,imid,ich);
}

void TimeSorter::PrintTopAll()
{
	uint8_t isid;
	uint8_t imid;
	uint8_t ich;
	
	for(isid=0; isid<Nsid; isid++)	for(imid=0; imid<Nmid; imid++)	for(ich=0; ich<Nch; ich++) if(imid%2==1)
	{
		if(Empty(isid,imid,ich)) continue;
		//fprintf(stdout, "sid %u mid %u ch %u\t", isid, imid, ich);
		//fprintf(stdout, "size %u\n", Size(isid, imid, ich) );
		PrintTop(isid, imid, ich);
	}
}




bool TimeSorter::checker(uint8_t isid, uint8_t imid, uint8_t ich)
{
	if(isid<Nsid && imid<Nmid && ich<Nch) return true;
	else
	{
		fprintf(stderr,"err: failed at (isid%u<Nsid%d && imid%u<Nmid%d && ich%u<ich%d)\n", isid,Nsid,imid,Nmid,ich,Nch);
		exit(-4);
		return false;
	}
	return false;
}

