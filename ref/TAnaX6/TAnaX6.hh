#include "TObject.h"

class TAnaX6: public TObject {
public:
  Int_t detID;
  Int_t stripID;
  Int_t padID;

  Int_t stripValU;
  Int_t stripValD;
  Int_t padVal;

  Bool_t isDet;
  
public:
  TAnaX6();
  virtual ~TAnaX6();

  virtual void Clear(Option_t* = "");

  void SetValues(Int_t _detID, Int_t _stripID, Int_t _padID,
		 Int_t _stripValU, Int_t _stripValD, Int_t _padVal);

  inline Int_t    GetStripSum()  { return (stripValU + stripValD); }
  inline Int_t    GetStripDiff() { return (stripValU - stripValD); }
  inline Double_t GetStripDiffR() { return double(GetStripDiff())/GetStripSum(); }
  
  ClassDef(TAnaX6, 1)
};

