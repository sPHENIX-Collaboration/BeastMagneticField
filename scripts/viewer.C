
void viewer(const char *fname, double minR = 0.0, double maxR = 400.0, 
	    double minZ = -450.0, double maxZ = 450.0, unsigned cvX = 1200, 
	    unsigned cvY = 1200, bool plotBz = true, bool plotBr = false, 
	    bool plotBm = true)
{
  gStyle->SetOptStat(0);

  auto bmf = new BeastMagneticField(fname);

  if (bmf->ValidMapImported()) {
    bool plot[3] = {plotBz, plotBr, plotBm};
    unsigned divY = plotBz + plotBr + plotBm;
    auto cv = new TCanvas("cv", "", 0, 0, cvX, cvY); cv->Divide(1,divY);

    unsigned dimR = 200, dimZ = 600;
    double cellR = (maxR - minR)/dimR, cellZ = (maxZ - minZ)/dimZ;
    
    auto hbr = new TH2D("hbr", "R-component",     dimZ, minZ, maxZ, dimR, minR, maxR);
    auto hbz = new TH2D("hbz", "Z-component",     dimZ, minZ, maxZ, dimR, minR, maxR);
    auto hbm = new TH2D("hbm", "SQRT(BR^2+BZ^2)", dimZ, minZ, maxZ, dimR, minR, maxR);
    TH2D *hh[3] = {hbz, hbr, hbm};
    
    for(unsigned ir=0; ir<dimR; ir++)
      for(unsigned iz=0; iz<dimZ; iz++) {
	double br, bz;
	bmf->GetFieldValue(minR + cellR*(ir + 0.5), minZ + cellZ*(iz + 0.5), br, bz);
	
	hbz->SetBinContent(iz+1, ir+1, bz);
	hbr->SetBinContent(iz+1, ir+1, br);
	hbm->SetBinContent(iz+1, ir+1, sqrt(bz*bz+br*br));
      } //for ir..iz
    
    {
      unsigned iYcurrent = 1;
      
      for(unsigned iy=0; iy<3; iy++) 
	if (plot[iy]) {
	  cv->cd(iYcurrent++);
	  hh[iy]->Draw("COLZ");
	} 
    }
  } else
    printf("\n  Was not able to import the field map!\n");
} // viewer()
