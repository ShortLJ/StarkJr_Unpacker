#include <cstdio>


#include "HitBuilder.h"


HitBuilder::HitBuilder(TimeSorter *timesorter)
	: timesorter(timesorter)
{
	//b_first=1;
	minlgt=0;
}

HitBuilder::~HitBuilder()
{	}


void HitBuilder::ReadMapFile(const char *file)
{
	vector<int> v_det; 
	vector<int> v_sid_junc, v_mid_junc, v_chf_junc, v_chl_junc;
	vector<int> v_sid_ohmic, v_mid_ohmic, v_chf_ohmic, v_chl_ohmic;
	
	FILE *fr = fopen(file,"r");
	if(fr==NULL)
	{
		fprintf(stderr,"map file is not opened.\n");
		exit(-6);
	}
	int a[9];
	//int det,	junc_sid,	junc_mid,	junc_ch_begin,	junc_ch_end,	ohmic_sid,	ohmic_mid,	ohmic_ch_begin,	ohmic_ch_end;
	char line[100];
	while (fgets(line, sizeof line, fr))
	{
		if (*line == '#') continue;
		if (sscanf(line, "%d %d %d %d %d %d %d %d %d",&a[0],&a[1],&a[2],&a[3],&a[4],&a[5],&a[6],&a[7],&a[8]) !=9)
		{
			fprintf(stderr,"failed to read map file\n");
			exit(-7);
		}
		else
		{
			fprintf(stdout,"%d %d %d %d %d %d %d %d %d\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8]);
			v_det		.push_back(a[0]);
			v_sid_junc	.push_back(a[1]);
			v_mid_junc	.push_back(a[2]);
			v_chf_junc	.push_back(a[3]);
			v_chl_junc	.push_back(a[4]);
			v_sid_ohmic	.push_back(a[1]);
			v_mid_ohmic	.push_back(a[2]);
			v_chf_ohmic	.push_back(a[3]);
			v_chl_ohmic	.push_back(a[4]);
		}
	}
		


}




void HitBuilder::Build()
{
	if(AllEmpty())
	{
		fprintf(stdout,"all q in timesorter is empty\n");
		return;
	}
	else	
	{
		minlgt = GetMinLGT();
		Clear();
		for (idet=0; idet<Ndet; idet++)	FindSigWithLGT(idet, minlgt, timewindow);
	}
}





uint64_t HitBuilder::GetMinLGT()
{
	uint64_t ret;
	bool fir=1;
	for (idet=0; idet<Ndet; idet++)
	{
		for (ipad=0; ipad<Npad; ipad++)	if(!timesorter->Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]))
		{
			if(fir) {ret=timesorter->Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time; fir=0; }
			if(ret>timesorter->Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time)
				ret=timesorter->Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time;
		}
		for (istr=0; istr<Nstr; istr++)	if(!timesorter->Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]))
		{
			if(fir) {ret=timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).local_gate_time; fir=0; }
			if(ret>timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).local_gate_time)
				ret=timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).local_gate_time;
		}
		for (istr=0; istr<Nstr; istr++)	if(!timesorter->Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]))
		{
			if(fir) {ret=timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).local_gate_time; fir=0; }
			if(ret>timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).local_gate_time)
				ret=timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).local_gate_time;
		}
	}
	return ret;
}

void HitBuilder::Clear(int idet)
{
	for (int itype=0; itype<Ntype; itype++)
		v_sig[itype][idet].clear();
}
void HitBuilder::Clear()
{
	for (int i=0; i<Ndet; i++) Clear(i);
}
void HitBuilder::Push_Back(int idet, Sig sig, int itype)
{
	if(itype>=0 && itype<3)
		v_sig[itype][idet].push_back(sig);
	else {fprintf(stderr,"HitBuilder::Push_Back(int idet, Sig sig, int type): Unknown type\n");}
}
void HitBuilder::Push_Back_AndPop(int idet, uint8_t isid, uint8_t imid, uint8_t ich, int type)
{
	Push_Back(idet, timesorter->Top(isid,imid,ich), type);
	timesorter->Pop(isid,imid,ich);
}
uint32_t HitBuilder::Size(int idet)
{
	uint32_t ret=0;
	for (int itype=0; itype<3; itype++)	ret+=Size(idet,itype);
	return ret;
}
uint32_t HitBuilder::Size(int idet, int itype)
{
	if(itype>=0 && itype<3)
		return v_sig[itype][idet].size();
	else {fprintf(stderr,"HitBuilder::Size(int idet, int type): Unknown type\n"); exit(-7);}
	return -1;
}



int HitBuilder::FindSigWithLGT(int idet, uint64_t lgt, int64_t tw)
{
	for (ipad=0; ipad<Npad; ipad++)	if(!timesorter->Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]))
	{
		if(timesorter->Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time-lgt<=tw)
			Push_Back_AndPop(idet,sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad],0);
	}
	for (istr=0; istr<Nstr; istr++)	if(!timesorter->Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]))
	{
		if(timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).local_gate_time-lgt<=tw) 
			Push_Back_AndPop(idet,sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr],1);
	}
	for (istr=0; istr<Nstr; istr++)	if(!timesorter->Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]))
	{
		if(timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).local_gate_time-lgt<=tw) 
			Push_Back_AndPop(idet,sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr],2);
	}
	return Size(idet);
}

void HitBuilder::PrintBuilt(int idet)
{
	fprintf(stdout,"//// Hit candidate on det%u built ////\n", idet);

	for (int itype=0; itype<Ntype; itype++)	for (int i=0; i<Size(idet, itype); i++)
	{
		fprintf(stdout,"type%d\t", itype);
		v_sig[itype][idet].at(i).Print();
	}
}
void HitBuilder::PrintBuilt()
{
	for (int idet=0; idet<Ndet; idet++)
	{
		if (Size(idet)>0) PrintBuilt(idet);
	}
}




bool HitBuilder::AllEmpty(int idet)
{
	for (ipad=0; ipad<Npad; ipad++)	if(!timesorter->Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad])) return 0;
	for (istr=0; istr<Nstr; istr++)	if(!timesorter->Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr])) return 0;
	for (istr=0; istr<Nstr; istr++)	if(!timesorter->Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr])) return 0;
	return 1;
}

bool HitBuilder::AllEmpty()
{
	for (idet=0; idet<Ndet; idet++)	if(!AllEmpty(idet)) return 0;
	return 1;
}






bool HitBuilder::isGood()
{
	return 1;
	///////////// where we will put constraints for identifying a particle hit
}








