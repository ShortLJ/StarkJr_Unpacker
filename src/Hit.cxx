#include "Hit.h"

ClassImp(Hit)

Hit::Hit(){	}

Hit::Hit(vector<Sig> vp, vector<Sig> vU, vector<Sig> vD)
	:v_sig_pad(vp), v_sig_stU(vU), v_sig_stD(vD)
{	

}

Hit::~Hit()
{	}



void Hit::Clear()
{
	v_sig_pad.clear();
	v_sig_stU.clear();
	v_sig_stD.clear();
}
