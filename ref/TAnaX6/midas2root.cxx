// Example Program for converting MIDAS format to ROOT format.
//
// T. Lindner (Jan 2016) 
//
// Example is for the CAEN V792 ADC module

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <vector>

#include "TRootanaEventLoop.hxx"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TClonesArray.h"

#include "TAnaManager.hxx"
#include "TV792Data.hxx"

#include "TAnaX6.hh"

class Analyzer: public TRootanaEventLoop {

public:

  // An analysis manager.  Define and fill histograms in 
  // analysis manager.
  TAnaManager *anaManager;

  // The tree to fill.
  TTree *fTree;

  int timestamp;
  int serialnumber;

  // CAEN V792 tree variables
  int adc[4][32];
  int tdc[4][32];

  int nch;
  int adc_all[128];
  int ch[128];

  // X6 ana
  TClonesArray *x6s = new TClonesArray("TAnaX6",5);
  //  TAnaX6 *x6 = new TAnaX6;
  
  Analyzer() {
    UseBatchMode();
    DisableAutoMainWindow();};
  virtual ~Analyzer() {};

  void Initialize(){}
  
  
  void BeginRun(int transition,int run,int time){
    
    // Create a TTree
    fTree = new TTree("midas","MIDAS data");
    
    fTree->Branch("adc0",adc[0],"adc0[32]/I");
    fTree->Branch("adc1",adc[1],"adc1[32]/I");
    fTree->Branch("adc2",adc[2],"adc2[32]/I");
    fTree->Branch("adc3",adc[3],"adc3[32]/I");
    fTree->Branch("tdc0",tdc[0],"tdc0[32]/I");
    fTree->Branch("tdc1",tdc[1],"tdc1[32]/I");
    fTree->Branch("tdc2",tdc[2],"tdc2[32]/I");
    fTree->Branch("tdc3",tdc[3],"tdc3[32]/I");

    fTree->Branch("nch",&nch,"nch/I");
    fTree->Branch("adc",adc_all,"adc[nch]/I");
    fTree->Branch("ch",ch,"ch[nch]/I");

    //    fTree->Branch("x6","TAnaX6",&x6);
    fTree->Branch("x6",x6s);
  }
    


  void EndRun(int transition,int run,int time){
        printf("\n"); }

  
  
