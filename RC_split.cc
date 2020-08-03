#include "RC_split.h"

using namespace std;

//Input data : root files to be treated should be in data_path/
#define data_path "/sps/compass/julien/CahnYSrc"

// Flags 
#define XMIN 0.004
#define XMAX 0.4
#define YMIN 0.1
#define YMAX 0.7

// Progress bar

# define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
# define PBWIDTH 60

void printProgress(int event, int total) // Prints progress during execution
{
    string points[6] = {"   ",".  ",".. ","..."," ..","  ."};
    float percentage = float(event)/float(total);
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r Progress%s %3d%% [%.*s%*s] (%d/%d)", points[int(event/24)%6].c_str(), val, lpad, PBSTR, rpad, "", event, total);
    fflush (stdout);
}

// BinLogX and BinLogY : create a log scale for a graph

void BinLogX(TH1*h)
{
   TAxis *axis = h->GetXaxis();
   int bins = axis->GetNbins();

   Axis_t from = axis->GetXmin();
   Axis_t to = axis->GetXmax();
   Axis_t width = (to - from) / bins;
   Axis_t *new_bins = new Axis_t[bins + 1];

   for (int i = 0; i <= bins; i++) {
     new_bins[i] = pow(10, from + i * width);
   }
   axis->Set(bins, new_bins);
   delete new_bins;
}

void BinLogY(TH1*h)
{
   TAxis *axis = h->GetYaxis();
   int bins = axis->GetNbins();

   Axis_t from = axis->GetXmin();
   Axis_t to = axis->GetXmax();
   Axis_t width = (to - from) / bins;
   Axis_t *new_bins = new Axis_t[bins + 1];

   for (int i = 0; i <= bins; i++) {
     new_bins[i] = pow(10, from + i * width);
   }
   axis->Set(bins, new_bins);
   delete new_bins;
}

void create_kin_plots() // creates kinematic graphs to be saved in kinSIDIS.pdf and kinSIDIS.root
{
  fKinematics[0] = new TH1F("Q^{2}", "Q^{2}", 100, 0, 2);
  fKinematics[1] = new TH1F("x_{Bj}", "x_{Bj}", 100, -3, 0);
  fKinematics[2] = new TH1F("y", "y", 100, 0, 1);
  fKinematics[3] = new TH1F("z", "z", 100, 0, 1);
  fKinematics[4] = new TH1F("W", "W", 100, 2, 18);
  fKinematics[5] = new TH1F("#nu", "#nu", 100, 0, 160);
  fKinematics2D[0] = new TH2F("DIS kin space (x,y)", "DIS kin space (x,y)", 300, -3, 0, 300, 0, 0.9);
  fKinematics2D[1] = new TH2F("DIS kin space (x,Q2)", "DIS kin space (x,Q2)", 300, -3, 0, 300, -0.097, 2);
  for(int j=0; j<6; j++)
  {
    stringstream nn;
    nn << "z for y bin " << j+1;
    fKinematicsZ[j] = new TH1F(nn.str().c_str(), nn.str().c_str(), 100, 0, 1);
  }
  fQ2k[0] = new TH1F("Q^{2}1","Q^{2}1", 200, -1*log10(8), 2);
  fQ2k[1] = new TH1F("Q^{2}2","Q^{2}2", 200, -1*log10(8), 2);
  fYk[0] = new TH1F("y1","y1", 200, 0, 1);
  fYk[1] = new TH1F("y2","y2", 200, 0, 1);
  fZk[0] = new TH1F("z1","z1", 200, 0, 1);
  fZk[1] = new TH1F("z2","z2", 200, 0, 1);
  fPk[0] = new TH1F("p_{h}1","p_{h}1", 200, 0, 80);
  fPk[1] = new TH1F("p_{h}2","p_{h}2", 200, 0, 80);
  BinLogX(fKinematics[0]);
  BinLogX(fKinematics[1]);
  BinLogX(fKinematics2D[0]);
  fKinematics2D[0]->SetStats(0);
  BinLogX(fKinematics2D[1]);
  BinLogY(fKinematics2D[1]);
  fKinematics2D[1]->SetStats(0);
  BinLogX(fQ2k[0]);
  BinLogX(fQ2k[1]);
}

