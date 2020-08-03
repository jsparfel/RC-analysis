#ifndef __CUTMAKER_SPLIT_H_INCLUDED__
#define __CUTMAKER_SPLIT_H_INCLUDED__

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
#include <TGaxis.h>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <math.h>
#include <TColor.h>
#include <TROOT.h>
#include <TStyle.h>

// COLORS

#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

using namespace std;

struct Wrapper { Float_t tab[2]; };
struct Recovery { vector<Float_t> vec[4]; };
struct Recovery_tank { Float_t tab[4]; };
struct Pvsz { vector<Float_t> vec; };

//Hadrons
vector<Float_t> fQ2;
vector<Float_t> fQ2kin;
vector<Float_t> fQ2local;
vector<Float_t> fXBj;
vector<Float_t> fXBjkin;
vector<Float_t> fYBj;
vector<Float_t> fYBjkin;
vector<Float_t> fWBj;
vector<Float_t> fWBjkin;
vector<Float_t> fNu;
vector<Float_t> fNukin;

//Counting
Double_t fQ2test = 0;
Double_t fYBjtest = 0;
Double_t fXBjtest = 0;
Double_t fWBjtest = 0;
Double_t fHadrons = 0;
Double_t fZtest = 0;
Double_t fFilesNumber = 0;

//Binning
Wrapper fBinning[9][6][12];
Recovery fMeanvalues[9][6][12]; // tab[][][i], iC[0,3] : x,y,Q2,z
Recovery_tank fMeanvalues_size[9][6][12];
Recovery_tank fMeanvalues_data[9][6][12];
Float_t fNDIS_evt[9][6][12];
Float_t fNDIS_evt_err[9][6][12];
Int_t xbin, ybin, zbin;
Float_t fYrange[7] = {.1,.15,.2,.3,.5,.7,.9};

//Draw
TH1F* fKinematics[6];
TH1F* fKinematicsZ[6];
TH2F* fKinematics2D[2];
TH1F* fQ2k[2];
TH1F* fYk[2];
TH1F* fPk[2];
TH1F* fZk[2];
TCanvas c1("Kin_Q^{2}","Kin_Q^{2}");
TCanvas c2("Kin_x^{Bj}","Kin_x^{Bj}");
TCanvas c3("Kin_y","Kin_y");
TCanvas c4("Kin_z","Kin_z");
TCanvas c5("Kin_w","Kin_w");
TCanvas c6("Kin_#nu","Kin_#nu");
TCanvas c7("Kin_xy","Kin_xy");
TCanvas c71("Kin_xQ2","Kin_xQ2");
TCanvas c8("Kin_z","Kin_z");

#endif
