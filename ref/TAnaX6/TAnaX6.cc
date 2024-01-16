#include "TAnaX6.hh"

#include "TMath.h"

ClassImp(TAnaX6)

TAnaX6::TAnaX6() {
  Clear();}

TAnaX6::~TAnaX6() {}

void TAnaX6::Clear(Option_t*) {
  detID = stripID = padID = -1;
  stripValU = stripValD = padVal = -1;
  isDet = false;}

void TAnaX6::SetValues(Int_t _detID, Int_t _stripID, Int_t _padID,
		       Int_t _stripValU, Int_t _stripValD, Int_t _padVal){
  detID = _detID;
  stripID = _stripID;
  padID = _padID;

  stripValU = _stripValU;
  stripValD = _stripValD;
  padVal    = _padVal;

  isDet = true;}
