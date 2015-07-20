// Class to handle reading Aleph data files
// Ian Connelly
// 17 July 2015

// Libraries
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class AlephReader{
public:
    AlephReader();
    ~AlephReader();
    void SetFilename(string);
    void SetTreename(string);
    void Init();
    void PrintInfo();
    void SetSelection(string);
    void ClearHists();
    void AddVariable(string, int, float, float);
    void Process();
    void Write(string);
    void Test();
    // User Access
    void GetEvent(int i);
    int ev;
    int pa;
    float th;
    float ph;
    float pc;
    float d0;
    float ee;
    float eh;
    float de;
    float sa;
    float mu;
    // Total number of events
    int nEvt;
    // This event number
    int thisEvt;

private:
    bool assigned;
    string filename;
    string treename;
    TFile* f;
    TTree* t;
    string selection;
    map<string,int> allVars;
    vector<string> selectedVars;
    map<string, TH1D*> varAndHist;
};
