
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "TimeSorter.h"



TimeSorter::TimeSorter()
{	};



TimeSorter::~TimeSorter()
{	};


void TimeSorter::Pop(uint8_t isid, uint8_t imid, uint8_t ich, bool del=false)
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
			Pop(isid,imid,ich,true);
		}
	}
}

void TimeSorter::PrintTop(uint8_t isid, uint8_t imid, uint8_t ich)
{
	checker(isid,imid,ich);
	if(Empty(isid,imid,ich)) return;
	fprintf(stdout, "TimeSorter::PrintTop(sid %u, mid %u, ch %u)\t",isid, imid, ich);
	fprintf(stdout, "size %u top local_trigger_number %u\n", Size(isid, imid, ich), Top(isid, imid, ich).local_trigger_number);
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

void TimeSorter::Print(uint8_t isid, uint8_t imid, uint8_t ich)
{
	checker(isid,imid,ich);
	if(Empty(isid,imid,ich)) return;
	fprintf(stdout, "TimeSorter::PrintTop(sid %u, mid %u, ch %u)\t",isid, imid, ich);
	fprintf(stdout, "size %u \n", Size(isid, imid, ich));
	while (!Empty(isid,imid,ich))
	{
		Sig temp = Top(isid,imid,ich);
		temp.Print();
		//q_sig[isid][imid][ich].Print();
		Pop(isid,imid,ich);
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

