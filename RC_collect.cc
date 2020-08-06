#include <iostream>
#include <iomanip>
#include <fstream>
#include <TFile.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TMatrixD.h>
#include <TMatrixTUtils.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TGaxis.h>

#include "RC_collect.h"

#define data_path "./Multiplicities"
#define YMULT 2 // 1: Mean, 2: Weighted Mean, 3: Integration (1 y-bin)
#define STAGGERED 1

using namespace std;

static int verbose;

void yavg()
{
  int pMean;

  for(int x=0; x<9; x++)
  {
    for(int z=0; z<12; z++)
    {
      pMean = 0;
      for(int i=0; i<6; i++)
      {
        fMultiplicities_yavg[x][z].tab[0]+=fMultiplicities[x][i][z].tab[0];
        if(fMultiplicities[x][i][z].tab[0]) pMean++;
        fMultiplicities_yavg[x][z].tab[1]+=fMultiplicities[x][i][z].tab[1];
        fMultiplicities_yavg[x][z].tab[2]+=fMultiplicities[x][i][z].tab[2];
      }
      if(pMean)
      {
        fMultiplicities_yavg[x][z].tab[0]/=pMean;
        fMultiplicities_yavg[x][z].tab[1]/=pow(pMean,2);
        fMultiplicities_yavg[x][z].tab[2]/=pow(pMean,2);
      }
    }
  }
}

void yweightedavg()
{
  for(int x=0; x<9; x++)
  {
    for(int z=0; z<12; z++)
    {
      for(int i=0; i<6; i++)
      {
        if(fMultiplicities[x][i][z].tab[0])
        {
          fMultiplicities_yavg[x][z].tab[0]+=fMultiplicities[x][i][z].tab[0]/fMultiplicities[x][i][z].tab[1];
          fMultiplicities_yavg[x][z].tab[1]+=1/fMultiplicities[x][i][z].tab[1];
          fMultiplicities_yavg[x][z].tab[2]+=1/fMultiplicities[x][i][z].tab[2];
          fMeanvalues_yavg[x][z].tab[0] += fMeanvalues_data[x][i][z].tab[0]/fMultiplicities[x][i][z].tab[1];
          fMeanvalues_yavg[x][z].tab[2] += fMeanvalues_data[x][i][z].tab[2]/fMultiplicities[x][i][z].tab[1];
          fMeanvalues_yavg[x][z].tab[3] += fMeanvalues_data[x][i][z].tab[3]/fMultiplicities[x][i][z].tab[1];
        }
        for(int pt=0; pt<5; pt++)
        {
          if(fMultiplicities_pt[x][i][z][pt].tab[0])
          {
              fMultiplicities_pt_yavg[x][z][pt].tab[0]+=fMultiplicities_pt[x][i][z][pt].tab[0]/fMultiplicities_pt[x][i][z][pt].tab[1];
              fMultiplicities_pt_yavg[x][z][pt].tab[1]+=1/fMultiplicities_pt[x][i][z][pt].tab[1];
              fMultiplicities_pt_yavg[x][z][pt].tab[2]+=1/fMultiplicities_pt[x][i][z][pt].tab[2];
          }
        }
      }
      if(fMultiplicities_yavg[x][z].tab[0])
      {
        fMultiplicities_yavg[x][z].tab[1]=1/fMultiplicities_yavg[x][z].tab[1];
        fMultiplicities_yavg[x][z].tab[2]=1/fMultiplicities_yavg[x][z].tab[2];
        fMultiplicities_yavg[x][z].tab[0]*=fMultiplicities_yavg[x][z].tab[1];
        fMeanvalues_yavg[x][z].tab[0] *= fMultiplicities_yavg[x][z].tab[1];
        fMeanvalues_yavg[x][z].tab[2] *= fMultiplicities_yavg[x][z].tab[1];
        fMeanvalues_yavg[x][z].tab[3] *= fMultiplicities_yavg[x][z].tab[1];
      }
      for(int pt=0; pt<5; pt++)
      {
        if(fMultiplicities_pt_yavg[x][z][pt].tab[0])
        {
          fMultiplicities_pt_yavg[x][z][pt].tab[1]=1/fMultiplicities_pt_yavg[x][z][pt].tab[1];
          fMultiplicities_pt_yavg[x][z][pt].tab[2]=1/fMultiplicities_pt_yavg[x][z][pt].tab[2];
          fMultiplicities_pt_yavg[x][z][pt].tab[0]*=fMultiplicities_pt_yavg[x][z][pt].tab[1];
        }
      }         
    }
  }
}

