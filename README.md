# Radiative Corrections Analysis

## Summary
 1. [Building](#Building)
 2. [Flow](#Flow)
 3. [RC_split](#RC-split)
 4. [RC_collect](#RC-collect)
 5. [RC_plot](#RC-plot)

## Building

 MAKEFILE PHONY TARGETS:
  - `make`: builds all targets
  - `make RC`: builds the RC analysis framework.
  - `make clean`

## Flow

**Radiative corrections Calculation** _[RC_split --> RC_collect --> RC_plot]_

## [C++] RC_split<a name="RC-split" />

**Description:**
Takes the TTree and does the cut of the analysis. Outputs DIS Event and Hadron counts. `RC_split` should be executed a first time on non-radiative MC data and a second time on radiative MC data.

**Requires:**
 - **ROOT TTree from Djangoh with or without radiative events** 

**User Dependence in file (see beginning of `RC_split.cc`):**
 - **data_path to MC Data: e.g. `#define data_path "/sps/compass/julien/CahnNOrc"`**

**In File Flags (see beginning of `RC_split.cc`):**
 - **X Boundaries**
 - **Y Boundaries**

**Directories/files to be created before execution:**
 - **`[data_path]/` is the directory containing .root MC files and `[data_path]` is defined in `RC_split.cc` (see above)**
 - **`[data_path]/filelist.txt` where `filelist.txt` is a text file containing the list of the names of .root files to be treated** 

**Call:**
```Bash
[exe_path]/RC_split [OPTIONAL FLAGS]
```
where `[exe_path]` is a directory containing the executable `RC_split`.

`[OPTIONAL FLAGS]`
 - `-k` : draw kinematic plots
 - `-q` : less verbosity

**Outputs:**
 - **Count files in `./` : the directory from which the execution of RC_split is launched**
 - **If option `-k` is passed, kinematic plots `kinSIDIS.pdf` and `kinSIDIS.root` in `./`**

## [C++] RC_collect<a name="RC-collect" />

**Description:**
Takes the output of `RC_split`, computes the Multiplicities and stores/plots them. `RC_collect` should be executed a first time with input from the first execution of `RC_split` and a second time with input from the second execution of `RC_split` (see above).

**Requires:**
 - **Output of `RC_split` in `./`: the directory from which `RC_collect` is launched**

**In File Flags**
 - **Y Integration or mean [Mean/Weighted Mean/Integration | 1/2/3] e.g. `#define YMULT 2` (my advice is to use 2)**
 - **Staggered Multiplicities in plot [YES/NO | 1/0] e.g. `#define STAGGERED 1` (if set to 0 multiplicities in different bins of y will not be staggered and hence may not be distinguishable)**
 - **Data path in which output files will be created: e.g. `#define data_path "./Multiplicities"`**

**Directories/files to be created before execution:**
 - **Output files of `RC_split` (`DIS.txt` and `hadron.txt`) should be in `./`**
 - **`[data_path]/` is the directory where output files will be written**

**Call:**
 ```Bash
 [exe_path]/RC_collect
 ```
where `[exe_path]` is a directory containing the executable `RC_collect`.

**Outputs:**
 - **Multiplicity text files in `[data_path]/`:**
  - **`multiplicities_hadron.txt`**
  - **`multiplicities_hadron_yavg.txt`**
  - **`multiplicities_raw.txt`**
 - **Multiplicity plots in `[data_path]/`:**
  - **`hadron_multiplicity_file.pdf` plots multiplicities in bins of x,y,z**
  - **`hadron_multiplicity_yavg_file.pdf` plots y-averaged multiplicities**
  - **`hadron_multiplicity_sum_file.pdf` plots y-averaged and z-integrated multiplicities sum**

## [C++] RC_plot<a name="RC_plot" />

**Description:**
Takes multiplicities results of RC_collect for radiative and non-radiative MC data and calculate the ratio. This ratio corresponds to the radiative correction factor for multiplicities in bins of (x,y,z).

**Requires:**
 - **Outputs of 2 different executions of `RC_collect` in different directories**

**Directories/files to be created before execution:**
 - **`./RC_plot/` is the directory into which output files will be created**

**Call:**
```Bash
[exe_path]/RC_plot [MULT_DIRECTORY_1] [MULT_DIRECTORY_2]
```
where `[exe_path]` is a directory containing the executable `RC_plot` and `[MULT_DIRECTORY_1]`(`[MULT_DIRECTORY_2]`) is the directory where the first(the second) multiplicities files are stored (e.g. `CahnNOrc/Multiplicities`).

**Outputs:**
 - **Files in `./RC_plot/`:**
  - **`RC_plot.pdf`: radiative corrections in bins of x,y,z**
  - **`RC_plot_yavg.pdf`: y-averaged radiative corrections in bins of x,z**
  - **`rad_corr.txt`: y-averaged radiative corrections**
