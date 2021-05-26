
using namespace std;

readFiss::ReadBeamInfo()
{
  // extract the histrograms from the file
  alphaSpecAll = (TH2D*)beamFile->Get("alphaChanSpec");
	macroPop = (TH1I*)beamFile->Get("macroPop");

  cd_beam->cd();
  alphaSpecAll->Write();
  macroPop->Write();

}
