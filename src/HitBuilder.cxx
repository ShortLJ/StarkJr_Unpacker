


#include "HitBuilder.h"






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

void HitBuilder::Clear()
{
	v_hit_sid.clear();
	v_hit_mid.clear();
	v_hit_ch .clear();
}
void HitBuilder::Push_Back(uint8_t isid, uint8_t imid, uint8_t ich)
{
	v_hit_sid.push_back(isid);
	v_hit_mid.push_back(imid);
	v_hit_ch .push_back(ich);
}
uint32_t HitBuilder::Size()
{
	return v_hit_sid.size();
}


int HitBuilder::FindSigWithLGT(int idet, uint64_t lgt)
{
	for (ipad=0; ipad<Npad; ipad++)	if(!timesorter->Empty(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]))
	{
		if(timesorter->Top(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]).local_gate_time-lgt<=timewindow)
			Push_Back(sid_ohmic[idet], mid_ohmic[idet], ch_ohmic[idet][ipad]);
	}
	for (istr=0; istr<Nstr; istr++)	if(!timesorter->Empty(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]))
	{
		if(timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]).local_gate_time-lgt<=timewindow) 
			Push_Back(sid_junc[idet], mid_junc[idet], ch_juncU[idet][istr]);
	}
	for (istr=0; istr<Nstr; istr++)	if(!timesorter->Empty(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]))
	{
		if(timesorter->Top(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]).local_gate_time-lgt<=timewindow) 
			Push_Back(sid_junc[idet], mid_junc[idet], ch_juncD[idet][istr]);
	}
	return Size();
}

void HitBuilder::PrintBuilt(int idet)
{
	fprintf(stdout,"//// Hit candidate on det%u built ////\n", idet);
	for (int i=0; i<Size(); i++)
	{
		fprintf(stdout,"\t");
		timesorter->PrintTopAndPop(v_hit_sid[i], v_hit_mid[i], v_hit_ch[i]);	///////// output of this "builder"
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