void save_kin_plots() // make the plots on the different TCanvas and save them in kinSIDIS.pdf and kinSIDIS.root
{
  gStyle->SetPalette(kRainBow);
  c1.Divide(1,1);
  c2.Divide(1,1);
  c3.Divide(1,1);
  c4.Divide(1,1);
  c5.Divide(1,1);
  c6.Divide(1,1);
  c7.Divide(1,1);
  c71.Divide(1,1);
  c8.Divide(3,2);
  c1.cd(1);
  fKinematics[0]->SetStats(0);
  fKinematics[0]->Draw();
  gPad->SetLogx();
  c1.Update();
  c2.cd(1);
  fKinematics[1]->SetStats(0);
  fKinematics[1]->Draw();
  gPad->SetLogx();
  c2.Update();
  c3.cd(1);
  fKinematics[2]->SetStats(0);
  fKinematics[2]->Draw();
  c3.Update();
  c4.cd(1);
  fKinematics[3]->SetStats(0);
  fKinematics[3]->Draw();
  c4.Update();
  c5.cd(1);
  fKinematics[4]->SetStats(0);
  fKinematics[4]->Draw();
  c5.Update();
  c6.cd(1);
  fKinematics[5]->SetStats(0);
  fKinematics[5]->Draw();
  c6.Update();
  c7.cd(1);
  fKinematics2D[0]->SetStats(0);
  fKinematics2D[0]->Draw("COLZ");
  gPad->SetLogx();
  c7.Update();
  c71.cd(1);
  fKinematics2D[1]->SetStats(0);
  fKinematics2D[1]->Draw("COLZ");
  gPad->SetLogx();
  gPad->SetLogy();
  gPad->SetLogz();
  c71.Update();
  for(int j=0; j<6; j++)
  {
    c8.cd(j+1);
    fKinematicsZ[j]->SetStats(0);
    fKinematicsZ[j]->Draw();
    c8.Update();
  }
  
  
  c1.Print("kinSIDIS.pdf(","pdf");
  c2.Print("kinSIDIS.pdf","pdf");
  c3.Print("kinSIDIS.pdf","pdf");
  c4.Print("kinSIDIS.pdf","pdf");
  c8.Print("kinSIDIS.pdf","pdf");
  c5.Print("kinSIDIS.pdf","pdf");
  c6.Print("kinSIDIS.pdf","pdf");
  c7.Print("kinSIDIS.pdf","pdf");
  c71.Print("kinSIDIS.pdf)","pdf");

  TDirectory *dSave = gDirectory;
  TFile *fout = TFile::Open("kinSIDIS.root","RECREATE");
  c1.Write();
  c2.Write();
  c3.Write();
  c4.Write();
  c8.Write();
  c5.Write();
  c6.Write();
  c7.Write();
  c71.Write();
  fout->Close();
  dSave->cd();
}

void resetValues() // reset different arrays to 0
{
  for(int i=0; i<9; i++)
  {
    for(int j=0; j<6; j++)
    {
      for(int k=0; k<12; k++)
      {
        fNDIS_evt[i][j][k]=0; fNDIS_evt_err[i][j][k]=0;
        fNDIS_evt[i][j][k]=0; fNDIS_evt_err[i][j][k]=0;
        fNDIS_evt[i][j][k]=0; fNDIS_evt_err[i][j][k]=0;
        fBinning[i][j][k].tab[0] = 0; fBinning[i][j][k].tab[1] = 0;
        fBinning[i][j][k].tab[0] = 0; fBinning[i][j][k].tab[1] = 0;
        fBinning[i][j][k].tab[0] = 0; fBinning[i][j][k].tab[1] = 0;
        fBinning[i][j][k].tab[0] = 0; fBinning[i][j][k].tab[1] = 0;
        fMeanvalues_data[i][j][k].tab[0]=0;
        fMeanvalues_data[i][j][k].tab[1]=0;
        fMeanvalues_data[i][j][k].tab[2]=0;
        fMeanvalues_data[i][j][k].tab[3]=0;
        fMeanvalues_size[i][j][k].tab[0]=0;
        fMeanvalues_size[i][j][k].tab[1]=0;
        fMeanvalues_size[i][j][k].tab[2]=0;
        fMeanvalues_size[i][j][k].tab[3]=0;
        fMeanvalues[i][j][k].vec[0].clear();
        fMeanvalues[i][j][k].vec[1].clear();
        fMeanvalues[i][j][k].vec[2].clear();
        fMeanvalues[i][j][k].vec[3].clear();
      }
    }
  }
}

