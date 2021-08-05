// Programmer: Stefano Marin, Jonathan Giha
// Purpose: Reading the fission events generated frmo ROOT_FAME
// Date: April 2021

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

QApplication* app;

int main(int argc, char** argv)
{

  gErrorIgnoreLevel = kError;

  cout << "\n Welcome to READ_FAME \n" << endl;


  QApplication a(argc, argv);
  cout << "Application launched...\n" << endl;
  app = &a;

  MainWindow w;
  cout << "Main Window constructed...\n" << endl;

  w.show();
  cout << "Main Window shown...\n" << endl;

  a.exec();
  cout << "\nDone.\n" << endl;

  return 0;
  /*
  readFiss* f = new readFiss(argc, argv);
  // TBrowser* browser = new TBrowser();

  delete f;
  return 0;*/
}
