/* PROSPECT - Light Injection Analysis
 * Preparation Works Light Injection Analysis System Header File
 * Created On: Dec 01, 2017
 * Last Modified: 
 * Reference Package: PulseCruncher
 */

#ifndef COMBINEFILE_HH
#define COMBINEFILE_HH

//Root Include 
#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
	
//C++ class
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>

// use namespace packages (don't want to load full std package? Use std::(package) instead.)
using namespace std;

class CombineFile {
public:
  //declaration of a string of File to be read by Pulse Cruncher
  string File[1000] = {}; 

  void CreateAoS(string, string);
  void CombinePCOutput(string, int);
  void RunExtractArea(string, int, int);
  //string String_File() {return File[int]};
};



#endif
