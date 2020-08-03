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

#include "RC_plot.h"

using namespace std;

void LoadMultiplicityFiles(string pfile1, string pfile2)
{
  string sdum;
  Double_t x,y,z;

  ifstream mult1_h(Form("%s/multiplicities_hadron.txt",pfile1.c_str()));

  for(int i=0; i<9; i++)
  {
    for(int j=0; j<6; j++)
    {
      for(int k=0; k<12; k++)
      {
          mult1_h >> x >> y >> z;
          for(int l=0; l<4; l++) mult1_h >> sdum;
          mult1_h >> fMultiplicities1[i][j][k].tab[0] >> fMultiplicities1[i][j][k].tab[1] >> fMultiplicities1[i][j][k].tab[2];
          mult1_h >> sdum;
      }
    }
  }
  mult1_h.close();

  ifstream mult2_h(Form("%s/multiplicities_hadron.txt",pfile2.c_str()));

  for(int i=0; i<9; i++)
  {
    for(int j=0; j<6; j++)
    {
      for(int k=0; k<12; k++)
      {
          mult2_h >> x >> y >> z;
          for(int l=0; l<4; l++) mult2_h >> sdum;
          mult2_h >> fMultiplicities2[i][j][k].tab[0] >> fMultiplicities2[i][j][k].tab[1] >> fMultiplicities2[i][j][k].tab[2];
          mult2_h >> sdum;
      }
    }
  }
  mult2_h.close();
}

void yweightedavg()
{
  for(int x=0; x<9; x++)
  {
    for(int z=0; z<12; z++)
    {
      for(int i=0; i<6; i++)
      {
        if(fMultiplicities1[x][i][z].tab[0])
        {
          fMultiplicities1_yavg[x][z].tab[0]+=fMultiplicities1[x][i][z].tab[0]/fMultiplicities1[x][i][z].tab[1];
          fMultiplicities1_yavg[x][z].tab[1]+=1/fMultiplicities1[x][i][z].tab[1];
          fMultiplicities1_yavg[x][z].tab[2]+=1/fMultiplicities1[x][i][z].tab[2];
        }
        if(fMultiplicities2[x][i][z].tab[0])
        {
          fMultiplicities2_yavg[x][z].tab[0]+=fMultiplicities2[x][i][z].tab[0]/fMultiplicities2[x][i][z].tab[1];
          fMultiplicities2_yavg[x][z].tab[1]+=1/fMultiplicities2[x][i][z].tab[1];
          fMultiplicities2_yavg[x][z].tab[2]+=1/fMultiplicities2[x][i][z].tab[2];
        }
      }
      if(fMultiplicities1_yavg[x][z].tab[0])
      {
        fMultiplicities1_yavg[x][z].tab[1]=1/fMultiplicities1_yavg[x][z].tab[1];
        fMultiplicities1_yavg[x][z].tab[2]=1/fMultiplicities1_yavg[x][z].tab[2];
        fMultiplicities1_yavg[x][z].tab[0]*=fMultiplicities1_yavg[x][z].tab[1];
      }
      if(fMultiplicities2_yavg[x][z].tab[0])
      {
        fMultiplicities2_yavg[x][z].tab[1]=1/fMultiplicities2_yavg[x][z].tab[1];
        fMultiplicities2_yavg[x][z].tab[2]=1/fMultiplicities2_yavg[x][z].tab[2];
        fMultiplicities2_yavg[x][z].tab[0]*=fMultiplicities2_yavg[x][z].tab[1];
      }
    }
  }
}

