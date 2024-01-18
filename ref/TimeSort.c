#include "Data.C"
//#include <boost/circular_buffer.hpp>
#include <queue>
#include <functional>    // greater, less

struct Hit{
	UShort_t        Channel;
	ULong64_t       Timestamp;
	UShort_t        Board;
	UShort_t        Energy;
	Double_t        CalibEnergy;
	UShort_t        EnergyShort;
	UInt_t          Flags;	
	Hit(UShort_t Channel, ULong64_t Timestamp, UShort_t Board, UShort_t Energy, Double_t CalibEnergy, UShort_t EnergyShort, UInt_t Flags)
		: Channel(Channel), Timestamp(Timestamp), Board(Board), Energy(Energy), CalibEnergy(CalibEnergy), EnergyShort(EnergyShort), Flags(Flags) {}
	bool operator<(const Hit hit) const{
		return this->Timestamp > hit.Timestamp;
	}
};



void TimeSort(){
	const UShort_t Nbr=3; 
	const UShort_t Nch=8;
	UShort_t ibr, ich; 
	UShort_t jbr, jch; 
	int boardpid[Nbr]={16606,16969,16605};
	//int boardpid[Nbr]={16606,16969,16605,1471};
	Bool_t Enabled[Nbr][Nch] = {
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,0,0,0,0}
	};

	TChain *chain[Nbr][Nch];
	Data *data[Nbr][Nch];
	Long64_t Entries[Nbr][Nch];
	Long64_t Entries_loop=0;
	Long64_t permille;
	Bool_t Enabled_in[Nbr][Nch];
	Bool_t Enabled_out[Nbr][Nch];
	for(ibr=0; ibr<Nbr; ibr++)
	{
		for(ich=0; ich<Nch; ich++)
		{
			Enabled_in[ibr][ich]=Enabled[ibr][ich];
			Enabled_out[ibr][ich]=Enabled[ibr][ich];

			if(!Enabled[ibr][ich]) continue;
			chain[ibr][ich] = new TChain("Data");
			chain[ibr][ich]->Add(Form("../UNFILTERED/Data_CH%u@V1751C_%d_run3.root", ich, boardpid[ibr]));
			//chain[ibr][ich]->Add(Form("../run_01_10kHz_10March/UNFILTERED/Data_CH%u@V1751C_%d_run_01_10kHz_10March.root", ich, boardpid[ibr]));
			data[ibr][ich] = new Data(chain[ibr][ich]);
			Entries[ibr][ich] = data[ibr][ich]->fChain->GetEntries();
			if(Entries[ibr][ich]>Entries_loop) Entries_loop=Entries[ibr][ich];
		}
	}
	permille = Entries_loop/1000;

	TFile *file_timesort = new TFile("Data_run_timesort.root","recreate");
	TTree *tree_timesort = new TTree("Data","timesorted_tree");
	UShort_t        Channel;
	ULong64_t       Timestamp;
	UShort_t        Board;
	UShort_t        Energy;
	Double_t        CalibEnergy;
	UShort_t        EnergyShort;
	UInt_t          Flags;	
	tree_timesort->Branch("Channel"		, &Channel		, "Channel/s");
	tree_timesort->Branch("Timestamp"	, &Timestamp		, "Timestamp/l");
	tree_timesort->Branch("Board"		, &Board		, "Board/s");
	tree_timesort->Branch("Energy"		, &Energy		, "Energy/s");
	tree_timesort->Branch("CalibEnergy"	, &CalibEnergy		, "CalibEnergy/D");
	tree_timesort->Branch("EnergyShort"	, &EnergyShort		, "EnergyShort/s");
	tree_timesort->Branch("Flags"		, &Flags		, "Flags/i");



	const int arraysize=1024;
	array< array< priority_queue<Hit>, Nch >,Nbr >q_hit;

	Long64_t offset[Nbr] = { -37, -13953, -73813 };
	//Long64_t offset[Nbr] = { 0, -59586, -119107};
	//Long64_t offset[Nbr] = { 0, -59586, -119107, -162832 };
	//ULong64_t offset[Nbr] = {0,0,0};


	Long64_t maxquesize=0;
	ULong_t count=0;

	UShort_t minch=0, minbr=0;
	bool allfilled=0;
	Long64_t ient=0;
	Long64_t entoffset[Nbr][Nch] = {0};
	//entoffset[2][2]=14400-2887/2+455;
	for(ient=0; ient<Entries_loop; ient++)
	{
		if(ient%permille==0)
		{
			printf("\r%lld ent of %lld, %.1f%%", ient, Entries_loop, (float)ient/Entries_loop*100);
			for(jbr=0; jbr<Nbr; jbr++)
			{
				for(jch=0; jch<Nch; jch++){
					if(!Enabled[jbr][jch]) continue;
					printf(" %lu", q_hit[jbr][jch].size());
				}
			}
			printf(" max %lld             ",maxquesize);
			fflush(stdout);
		}
		for(ibr=0; ibr<Nbr; ibr++)
		{
			for(ich=0; ich<Nch; ich++)
			{
				if(!Enabled_in[ibr][ich]) continue;
				if(ient==Entries[ibr][ich])
				{
					fprintf(stdout, "\n file %s finished with entry %lld\n", chain[ibr][ich]->GetCurrentFile()->GetName(), ient);
					Enabled_in[ibr][ich]=false;
					continue;
				}
				data[ibr][ich]->GetEntry(ient);

				//if(!data[ibr][ich]->GetEntry(ient+entoffset[ibr][ich])) continue;
				if(q_hit[ibr][ich].size()>0)
				{
					if( q_hit[ibr][ich].top().Timestamp > data[ibr][ich]->Timestamp)
					{
						printf("\nsort sort sort sort sort \n");
						return;
					}
				}
			}
		}
		for(ibr=0; ibr<Nbr; ibr++)
		{
			for(ich=0; ich<Nch; ich++)
			{
				if(!Enabled_in[ibr][ich]) continue;
				//if(data[ibr][ich]->Energy < 1000) continue;
				ULong64_t ts = data[ibr][ich]->Timestamp;
				ts=ts-offset[ibr];

				q_hit[ibr][ich].push(
						Hit(
							data[ibr][ich]->Channel,
							/*data->Timestamp,*/ ts,
							data[ibr][ich]->Board,
							data[ibr][ich]->Energy,
							/*data[ibr][ich]->CalibEnergy,*/ 0,
							data[ibr][ich]->EnergyShort,
							data[ibr][ich]->Flags
						   )
						);

				if(maxquesize<q_hit[ibr][ich].size()) maxquesize=q_hit[ibr][ich].size();
			}
		}

		allfilled=true;
		for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
		{
			//if(!Enabled_in[jbr][jch]) continue;
			//printf("ient %lld jch %d allfilled %d ent[jch].empty() %d\n", ient, jch, allfilled, ent[jch].empty());
			//allfilled &= ~q_hit[jch].empty(); 
			if(Enabled_in[jbr][jch] && q_hit[jbr][jch].size()<arraysize) allfilled=false;
		}

		if(allfilled) 
			if (ient%permille==0)
			{
				for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
				{
					//fprintf(stdout,"\t%llu", q_hit[jbr][jch].top().Timestamp);
				}
				//fprintf(stdout,"\n");
				fflush(stdout);
			}
		while(allfilled)
		{
			//minch=0;
			for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
			{
				if(!Enabled_in[jbr][jch] && q_hit[jbr][jch].empty()) Enabled_out[jbr][jch]=false;
				if(!Enabled_out[jbr][jch]) continue;
				if(q_hit[jbr][jch].top().Timestamp < q_hit[minbr][minch].top().Timestamp){minbr=jbr; minch=jch;}
			}
			Channel		=q_hit[minbr][minch].top().Channel		;
			Timestamp	=q_hit[minbr][minch].top().Timestamp	;
			Board		=q_hit[minbr][minch].top().Board		;
			Energy		=q_hit[minbr][minch].top().Energy		;
			CalibEnergy	=q_hit[minbr][minch].top().CalibEnergy	;
			EnergyShort	=q_hit[minbr][minch].top().EnergyShort	;
			Flags		=q_hit[minbr][minch].top().Flags		;

			tree_timesort->Fill();
			q_hit[minbr][minch].pop(); 
			for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
			{
				if (q_hit[jbr][jch].size()<arraysize) allfilled=false;
				else if(!Enabled_in[jbr][jch])
				{
					if (q_hit[jbr][jch].empty()) 
					{
						Enabled_out[jbr][jch]=false;
						//allfilled &= ~q_hit[jch].empty(); 
					}
				}
				//if(!Enabled_out[jbr][jch]) continue;
			}
		}
	}
	printf("\r%lld ent of %lld, %.1f%%", ient, Entries_loop, (float)ient/Entries_loop*100);
	for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
	{
		printf(" %lu", q_hit[jbr][jch].size());
	}
	printf(" max %lld ",maxquesize);
	count=0;
	for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
	{
		count+=q_hit[jbr][jch].size();
	}
	fprintf(stdout, "left q %lu \n", count);
	fflush(stdout);

	bool allempty=0;
	ULong64_t mintime=0;

	while (1){
		printf("\r%lld ent of %lld, %.1f%%", ient, Entries_loop, (float)ient/Entries_loop*100);
		for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
		{
			printf(" %lu", q_hit[jbr][jch].size());
		}
		printf(" max %lld ",maxquesize);

		count=0;
		for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
		{
			count+=q_hit[jbr][jch].size();
		}
		fprintf(stdout, "left q %lu               ", count);
		fflush(stdout);

		if(count==0) break;



		//minch=0;
		//allempty=true; 
		for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
		{
			//allempty &= hit[jch].empty();
			if (!q_hit[jbr][jch].empty())
			{
				//fprintf(stdout, "!q_hit[jbr%d][jch%d].empty()\n", jbr,jch);
				minbr=jbr; minch=jch;
				//allempty=false;
			}
		}
		//if(allempty) break;
		for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
		{
			if(q_hit[jbr][jch].empty()) continue;
			//allempty &= ~q_hit[jbr][jch].empty(); 
			if(q_hit[jbr][jch].top().Timestamp < q_hit[minbr][minch].top().Timestamp)
			{
				//fprintf(stdout, "q_hit[jbr%d][jch%d].top().Timestamp%llu < q_hit[minbr%d][minch%d].top().Timestamp%llu\n", jbr,jch,q_hit[jbr][jch].top().Timestamp, minbr,minch,q_hit[minbr][minch].top().Timestamp);
				minbr=jbr; minch=jch;
			}
		}
		Channel		=q_hit[minbr][minch].top().Channel		;
		Timestamp	=q_hit[minbr][minch].top().Timestamp	;
		Board		=q_hit[minbr][minch].top().Board		;
		Energy		=q_hit[minbr][minch].top().Energy		;
		CalibEnergy	=q_hit[minbr][minch].top().CalibEnergy	;
		EnergyShort	=q_hit[minbr][minch].top().EnergyShort	;
		Flags		=q_hit[minbr][minch].top().Flags		;

		//fprintf(stdout, "q_hit[minbr%d][minch%d].top().Timestamp %llu \n", minbr,minch,q_hit[minbr][minch].top().Timestamp);
		//fprintf(stdout, "q_hit[minbr%d][minch%d].size() %lu sum %lu\n", minbr,minch,q_hit[minbr][minch].size(),count);
		tree_timesort->Fill();
		q_hit[minbr][minch].pop(); 

		if(q_hit[minbr][minch].empty()) Enabled_out[minbr][minch]=false;

		count=0;
		for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
		{
			count+=q_hit[jbr][jch].size();
		}
		if(count==0) break;
	}
	printf("\r%lld ent of %lld, %.1f%%", ient, Entries_loop, (float)ient/Entries_loop*100);
	for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
	{
		printf(" %lu", q_hit[jbr][jch].size());
	}
	count=0;
	for(jbr=0; jbr<Nbr; jbr++)	for(jch=0; jch<Nch; jch++)	if(Enabled[jbr][jch])
	{
		count+=q_hit[jbr][jch].size();
	}
	fprintf(stdout, "left q %lu\n", count);
	fflush(stdout);





	//tree_timesort->Scan("Board:Channel:Timestamp:Energy", "","col=::25ld:20ld");
	tree_timesort->Write();
	file_timesort->Close();
}




