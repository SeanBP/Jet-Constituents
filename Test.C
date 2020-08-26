/*
Test macro to figure out why the deltaR for jet constituents are > 1

root -l Test.C'("sims/NC_DIS_EIC.root")'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

//------------------------------------------------------------------------------

void Test(const char *inputFile)
{
  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  // Get pointers to branches used in this analysis
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchEvent = treeReader->UseBranch("Event");
  TClonesArray *branchTower = treeReader->UseBranch("Tower");
  // Book histograms

  TH2 *histTowerEDeltaR = new TH2F("tower_e_deltaR","Tower Energy vs Delta R; Energy; Delta R", 100, 0, 50, 100, 0, 10);

  // Loop over all events
  Jet *jet;
  TObject *object; 
  Tower *tower;
  float deltaR = 0;

  for(Int_t entry = 0; entry < numberOfEntries; ++entry){
        // Load selected branches with data from specified event
	treeReader->ReadEntry(entry);
	for(int i = 0; i < branchJet->GetEntriesFast(); i++){
		jet = (Jet*) branchJet->At(i);
		for(int j = 0; j < jet->Constituents.GetEntriesFast(); j++){
					
			object = jet->Constituents.At(j);
                        if(object == 0) continue;
			if(object->IsA() == Tower::Class()){
				tower = (Tower*) jet->Constituents.At(j);
				deltaR = sqrt( pow(tower->Eta - jet->Eta, 2) + pow(tower->Phi - jet->Phi, 2) );
				histTowerEDeltaR->Fill(tower->E, deltaR);
			}
		}
	}	

  }

  auto c1=new TCanvas("Canvas","Canvas",1500,700);
  c1->SetLogz();
  histTowerEDeltaR->Draw("Colz");
}

