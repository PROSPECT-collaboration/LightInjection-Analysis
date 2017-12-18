/* PROSPECT - Light Injection Analysis
 * Combine Files;
 * Created On: Dec 01, 2017
 * Last Modified: 
 * Reference Package: PulseCruncher
 */

#include "CombineFile.hh"
//Create array of string
void CombineFile::CreateAoS(string Output_Dir, string AddFile) {
  //open .txt file containing a list of file to combine
  ifstream fin;
  fin.open(AddFile.c_str());
  
  ostringstream AddDir;
  string AddDirName="";
  //Read File and store string to a string of file
  string line;
  int i = 0;
  while (!fin.eof()){
    getline(fin, line);
    AddDir << Output_Dir << line;
    AddDirName = AddDir.str();
    File[i] = AddDirName.c_str();
    AddDir.str("");
    AddDir.clear();
    cout << "processing: " << File[i] << endl;
    i++;    
  }
}

//Combine PulseCruncher Output
void CombineFile::CombinePCOutput(string Output_Dir, int noF){

  //Chain Tree of Different Files together
  TChain TreeChain("DetPulse");
  for(int i = 0; i < noF ; i++){
    TreeChain.Add(File[i].c_str());
  }
  //Name a File in the output directory
  ostringstream AddDir;
  AddDir << Output_Dir << "Combined_File_Set.root";
  string AddDirName;
  AddDirName = AddDir.str();
  //Create a base to store combined Ouput
  TFile *WriteFile = new TFile(AddDirName.c_str(),"RECREATE");
  TTree *WriteTree = new TTree("DetPulse","");

  //Declarations of variables for Tree writing
  Long64_t event;				         //event number assigned
  Int_t detector;				         //Detector ID
  Double_t time;				         //arrival time
  Float_t height;					 //height (ADC counts)
  Float_t area;				                 //area (ADC sum)
  Float_t baseline;					 //baseline (ADC counts)
  Float_t risetime;		                         //rise time (ns)
  Float_t PSDIS;                                         //PSD parameter(arb.)

  //Create Branches in ReadTree
  TBranch *evtBranch = WriteTree->Branch("evt", &event, "event/L");
  TBranch *detBranch = WriteTree->Branch("det", &detector, "detector/I");
  TBranch *tBranch = WriteTree->Branch("t", &time, "time/D");
  TBranch *hBranch = WriteTree->Branch("h", &height, "height/F");
  TBranch *aBranch = WriteTree->Branch("a", &area, "area/F");
  TBranch *bBranch = WriteTree->Branch("b", &baseline, "baseline/F");
  TBranch *riseBranch = WriteTree->Branch("rise", &risetime, "risetime/F");
  TBranch *PSDBranch = WriteTree->Branch("PSD", &PSDIS, "PSDIS/F");

  //Declarations of variables for Tree Chaining
  Long64_t evt=0;				 //event number assigned
  Int_t det=0;				         //Detector ID
  Double_t t=0;				         //arrival time
  Float_t h=0;					 //height (ADC counts)
  Float_t a=0;				         //area (ADC sum)
  Float_t b=0;					 //baseline (ADC counts)
  Float_t rise=0;		                 //rise time (ns)
  Float_t PSD=0;				 //PSD parameter(arb.)
  //Declarations of trees to read and store as value
  TreeChain.SetBranchAddress("evt", &evt);
  TreeChain.SetBranchAddress("det", &det);
  TreeChain.SetBranchAddress("t", &t);
  TreeChain.SetBranchAddress("h", &h);
  TreeChain.SetBranchAddress("a", &a);
  TreeChain.SetBranchAddress("b", &b);
  TreeChain.SetBranchAddress("rise", &rise);
  TreeChain.SetBranchAddress("PSD", &PSD);
  //Declaring the end value for the loop to get all of the data in an array.
  int n = TreeChain.GetEntries();
  //loop through the tree to read file
  for(int i = 0; i < n; i++){
    TreeChain.GetEntry(i);
    event = evt;
    detector = det;
    time = t;
    height = h;
    area = a;
    baseline = b;
    risetime = rise;
    PSDIS = PSD;
    WriteTree->Fill();
  }
  WriteTree->Write();
  WriteFile->Close();
}





