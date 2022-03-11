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


int main(int argc, char** argv)
{
  QApplication* app;

  gErrorIgnoreLevel = kError;

  cout << "\n Welcome to READ_FAME \n" << endl;


  QApplication a(argc, argv);
  cout << "Application launched...\n" << endl;
  app = &a;

  MainWindow w;
  cout << "Main Window constructed...\n" << endl;

  if(argc == 2)
  {
    ifstream inputfile(argv[1]);
    cout << "creating streamer" << endl;
    w.f->SetInfo(&w);

    w.f->LoadInput(inputfile);
    cout << "loading input" << endl;
    inputfile.close();

    cout << "running fame" << endl;
    w.f->SetInfo(&w);
    w.f->Run();

    cout << "done" << endl;
    return 0;
    // ****************
    cout << "after return" << endl;
    // ******************
  }
  else
  {
    w.show();
    cout << "Main Window shown...\n" << endl;
  }
  // ****************
  cout << "before Exec" << endl;
  // ******************
  a.exec();
  // ****************
  cout << "after Exec" << endl;
  // ******************
  return 0;
  // ****************
  cout << "after final return" << endl;
  // ******************
}