void resetValues()
{
  for(int i=0; i<9; i++)
  {
    for(int j=0; j<6; j++)
    {
      for(int k=0; k<12; k++)
      {
        fMeanvalues_temp[i][j][k].tab[0] = 0;
        fMeanvalues_temp[i][j][k].tab[1] = 0;
        fMeanvalues_temp[i][j][k].tab[2] = 0;
        fMeanvalues_temp[i][j][k].tab[3] = 0;
      }
    }
  }
}

int main(int argc, char **argv)
{
  verbose = 1;
  for (int i = 1; i < argc; i++) 
  {
    if (string(argv[i]) == "-h")
    {
      cout << FCYN("HELP : available flags :\n");
      cout << FCYN(", -q: Quiet\n");
      return 1;
    }
    if (string(argv[i])=="-q") verbose = 0;
  }

  float dummy;
  string filelist;

  ifstream dis_file("DIS.txt");
  if (dis_file.fail()) 
  {
    string fName = "DIS.txt";
    cerr << "\nERROR opening \""<< fName <<"\": " << strerror(errno) << endl;
    abort();
  }

  ifstream had_file("hadron.txt");
  ifstream had_pt_file("hadron_pT.txt");

  for(int i=0; i<9; i++)
  {
    for(int j=0; j<6; j++)
    {
      for(int k=0; k<12; k++)
      {
        dis_file >> dummy;
        fNDIS_evt[i][j][k] += dummy;
        dis_file >> dummy;
        fNDIS_evt_err[i][j][k] += dummy;

        dis_file >> dummy;
        fMeanvalues_temp[i][j][k].tab[0] += dummy;
        dis_file >> dummy;
        fMeanvalues_temp[i][j][k].tab[1] += dummy;
        dis_file >> dummy;
        fMeanvalues_temp[i][j][k].tab[2] += dummy;
        dis_file >> dummy;
        fMeanvalues_temp[i][j][k].tab[3] += dummy;
        dis_file >> dummy;
        fMeanvalues_size[i][j][k].tab[0] += dummy;

        had_file >> dummy;
        fBinning[i][j][k].tab[0] += dummy;
        had_file >> dummy;
        fBinning[i][j][k].tab[1] += dummy;
        for(int pt=0; pt<5; pt++)
        {
          had_pt_file >> dummy;
          fBinning_pt[i][j][k][pt].tab[0] += dummy;
          had_pt_file >> dummy;
          fBinning_pt[i][j][k][pt].tab[1] += dummy;
        }
      }
    }
  }




  for(int i=0; i<9; i++)
  {
    for(int j=0; j<6; j++)
    {
      for(int k=0; k<12; k++)
      {
        if(int(fMeanvalues_size[i][j][k].tab[0]))
        {
          fMeanvalues_data[i][j][k].tab[0] = fMeanvalues_temp[i][j][k].tab[0]/int(fMeanvalues_size[i][j][k].tab[0]);
          fMeanvalues_data[i][j][k].tab[1] = fMeanvalues_temp[i][j][k].tab[1]/int(fMeanvalues_size[i][j][k].tab[0]);
          fMeanvalues_data[i][j][k].tab[2] = fMeanvalues_temp[i][j][k].tab[2]/int(fMeanvalues_size[i][j][k].tab[0]);
          fMeanvalues_data[i][j][k].tab[3] = fMeanvalues_temp[i][j][k].tab[3]/int(fMeanvalues_size[i][j][k].tab[0]);
        }
      }
    }
  }

  resetValues();

  TCanvas* c51;
  c51 = new TCanvas("Hadron_Multiplicities","Hadron_Multiplicities",3200,1600);

  TCanvas* c9;
  c9 = new TCanvas("Hadron_Multiplicities_yavg","Hadron_Multiplicities_yavg",3200,1600);

  TCanvas* c13;
  c13 = new TCanvas("Hadron_Multiplicities_sum","Hadron_Multiplicities_sum",3200,1600);

  c51->SetFillColor(0);
  c9->SetFillColor(0);
  c13->SetFillColor(0);

  c51->Divide(5,2,0,0);
  c9->Divide(5,2,0,0);
  c13->Divide(1,1);

  TGraphErrors* H_mult[9][6];
  TGraphErrors* H_y[9];
  TGraphErrors* sH_y;

  TGraphAsymmErrors* H_sys[9][5];
  TGraphAsymmErrors* H_ysys[9];

  Float_t z_range[12] = {.225,.275,.325,.375,.425,.475,.525,.575,.625,.675,.725,.8};
  Float_t x_range[9] = {.008,.015,.025,.035,.05,.08,.12,.16,.29};
  Float_t errorx[12] = {0.05/2,0.05/2,0.05/2,0.05/2,0.05/2,0.05/2,0.05/2,0.05/2,0.05/2,0.05/2,0.05/2,0.1/2};
  Float_t h_yoffset[12] = {-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2};
  Float_t h_yoffset2[12] = {-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3};

  ofstream ofs_t(Form("%s/multiplicities_raw.txt",data_path), ofstream::out | ofstream::trunc);
  ofstream ofs_h(Form("%s/multiplicities_hadron.txt",data_path), ofstream::out | ofstream::trunc);
  ofstream ofs_yah(Form("%s/multiplicities_hadron_yavg.txt",data_path), ofstream::out | ofstream::trunc);
  ofstream ofs_hpt(Form("%s/multiplicities_hadron_pT_yavg.txt",data_path), ofstream::out | ofstream::trunc);

  vector<Float_t> h_m[9][6];
  vector<Float_t> h_err[9][6];
  vector<Float_t> h_sys[9][6];
  vector<Float_t> z_range_h[9][6];
  vector<Float_t> h_y[9];
  vector<Float_t> h_y_err[9];
  vector<Float_t> h_y_sys[9];
  vector<Float_t> z_range_h_y[9];
  vector<Float_t> sh_y;
  vector<Float_t> sh_y_err;
  vector<Float_t> sx_range_h_y;
  vector<Float_t> rh_y;
  vector<Float_t> rh_y_err;
  vector<Float_t> rx_range_h_y;

  TLine lsys(0.1,0,0.9,0);
  lsys.SetLineStyle(2);

  for(int i=0; i<9; i++)
  {
    int axisflagh1 = 0;
    for(int jj=0; jj<6; jj++)
    {
      for(int k=0; k<12; k++)
      {
        fMultiplicities[i][jj][k].tab[0] += (fBinning[i][jj][k].tab[0] && fNDIS_evt[i][jj][k] ?
                                                  Float_t(fBinning[i][jj][k].tab[0]/(fNDIS_evt[i][jj][k]*fZ_bin_width[k]))
                                                  : 0);

        fMultiplicities[i][jj][k].tab[1] += (fNDIS_evt[i][jj][k] ?
                                                  Float_t((fBinning[i][jj][k].tab[1]/pow(fNDIS_evt[i][jj][k],2)-pow(fBinning[i][jj][k].tab[0],2)*
                                                  fNDIS_evt_err[i][jj][k]/pow(fNDIS_evt[i][jj][k],4))*pow(1./(fZ_bin_width[k]),2))
                                                  : 0);

        fMultiplicities[i][jj][k].tab[2] += 0;

        if(fMultiplicities[i][jj][k].tab[0]<=0 || fMultiplicities[i][jj][k].tab[0]*0.5<fMultiplicities[i][jj][k].tab[1])
        {
          fMultiplicities[i][jj][k].tab[0] = 0 ;
          fMultiplicities[i][jj][k].tab[1] = 0 ;
          fMultiplicities[i][jj][k].tab[2] = 0 ;
        }

        for(int pt=0; pt<5; pt++)
        {
          fMultiplicities_pt[i][jj][k][pt].tab[0] += (fBinning_pt[i][jj][k][pt].tab[0] && fNDIS_evt[i][jj][k] ?
                                                    Float_t(fBinning_pt[i][jj][k][pt].tab[0]/(fNDIS_evt[i][jj][k]*fZ_bin_width[k]))
                                                    : 0);

          fMultiplicities_pt[i][jj][k][pt].tab[1] += (fNDIS_evt[i][jj][k] ?
                                                    Float_t((fBinning_pt[i][jj][k][pt].tab[1]/pow(fNDIS_evt[i][jj][k],2)-pow(fBinning_pt[i][jj][k][pt].tab[0],2)*
                                                    fNDIS_evt_err[i][jj][k]/pow(fNDIS_evt[i][jj][k],4))*pow(1./(fZ_bin_width[k]),2))
                                                    : 0);

          fMultiplicities_pt[i][jj][k][pt].tab[2] += 0;

          if(fMultiplicities_pt[i][jj][k][pt].tab[0]<=0 || fMultiplicities_pt[i][jj][k][pt].tab[0]*0.5<fMultiplicities_pt[i][jj][k][pt].tab[1])
          {
            fMultiplicities_pt[i][jj][k][pt].tab[0] = 0 ;
            fMultiplicities_pt[i][jj][k][pt].tab[1] = 0 ;
            fMultiplicities_pt[i][jj][k][pt].tab[2] = 0 ;
          }
        }

        ofs_t << fXrange[i] << " " << fYrange[jj] << " " << fZrange[k] << " ";

        ofs_t <<
        fMeanvalues_data[i][jj][k].tab[0] << " " << fMeanvalues_data[i][jj][k].tab[1] << " " <<
        fMeanvalues_data[i][jj][k].tab[2] << " " << fMeanvalues_data[i][jj][k].tab[3] << " " <<
        fMultiplicities[i][jj][k].tab[0] << endl;

        ofs_h << fXrange[i] << " " << fYrange[jj] << " " << fZrange[k] << " ";

        ofs_h <<
        fMeanvalues_data[i][jj][k].tab[0] << " " << fMeanvalues_data[i][jj][k].tab[1] << " " <<
        fMeanvalues_data[i][jj][k].tab[2] << " " << fMeanvalues_data[i][jj][k].tab[3] << " " <<
        fMultiplicities[i][jj][k].tab[0] << " " <<
        fMultiplicities[i][jj][k].tab[1] << " " <<
        fMultiplicities[i][jj][k].tab[2] << " " <<
        (fMultiplicities[i][jj][k].tab[0] ? 1 : 0) << endl;

        if(STAGGERED)
        {
          h_m[i][jj].push_back(fMultiplicities[i][jj][k].tab[0]>0 ? fMultiplicities[i][jj][k].tab[0]+jj*0.25 : 0);
        }
        else
        {
          h_m[i][jj].push_back(fMultiplicities[i][jj][k].tab[0]>0 ? fMultiplicities[i][jj][k].tab[0] : 0);
        }
        h_err[i][jj].push_back(fMultiplicities[i][jj][k].tab[1] ? sqrt(fMultiplicities[i][jj][k].tab[1]) : 0);
        h_sys[i][jj].push_back(fMultiplicities[i][jj][k].tab[2] ? sqrt(fMultiplicities[i][jj][k].tab[2]) : 0);
      }

      for(int l=0; l<12; l++)
      {
        z_range_h[i][jj].push_back(z_range[l]);
      }

      for(int k=12; k>0; k--)
      {
        if(!h_m[i][jj][k-1]) {h_m[i][jj].erase(h_m[i][jj].begin()+k-1); h_err[i][jj].erase(h_err[i][jj].begin()+k-1); h_sys[i][jj].erase(h_sys[i][jj].begin()+k-1); z_range_h[i][jj].erase(z_range_h[i][jj].begin()+k-1);}
      }

      bool h_m_empty = 0;
      if(!(Int_t(h_m[i][jj].size()))) h_m_empty = 1;

      H_mult[i][jj] = new TGraphErrors(Int_t(h_m[i][jj].size()),&(z_range_h[i][jj][0]),&(h_m[i][jj][0]),0,&(h_err[i][jj][0]));
      H_sys[i][jj] = new TGraphAsymmErrors(Int_t(h_m[i][jj].size()),&(z_range_h[i][jj][0]), &h_yoffset[0], &errorx[0], &errorx[0], 0, &(h_sys[i][jj][0]));

      H_mult[i][jj]->SetMarkerColor(fMarkerColor[jj]);
      H_mult[i][jj]->SetMarkerSize(2);
      H_mult[i][jj]->SetMarkerStyle(fMarkerStyle[0]);
      H_mult[i][jj]->SetTitle("");
      H_mult[i][jj]->GetYaxis()->SetTitle("");
      H_sys[i][jj]->SetFillColor(fMarkerColor[jj]);

      if(!h_m_empty)
      {
        c51->cd(i+1);
        if(H_mult[i][jj])
        {
          if(!axisflagh1) H_mult[i][jj]->Draw("SAMEPA");
          else H_mult[i][jj]->Draw("SAMEP");
          axisflagh1 = 1;
          H_mult[i][jj]->GetXaxis()->SetLimits(0.1,0.9);
          H_mult[i][jj]->SetMinimum(-0.4);
          H_mult[i][jj]->SetMaximum(5.);
          H_mult[i][jj]->GetXaxis()->SetLabelSize(0.08);
          H_mult[i][jj]->GetYaxis()->SetLabelSize(0.08);
          H_mult[i][jj]->SetTitle("");
          if(i>4) gPad->SetBottomMargin(.15);
          if(i==0 || i==5) gPad->SetLeftMargin(.22);
          if(i==8)
          {
            H_mult[i][jj]->GetXaxis()->SetTitle("#font[ 12]{z}");
            H_mult[i][jj]->GetXaxis()->SetTitleSize(0.09);
            H_mult[i][jj]->GetXaxis()->SetTitleOffset(.6);
          }
          H_mult[i][jj]->GetXaxis()->SetNdivisions(304,kTRUE);
          H_mult[i][jj]->GetYaxis()->SetNdivisions(304,kTRUE);
          if(i==0)
          {
            H_mult[i][jj]->GetYaxis()->SetTitle("#frac{#font[12]{dM}^{#font[ 12]{h}}}{#font[12]{dz}}+ #font[ 12]{#delta}");
            H_mult[i][jj]->GetYaxis()->SetTitleSize(0.09);
            H_mult[i][jj]->GetYaxis()->SetTitleOffset(.9);
          }
          lsys.Draw();
          c51->Range(0.1,-0.4,0.9,5.);
        }
        c51->Update();
      }

      z_range_h[i][jj].clear();
    }
    Float_t MultiplicitiesSum[2];
    MultiplicitiesSum[0] = 0;
    MultiplicitiesSum[1] = 0;

    if(YMULT == 1) yavg();
    else if(YMULT == 2) yweightedavg();

    for(int k=0; k<12; k++)
    {
      if(fMultiplicities_yavg[i][k].tab[0]<0)
      {
        fMultiplicities_yavg[i][k].tab[0] = 0 ;
        fMultiplicities_yavg[i][k].tab[1] = 0 ;
        fMultiplicities_yavg[i][k].tab[2] = 0 ;
      }

      ofs_yah << fXrange[i] << " " << fZrange[k] << " ";

      ofs_yah <<
      fMeanvalues_yavg[i][k].tab[0] << " " <<
      fMeanvalues_yavg[i][k].tab[2] << " " << fMeanvalues_yavg[i][k].tab[3] << " " <<
      fMultiplicities_yavg[i][k].tab[0] << " " <<
      fMultiplicities_yavg[i][k].tab[1] << " " <<
      fMultiplicities_yavg[i][k].tab[2] << " " <<
      (fMultiplicities_yavg[i][k].tab[0] ? 1 : 0) << " ";

      for(int pt=0; pt<5; pt++)
      {
        ofs_hpt <<
        fMultiplicities_pt_yavg[i][k][pt].tab[0] << " " <<
        fMultiplicities_pt_yavg[i][k][pt].tab[1] << " " <<
        fMultiplicities_pt_yavg[i][k][pt].tab[2] << " ";
      }

      h_y[i].push_back(fMultiplicities_yavg[i][k].tab[0]);
      h_y_err[i].push_back(sqrt(fMultiplicities_yavg[i][k].tab[1]));
      h_y_sys[i].push_back(sqrt(fMultiplicities_yavg[i][k].tab[2]));

      MultiplicitiesSum[0] += fMultiplicities_yavg[i][k].tab[0]*fZ_bin_width[k];
      MultiplicitiesSum[1] += fMultiplicities_yavg[i][k].tab[1]*pow(fZ_bin_width[k],2);
    }

    for(int l=0; l<12; l++)
    {
      z_range_h_y[i].push_back(z_range[l]);
    }

    for(int k=12; k>0; k--)
    {
      if(!h_y[i][k-1]) {h_y[i].erase(h_y[i].begin()+k-1); h_y_err[i].erase(h_y_err[i].begin()+k-1); h_y_sys[i].erase(h_y_sys[i].begin()+k-1); z_range_h_y[i].erase(z_range_h_y[i].begin()+k-1);}
    }

    bool h_y_empty = 0;
    if(!(Int_t(h_y[i].size()))) h_y_empty = 1;

    H_y[i] = new TGraphErrors(Int_t(h_y[i].size()),&(z_range_h_y[i][0]),&(h_y[i][0]),0,&(h_y_err[i][0]));
    H_ysys[i] = new TGraphAsymmErrors(Int_t(h_y[i].size()),&(z_range_h_y[i][0]),&h_yoffset2[0], &errorx[0], &errorx[0], 0, &(h_y_sys[i][0]));

    H_y[i]->SetMarkerColor(fMarkerColor[4]);
    H_ysys[i]->SetFillColor(fMarkerColor[4]);
    H_y[i]->SetMarkerSize(2);
    H_y[i]->SetMarkerStyle(fMarkerStyle[0]);
    H_y[i]->SetTitle("");
    H_y[i]->GetYaxis()->SetTitle("");

    if(!h_y_empty)
    {
      c9->cd(i+1);
      gPad->SetFillStyle(4000);
      if(H_y[i])
      {
        H_y[i]->Draw("SAMEPA");
        H_y[i]->GetXaxis()->SetLimits(0.1,0.9);
        H_y[i]->SetMinimum(-0.5);
        H_y[i]->SetMaximum(4.);
        H_y[i]->GetXaxis()->SetLabelSize(0.06);
        H_y[i]->GetYaxis()->SetLabelSize(0.06);
        H_y[i]->SetTitle("");
        if(i>4) gPad->SetBottomMargin(.15);
        if(i==0 || i==5) gPad->SetLeftMargin(.22);
        if(i==8)
        {
          H_y[i]->GetXaxis()->SetTitle("#font[ 12]{z}");
          H_y[i]->GetXaxis()->SetTitleSize(0.08);
          H_y[i]->GetXaxis()->SetTitleOffset(.8);
        }
        H_y[i]->GetXaxis()->SetNdivisions(304,kTRUE);
        H_y[i]->GetYaxis()->SetNdivisions(304,kTRUE);
        if(i==0)
        {
          H_y[i]->GetYaxis()->SetTitle("#frac{#font[12]{dM}^{#font[12]{h}}}{#font[12]{dz}}");
          H_y[i]->GetYaxis()->SetTitleSize(0.11);
          H_y[i]->GetYaxis()->SetTitleOffset(.6);
        }
        lsys.Draw();
        H_y[i]->Draw("SAMEP");
        H_ysys[i]->Draw("SAME3");
        H_y[i]->GetXaxis()->SetLimits(0.1,0.9);
        H_y[i]->SetMinimum(-0.5);
        H_y[i]->SetMaximum(4.);
        c9->Range(0.1,-0.5,0.9,4.);
        c9->cd(10);
      }
      c9->Update();
    }

    z_range_h_y[i].clear();

    sh_y.push_back(MultiplicitiesSum[0]);
    sh_y_err.push_back(sqrt(MultiplicitiesSum[1]));
  }

  for(int l=0; l<9; l++)
  {
    sx_range_h_y.push_back(x_range[l]);
  }

  for(int k=9; k>0; k--)
  {
    if(!sh_y[k-1]) {sh_y.erase(sh_y.begin()+k-1); sh_y_err.erase(sh_y_err.begin()+k-1); sx_range_h_y.erase(sx_range_h_y.begin()+k-1);}   
  }

  sH_y = new TGraphErrors(Int_t(sh_y.size()),&(sx_range_h_y[0]),&(sh_y[0]),0,&(sh_y_err[0]));
  sH_y->SetMarkerColor(fMarkerColor[0]);
  sH_y->SetMarkerSize(2);
  sH_y->SetMarkerStyle(fMarkerStyle[0]);
  sH_y->SetTitle("#font[12]{M}^{#font[ 12]{h^{+}}}+#font[12]{M}^{#font[ 12]{h^{-}}} vs. #font[12]{x}");
  sH_y->GetYaxis()->SetTitle("");


  c13->cd(1);
  gPad->SetFillStyle(4000);
  sH_y->Draw("PA");
  sH_y->GetXaxis()->SetLimits(0.01,1.);
  sH_y->SetMinimum(0.);sH_y->SetMaximum(1.);
  sH_y->GetXaxis()->SetTitle("#font[12]{x}");
  sH_y->GetXaxis()->SetNdivisions(304,kTRUE);
  sH_y->GetYaxis()->SetNdivisions(304,kTRUE);
  sH_y->GetYaxis()->SetTitle("#font[12]{M}^{#font[12]{h}}");
  c13->Range(0.1,0.,0.9,1.);
  gPad->SetLogx();
  c13->Update();

  TLatex fTitle;

  c51->cd(1);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 4.4,"0.004#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.01");

  c51->cd(2);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 4.4,"0.01#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.02");

  c51->cd(3);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 4.4,"0.02#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.03");

  c51->cd(4);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 4.4,"0.03#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.04");

  c51->cd(5);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 4.4,"0.04#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.06");

  c51->cd(6);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 4.4,"0.06#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.1");

  c51->cd(7);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 4.4,"0.1#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.14");

  c51->cd(8);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 4.4,"0.14#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.18");

  c51->cd(9);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 4.4,"0.18#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.4");

  c51->cd(10);
  fTitle.SetTextSize(0.095);
  fTitle.SetTextAlign(11);
  fTitle.DrawLatex(0.05, 0.64,"#color[4]{0.50#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.70, #delta = 1.0}");
  fTitle.DrawLatex(0.05, 0.56,"#color[226]{0.30#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.50, #delta = 0.75}");
  fTitle.DrawLatex(0.05, 0.48,"#color[209]{0.20#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.30, #delta = 0.50}");
  fTitle.DrawLatex(0.05, 0.40,"#color[95]{0.15#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.20, #delta = 0.25}");
  fTitle.DrawLatex(0.05, 0.32,"#color[2]{0.10#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.15, #delta = 0}");

  c9->cd(1);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 3.7,"0.004#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.01");

  c9->cd(2);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 3.7,"0.01#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.02");

  c9->cd(3);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 3.7,"0.02#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.03");

  c9->cd(4);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 3.7,"0.03#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.04");

  c9->cd(5);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 3.7,"0.04#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.06");

  c9->cd(6);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 3.7,"0.06#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.1");

  c9->cd(7);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 3.7,"0.1#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.14");

  c9->cd(8);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 3.7,"0.14#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.18");

  c9->cd(9);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 3.7,"0.18#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.4");

  c51->Update();
  c9->Update();
  c13->Update();

  c51->Print(Form("%s/hadron_multiplicity_file.pdf",data_path),"pdf");
  c9->Print(Form("%s/hadron_multiplicity_yavg_file.pdf",data_path));
  c13->Print(Form("%s/hadron_multiplicity_sum_file.pdf",data_path));

  ofs_h.close();
  ofs_yah.close();
  ofs_hpt.close();

  return 0;
}
