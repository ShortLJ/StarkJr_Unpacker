//#ifndef TX6raw_h
//#define TX6raw_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class TX6raw {
	public :
		TTree          *fChain;   //!pointer to the analyzed TTree or TChain
		Int_t           fCurrent; //!current Tree number in a TChain

		// Fixed size dimensions of array or collections stored in the TTree if any.

		// Declaration of leaf types
		Int_t           M;
		Long64_t        TT_T[34];   //[M]
		Int_t           mid[34];   //[M]
		Int_t           ch[34];   //[M]
		Int_t           Energy[34];   //[M]
		Int_t           TDC[34];   //[M]

		// List of branches
		TBranch        *b_M;   //!
		TBranch        *b_TT_T;   //!
		TBranch        *b_mid;   //!
		TBranch        *b_ch;   //!
		TBranch        *b_Energy;   //!
		TBranch        *b_TDC;   //!

		TX6raw(const char* filename, TTree *tree=0);
		~TX6raw();
		Int_t    GetEntry(Long64_t entry);
		void     Init(TTree *tree);
};

//#endif

TX6raw::TX6raw(const char* filename, TTree *tree)
{
    if (tree == 0) {
        TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
        if (!f || !f->IsOpen()) {
            f = new TFile(filename);
        }
        f->GetObject("t1",tree);

    }
    Init(tree);
}

TX6raw::~TX6raw()
{
    fChain->Delete();
//    delete f;
}

Int_t TX6raw::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}

void TX6raw::Init(TTree *tree)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("M", &M, &b_M);
    fChain->SetBranchAddress("TT_T", TT_T, &b_TT_T);
    fChain->SetBranchAddress("mid", mid, &b_mid);
    fChain->SetBranchAddress("ch", ch, &b_ch);
    fChain->SetBranchAddress("Energy", Energy, &b_Energy);
    fChain->SetBranchAddress("TDC", TDC, &b_TDC);
}