int main(int argc, char **argv)
{

  if(argc < 3)
  {
    cout << "ERROR : Not enough arguments." << endl;
    cout << "Asked : 2 *** Received : " << argc-1 << endl;
    cout << "./RC_plot MultNoRC MultRC" << endl;

    return 1;
  }

  LoadMultiplicityFiles(argv[1],argv[2]);

  TCanvas c10("Mult_comparison_hadron","Mult_comparison_hadron",3200,1600);
  TCanvas c20("Mult_comparison_yavg_hadron","Mult_comparison_yavg_hadron",3200,1600);
  c10.SetFillColor(0);
  c20.SetFillColor(0);
  c10.Divide(5,2,0,0);
  c20.Divide(5,2,0,0);

  ofstream ofs_rc("rad_corr.txt", std::ofstream::out | std::ofstream::trunc);

  TGraphErrors* R_h[9][6];
  TGraphErrors* R_y_h[9];

  for(int i=0; i<9; i++)
  {
    vector<double> r_y_h;
    vector<double> r_y_h_err;
    vector<double> z_range_r_y_h;

    int axisflagh1 = 0;

    for(int l=0; l<12; l++)
    {
      z_range_r_y_h.push_back(z_range[l]);
    }

    for(int j=0; j<6; j++)
    {
      std::vector<double> r_h;
      std::vector<double> r_h_err;
      std::vector<double> z_range_r_h;

      for(int l=0; l<12; l++)
      {
        z_range_r_h.push_back(z_range[l]);
      }

      for(int k=0; k<12; k++)
      {
        r_h.push_back(fMultiplicities2[i][j][k].tab[0] ? fMultiplicities1[i][j][k].tab[0]/fMultiplicities2[i][j][k].tab[0] + j*0.1 : 0);
        r_h_err.push_back(sqrt((fMultiplicities1[i][j][k].tab[1]+pow(fMultiplicities2[i][j][k].tab[1],2)*fMultiplicities1[i][j][k].tab[0]
                                /pow(fMultiplicities2[i][j][k].tab[0],2))/pow(fMultiplicities2[i][j][k].tab[0],2)));
      }

      for(int k=12; k>0; k--)
      {
        if(!r_h[k-1]) {r_h.erase(r_h.begin()+k-1); r_h_err.erase(r_h_err.begin()+k-1); z_range_r_h.erase(z_range_r_h.begin()+k-1);}
      }

      bool r_h_empty = 0;
      if(!(int(r_h.size()))) {r_h_empty = 1; cout << "R_h[" << i << "][" << j << "] is empty !" << endl;}

      R_h[i][j] = new TGraphErrors(int(r_h.size()),&(z_range_r_h[0]),&(r_h[0]),0,&(r_h_err[0]));

      R_h[i][j]->SetMarkerColor(fMarkerColor[j]);
      R_h[i][j]->SetMarkerSize(2);
      R_h[i][j]->SetMarkerStyle(fMarkerStyle[0]);

      if(!r_h_empty)
      {
        c10.cd(i+1);
        gPad->SetFillStyle(4000);
        if(R_h[i][j])
        {
          if(!axisflagh1) R_h[i][j]->Draw("SAMEPA");
          axisflagh1 = 1;
          R_h[i][j]->GetXaxis()->SetLimits(0.1,0.9);
          R_h[i][j]->SetMinimum(1.);
          R_h[i][j]->SetMaximum(1.65);
          R_h[i][j]->GetXaxis()->SetLabelSize(0.06);
          R_h[i][j]->GetYaxis()->SetLabelSize(0.06);
          R_h[i][j]->SetTitle("");
          if(i>=5) gPad->SetBottomMargin(.15);
          if(i==0 || i ==5) gPad->SetLeftMargin(.22);
          if(i==8)
          {
            R_h[i][j]->GetXaxis()->SetTitle("#font[12]{z}");
            R_h[i][j]->GetXaxis()->SetTitleSize(0.08);
            R_h[i][j]->GetXaxis()->SetTitleOffset(.8);
          }
          R_h[i][j]->GetXaxis()->SetNdivisions(304,kTRUE);
          R_h[i][j]->GetYaxis()->SetNdivisions(304,kTRUE);
          if(i==0)
          {
            R_h[i][j]->GetYaxis()->SetTitle("#font[12]{#eta}^{#font[12]{h}} + #font[12]{#delta}");
            R_h[i][j]->GetYaxis()->SetTitleSize(0.08);
          }
          R_h[i][j]->Draw("SAMEP");
          R_h[i][j]->GetXaxis()->SetLimits(0.1,0.9);
          R_h[i][j]->SetMinimum(1.);
          R_h[i][j]->SetMaximum(1.65);
          c10.Range(0.1,1.,0.9,1.65);
        }
        c10.Update();
      }
    }

    yweightedavg();

    for(int k=0; k<12; k++)
    {
      double multy, multye;
      multy = fMultiplicities2_yavg[i][k].tab[0] ? fMultiplicities1_yavg[i][k].tab[0]/fMultiplicities2_yavg[i][k].tab[0] : 0;
      multye = fMultiplicities2_yavg[i][k].tab[0] ? sqrt((fMultiplicities1_yavg[i][k].tab[1]+fMultiplicities2_yavg[i][k].tab[1]*pow(fMultiplicities1_yavg[i][k].tab[0],2)
                              /pow(fMultiplicities2_yavg[i][k].tab[0],2))/pow(fMultiplicities2_yavg[i][k].tab[0],2)) : 0;
      r_y_h.push_back(multy);
      r_y_h_err.push_back(multye);
      ofs_rc << i << " " << k << " " << multy << " " << multye << " " << endl;
    }

    for(int k=12; k>0; k--)
    {
      if(!r_y_h[k-1]) {r_y_h.erase(r_y_h.begin()+k-1); r_y_h_err.erase(r_y_h_err.begin()+k-1); z_range_r_y_h.erase(z_range_r_y_h.begin()+k-1);}
    }

    bool r_y_h_empty = 0;

    if(!(int(r_y_h.size()))) {r_y_h_empty = 1; cout << "R_y_h[" << i << "] is empty !" << endl;}

    R_y_h[i] = new TGraphErrors(int(r_y_h.size()),&(z_range_r_y_h[0]),&(r_y_h[0]),0,&(r_y_h_err[0]));
    R_y_h[i]->SetMarkerColor(fMarkerColor[4]);
    R_y_h[i]->SetMarkerSize(3);
    R_y_h[i]->SetMarkerStyle(fMarkerStyle[0]);

    if(!r_y_h_empty)
    {
      c20.cd(i+1);
      gPad->SetFillStyle(4000);
      if(R_y_h[i])
      {
        R_y_h[i]->Draw("SAMEPA");
        R_y_h[i]->GetXaxis()->SetLimits(0.1,0.9);
        R_y_h[i]->SetMinimum(0.);
        R_y_h[i]->SetMaximum(2.);
        R_y_h[i]->GetXaxis()->SetLabelSize(0.06);
        R_y_h[i]->GetYaxis()->SetLabelSize(0.06);
        R_y_h[i]->SetTitle("");
        if(i>4) gPad->SetBottomMargin(.15);
        if(i==0 || i==5) gPad->SetLeftMargin(.22);
        if(i==8)
        {
          R_y_h[i]->GetXaxis()->SetTitle("#font[12]{z}");
          R_y_h[i]->GetXaxis()->SetTitleSize(0.08);
          R_y_h[i]->GetXaxis()->SetTitleOffset(.8);
        }
        R_y_h[i]->GetXaxis()->SetNdivisions(304,kTRUE);
        R_y_h[i]->GetYaxis()->SetNdivisions(304,kTRUE);
        if(i==0)
        {
          R_y_h[i]->GetYaxis()->SetTitle("#font[12]{#eta}^{#font[12]{h}}");
          R_y_h[i]->GetYaxis()->SetTitleSize(0.08);
        }
        R_y_h[i]->Draw("SAMEP");
        R_y_h[i]->GetXaxis()->SetLimits(0.1,0.9);
        R_y_h[i]->SetMinimum(0.);
        R_y_h[i]->SetMaximum(2.);
        c20.Range(0.1,0.,0.9,2.);
      }
      c20.Update();
    }
  }

  TLatex fTitle;

  c10.cd(1);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.59,"0.004#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.01");

  c10.cd(2);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.59,"0.01#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.02");

  c10.cd(3);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.59,"0.02#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.03");

  c10.cd(4);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.59,"0.03#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.04");

  c10.cd(5);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.59,"0.04#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.06");

  c10.cd(6);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.59,"0.06#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.1");

  c10.cd(7);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.59,"0.1#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.14");

  c10.cd(8);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.59,"0.14#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.18");

  c10.cd(9);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.59,"0.18#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.4");

  c10.cd(10);
  fTitle.SetTextSize(0.095);
  fTitle.SetTextAlign(11);
  fTitle.DrawLatex(0.05, 0.64,"#color[4]{0.50#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.70, #delta = 0.4}");
  fTitle.DrawLatex(0.05, 0.56,"#color[226]{0.30#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.50, #delta = 0.3}");
  fTitle.DrawLatex(0.05, 0.48,"#color[209]{0.20#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.30, #delta = 0.2}");
  fTitle.DrawLatex(0.05, 0.40,"#color[95]{0.15#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.20, #delta = 0.1}");
  fTitle.DrawLatex(0.05, 0.32,"#color[2]{0.10#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.15, #delta = 0}");

  c10.Update();
  c20.Update();

  c10.Print("RC_plot/RC_plot.pdf");
  c20.Print("RC_plot/RC_plot_yavg.pdf");

  ofs_rc.close();

  return 0;
}
