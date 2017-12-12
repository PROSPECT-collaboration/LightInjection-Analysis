#include "LI_PrepSpace.hh"


int main(int argc, char** argv) {

  //program fail if arguments are incorrect.
  if(argc < 2) {
    printf("Arguments: <PulseCruncher config file> <LIAnalysis config file>\n");
    return EXIT_FAILURE;
  }
  
  //pathing to all files needed to run Pulse Cruncher
  string PC_cfg = argv[1];     // PulseCruncher config file name ".cfg"
  string LIA_cfg = argv[2];    // LIAnalysis config file name ".cfg"

  string WFinput, CHinput;                           //Input Parameters 
  string PCPath, H5toROOTPath, PCoutput;             //path to executing Pulsecruncher and File Converter
  string outputDir, outputDirPath;                   //Output Directory
  //Read Config File

  Config cfg;
  cfg.readFile(LIA_cfg.c_str());
  LI_PrepSpace LIPrep(cfg.getRoot());
  
  LIPrep.updateVariables(CHinput, WFinput, PCPath, H5toROOTPath, PCOutput, outputDir);
  LIPrep.makeOutputDir(outputDir, PCOutput, outputDirPath);
  LIPrep.PulseCrunching(PC_cfg, WFinput, PCPath, H5toROOTPath, PCOutput, outputDirPath);
  return 0;
}
