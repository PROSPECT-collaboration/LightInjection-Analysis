

#include"PltMonitor.hh"

//Create array of string
void PLTMonitor::CreateAoS(string Output_Dir, string AddFile) {
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
    File.push_back(AddDirName.c_str());
    AddDir.str("");
    AddDir.clear();
    cout << "processing: " << File[i] << endl;
    i++;    
  }
}

void PLTMonitor::CalcIntensity(string PC_Output, string Output_Dir, int Ch){
  //Open PCOutput File and called in DetPulse Tree and gets all of its parameters
  TFile *ReadFile = TFile::Open(PC_Output.c_str());
  TTree *ReadTree = (TTree*)ReadFile->Get("DetPulse");
  int n = ReadTree->GetEntries();                        //Get the amount of entries
  int number_of_channels = Ch;                           //Store the amount of Channels
 
  //path to output
  ostringstream AreaCH;
  AreaCH << Output_Dir << "/PulseArea.root";
  string AreaCHName = AreaCH.str();
  //Create a new Root file and a new tree for refernces
  TFile *WriteFile = new TFile(AreaCHName.c_str(),"RECREATE");
  //TTree *WriteTree = new TTree("DATA","");
  //WriteTree->SetEntries(n/(number_of_channels-1));      //Ignore Trigger Channel
  
  //Declare a sample with an array corresponding to the amount of values in that array.
  //Set the array to the branch of interested.
  float area=0;
  float baseline=0;
  int detector=0;
  Long64_t event=0;
  ReadTree->SetBranchAddress("a", &area);
  ReadTree->SetBranchAddress("b", &baseline);
  ReadTree->SetBranchAddress("det", &detector);
  ReadTree->SetBranchAddress("evt", &event);

  //Create values to store the historgram array
  float A;

  //declaration of variables to locate spe trigger
  Long64_t evtmap=0, evtprev=0;
  float Amap=0;
  int Dmap=0;


  //create a flag one of events (Filtering Flag system)
  //THis is map to PulseCruncher output. Change this is PulseCruncher change somehow... always a good a idea to look at the .h5 files first

  vector<long> Pulse;                           //initialize a trigger array
  for (int p = 0; p < n; p++){                  //loop through each events in the file
    ReadTree->GetEntry(p);             
    evtmap = event;
    Dmap = detector;
    if (Dmap == -100) {                         //run to each set of PMT event to locate a counter, add a counter to Remember this event
      evtprev = evtmap;
      Pulse.push_back(event);
      cout << Pulse.back() << endl;
    }
    //self-checking module 
    //if (evtmap == evtprev) flag = 1 ;                   //if the next event is equal to the previous event flag = 1.
    if ((evtmap == evtprev) && (Dmap == -100)) continue;  //Get rid of repeating -100 Ch
  }
  //Create n amount of histogram corresponding to number of PMT to be fill by SPE Pulse Integrated Area
  int range_low=-50, range_high=500;         //I think this could be move to constructor of the class...
  TH1F *Areahis[number_of_channels];
  float PulseArea[number_of_channels];
  for(int l = 0; l < number_of_channels; l++) {
    ostringstream areabranch;
    areabranch << "areac" << l;
    string areabranchname = areabranch.str();
    ostringstream his;
    his << "his" << l;
    string hisname = his.str();
    Areahis[l] = new TH1F(hisname.c_str(),areabranchname.c_str(),100,range_low,range_high);
  }
  //Begin Filtering Events to its respected PMT
  cout << "Begin Processing Run Event\n[";
  Dmap = 0;
  evtmap = 0;
  int w = 0;
  int counter =0;
  int traceback = 0;
  int c[number_of_channels];
  for(int j = 0; j < n; j++) {
    ReadTree->GetEntry(j);
    evtmap = event;
    Dmap = detector;
    evtmap = event;
    A = area;
    if (Pulse[w] == evtmap) {
      if (Dmap != -100){
        Areahis[Dmap]->Fill(A);  //If channel isn't -100
        PulseArea[Dmap] += A;
        c[Dmap] +=1;
      } 
      if (Dmap == 1000) {
        Areahis[number_of_channels-2]->Fill(A);
        PulseArea[number_of_channels-2] += A; 
        c[number_of_channels-2] += 1;
      }
      if (Dmap == 1001) {
        Areahis[number_of_channels-1]->Fill(A);
        PulseArea[number_of_channels-1] += A;
        c[number_of_channels-1] += 1;
      }
    }
    else if (Pulse[w] < evtmap){
      w++;
      traceback +=1;
      if (Pulse[w] == evtmap) {
        j = j - traceback;
        traceback = 0;
      }
    } else continue 
    if (j%1000000==0) cout << "*";  //Sent Wright a Printout so that it doesnt kick you out
  } 
  cout << "]" <<endl;
  for(int m = 0; m < number_of_channels; m++) {     
    Areahis[m]->Write();
    PulseArea[m] = PulseArea[m]/c[m]; 
    cout << "Finished Processing Channel: " << m << " of " << number_of_channels << endl;
  }
  
  WriteFile->Close();
  ReadFile->Close();
}
