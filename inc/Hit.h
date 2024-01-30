#include "TObject.h"

#include "Sig.h"

#ifndef __HIT__
#define __HIT__

using namespace std;

class Hit : public TObject
{
	public:
		vector<Sig> v_sig_pad;
		vector<Sig> v_sig_stU;
		vector<Sig> v_sig_stD;
		Hit();
		Hit(vector<Sig>, vector<Sig>, vector<Sig>);
		~Hit();

		void Clear();

		ClassDef(Hit, 1)

		/*
		uint8_t detID;
		uint8_t stripID;
		uint8_t padID;

		Int_t stripValU;
		Int_t stripValD;
		Int_t padVal;



		Bool_t isDet;
		*/
	public:
		/*
		virtual void Clear(Option_t* = "");

		void SetValues(Int_t _detID, Int_t _stripID, Int_t _padID,
				Int_t _stripValU, Int_t _stripValD, Int_t _padVal);

		inline Int_t    GetStripSum()  { return (stripValU + stripValD); }
		inline Int_t    GetStripDiff() { return (stripValU - stripValD); }
		inline Double_t GetStripDiffR() { return double(GetStripDiff())/GetStripSum(); }

		ClassDef(TAnaX6, 1)
		*/
};



#endif // __HIT__



