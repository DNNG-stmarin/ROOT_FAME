// Programmer: Stefano Marin, Jonathan Giha
// Purpose: Reading the fission events generated frmo ROOT_FAME
// Date: April 2021

// NOTE: Beam functionality is not at all implemented!
// I've left some placeholders, but not for everything.

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TBrowser.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "readFiss.h"

using namespace std;

int main(int argc, char** argv)
{

  gErrorIgnoreLevel = kWarning;

  cout << "\n Welcome to READ_FAME \n" << endl;

  readFiss* f = new readFiss(argc, argv);
  // TBrowser* browser = new TBrowser();

  delete f;
  return 0;
}
