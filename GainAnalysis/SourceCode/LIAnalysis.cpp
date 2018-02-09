/* LIAnalysi
 * Date Created: Aug 28, 2017
 * Date Modified: Dec 17, 2017
 */

// Compile with:
// g++ -o LIAnalysis LIAnalysis.cpp + all .cpp essentials `root-config --cflags --libs`
//
// Run with:
// ./LIAnalysis <Output_Dir> <Number of Channel> <y or n>

#include "LIAnalysis.hh"

int main(int argc, char** argv) {
  //program fail if arguments are incorrect.
  if(argc < 3) {
    printf("Arguments: <Output_Dir> <Number of Channel> <y or n or dV>");
    return EXIT_FAILURE;
  }

  string Output_Dir = argv[1]; // Output Directory
  string nCH = argv[2];        // Number of Channel in Each PulseCrunched output
  string CA = argv[3];         // yes or no for combine files
  int iCH = stoi(nCH);         //Change from a string
  
  int NoC;                     // number of files to combine
  string GainAFile="";         // File to be GainAnalyzed

  ostringstream AddDir;
  string AddDirName;

  //if yes then combine file and proceed through GainAnalysis
  if (CA == "y") {
    cout << "How many Files are in FileCombineList.txt?: ";
    cin >> NoC;
    //Combine Files
    CombineFile COB;
    string AddFile = "FileCombineList.txt";
    COB.CreateAoS(Output_Dir, AddFile);
    COB.CombinePCOutput(Output_Dir, NoC);

    //Name a File in the output directory
    AddDir << Output_Dir << "Combined_File_Set.root";
    AddDirName = AddDir.str();

    //Run Gain Analysis
    GainAnalysis Gain;
    Gain.ExtractArea(AddDirName.c_str(), Output_Dir, iCH);

    //Open File to write PMT Gain and Occupancy

   
    Gain.SPEFit(Output_Dir, iCH);

    Gain.CalcGain(Output_Dir);
  } 
  //Run A single File
  else if (CA == "n") {
    cout << "Input Pulse Crunched File Name: ";
    cin >> GainAFile;
    //Read Files
    //Name a File in the output directory
    AddDir << Output_Dir << GainAFile;
    AddDirName = AddDir.str();
    //Run Gain Analysis
    GainAnalysis Gain;
    Gain.ExtractArea(AddDirName.c_str(), Output_Dir, iCH);
    //Open File to write PMT Gain and Occupancy

    Gain.SPEFit(Output_Dir, iCH);

    Gain.CalcGain(Output_Dir);


  }
  //Run Multiple Different Voltage File
  else if (CA == "dV") {
    cout << "How many Files are in DifferentVoltRun.txt?: ";
    cin >> NoC;
    CombineFile COB;
    string AddFile = "DifferentVoltRun.txt";
    COB.CreateAoS(Output_Dir, AddFile);
    COB.RunExtractArea(Output_Dir, NoC, iCH);
    GainAnalysis Gain;
    Gain.SPEFit(Output_Dir, iCH);
    Gain.CalcGain(Output_Dir);
  }
  return 0;
}
