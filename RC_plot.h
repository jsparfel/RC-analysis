#ifndef __RC_PLOT_H_INCLUDED__
#define __RC_PLOT_H_INCLUDED__

#include <vector>
#include <set>
#include <map>
#include <utility>
#include <TLatex.h>
#include <TLegend.h>

using namespace std;

struct Multiplicities { Double_t tab[3]; };

Multiplicities fMultiplicities1[9][6][12];
Multiplicities fMultiplicities2[9][6][12];
Multiplicities fMultiplicities1_yavg[9][12];
Multiplicities fMultiplicities2_yavg[9][12];
Multiplicities fMultiplicities1_pt[9][12][5];
Multiplicities fMultiplicities2_pt[9][12][5];

double z_range[12] = {.225,.275,.325,.375,.425,.475,.525,.575,.625,.675,.725,.8};

//Graphic Style

int fMarkerColor[6] = {2,95,209,226,4,221};
int fMarkerColorpt[5] = {2,95,209,226,4};
int fMarkerStyle[6] = {20,26,25,27,28,30};

#endif
