#include "AlephReader.h"
#include <iostream>
#include <sstream>
#include "TTreeFormula.h"
#include "TCanvas.h"

AlephReader::AlephReader(){
    cout << "- Creating AlephReader tool." << endl;
    selection = "";
    allVars.clear();
    allVars.insert({"ev",-2});
    allVars.insert({"pa",-1});
    allVars.insert({"th",0});
    allVars.insert({"ph",1});
    allVars.insert({"pc",2});
    allVars.insert({"d0",3});
    allVars.insert({"ee",4});
    allVars.insert({"eh",5});
    allVars.insert({"de",6});
    allVars.insert({"sa",7});
    allVars.insert({"mu",8});
    
    //map<string,int>::iterator it;
    //for(it = allVars.begin(); it != allVars.end(); it++) cout << it->first << " -> " << it->second << endl;

}

AlephReader::~AlephReader(){
    
}

void AlephReader::SetFilename(string fname){
    filename = fname;
    return;
}

void AlephReader::SetTreename(string tname){
    treename = tname;
    return;
}

void AlephReader::Init(){
    cout << "- Reading " << filename << " for tree " << treename << endl;
    f = TFile::Open(filename.c_str());
    t = (TTree*)f->Get(treename.c_str());
    thisEvt = -1;
    nEvt = t->GetEntries();
    assigned = false;
    return;
}

void AlephReader::PrintInfo(){
    cout << "- AlephReader - Tool to read Aleph events" << endl;
    cout << " - Variables " << endl;
    cout << " - ev Event number \n ";
    cout << " - pa Particle type \n ";
    cout << " - th Theta \n ";
    cout << " - ph Phi \n ";
    cout << " - pc Momentum × Charge \n ";
    cout << " - d0 d0 - closest distance of track to IP \n ";
    cout << " - ee ECAL energy \n ";
    cout << " - eh HCAL energy \n ";
    cout << " - de dE/dX - rate of loss of energy by ionisation \n ";
    cout << " - sa dE/dx samples \n ";
    cout << " - mu MUON hits \n " << endl;
}

void AlephReader::SetSelection(string sel){
    selection = sel;
    return;
}

void AlephReader::ClearHists(){
    varAndHist.clear();
    return;
}

void AlephReader::AddVariable(string var, int nbin, float low, float high){
    
    TH1D* tmp = new TH1D(var.c_str(), "", nbin, low, high);
    varAndHist.insert({var,tmp});
    selectedVars.push_back(var);
    
    return;
}

// This function is used to loop over tree and fill histograms defined in the tool passing a selection
void AlephReader::Process(){
    
    TTreeFormula* formula = new TTreeFormula("selection",selection.c_str(),t);
    
    int varev, varpa;
    vector<float> vars = {-1,9};
    t->SetBranchAddress("ev",&varev);
    t->SetBranchAddress("pa",&varpa);
    t->SetBranchAddress("th",&vars[0]);
    t->SetBranchAddress("ph",&vars[1]);
    t->SetBranchAddress("pc",&vars[2]);
    t->SetBranchAddress("d0",&vars[3]);
    t->SetBranchAddress("ee",&vars[4]);
    t->SetBranchAddress("eh",&vars[5]);
    t->SetBranchAddress("de",&vars[6]);
    t->SetBranchAddress("sa",&vars[7]);
    t->SetBranchAddress("mu",&vars[8]);
                        
    int nentries = t->GetEntries();
    
    map<string,TH1D*>::iterator it;
    
    for(int iEvt = 0 ; iEvt < nentries; iEvt++){
        t->GetEntry(iEvt);
        
        if(formula->EvalInstance() == 0) continue;
        
        // Pass selection
        for(it = varAndHist.begin(); it != varAndHist.end(); it++){
            //cout << it->first << " " << it->second->GetName() << " , " << allVars[it->first] << endl;
            int varNum = allVars.find(it->first)->second;
            cout << varNum << " -> " << vars[varNum] << endl;
            if(varNum >= 0 ) it->second->Fill(vars[varNum]);
            if(varNum == -1) it->second->Fill(varpa);
            if(varNum == -2) it->second->Fill(varev);

        }
        
    }
    
    
    
    return;
}

// Only writes out histograms defined and run with Process()
void AlephReader::Write(string outname){
    // Write out a root file with histogram
    TFile* f = TFile::Open(string(outname+".root").c_str(),"recreate");
    map<string,TH1D*>::iterator it;
    for(it = varAndHist.begin(); it != varAndHist.end(); it++) it->second->Write();
    f->Write();
    f->Close();
    
    // Create .pdfs
    for(it = varAndHist.begin(); it != varAndHist.end(); it++){
        TCanvas c ("c","c",1800,1200);
        c.cd();
        it->second->Draw();
        c.Print(string(outname+"_"+it->first+".pdf").c_str());
    }

    return;
}

void AlephReader::Test(){
    SetFilename("zee.root");
    SetTreename("zee");
    SetSelection("pa == 0 && ee > 40");
    AddVariable("ee",10,0,60);
    Init();
    Process();
    Write("output");
    return;
}

// This function lets the user just read the tree but not make histograms
void AlephReader::GetEvent(int i){
    thisEvt = i;
    if(i >= nEvt){
        cout << " - Reached end of events - " << nEvt << endl;
        thisEvt = nEvt - 1;
    }
    
    if(!assigned){
        t->SetBranchAddress("ev",&ev);
        t->SetBranchAddress("pa",&pa);
        t->SetBranchAddress("th",&th);
        t->SetBranchAddress("ph",&ph);
        t->SetBranchAddress("pc",&pc);
        t->SetBranchAddress("d0",&d0);
        t->SetBranchAddress("ee",&ee);
        t->SetBranchAddress("eh",&eh);
        t->SetBranchAddress("de",&de);
        t->SetBranchAddress("sa",&sa);
        t->SetBranchAddress("mu",&mu);
        assigned = true;
    }
    
    // Set event
    t->GetEntry(thisEvt);
    // User can access the variabels through the tool
    return;
}


