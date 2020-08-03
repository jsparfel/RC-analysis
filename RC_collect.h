#ifndef __CUTMAKER_COLLECT_H_INCLUDED__
#define __CUTMAKER_COLLECT_H_INCLUDED__

#include <vector>
#include <set>
#include <map>
#include <utility>
#include <TLatex.h>
#include <TLegend.h>

#define RST  "\x1B[0m"
#define KCYN  "\x1B[36m"
#define FCYN(x) KCYN x RST


using namespace std;

struct Wrapper { Double_t tab[2]; };
struct Multiplicities { Double_t tab[3]; };
struct Recovery_tank { Double_t tab[4]; };

//Binning
Wrapper fBinning[9][6][12];
Multiplicities fMultiplicities[9][6][12];
Multiplicities fMultiplicities_yavg[9][12];
Recovery_tank fMeanvalues_size[9][6][12];
Recovery_tank fMeanvalues_data[9][6][12];
Recovery_tank fMeanvalues_yavg[9][12];
Recovery_tank fMeanvalues_temp[9][6][12];
Double_t fNDIS_evt[9][6][12];
Double_t fNDIS_evt_yavg[9][12];
Double_t fNDIS_evt_err[9][6][12];
Double_t fNDIS_evt_err_yavg[9][12];
Double_t fZrange[13] = {.20,.25,.30,.35,.40,.45,.50,.55,.60,.65,.70,.75,.85};
Double_t fXrange[10] = {.004,.01,.02,.03,.04,.06,.1,.14,.18,.4};
Double_t fYrange[7] = {.1,.15,.2,.3,.5,.7,.9};
Double_t fZ_bin_width[12] = {.05,.05,.05,.05,.05,.05,.05,.05,.05,.05,.05,.1};

//Graphic Style

int fMarkerColor[6] = {2,95,209,226,4,221};
int fMarkerStyle[6] = {20,26,25,27,28,30};

#endif
