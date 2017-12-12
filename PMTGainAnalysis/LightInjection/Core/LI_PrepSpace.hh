/* PROSPECT - Light Injection Analysis
 * Preparation Works Light Injection Analysis System Header File
 * Created On: Dec 01, 2017
 * Last Modified: 
 * Reference Package: PulseCruncher
 */

#ifndef LI_PREPSPACE_HH
#define LI_PREPSPACE_HH


// include packages 
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include "libconfig.h++"

// use namespace packages (don't want to load full std package? Use std::(package) instead.)
using namespace std;

class LI_PrepSpace {
public: 
  //Constructor settings info
  LI_PrepSpace(const Setting& S);

  //Destructor
  virtual ~LI_PrepSpace();

  // Configuration Parameters
  int Number_of_Channels;			// Number of Channels in this run
  string Path_to_Raw_Waveform_File = "";        // Input Raw Waveform File
  string Path_to_PulseCruncher = "";            // Location of Pulse Cruncher 
  string Path_to_H5_to_ROOT_Converter = "";     // Location of HDF5 to Root Converter
  string Pulse_Crunched_File_Name = "";         // name of the PulseCrunched File
  string Output_Directory_Location = "";        // Location to Output Light Injection Analysis Data

};


#endif