  // Main work here; create ttree events for every sequenced event in 
  // Lecroy data packets.
  bool ProcessMidasEvent(TDataContainer& dataContainer){
    char temp[100];

    x6s->Clear("C");
    
    nch = 0;
    ////////////////////////////////////////////////////////////
    // raw data: ADC
    Int_t valCutL =  200;
    Int_t valCutU = 4000;
    for (int k = 0 ; k < 4 ; k++) {
      snprintf(temp, 100, "ADC%d", k);
      TV792Data *data = dataContainer.GetEventData<TV792Data>(temp);
      if(data){
	int nchannels = 32;
	for(int i = 0; i < nchannels;i++) adc[k][i] = -1;
      
	/// Get the Vector of ADC Measurements.
	std::vector<VADCMeasurement> measurements = data->GetMeasurements();
	for(unsigned int i = 0; i < measurements.size(); i++){ // loop over measurements
        
	  int chan = measurements[i].GetChannel();
	  uint32_t val = measurements[i].GetMeasurement();
        
	  if(chan >= 0 && chan < nchannels && val > valCutL && val < valCutU) {
	    ch[nch] = k * 32 + chan;
	    adc_all[nch++] = (adc[k][chan] = val); }
	}}}

    // raw data: TDC
    valCutL =    0;
    valCutU = 4000;
    for (int k = 0 ; k < 4 ; k++) {
      snprintf(temp, 100, "TDC%d", k);
      TV792Data *data = dataContainer.GetEventData<TV792Data>(temp);
      if (data) {
	int nchannels = 32;
	for (int i = 0; i < nchannels;i++) tdc[k][i] = -1;
	std::vector<VADCMeasurement> measurements = data->GetMeasurements();
	for (unsigned int i = 0; i < measurements.size(); i++){ // loop over measurements
	  int chan = measurements[i].GetChannel();
	  uint32_t val = measurements[i].GetMeasurement();        
	  if(chan >= 0 && chan < nchannels && val > valCutL && val < valCutU) {
	    tdc[k][chan] = val; }}}}

    
    ////////////////////////////////////////////////////////////
    // X6 analysis
    if (nch < 3) {
      fTree->Fill(); return true; }

    std::vector<Int_t> vStrip;
    std::vector<Int_t> vPad;

    // Channel sorting and vector pushing
    Int_t ind[128];
    TMath::Sort(nch, ch, ind, false);
    for (Int_t i = 0 ; i < nch ; i++) {
      if (ch[ind[i]] > 95) vPad.push_back(ind[i]); // ohmic
      else { // junction
	if ( i + 1 == nch ) break; // no more channel to pair 
	if ( ch[ind[i]] % 2 ) continue; // first strip should be even
	if ( ch[ind[i]] + 1 != ch[ind[i+1]] ) continue; // second strips is not adjacent
	vStrip.push_back(ind[i]);
	i++;}}

    // for each ohmic pad
    for (auto it = vPad.begin() ; it != vPad.end() ; it++) {
      Int_t detID = (127 - ch[*it]) / 4;
      detID = detID > 2 ? detID - 4 : detID + 3;
      Int_t padID = (127 - ch[*it]) % 4;

      // check for each junction strip
      for (auto it2 = vStrip.begin() ; it2 != vStrip.end() ; it2++) {
	if (detID == ch[*it2] / 16) {
	  Int_t stripID = (ch[*it2] % 16)/2;
	  // add all combinations
	  TAnaX6 *x6 = static_cast<TAnaX6*>(x6s->ConstructedAt(x6s->GetEntriesFast()));
	  x6->SetValues(detID, stripID, padID,
			adc_all[*it2], adc_all[*it2 + 1], adc_all[*it]);
	}}}

    /*
    for (Int_t i = 0 ; i < (nch - 2) ; i++) {
      Int_t strip1Ind = ind[i];
      Int_t strip1 = ch[ind[i]];
      if (strip1 > 95) continue; // strip is on junction sides
      if (strip1 % 2) continue; // first strip should be even
      Int_t strip2Ind = ind[i+1];
      Int_t strip2 = ch[ind[i+1]];
      if (strip2 != (strip1 + 1)) continue; // second strip should be adjacent

      Int_t stripID = (strip1 % 16)/2; // stripID (0 ~ 7)
      Int_t detID   =  strip1 / 16;    // detID (0 ~ 5)

      // finding ohmic pad
      Int_t padID = -1;
      Int_t pad   = -1;
      Int_t padInd = 0;
      for (Int_t j = i+2 ; j < nch ; j++) {
	padInd = ind[j];
	pad = ch[ind[j]];
	if (pad <= 95) continue; // pad is on ohmic sides
	Int_t detID2 = (127 - pad) / 4;
	if (detID2 > 2) detID2 -= 4;
	else            detID2 += 3;
	if (detID2 != detID) continue;
	else {
	  padID  = (127 - pad) % 4;  // padID (0 ~ 3)
	  break;}}
      if (padID >= 0) {
	x6->SetValues(detID, stripID, padID,
		      adc_all[strip1Ind], adc_all[strip2Ind], adc_all[padInd]);
	break;
      }}
    */
    ////////////////////////////////////////////////////////////
    fTree->Fill();
    
    return true;}
  
  // Complicated method to set correct filename when dealing with subruns.
  std::string SetFullOutputFileName(int run, std::string midasFilename)
  {
    char buff[128]; 
    Int_t in_num = 0, part = 0;
    Int_t num[2] = { 0, 0 }; // run and subrun values
    // get run/subrun numbers from file name
    for (int i=0; ; ++i) {
      char ch = midasFilename[i];
        if (!ch) break;
        if (ch == '/') {
          // skip numbers in the directory name
          num[0] = num[1] = in_num = part = 0;
        } else if (ch >= '0' && ch <= '9' && part < 2) {
          num[part] = num[part] * 10 + (ch - '0');
          in_num = 1;
        } else if (in_num) {
          in_num = 0;
          ++part;
        }
    }
    if (part == 2) {
      if (run != num[0]) {
        std::cerr << "File name run number (" << num[0]
                  << ") disagrees with MIDAS run (" << run << ")" << std::endl;
        exit(1);
      }
      snprintf(buff,100,"output_%.6d_%.4d.root", run, num[1]);
      printf("Using filename %s\n",buff);
    } else {
      snprintf(buff,100,"output_%.6d.root", run);
    }
    return std::string(buff);
  };





}; 


int main(int argc, char *argv[])
{
  Analyzer::CreateSingleton<Analyzer>();
  return Analyzer::Get().ExecuteLoop(argc, argv);

}

