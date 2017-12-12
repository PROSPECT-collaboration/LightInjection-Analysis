/* Run PulseCruncher, PMTGain Value are extracted from the output
 * Date Created: Aug 28, 2017
 * Date Modified: 
 */


#include"TMacros.hh"
#include"GMacros.hh"

// Compile with:
// g++ -o LIAnalysis LIAnalysis.cpp `root-config --cflags --libs`
//
// Run with:
// ./LIAnalysis <path to PulseCruncher config file> <path to Light Injection System Analysis config file>
//
// 
// (NOTE: Questions will be ask during the run to ensure where to place the final file and what to name it)
/* (The <filename>.h5 file will be convert to <filename>.root file in the process)
 * 
 *
 */


int main(int argc, char** argv) {

  //program fail if arguments are incorrect.
  if(argc < 2) {
    printf("Arguments: <PulseCruncher config file> <LIAnalysis config file>\n");
    return EXIT_FAILURE;
  }
  
  //pathing to all files needed to run Pulse Cruncher
  string PC_cfg = argv[1];     // PulseCruncher config file name ".cfg"
  string LIA_cfg = argv[2];    // LIAnalysis config file name ".cfg"

  //Evaluate LIAnalysis config file
  string PCPath, H5toROOTPath;                       //path to existing Pulsecruncher and File Converter
  string WFinput, PCoutput, CHinput;                 //path to PulseCruncher Input/Output file name
  string FOutput;                                 //path to Light Injection Analysis Output
  LIParameterAnalyzer(LIA_cfg, PCPath, H5toROOTPath, WFinput, PCoutput, CHinput, FOutput);


  int iCHinput = stoi(CHinput);                              //Change from a string to an integer
  
  //create pathing to store all LIAnalysis output
  int PCOLength = PCoutput.size();                      // Get the .h5 string length
  string PCORepath = PCoutput.substr(0,PCOLength-3);    // locate .h5 and delete it
  //Make Output Folder
  ostringstream analysisOutput;
  analysisOutput << "mkdir " << FOutput << "LIAnalysisOutput_" << PCORepath;     
  string analysisOutputName = analysisOutput.str();
  system (analysisOutputName.c_str());
  
  //Path to output folder
  ostringstream LIOutputPath;
  LIOutputPath << FOutput << "LIAnalysisOutput_" << PCORepath;                 
  string LIOutputPathName = LIOutputPath.str();

  //path to all pulsecruncher essensials
  ostringstream PCOpath, execPC, execDPRoot;
  PCOpath << LIOutputPathName.c_str() << "/" << PCoutput;                              
  string PCOpathname = PCOpath.str();
  execPC << PCPath << " " << PC_cfg << " " << WFinput << " " << PCOpathname.c_str();
  execDPRoot << H5toROOTPath << " " << PCOpathname.c_str(); 
  string execPCName = execPC.str();
  string execDPRootName = execDPRoot.str();

  //execute PC-related programs
  system (execPCName.c_str());       //Execute PulseCruncher (need to rename the simulation file but not the .h5)
  system (execDPRootName.c_str());   //Convert .h5 to .root file (do not rename the .h5 file)


  //Performing Timing Analysis
  AnalyzeTiming(WFinput, LIOutputPathName.c_str());

  //begin PMTGainAnalysis
  //Repath .h5 file to .root file
  cout << "Your original <filename>.h5 file is now <filename>.root" <<endl;
  int OH5Length = PCOpathname.size();                   // Get the .h5 string length
  string Repath = PCOpathname.substr(0,OH5Length-2);    // locate .h5 and replace it with .root
  ostringstream SLFilename;
  SLFilename << Repath << "root";
  string SLFile = SLFilename.str();
  
  
  //Perform PMTGain Analysis
  ExtractArea(SLFile.c_str(), LIOutputPathName.c_str(), iCHinput);
  PCFitting(LIOutputPathName.c_str(), iCHinput);
  CalcGain(LIOutputPathName.c_str());

  return 0;
}

