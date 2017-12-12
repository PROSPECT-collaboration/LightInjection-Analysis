/* PROSPECT - Light Injection Analysis
 * Preparation Works for Light Injection Analysis System
 * Created On: Dec 01, 2017
 * Last Modified: 
 * Reference Package: PulseCruncher
 */

#include "LI_PrepSpace.hh"
#include <stdlib.h> 

LI_PrepSpace::LI_Prepspace(Setting& S){
  _EXPLAIN("Initializing Light Injection Preparation from config file");
  //User Input Parameters
  Settings &inputF = S["Run"]["Input_File"];
  inputF.lookupValue("Number_of_Channels", Number_of_Channels);
  inputF.lookupValue("Path_to_Raw_Waveform_File",Path_to_Raw_Waveform_File);

  //Pulse Cruncher Essenstials
  Setting &PCS = S["Run"]["PulseCruncher_Dir"];
  PCS.lookupValue("Path_to_PulseCruncher",Path_to_PulseCruncher);
  PCS.lookupValue("Path_to_H5_to_ROOT_Converter",Path_to_H5_to_ROOT_Converter);
  PCS.lookupValue("Pulse_Crunched_File_Name",Pulse_Crunched_File_Name);

  //Output Directory Builder
  Setting &outputDir = S["Run"]["Output_Dir"];
  outputDir.lookupValue("Output_Directory_Location",Output_Directory_Location);
  
}

string LI_PrepSpace::updateVariables(string& CHinput, string& WFinput, string& PCPath, string& H5toROOTPath, string& PCoutput,  string& FOutput){
  CHinput = Number_of_Channels;
  WFinput = Path_to_Raw_Waveform_File;
  PCPath = Path_to_PulseCruncher;
  H5toROOTPath = Path_to_H5_to_ROOT_Converter;
  PCOutput = Pulse_Crunched_File_Name;
  outputDir Output_Directory_Location;
  return CHinput, WFinput, PCPath, H5toROOTPath, PCOutput, outputDir;
  
}

// Create a Folder to Store Light Injection Analysis Output.
string LI_PrepSpace::makeOutputDir(string& outputDir, string& PCOutput, string& Output_Path) {
  
  //create path to store all LIAnalysis output
  int OCOLength = PCOutput.size();                      // Get the .h5 string length
  string PCORepath = PCOutput.substr(0,PCOLength-3);    // locate .h5 and delete it
  Make_Dir_Command << "mkdir " << outputDir n << "LI_AnalysisOutput_" << PCORepath;     
  string Make_Dir = Make_Dir_Command.str();
  
  //make the folder
  system (Make_Dir.c_str());

  //Store Output Path string
  ostringstream Path_to_Dir;
  Path_to_Dir << outputDir << "LI_AnalysisOutput_" << PCORepath;
  Output_Path = Path_to_Dir.str();
  return (Output_Path);
}

// Locate and Run PulseCruncher 
string LI_PrepSpace::PulseCrunching(string& PC_cfg, string& WFinput, string& PCPath, string& H5toROOTPath, string& PCOutput, string& Output_Path) {

  //path to all pulsecruncher essensials
  ostringstream PCOpath, execPC, execDPRoot;
  PCOpath << Output_Path << "/" << PCOutput;               
  string PCOpathname = PCOpath.str();
  execPC << PCPath << " " << PC_cfg << " " << WFile << " " << PCOpathname.c_str();
  execDPRoot << H5toROOTPath << " " << PCOpathname.c_str(); 
  string execPCName = execPC.str();
  string execDPRootName = execDPRoot.str();

  //execute PC-related programs
  system (execPCName.c_str());       //Execute PulseCruncher (need to rename the simulation file but not the .h5)
  system (execDPRootName.c_str());   //Convert .h5 to .root file (do not rename the .h5 file)

}

