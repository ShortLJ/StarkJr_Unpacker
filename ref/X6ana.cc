#include <stdio.h>
#include <iostream>
#include <time.h>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TClonesArray.h"

#include "inc/TX6raw.hh"
#include "inc/TX6ana.hh"

void X6ana(Int_t nrun=56)
{
	TX6raw* rtree = new TX6raw(Form("data/r%.4d_000_cor.root", nrun));

	Long64_t nEnt = rtree->fChain->GetEntries();
	cout << "Number of entries: " << nEnt << endl;

	Long64_t ts[34];
	Int_t mult;

	Int_t nch = 128;
	Int_t adc[128];
	Int_t tdc[128];
	Int_t ch[128];

	TClonesArray *x6s = new TClonesArray("TX6ana", 5);

	TFile* fout = new TFile(Form("data/r%.4d_ana.root", nrun), "recreate");
	TTree* ftree = new TTree("stark", "STARK Jr DAQ data");
	ftree->Branch("mult", &mult, "mult/I");
	ftree->Branch("ts", ts, "ts/L");
	ftree->Branch("x6",x6s);

	std::vector<Int_t> vStrip;
	std::vector<Int_t> vPad;
	TX6ana* x6;

	for (Long64_t ient = 0; ient < nEnt; ient++) {
cout << ient << endl;
		rtree->fChain->GetEntry(ient);
		mult = rtree->M;
		x6s->Clear("C");
		if (mult == 3) {

//cout << "here1" << endl;
			Int_t idx[mult];
			vStrip.clear();
			vPad.clear();

//cout << "here2" << endl;
			for (auto imult = 0; imult < mult; imult++) {
				ts[imult] = rtree->TT_T[imult];
				ch[imult] = rtree->ch[imult] + (rtree->mid[imult]/2-1)*32 - 1;
				adc[imult] = rtree->Energy[imult];
				tdc[imult] = rtree->TDC[imult];
			}

//cout << "here3" << endl;
			TMath::Sort(mult, ch, idx, false);
			for (auto ich = 0; ich < mult; ich++) {
//cout << mult << " " << ch[idx[ich]] << endl;
				if (ch[idx[ich]] > 95) vPad.push_back(idx[ich]);
				else {
					if (ich+1 == nch) break;
					if (ch[idx[ich]]%2) continue;
					if (ch[idx[ich]]+1 != ch[idx[ich+1]]) continue;
					vStrip.push_back(idx[ich]);
					ich++;
				}
			}

//cout << "here4" << endl;
			for (auto it = vPad.begin(); it != vPad.end(); it++) {
				Int_t detID = (ch[*it] - 96)/4;
				detID = detID > 2 ? detID - 1 : detID;
				Int_t padID = (ch[*it]) % 4;

				for (auto it2 = vStrip.begin(); it2 != vStrip.end(); it2++) {
					if (detID == ch[*it2]/16) {
						Int_t stripID = (ch[*it2] % 16)/2;
						x6 = static_cast<TX6ana*>(x6s->ConstructedAt(x6s->GetEntriesFast()));
						x6->SetValues(detID, stripID, padID, adc[*it2], adc[*it2+1], adc[*it]);
//			cout << ient << " " << detID << " " << padID << " " << stripID << " " << adc[*it2] << " " << adc[*it2+1] << " " << adc[*it] <<  endl;
					}
				}
			}
//cout << "here5" << endl;
			ftree->Fill();
//			x6->Clear();
		}
	}

//	fout->cd();
	ftree->Write();
	fout->Close();

}
