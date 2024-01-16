#include "TObject.h"

class TX6ana: public TObject {
	public:
		Int_t detID;
		Int_t stripID;
		Int_t padID;

		Int_t stripValU;
		Int_t stripValD;
		Int_t padVal;

		Bool_t isDet;

	public:
		TX6ana();
		virtual ~TX6ana();

		virtual void Clear(Option_t* = "");

		void SetValues(Int_t _detID, Int_t _stripID, Int_t _padID, Int_t _stripValU, Int_t _stripValD, Int_t _padVal);

		inline Int_t    GetStripSum()  { return (stripValU + stripValD); }
		inline Int_t    GetStripDiff() { return (stripValU - stripValD); }
		inline Double_t GetStripDiffR() { return double(GetStripDiff())/GetStripSum(); }

		//  ClassDef(TX6ana, 1)
};


//ClassImp(TX6ana)

TX6ana::TX6ana() {
	Clear();
}

TX6ana::~TX6ana() {}

void TX6ana::Clear(Option_t*) {
	detID = stripID = padID = -1;
	stripValU = stripValD = padVal = -1;
	isDet = false;
}

void TX6ana::SetValues(Int_t _detID, Int_t _stripID, Int_t _padID, Int_t _stripValU, Int_t _stripValD, Int_t _padVal){
	detID = _detID;
	stripID = _stripID;
	padID = _padID;

	stripValU = _stripValU;
	stripValD = _stripValD;
	padVal    = _padVal;

	isDet = true;
}