int main(int argc, char **argv) // main function
{
  int kin_flag = 0, verbose = 1;
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-h") {
      cout << FCYN("HELP : available flags :\n");
      cout << FCYN(" -k: Draw kinematic plots") << FCYN(", -q: Quiet\n");
      return 1;
    }
    if (string(argv[i])=="-k") kin_flag = 1; // if option "-k" is passed, kinematic plots will be drawn
    if (string(argv[i])=="-q") verbose = 0; // if option "-q" is passed, the program will be more "quiet"
  }

  if(kin_flag) create_kin_plots(); // create kinematic plots if analySIDIS_split is executed with option -k

  // List of files
  string filelist;
    filelist = Form("%s/filelist.txt",data_path); // In data_path/filelist.txt one should have the list of root files to be treated
    ifstream list(filelist);
    if (list.fail()) {
      cerr << "\nERROR accessing \"filelist.txt\" in directory \""<< data_path << "\":" << strerror(errno) << endl;
      return 1;
    }
    string filename;

    while(list >> filename) // Loop over the root files
    {
      TFile *f;

      cout << ".. Processing file " << filename << " .." << endl;
      f = TFile::Open(Form("%s/%s",data_path,filename.c_str())); // open the .root file

      if(!f) continue;

      fFilesNumber++;

      TTree* tree = (TTree*) f->Get("dis"); // Get the TTree of the .root file

      // ---------------------------------------------------------------------------
      // --------- Reading of the TTree --------------------------------------------
      // ---------------------------------------------------------------------------

      //DISEvt
      TBranch *beam_p = (TBranch*) tree->FindBranch("beam_p");
      TBranch *beamph = (TBranch*) tree->FindBranch("beamph");
      TBranch *beamth = (TBranch*) tree->FindBranch("beamth");
      TBranch *mupr_p = (TBranch*) tree->FindBranch("mupr_p");
      TBranch *muprph = (TBranch*) tree->FindBranch("muprph");
      TBranch *muprth = (TBranch*) tree->FindBranch("muprth");
      TBranch *gaene = (TBranch*) tree->FindBranch("gaene");
      TBranch *gaphi = (TBranch*) tree->FindBranch("gaphi");
      TBranch *gathe = (TBranch*) tree->FindBranch("gathe");
      TBranch *phi_s = (TBranch*) tree->FindBranch("phi_s");
      TBranch *phi_str = (TBranch*) tree->FindBranch("phi_str");
      TBranch *ene_rp = (TBranch*) tree->FindBranch("ene_rp");
      TBranch *the_rp = (TBranch*) tree->FindBranch("the_rp");
      TBranch *phi_rp = (TBranch*) tree->FindBranch("phi_rp");
      TBranch *phi_hp = (TBranch*) tree->FindBranch("phi_hp");
      TBranch *bcm = (TBranch*) tree->FindBranch("bcm");
      TBranch *gcm = (TBranch*) tree->FindBranch("gcm");
      TBranch *bcmtr = (TBranch*) tree->FindBranch("bcmtr");
      TBranch *gcmtr = (TBranch*) tree->FindBranch("gcmtr");
      TBranch *nu = (TBranch*) tree->FindBranch("nu");
      TBranch *Q2 = (TBranch*) tree->FindBranch("Q2");
      TBranch *xbj = (TBranch*) tree->FindBranch("xbj");
      TBranch *y = (TBranch*) tree->FindBranch("y");
      TBranch *W = (TBranch*) tree->FindBranch("W");
      TBranch *nutr = (TBranch*) tree->FindBranch("nutr");
      TBranch *Q2tr = (TBranch*) tree->FindBranch("Q2tr");
      TBranch *xbjtr = (TBranch*) tree->FindBranch("xbjtr");
      TBranch *ytr = (TBranch*) tree->FindBranch("ytr");
      TBranch *Wtr = (TBranch*) tree->FindBranch("Wtr");

      //Hadrons
      TBranch *HadNb = (TBranch*) tree->FindBranch("HadNb");
      TBranch *ch = (TBranch*) tree->FindBranch("ch");
      TBranch *zh = (TBranch*) tree->FindBranch("zh");
      TBranch *eh = (TBranch*) tree->FindBranch("eh");
      TBranch *ph = (TBranch*) tree->FindBranch("ph");
      TBranch *theha = (TBranch*) tree->FindBranch("theha");
      TBranch *phiha = (TBranch*) tree->FindBranch("phiha");
      TBranch *xfh = (TBranch*) tree->FindBranch("xfh");
      TBranch *etah = (TBranch*) tree->FindBranch("etah");
      TBranch *pth = (TBranch*) tree->FindBranch("pth");
      TBranch *phi_h = (TBranch*) tree->FindBranch("phi_h");
      TBranch *zhtr = (TBranch*) tree->FindBranch("zhtr");
      TBranch *pthtr = (TBranch*) tree->FindBranch("pthtr");
      TBranch *phi_ht = (TBranch*) tree->FindBranch("phi_ht");

      Int_t N = (Int_t) tree->GetEntries();// N is the number of events in the TTree
      cout << "Number of events = " << N << endl;

      vector<Pvsz> Pvszlocal;
      vector<Pvsz> Pvszloose;
      vector<Pvsz> Pvszsevere;
      vector<Pvsz> Pvsz_errlocal;
      vector<Float_t> XBjlocal;
      vector<Float_t> YBjlocal;
      vector<Float_t> Q2local;
      vector<Float_t> Zvtxlocal;
      vector<Float_t> XBjloose;
      vector<Float_t> YBjloose;
      vector<Float_t> Q2loose;
      vector<Float_t> XBjsevere;
      vector<Float_t> YBjsevere;
      vector<Float_t> Q2severe;

      for (Int_t ip = 0; ip < N; ip++) { // Loop over the events
        if (verbose) printProgress(ip,N); // printProgress will be called unless option -q is passed to analySIDIS_split

        //DISEvt : get the values of the different kinematic variables for the event ip
        beam_p->GetEntry(ip);
        beamph->GetEntry(ip);
        beamth->GetEntry(ip);
        mupr_p->GetEntry(ip);
        muprph->GetEntry(ip);
        muprth->GetEntry(ip);
        gaene->GetEntry(ip);
        gaphi->GetEntry(ip);
        gathe->GetEntry(ip);
        phi_s->GetEntry(ip);
        phi_str->GetEntry(ip);
        ene_rp->GetEntry(ip);
        the_rp->GetEntry(ip);
        phi_rp->GetEntry(ip);
        phi_hp->GetEntry(ip);
        bcm->GetEntry(ip);
        gcm->GetEntry(ip);
        bcmtr->GetEntry(ip);
        gcmtr->GetEntry(ip);
        nu->GetEntry(ip);
        Q2->GetEntry(ip);
        xbj->GetEntry(ip);
        y->GetEntry(ip);
        W->GetEntry(ip);
        nutr->GetEntry(ip);
        Q2tr->GetEntry(ip);
        xbjtr->GetEntry(ip);
        ytr->GetEntry(ip);
        Wtr->GetEntry(ip);

        //Hadrons : get the values of the different variables related to hadrons
        HadNb->GetEntry(ip);
        ch->GetEntry(ip);
        zh->GetEntry(ip);
        eh->GetEntry(ip);
        ph->GetEntry(ip);
        theha->GetEntry(ip);
        phiha->GetEntry(ip);
        xfh->GetEntry(ip);
        etah->GetEntry(ip);
        pth->GetEntry(ip);
        phi_h->GetEntry(ip);
        zhtr->GetEntry(ip);
        pthtr->GetEntry(ip);
        phi_ht->GetEntry(ip);

        // -------------------------------------------------------------------------
        // --------- DIS Selection -------------------------------------------------
        // -------------------------------------------------------------------------

        if(kin_flag) fQ2k[0]->Fill(Q2->GetLeaf("Q2")->GetValue());

        // cout << "Test before cut !!!" << endl;

        // Different kinematical cuts (Q2,x,y,W) : if the event is not in the kinematical region of interest, go on to the next event
        // Q2 cut
        if(!(Q2->GetLeaf("Q2")->GetValue()>1)) continue;
        fQ2test++;

        if(kin_flag)
        {
          fQ2k[1]->Fill(Q2->GetLeaf("Q2")->GetValue());
          fYk[0]->Fill(y->GetLeaf("y")->GetValue());
        }

        // y cut
        if(!(YMIN<y->GetLeaf("y")->GetValue() && y->GetLeaf("y")->GetValue()<YMAX)) continue;
        fYBjtest++;

        if(kin_flag) fYk[1]->Fill(y->GetLeaf("y")->GetValue());

        // x cut
        if(!(XMIN<xbj->GetLeaf("xbj")->GetValue() && xbj->GetLeaf("xbj")->GetValue()<XMAX)) continue;
        fXBjtest++;

        // cout << "Test after cut !!!" << endl;

        if(kin_flag)
        {
          // cout << "Test !!!" << endl;
          // cout << "Q2tr = " << Q2->GetLeaf("Q2")->GetValue() << endl;
          fQ2kin.push_back(Q2->GetLeaf("Q2")->GetValue());
          fXBjkin.push_back(xbj->GetLeaf("xbj")->GetValue());
          fYBjkin.push_back(y->GetLeaf("y")->GetValue());
          fWBjkin.push_back(W->GetLeaf("W")->GetValue());
          fNukin.push_back(nu->GetLeaf("nu")->GetValue());
        }

        // -------------------------------------------------------------------------
        // --------- DIS event calculation -----------------------------------------
        // -------------------------------------------------------------------------

        // x binning

        if(0.004<=xbj->GetLeaf("xbj")->GetValue() && xbj->GetLeaf("xbj")->GetValue()<0.01) xbin = 0;
        else if(0.01<=xbj->GetLeaf("xbj")->GetValue() && xbj->GetLeaf("xbj")->GetValue()<0.02) xbin = 1;
        else if(0.02<=xbj->GetLeaf("xbj")->GetValue() && xbj->GetLeaf("xbj")->GetValue()<0.03) xbin = 2;
        else if(0.03<=xbj->GetLeaf("xbj")->GetValue() && xbj->GetLeaf("xbj")->GetValue()<0.04) xbin = 3;
        else if(0.04<=xbj->GetLeaf("xbj")->GetValue() && xbj->GetLeaf("xbj")->GetValue()<0.06) xbin = 4;
        else if(0.06<=xbj->GetLeaf("xbj")->GetValue() && xbj->GetLeaf("xbj")->GetValue()<0.1) xbin = 5;
        else if(0.1<=xbj->GetLeaf("xbj")->GetValue() && xbj->GetLeaf("xbj")->GetValue()<0.14) xbin = 6;
        else if(0.14<=xbj->GetLeaf("xbj")->GetValue() && xbj->GetLeaf("xbj")->GetValue()<0.18) xbin = 7;
        else xbin = 8;

        // y binning

        if(0.1<y->GetLeaf("y")->GetValue() && y->GetLeaf("y")->GetValue()<0.15) ybin = 0;
        else if(0.15<y->GetLeaf("y")->GetValue() && y->GetLeaf("y")->GetValue()<0.2) ybin = 1;
        else if(0.2<y->GetLeaf("y")->GetValue() && y->GetLeaf("y")->GetValue()<0.3) ybin = 2;
        else if(0.3<y->GetLeaf("y")->GetValue() && y->GetLeaf("y")->GetValue()<0.5) ybin = 3;
        else if(0.5<y->GetLeaf("y")->GetValue() && y->GetLeaf("y")->GetValue()<0.7) ybin = 4;
        else ybin = 5;


        // z binning

        for(int i=0; i<12; i++)
        {
          fNDIS_evt[xbin][ybin][i] += 1*1;
          fNDIS_evt_err[xbin][ybin][i] += pow(1,2);
        }

        // -------------------------------------------------------------------------
        // --------- Hadrons Selection ---------------------------------------------
        // -------------------------------------------------------------------------

        Pvsz pzcontainer;
        Pvsz pzcontainer_err;

        for(int i=0; i<HadNb->GetLeaf("HadNb")->GetValue(); i++) // Loop over hadrons selected for this event
        {
          fHadrons++;

          if(kin_flag)
          {
            fPk[0]->Fill(ph->GetLeaf("ph")->GetValue(i));
          }

          if(kin_flag) fPk[1]->Fill(ph->GetLeaf("ph")->GetValue(i));
          if(kin_flag) fZk[0]->Fill(zh->GetLeaf("zh")->GetValue(i));

          // z cut
          if(!(0.2<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.85)) continue;
          fZtest++;

          if(kin_flag)
          {
            fKinematics[3]->Fill(zh->GetLeaf("zh")->GetValue(i));
            fKinematicsZ[ybin]->Fill(zh->GetLeaf("zh")->GetValue(i));
            fZk[1]->Fill(zh->GetLeaf("zh")->GetValue(i));
          }

          if(0.2<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.25) zbin = 0;
          else if(0.25<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.30) zbin = 1;
          else if(0.30<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.35) zbin = 2;
          else if(0.35<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.40) zbin = 3;
          else if(0.40<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.45) zbin = 4;
          else if(0.45<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.50) zbin = 5;
          else if(0.50<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.55) zbin = 6;
          else if(0.55<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.60) zbin = 7;
          else if(0.60<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.65) zbin = 8;
          else if(0.65<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.70) zbin = 9;
          else if(0.70<zh->GetLeaf("zh")->GetValue(i) && zh->GetLeaf("zh")->GetValue(i)<0.75) zbin = 10;
          else zbin = 11;

          //**********************************************************************

          // Save of hadrons
          pzcontainer.vec.push_back(zh->GetLeaf("zh")->GetValue(i));
          pzcontainer_err.vec.push_back(zh->GetLeaf("zh")->GetValue(i));
        }

        //Misc
        fQ2.push_back(Q2->GetLeaf("Q2")->GetValue());
        fXBj.push_back(xbj->GetLeaf("xbj")->GetValue());
        fYBj.push_back(y->GetLeaf("y")->GetValue());
        fWBj.push_back(W->GetLeaf("W")->GetValue());
        fNu.push_back(nu->GetLeaf("nu")->GetValue());

        Q2local.push_back(Q2->GetLeaf("Q2")->GetValue());
        Pvszlocal.push_back(pzcontainer);
        Pvsz_errlocal.push_back(pzcontainer_err);
        XBjlocal.push_back(xbj->GetLeaf("xbj")->GetValue());
        YBjlocal.push_back(y->GetLeaf("y")->GetValue());
      }

      for(int i=0; i<int(Q2local.size()); i++)
      {
        if(0.004<=XBjlocal[i] && XBjlocal[i]<0.01) xbin = 0;
        else if(0.01<=XBjlocal[i] && XBjlocal[i]<0.02) xbin = 1;
        else if(0.02<=XBjlocal[i] && XBjlocal[i]<0.03) xbin = 2;
        else if(0.03<=XBjlocal[i] && XBjlocal[i]<0.04) xbin = 3;
        else if(0.04<=XBjlocal[i] && XBjlocal[i]<0.06) xbin = 4;
        else if(0.06<=XBjlocal[i] && XBjlocal[i]<0.1) xbin = 5;
        else if(0.1<=XBjlocal[i] && XBjlocal[i]<0.14) xbin = 6;
        else if(0.1<=XBjlocal[i] && XBjlocal[i]<0.18) xbin = 7;
        else xbin = 8;

        if(0.1<YBjlocal[i] && YBjlocal[i]<0.15) ybin = 0;
        else if(0.15<YBjlocal[i] && YBjlocal[i]<0.2) ybin = 1;
        else if(0.2<YBjlocal[i] && YBjlocal[i]<0.3) ybin = 2;
        else if(0.3<YBjlocal[i] && YBjlocal[i]<0.5) ybin = 3;
        else if(0.5<YBjlocal[i] && YBjlocal[i]<0.7) ybin = 4;
        else ybin = 5;

        for(int l=0; l<int(Pvszlocal[i].vec.size()); l++)
        {
          if(0.2<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.25) zbin = 0;
          else if(0.25<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.30) zbin = 1;
          else if(0.30<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.35) zbin = 2;
          else if(0.35<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.40) zbin = 3;
          else if(0.40<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.45) zbin = 4;
          else if(0.45<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.50) zbin = 5;
          else if(0.50<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.55) zbin = 6;
          else if(0.55<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.60) zbin = 7;
          else if(0.60<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.65) zbin = 8;
          else if(0.65<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.70) zbin = 9;
          else if(0.70<Pvszlocal[i].vec[l] && Pvszlocal[i].vec[l]<0.75) zbin = 10;
          else zbin = 11;

          fBinning[xbin][ybin][zbin].tab[0] += Pvszlocal[i].vec[l];
          fBinning[xbin][ybin][zbin].tab[1] += Pvsz_errlocal[i].vec[l];
          fMeanvalues[xbin][ybin][zbin].vec[2].push_back(Q2local[i]);
          fMeanvalues[xbin][ybin][zbin].vec[0].push_back(XBjlocal[i]);
          fMeanvalues[xbin][ybin][zbin].vec[1].push_back(YBjlocal[i]);
          fMeanvalues[xbin][ybin][zbin].vec[3].push_back(Pvszlocal[i].vec[l]);
        }
      }

      Pvszlocal.clear();
      Pvsz_errlocal.clear();
      XBjlocal.clear();
      YBjlocal.clear();
      Q2local.clear();
      f->Close();
    }

    for(int i=0; i<9; i++)
    {
      for(int j=0; j<6; j++)
      {
        for(int k=0; k<12; k++)
        {
          for(int sv=0; sv<int(fMeanvalues[i][j][k].vec[0].size()); sv++)
          {
            fMeanvalues_data[i][j][k].tab[0] += fMeanvalues[i][j][k].vec[0][sv];
            fMeanvalues_data[i][j][k].tab[1] += fMeanvalues[i][j][k].vec[1][sv];
            fMeanvalues_data[i][j][k].tab[2] += fMeanvalues[i][j][k].vec[2][sv];
            fMeanvalues_data[i][j][k].tab[3] += fMeanvalues[i][j][k].vec[3][sv];
          }
          if(int(fMeanvalues[i][j][k].vec[0].size()))
          {
            fMeanvalues_size[i][j][k].tab[0] = int(fMeanvalues[i][j][k].vec[0].size());
            fMeanvalues_size[i][j][k].tab[1] = int(fMeanvalues[i][j][k].vec[1].size());
            fMeanvalues_size[i][j][k].tab[2] = int(fMeanvalues[i][j][k].vec[2].size());
            fMeanvalues_size[i][j][k].tab[3] = int(fMeanvalues[i][j][k].vec[3].size());
          }
        }
      }
    }

    ofstream ofs_h("hadron.txt", std::ofstream::out | std::ofstream::trunc);
    ofstream ofs_d("DIS.txt", std::ofstream::out | std::ofstream::trunc);

    for(int i=0; i<9; i++)
    {
      for(int j=0; j<6; j++)
      {
        for(int k=0; k<12; k++)
        {
          ofs_d << fNDIS_evt[i][j][k] << " " << fNDIS_evt_err[i][j][k];

          ofs_d << " " << fMeanvalues_data[i][j][k].tab[0] << " " <<
                          fMeanvalues_data[i][j][k].tab[1] << " " <<
                          fMeanvalues_data[i][j][k].tab[2] << " " <<
                          fMeanvalues_data[i][j][k].tab[3] << " " << fMeanvalues_size[i][j][k].tab[0];
          ofs_d << endl;

          ofs_h << fBinning[i][j][k].tab[0] << " " << fBinning[i][j][k].tab[1] << endl;
        }
      }
    }

    ofs_h.close();
    ofs_d.close();
    resetValues();

  if(kin_flag)
  {
    cout << "int(fQ2kin.size()) = " << int(fQ2kin.size()) << endl;
    for(int i=0; i<int(fQ2kin.size()); i++)
    {
      fKinematics[0]->Fill(fQ2kin[i]);
      fKinematics[1]->Fill(fXBjkin[i]);
      fKinematics[2]->Fill(fYBjkin[i]);
      fKinematics[4]->Fill(fWBjkin[i]);
      fKinematics[5]->Fill(fNukin[i]);
      fKinematics2D[0]->Fill(fXBjkin[i],fYBjkin[i]);
      fKinematics2D[1]->Fill(fXBjkin[i],fQ2kin[i]);
    }
    save_kin_plots();
  }

  return 0;
}
