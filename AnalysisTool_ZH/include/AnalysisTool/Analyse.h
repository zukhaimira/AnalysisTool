#ifndef ACANALYSE
#define ACANALYSE
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TMatrixFSym.h>
#include <TStopwatch.h>
#include <TRandom3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>

#include "DataForm.h"
#include "rochester/rochcor2012v2.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <sys/types.h>
#include <dirent.h>

using namespace std;
using namespace TMath;

const UInt_t M_trackmaxcount = 500;
const UInt_t M_superclustermaxcount = 1000;
const UInt_t M_superclustermembermaxcount = 1000;
const UInt_t M_superclusterhitmaxcount = 5000;
const UInt_t M_primvertexmaxcount = 500;
const UInt_t M_muonmaxcount = 500;
const UInt_t M_taumaxcount = 500;
const UInt_t M_electronmaxcount = 500;
const UInt_t M_photonmaxcount = 500;
const UInt_t M_conversionmaxcount = 500;
const UInt_t M_jetmaxcount = 500;
const UInt_t M_musecverticesmaxcount = 500;
const UInt_t M_secverticesmaxcount = 1000;
const UInt_t M_genallparticlesmaxcount = 10000;
const UInt_t M_genparticlesmaxcount = 500;
const UInt_t M_genjetmaxcount = 500;
const UInt_t M_genmotherdaughtermaxcount = 100000;

bool sortac1b(string a, string b);
bool sortroot(string a, string b);

class Analyse
{
	friend class GenParticle;
	friend class Tau;
	friend class Muon;
	friend class Electron;
	friend class Jet;
	friend class Photon;
	friend class TriggerSelection;
	private:
	vector<string> filenames;
	vector<Long64_t> filelimits;
	Long64_t currentmin, currentmax;
	TFile* currentfile;
	Int_t currentloadtype;
	TTree* tree;
	string treename;
	Int_t printinfo;
	Long64_t processed;
	Int_t loadbeamspot;
	Int_t loadsecvertices;
	Int_t loadmuons;
	Int_t loadelectrons;
	Int_t loadphotons;
	Int_t loadtaus;
	Int_t loadmet;
	Int_t loadak4calojets;
	Int_t loadak4jptjets;
	Int_t loadak4pfchsjets;
	Int_t loadak4pfjets;
	Int_t loadtracks;
	Int_t loadsuperclusters;
	Int_t loadprimvertices;
	Int_t loadtrigger;
	Int_t loadgeninfo;
	Int_t loadgenparticles;
	Int_t loadgenak4jets;
	Int_t loadallgenparticles;
	void SetLoad();
	void Load();
	void GetEvent(Long64_t num, Int_t loadtype = 0);

	//dupcheck
	bool duplicatecheck;
	map< UInt_t, map< UInt_t, map< Double_t, UInt_t> > > eventlist;

	//Skimming
	TTree* skimtree;
	map< UInt_t, map< UInt_t, UInt_t > > selected;
	string skimfilename;
	TFile* skimfile;

	//Lumi calculation
	map< UInt_t, map< UInt_t, Luminosity > > lumilist;
	map< UInt_t, RunInfo> runlist;
	map<string, TriggerSelection*> triggerselections;
	bool lumicalculation;
	bool IsInRange(UInt_t Run, UInt_t LumiBlock);
	UInt_t minRun;
	UInt_t minLumi;
	UInt_t maxRun;
	UInt_t maxLumi;
	//JSON filter
	bool jsonfilter;
	map< UInt_t, map< UInt_t, bool > > jsonlist;

	//MPI
	Int_t batch_myid; 
	Int_t batch_numjobs; 
	bool batch_emptyjob;
	map< UInt_t, map< UInt_t, bool > > batchselection;
	bool IsBatchSelected(UInt_t run, UInt_t lumiblock);

	//Pileup
	bool usepileupinfo;
	double pileupscale;
	vector<Double_t> pileUpDistMinus;
	vector<Double_t> pileUpDist;
	vector<Double_t> pileUpDistPlus;
	bool useprimvertexinfo;
	vector<Double_t> primVertexDist;

	//Rocherster Corrections
	rochcor2012* rochester2012;

	TRandom3* analysisrandom;

	//Data		
	UInt_t errors;
	Double_t event_nr;
	UInt_t event_luminosityblock;
	UInt_t event_run;
	UInt_t event_timeunix;
	UInt_t event_timemicrosec;
	UChar_t trigger_level1bits[8];
	UChar_t trigger_level1[128];
	UChar_t trigger_HLT[128];

	Float_t beamspot_x;
	Float_t beamspot_y;
	Float_t beamspot_z;
	Float_t beamspot_xwidth;
	Float_t beamspot_ywidth;
	Float_t beamspot_zsigma;
	Float_t beamspot_cov[6];

	UInt_t track_count;
	Int_t track_vtx[M_trackmaxcount];
	Float_t track_px[M_trackmaxcount];
	Float_t track_py[M_trackmaxcount];
	Float_t track_pz[M_trackmaxcount];
	Float_t track_outerx[M_trackmaxcount];
	Float_t track_outery[M_trackmaxcount];
	Float_t track_outerz[M_trackmaxcount];
	Float_t track_closestpointx[M_trackmaxcount];
	Float_t track_closestpointy[M_trackmaxcount];
	Float_t track_closestpointz[M_trackmaxcount];
	Float_t track_chi2[M_trackmaxcount];
	Float_t track_ndof[M_trackmaxcount];
	Float_t track_dxy[M_trackmaxcount];
	Float_t track_dxyerr[M_trackmaxcount];
	Float_t track_dz[M_trackmaxcount];
	Float_t track_dzerr[M_trackmaxcount];
	Float_t track_dedxharmonic2[M_trackmaxcount];
	Int_t track_charge[M_trackmaxcount];
	UChar_t track_nhits[M_trackmaxcount];
	UChar_t track_nmissinghits[M_trackmaxcount];
	UChar_t track_npixelhits[M_trackmaxcount];
	UChar_t track_npixellayers[M_trackmaxcount];
	UChar_t track_nstriplayers[M_trackmaxcount];

	UInt_t primvertex_count;
	Float_t primvertex_x[M_primvertexmaxcount];
	Float_t primvertex_y[M_primvertexmaxcount];
	Float_t primvertex_z[M_primvertexmaxcount];
	UInt_t primvertex_info[M_primvertexmaxcount];
	Float_t primvertex_chi2[M_primvertexmaxcount];
	Float_t primvertex_ndof[M_primvertexmaxcount];
	Float_t primvertex_ptq[M_primvertexmaxcount];
	Int_t primvertex_ntracks[M_primvertexmaxcount];
	Float_t primvertex_cov[M_primvertexmaxcount][6];

	UInt_t supercluster_count;
	Float_t supercluster_e[M_superclustermaxcount];
	Float_t supercluster_x[M_superclustermaxcount];
	Float_t supercluster_y[M_superclustermaxcount];
	Float_t supercluster_z[M_superclustermaxcount];
	Float_t supercluster_rawe[M_superclustermaxcount];
	Float_t supercluster_phiwidth[M_superclustermaxcount];
	Float_t supercluster_etawidth[M_superclustermaxcount];
	Int_t supercluster_nbasiccluster[M_superclustermaxcount];
	Int_t supercluster_basicclusterbegin[M_superclustermaxcount];
	Int_t supercluster_esclusterbegin[M_superclustermaxcount];

	UInt_t supercluster_basiccluster_count;
	Float_t supercluster_basiccluster_e[M_superclustermembermaxcount];
	Float_t supercluster_basiccluster_x[M_superclustermembermaxcount];
	Float_t supercluster_basiccluster_y[M_superclustermembermaxcount];
	Float_t supercluster_basiccluster_z[M_superclustermembermaxcount];
	Int_t supercluster_basiccluster_nhit[M_superclustermembermaxcount];
	Int_t supercluster_basiccluster_hitbegin[M_superclustermembermaxcount];

	UInt_t supercluster_basiccluster_hit_count;
	Float_t supercluster_basiccluster_hit_e[M_superclusterhitmaxcount];
	Float_t supercluster_basiccluster_hit_x[M_superclusterhitmaxcount];
	Float_t supercluster_basiccluster_hit_y[M_superclusterhitmaxcount];
	Float_t supercluster_basiccluster_hit_z[M_superclusterhitmaxcount];

	UInt_t supercluster_escluster_count;
	Float_t supercluster_escluster_e[M_superclustermembermaxcount];
	Float_t supercluster_escluster_x[M_superclustermembermaxcount];
	Float_t supercluster_escluster_y[M_superclustermembermaxcount];
	Float_t supercluster_escluster_z[M_superclustermembermaxcount];
	Int_t supercluster_escluster_nhit[M_superclustermembermaxcount];
	Int_t supercluster_escluster_hitbegin[M_superclustermembermaxcount];

	UInt_t supercluster_escluster_hit_count;
	Float_t supercluster_escluster_hit_e[M_superclusterhitmaxcount];
	Float_t supercluster_escluster_hit_x[M_superclusterhitmaxcount];
	Float_t supercluster_escluster_hit_y[M_superclusterhitmaxcount];
	Float_t supercluster_escluster_hit_z[M_superclusterhitmaxcount];

	UInt_t muon_count;
	Float_t muon_px[M_muonmaxcount];
	Float_t muon_py[M_muonmaxcount];
	Float_t muon_pz[M_muonmaxcount];
	Float_t muon_pterror[M_muonmaxcount];
	Float_t muon_chi2[M_muonmaxcount];
	Float_t muon_ndof[M_muonmaxcount];
	Int_t muon_innertrack_vtx[M_muonmaxcount];
	Float_t muon_innertrack_px[M_muonmaxcount];
	Float_t muon_innertrack_py[M_muonmaxcount];
	Float_t muon_innertrack_pz[M_muonmaxcount];
	Float_t muon_innertrack_outerx[M_muonmaxcount];
	Float_t muon_innertrack_outery[M_muonmaxcount];
	Float_t muon_innertrack_outerz[M_muonmaxcount];
	Float_t muon_innertrack_closestpointx[M_muonmaxcount];
	Float_t muon_innertrack_closestpointy[M_muonmaxcount];
	Float_t muon_innertrack_closestpointz[M_muonmaxcount];
	Float_t muon_innertrack_chi2[M_muonmaxcount];
	Float_t muon_innertrack_ndof[M_muonmaxcount];
	Float_t muon_innertrack_dxy[M_muonmaxcount];
	Float_t muon_innertrack_dxyerr[M_muonmaxcount];
	Float_t muon_innertrack_dz[M_muonmaxcount];
	Float_t muon_innertrack_dzerr[M_muonmaxcount];
	Float_t muon_innertrack_dedxharmonic2[M_muonmaxcount];
	Int_t muon_innertrack_charge[M_muonmaxcount];
	UChar_t muon_innertrack_nhits[M_muonmaxcount];
	UChar_t muon_innertrack_nmissinghits[M_muonmaxcount];
	UChar_t muon_innertrack_npixelhits[M_muonmaxcount];
	UChar_t muon_innertrack_npixellayers[M_muonmaxcount];
	UChar_t muon_innertrack_nstriplayers[M_muonmaxcount];
	Float_t muon_outertrack_px[M_muonmaxcount];
	Float_t muon_outertrack_py[M_muonmaxcount];
	Float_t muon_outertrack_pz[M_muonmaxcount];
	UChar_t muon_outertrack_hits[M_muonmaxcount];
	UChar_t muon_outertrack_missinghits[M_muonmaxcount];
	Float_t muon_outertrack_chi2[M_muonmaxcount];
	Float_t muon_outertrack_ndof[M_muonmaxcount];
	Float_t muon_isolationr3track[M_muonmaxcount];
	Int_t muon_isolationr3ntrack[M_muonmaxcount];
	Float_t muon_isolationr3ecal[M_muonmaxcount];
	Float_t muon_isolationr3hcal[M_muonmaxcount];
	Float_t muon_ecalenergy[M_muonmaxcount];
	Float_t muon_hcalenergy[M_muonmaxcount];
	Float_t muon_pfisolationr3_sumchargedhadronpt[M_muonmaxcount];
	Float_t muon_pfisolationr3_sumchargedparticlept[M_muonmaxcount];
	Float_t muon_pfisolationr3_sumneutralhadronet[M_muonmaxcount];
	Float_t muon_pfisolationr3_sumphotonet[M_muonmaxcount];
	Float_t muon_pfisolationr3_sumneutralhadronethighthreshold[M_muonmaxcount];
	Float_t muon_pfisolationr3_sumphotonethighthreshold[M_muonmaxcount];
	Float_t muon_pfisolationr3_sumpupt[M_muonmaxcount];
	Float_t muon_pfisolationr4_sumchargedhadronpt[M_muonmaxcount];
	Float_t muon_pfisolationr4_sumchargedparticlept[M_muonmaxcount];
	Float_t muon_pfisolationr4_sumneutralhadronet[M_muonmaxcount];
	Float_t muon_pfisolationr4_sumphotonet[M_muonmaxcount];
	Float_t muon_pfisolationr4_sumneutralhadronethighthreshold[M_muonmaxcount];
	Float_t muon_pfisolationr4_sumphotonethighthreshold[M_muonmaxcount];
	Float_t muon_pfisolationr4_sumpupt[M_muonmaxcount];
	Int_t muon_charge[M_muonmaxcount];
	Int_t muon_numchambers[M_muonmaxcount];
	Int_t muon_numchamberswithsegments[M_muonmaxcount];
	Int_t muon_numvalidmuonhits[M_muonmaxcount];
	Int_t muon_nummatchedstations[M_muonmaxcount];
	UChar_t muon_type[M_muonmaxcount];
	UInt_t muon_trigger[M_muonmaxcount];
	UInt_t muon_trackermuonquality[M_muonmaxcount];

	UInt_t ak4calojet_count;
	Float_t ak4calojet_e[M_jetmaxcount];
	Float_t ak4calojet_px[M_jetmaxcount];
	Float_t ak4calojet_py[M_jetmaxcount];
	Float_t ak4calojet_pz[M_jetmaxcount];
	Float_t ak4calojet_hadronicenergy[M_jetmaxcount];
	Float_t ak4calojet_emenergy[M_jetmaxcount];
	Float_t ak4calojet_energycorr[M_jetmaxcount];
	Float_t ak4calojet_energycorrl7uds[M_jetmaxcount];
	Float_t ak4calojet_energycorrl7bottom[M_jetmaxcount];
	Float_t ak4calojet_fhpd[M_jetmaxcount];
	Float_t ak4calojet_restrictedemf[M_jetmaxcount];
	Float_t ak4calojet_btag[M_jetmaxcount][M_btagmax];
	UInt_t ak4calojet_n90[M_jetmaxcount];
	UInt_t ak4calojet_n60[M_jetmaxcount];

	UInt_t ak4jptjet_count;
	Float_t ak4jptjet_e[M_jetmaxcount];
	Float_t ak4jptjet_px[M_jetmaxcount];
	Float_t ak4jptjet_py[M_jetmaxcount];
	Float_t ak4jptjet_pz[M_jetmaxcount];
	Float_t ak4jptjet_hadronicenergy[M_jetmaxcount];
	Float_t ak4jptjet_chargedhadronicenergy[M_jetmaxcount];
	Float_t ak4jptjet_emenergy[M_jetmaxcount];
	Float_t ak4jptjet_chargedemenergy[M_jetmaxcount];
	UInt_t ak4jptjet_chargedmulti[M_jetmaxcount];	
	Float_t ak4jptjet_energycorr[M_jetmaxcount];
	Float_t ak4jptjet_energycorrl7uds[M_jetmaxcount];
	Float_t ak4jptjet_energycorrl7bottom[M_jetmaxcount];
	Float_t ak4jptjet_fhpd[M_jetmaxcount];
	Float_t ak4jptjet_restrictedemf[M_jetmaxcount];
	Float_t ak4jptjet_btag[M_jetmaxcount][M_btagmax];
	UInt_t ak4jptjet_n90[M_jetmaxcount];

	UInt_t ak4pfjet_count;
	Float_t ak4pfjet_e[M_jetmaxcount];
	Float_t ak4pfjet_px[M_jetmaxcount];
	Float_t ak4pfjet_py[M_jetmaxcount];
	Float_t ak4pfjet_pz[M_jetmaxcount];
	Float_t ak4pfjet_area[M_jetmaxcount];
	Float_t ak4pfjet_hadronicenergy[M_jetmaxcount];
	Float_t ak4pfjet_chargedhadronicenergy[M_jetmaxcount];
	Float_t ak4pfjet_emenergy[M_jetmaxcount];
	Float_t ak4pfjet_chargedemenergy[M_jetmaxcount];
	Float_t ak4pfjet_hfemenergy[M_jetmaxcount];
	Float_t ak4pfjet_hfhadronicenergy[M_jetmaxcount];
	Float_t ak4pfjet_electronenergy[M_jetmaxcount];
	Float_t ak4pfjet_muonenergy[M_jetmaxcount];
	UInt_t ak4pfjet_chargedmulti[M_jetmaxcount];	
	UInt_t ak4pfjet_neutralmulti[M_jetmaxcount];	
	UInt_t ak4pfjet_hfhadronicmulti[M_jetmaxcount];	
	UInt_t ak4pfjet_hfemmulti[M_jetmaxcount];	
	UInt_t ak4pfjet_electronmulti[M_jetmaxcount];	
	UInt_t ak4pfjet_muonmulti[M_jetmaxcount];	
	Float_t ak4pfjet_chargeda[M_jetmaxcount];
	Float_t ak4pfjet_chargedb[M_jetmaxcount];
	Float_t ak4pfjet_neutrala[M_jetmaxcount];
	Float_t ak4pfjet_neutralb[M_jetmaxcount];
	Float_t ak4pfjet_alla[M_jetmaxcount];
	Float_t ak4pfjet_allb[M_jetmaxcount];
	Float_t ak4pfjet_chargedfractionmv[M_jetmaxcount];
	Float_t ak4pfjet_energycorr[M_jetmaxcount];
	Float_t ak4pfjet_energycorrunc[M_jetmaxcount];
	Float_t ak4pfjet_energycorrl7uds[M_jetmaxcount];
	Float_t ak4pfjet_energycorrl7bottom[M_jetmaxcount];
	Float_t ak4pfjet_puidfull[M_jetmaxcount];
	Float_t ak4pfjet_puidsimple[M_jetmaxcount];
	Float_t ak4pfjet_puidcutbased[M_jetmaxcount];
	Float_t ak4pfjet_btag[M_jetmaxcount][M_btagmax];
	UInt_t ak4pfjet_trigger[M_jetmaxcount];
	Int_t ak4pfjet_mcflavour[M_jetmaxcount];

	UInt_t ak4pfchsjet_count;
	Float_t ak4pfchsjet_e[M_jetmaxcount];
	Float_t ak4pfchsjet_px[M_jetmaxcount];
	Float_t ak4pfchsjet_py[M_jetmaxcount];
	Float_t ak4pfchsjet_pz[M_jetmaxcount];
	Float_t ak4pfchsjet_area[M_jetmaxcount];
	Float_t ak4pfchsjet_hadronicenergy[M_jetmaxcount];
	Float_t ak4pfchsjet_chargedhadronicenergy[M_jetmaxcount];
	Float_t ak4pfchsjet_emenergy[M_jetmaxcount];
	Float_t ak4pfchsjet_chargedemenergy[M_jetmaxcount];
	Float_t ak4pfchsjet_hfemenergy[M_jetmaxcount];
	Float_t ak4pfchsjet_hfhadronicenergy[M_jetmaxcount];
	Float_t ak4pfchsjet_electronenergy[M_jetmaxcount];
	Float_t ak4pfchsjet_muonenergy[M_jetmaxcount];
	UInt_t ak4pfchsjet_chargedmulti[M_jetmaxcount];	
	UInt_t ak4pfchsjet_neutralmulti[M_jetmaxcount];	
	UInt_t ak4pfchsjet_hfhadronicmulti[M_jetmaxcount];	
	UInt_t ak4pfchsjet_hfemmulti[M_jetmaxcount];	
	UInt_t ak4pfchsjet_electronmulti[M_jetmaxcount];	
	UInt_t ak4pfchsjet_muonmulti[M_jetmaxcount];	
	Float_t ak4pfchsjet_chargeda[M_jetmaxcount];
	Float_t ak4pfchsjet_chargedb[M_jetmaxcount];
	Float_t ak4pfchsjet_neutrala[M_jetmaxcount];
	Float_t ak4pfchsjet_neutralb[M_jetmaxcount];
	Float_t ak4pfchsjet_alla[M_jetmaxcount];
	Float_t ak4pfchsjet_allb[M_jetmaxcount];
	Float_t ak4pfchsjet_chargedfractionmv[M_jetmaxcount];
	Float_t ak4pfchsjet_energycorr[M_jetmaxcount];
	Float_t ak4pfchsjet_energycorrunc[M_jetmaxcount];
	Float_t ak4pfchsjet_energycorrl7uds[M_jetmaxcount];
	Float_t ak4pfchsjet_energycorrl7bottom[M_jetmaxcount];
	Float_t ak4pfchsjet_btag[M_jetmaxcount][M_btagmax];
	UInt_t ak4pfchsjet_trigger[M_jetmaxcount];
	Int_t ak4pfchsjet_mcflavour[M_jetmaxcount];

	UInt_t electron_count;
	Int_t electron_vtx[M_electronmaxcount];
	Float_t electron_px[M_electronmaxcount];
	Float_t electron_py[M_electronmaxcount];
	Float_t electron_pz[M_electronmaxcount];
	Float_t electron_correctedecalenergy[M_electronmaxcount];
	Float_t electron_trackchi2[M_electronmaxcount];
	Float_t electron_trackndof[M_electronmaxcount];
	Float_t electron_outerx[M_electronmaxcount];
	Float_t electron_outery[M_electronmaxcount];
	Float_t electron_outerz[M_electronmaxcount];
	Float_t electron_closestpointx[M_electronmaxcount];
	Float_t electron_closestpointy[M_electronmaxcount];
	Float_t electron_closestpointz[M_electronmaxcount];
	Float_t electron_esuperclusterovertrack[M_electronmaxcount];
	Float_t electron_eseedclusterovertrack[M_electronmaxcount];
	Float_t electron_deltaetasuperclustertrack[M_electronmaxcount];
	Float_t electron_deltaphisuperclustertrack[M_electronmaxcount];
	Float_t electron_e1x5[M_electronmaxcount];
	Float_t electron_e2x5[M_electronmaxcount];
	Float_t electron_e5x5[M_electronmaxcount];
	Float_t electron_r9[M_electronmaxcount];
	Float_t electron_sigmaetaeta[M_electronmaxcount];
	Float_t electron_sigmaietaieta[M_electronmaxcount];
	Float_t electron_sigmaiphiiphi[M_electronmaxcount];
	Float_t electron_ehcaloverecaldepth1[M_electronmaxcount];
	Float_t electron_ehcaloverecaldepth2[M_electronmaxcount];
	Float_t electron_ehcaltoweroverecaldepth1[M_electronmaxcount];
	Float_t electron_ehcaltoweroverecaldepth2[M_electronmaxcount];
	Float_t electron_isolationr3track[M_electronmaxcount];
	Float_t electron_isolationr3ecal[M_electronmaxcount];
	Float_t electron_isolationr3hcal[M_electronmaxcount];
	Float_t electron_isolationr4track[M_electronmaxcount];
	Float_t electron_isolationr4ecal[M_electronmaxcount];
	Float_t electron_isolationr4hcal[M_electronmaxcount];
	Float_t electron_isolationpfr3charged[M_electronmaxcount];
	Float_t electron_isolationpfr3photon[M_electronmaxcount];
	Float_t electron_isolationpfr3neutral[M_electronmaxcount];
	Int_t electron_charge[M_electronmaxcount];
	UChar_t electron_nhits[M_electronmaxcount];
	UChar_t electron_nmissinghits[M_electronmaxcount];
	UChar_t electron_npixelhits[M_electronmaxcount];
	UChar_t electron_npixellayers[M_electronmaxcount];
	UChar_t electron_nstriplayers[M_electronmaxcount];
	UChar_t electron_nhitsexpected[M_electronmaxcount];
	Float_t electron_dxy[M_electronmaxcount];
	Float_t electron_dxyerr[M_electronmaxcount];
	Float_t electron_dz[M_electronmaxcount];
	Float_t electron_dzerr[M_electronmaxcount];
	Float_t electron_convdist[M_electronmaxcount];
	Float_t electron_convdcot[M_electronmaxcount];
	Float_t electron_convradius[M_electronmaxcount];
	UInt_t electron_gapinfo[M_electronmaxcount];
	Float_t electron_fbrems[M_electronmaxcount];
	Int_t electron_numbrems[M_electronmaxcount];
	UChar_t electron_info[M_electronmaxcount];
	UInt_t electron_trigger[M_electronmaxcount];
	UChar_t electron_eID[M_electronmaxcount];
	Float_t electron_supercluster_e[M_electronmaxcount];
        Float_t electron_supercluster_x[M_electronmaxcount];
        Float_t electron_supercluster_y[M_electronmaxcount];
        Float_t electron_supercluster_z[M_electronmaxcount];
        Float_t electron_supercluster_rawe[M_electronmaxcount];
        Float_t electron_supercluster_phiwidth[M_electronmaxcount];
        Float_t electron_supercluster_etawidth[M_electronmaxcount];
        Int_t electron_supercluster_nbasiccluster[M_electronmaxcount];

	UInt_t photon_count;
	Float_t photon_px[M_photonmaxcount];
	Float_t photon_py[M_photonmaxcount];
	Float_t photon_pz[M_photonmaxcount];
	Float_t photon_e1x5[M_photonmaxcount];
	Float_t photon_e2x5[M_photonmaxcount];
	Float_t photon_e3x3[M_photonmaxcount];
	Float_t photon_e5x5[M_photonmaxcount];
	Float_t photon_sigmaietaieta[M_photonmaxcount];
	Float_t photon_sigmaietaiphi[M_photonmaxcount];
	Float_t photon_sigmaiphiiphi[M_photonmaxcount];
	Float_t photon_ehcaloverecaldepth1[M_photonmaxcount];
	Float_t photon_ehcaloverecaldepth2[M_photonmaxcount];
	Float_t photon_ehcaltoweroverecaldepth1[M_photonmaxcount];
	Float_t photon_ehcaltoweroverecaldepth2[M_photonmaxcount];
	Float_t photon_maxenergyxtal[M_photonmaxcount];
	Float_t photon_isolationr3track[M_photonmaxcount];
	Float_t photon_isolationr3trackhollow[M_photonmaxcount];
	UInt_t photon_isolationr3ntrack[M_photonmaxcount];
	UInt_t photon_isolationr3ntrackhollow[M_photonmaxcount];
	Float_t photon_isolationr3ecal[M_photonmaxcount];
	Float_t photon_isolationr3hcal[M_photonmaxcount];
	Float_t photon_isolationr4track[M_photonmaxcount];
	Float_t photon_isolationr4trackhollow[M_photonmaxcount];
	UInt_t photon_isolationr4ntrack[M_photonmaxcount];
	UInt_t photon_isolationr4ntrackhollow[M_photonmaxcount];
	Float_t photon_isolationr4ecal[M_photonmaxcount];
	Float_t photon_isolationr4hcal[M_photonmaxcount];
	Float_t photon_isolationpfr3charged[M_photonmaxcount];
	Float_t photon_isolationpfr3photon[M_photonmaxcount];
	Float_t photon_isolationpfr3neutral[M_photonmaxcount];
	Float_t photon_isolationpfr4charged[M_photonmaxcount];
	Float_t photon_isolationpfr4photon[M_photonmaxcount];
	Float_t photon_isolationpfr4neutral[M_photonmaxcount];
	Float_t photon_isolationpfr4noscfootprintcharged[M_photonmaxcount];
	Float_t photon_isolationpfr4noscfootprintphoton[M_photonmaxcount];
	Float_t photon_isolationpfr4noscfootprintneutral[M_photonmaxcount];
	Float_t photon_supercluster_e[M_photonmaxcount];
        Float_t photon_supercluster_x[M_photonmaxcount];
        Float_t photon_supercluster_y[M_photonmaxcount];
        Float_t photon_supercluster_z[M_photonmaxcount];
        Float_t photon_supercluster_rawe[M_photonmaxcount];
        Float_t photon_supercluster_phiwidth[M_photonmaxcount];
        Float_t photon_supercluster_etawidth[M_photonmaxcount];
        Int_t photon_supercluster_nbasiccluster[M_photonmaxcount];
	UChar_t photon_info[M_photonmaxcount];
	UInt_t photon_gapinfo[M_photonmaxcount];
	UInt_t photon_trigger[M_photonmaxcount];
	UInt_t photon_conversionbegin[M_photonmaxcount];

	UInt_t conversion_count;
	UChar_t conversion_info[M_conversionmaxcount];
	Float_t conversion_vx[M_conversionmaxcount];
	Float_t conversion_vy[M_conversionmaxcount];
	Float_t conversion_vz[M_conversionmaxcount];
	Float_t conversion_chi2[M_conversionmaxcount];
	Float_t conversion_ndof[M_conversionmaxcount];
	Float_t conversion_cov[M_conversionmaxcount][6];
	Float_t conversion_mvaout[M_conversionmaxcount];
	Float_t conversion_trackecalpointx[M_conversionmaxcount][2];
	Float_t conversion_trackecalpointy[M_conversionmaxcount][2];
	Float_t conversion_trackecalpointz[M_conversionmaxcount][2];
	Float_t conversion_trackpx[M_conversionmaxcount][2];
	Float_t conversion_trackpy[M_conversionmaxcount][2];
	Float_t conversion_trackpz[M_conversionmaxcount][2];
	Float_t conversion_trackclosestpointx[M_conversionmaxcount][2];
	Float_t conversion_trackclosestpointy[M_conversionmaxcount][2];
	Float_t conversion_trackclosestpointz[M_conversionmaxcount][2];
	Float_t conversion_trackchi2[M_conversionmaxcount][2];
	Float_t conversion_trackndof[M_conversionmaxcount][2];
	Float_t conversion_trackdxy[M_conversionmaxcount][2];
	Float_t conversion_trackdxyerr[M_conversionmaxcount][2];
	Float_t conversion_trackdz[M_conversionmaxcount][2];
	Float_t conversion_trackdzerr[M_conversionmaxcount][2];
	Int_t conversion_trackcharge[M_conversionmaxcount][2];
	UChar_t conversion_tracknhits[M_conversionmaxcount][2];
	UChar_t conversion_tracknmissinghits[M_conversionmaxcount][2];
	UChar_t conversion_tracknpixelhits[M_conversionmaxcount][2];
	UChar_t conversion_tracknpixellayers[M_conversionmaxcount][2];
	UChar_t conversion_tracknstriplayers[M_conversionmaxcount][2];

	UInt_t allconversion_count;
	UChar_t allconversion_info[M_conversionmaxcount];
	Float_t allconversion_vx[M_conversionmaxcount];
	Float_t allconversion_vy[M_conversionmaxcount];
	Float_t allconversion_vz[M_conversionmaxcount];
	Float_t allconversion_chi2[M_conversionmaxcount];
	Float_t allconversion_ndof[M_conversionmaxcount];
	Float_t allconversion_cov[M_conversionmaxcount][6];
	Float_t allconversion_mvaout[M_conversionmaxcount];
	Float_t allconversion_trackecalpointx[M_conversionmaxcount][2];
	Float_t allconversion_trackecalpointy[M_conversionmaxcount][2];
	Float_t allconversion_trackecalpointz[M_conversionmaxcount][2];
	Float_t allconversion_trackpx[M_conversionmaxcount][2];
	Float_t allconversion_trackpy[M_conversionmaxcount][2];
	Float_t allconversion_trackpz[M_conversionmaxcount][2];
	Float_t allconversion_trackclosestpointx[M_conversionmaxcount][2];
	Float_t allconversion_trackclosestpointy[M_conversionmaxcount][2];
	Float_t allconversion_trackclosestpointz[M_conversionmaxcount][2];
	Float_t allconversion_trackchi2[M_conversionmaxcount][2];
	Float_t allconversion_trackndof[M_conversionmaxcount][2];
	Float_t allconversion_trackdxy[M_conversionmaxcount][2];
	Float_t allconversion_trackdxyerr[M_conversionmaxcount][2];
	Float_t allconversion_trackdz[M_conversionmaxcount][2];
	Float_t allconversion_trackdzerr[M_conversionmaxcount][2];
	Int_t allconversion_trackcharge[M_conversionmaxcount][2];
	UChar_t allconversion_tracknhits[M_conversionmaxcount][2];
	UChar_t allconversion_tracknmissinghits[M_conversionmaxcount][2];
	UChar_t allconversion_tracknpixelhits[M_conversionmaxcount][2];
	UChar_t allconversion_tracknpixellayers[M_conversionmaxcount][2];
	UChar_t allconversion_tracknstriplayers[M_conversionmaxcount][2];

	UInt_t tau_count;
	Float_t tau_px[M_taumaxcount];
	Float_t tau_py[M_taumaxcount];
	Float_t tau_pz[M_taumaxcount];
	Float_t tau_isolationneutralspt[M_taumaxcount];
	UInt_t tau_isolationneutralsnum[M_taumaxcount];
	Float_t tau_isolationchargedpt[M_taumaxcount];
	UInt_t tau_isolationchargednum[M_taumaxcount];
	Float_t tau_isolationgammapt[M_taumaxcount];
	UInt_t tau_isolationgammanum[M_taumaxcount];
	Int_t tau_charge[M_taumaxcount];
	UInt_t tau_dishps[M_taumaxcount];
	Float_t tau_emfraction[M_taumaxcount];
	Float_t tau_hcaltotoverplead[M_taumaxcount];
	Float_t tau_hcal3x3overplead[M_taumaxcount];
	Float_t tau_ecalstripsumeoverplead[M_taumaxcount];
	Float_t tau_bremsrecoveryeoverplead[M_taumaxcount];
	Float_t tau_calocomp[M_taumaxcount];
	Float_t tau_segcomp[M_taumaxcount];
	UInt_t tau_trigger[M_taumaxcount];
	Float_t tau_ak4pfjet_e[M_taumaxcount];
	Float_t tau_ak4pfjet_px[M_taumaxcount];
	Float_t tau_ak4pfjet_py[M_taumaxcount];
	Float_t tau_ak4pfjet_pz[M_taumaxcount];
	Float_t tau_ak4pfjet_hadronicenergy[M_taumaxcount];
	Float_t tau_ak4pfjet_chargedhadronicenergy[M_taumaxcount];
	Float_t tau_ak4pfjet_emenergy[M_taumaxcount];
	Float_t tau_ak4pfjet_chargedemenergy[M_taumaxcount];
	UInt_t tau_ak4pfjet_chargedmulti[M_taumaxcount];	
	UInt_t tau_ak4pfjet_neutralmulti[M_taumaxcount];	
	UInt_t tau_ak4pfjet_trigger[M_taumaxcount];
	UInt_t tau_chargedbegin[M_taumaxcount];
	UInt_t tau_charged_count;
	Float_t tau_charged_px[M_taumaxcount*10];
	Float_t tau_charged_py[M_taumaxcount*10];
	Float_t tau_charged_pz[M_taumaxcount*10];
	Float_t tau_charged_outerx[M_taumaxcount*10];
	Float_t tau_charged_outery[M_taumaxcount*10];
	Float_t tau_charged_outerz[M_taumaxcount*10];
	Float_t tau_charged_closestpointx[M_taumaxcount*10];
	Float_t tau_charged_closestpointy[M_taumaxcount*10];
	Float_t tau_charged_closestpointz[M_taumaxcount*10];
	Float_t tau_charged_chi2[M_taumaxcount*10];
	Float_t tau_charged_ndof[M_taumaxcount*10];
	Float_t tau_charged_dxy[M_taumaxcount*10];
	Float_t tau_charged_dxyerr[M_taumaxcount*10];
	Float_t tau_charged_dz[M_taumaxcount*10];
	Float_t tau_charged_dzerr[M_taumaxcount*10];
	Float_t tau_charged_dedxharmonic2[M_taumaxcount*10];
	Int_t tau_charged_charge[M_taumaxcount*10];
	UChar_t tau_charged_nhits[M_taumaxcount*10];
	UChar_t tau_charged_nmissinghits[M_taumaxcount*10];
	UChar_t tau_charged_npixelhits[M_taumaxcount*10];
	UChar_t tau_charged_npixellayers[M_taumaxcount*10];
	UChar_t tau_charged_nstriplayers[M_taumaxcount*10];

	Float_t ak4pfjet_rho;
	Float_t ak4pfjet_sigma;

	Float_t pfmet_ex;
	Float_t pfmet_ey;
	Float_t pfmettype1_ex;
	Float_t pfmettype1_ey;
	Float_t pfmettype0type1_ex;
	Float_t pfmettype0type1_ey;

	UInt_t secvertices_count;
	Float_t secvertices_vx[M_secverticesmaxcount];
	Float_t secvertices_vy[M_secverticesmaxcount];
	Float_t secvertices_vz[M_secverticesmaxcount];
	Float_t secvertices_chi2[M_secverticesmaxcount];
	Float_t secvertices_ndof[M_secverticesmaxcount];
	Float_t secvertices_cov[M_secverticesmaxcount][6];
	Float_t secvertices_track_px[M_secverticesmaxcount][2];
	Float_t secvertices_track_py[M_secverticesmaxcount][2];
	Float_t secvertices_track_pz[M_secverticesmaxcount][2];
	Float_t secvertices_track_closestpointx[M_secverticesmaxcount][2];
	Float_t secvertices_track_closestpointy[M_secverticesmaxcount][2];
	Float_t secvertices_track_closestpointz[M_secverticesmaxcount][2];
	Float_t secvertices_track_chi2[M_secverticesmaxcount][2];
	Float_t secvertices_track_ndof[M_secverticesmaxcount][2];
	Float_t secvertices_track_dxy[M_secverticesmaxcount][2];
	Float_t secvertices_track_dxyerr[M_secverticesmaxcount][2];
	Float_t secvertices_track_dz[M_secverticesmaxcount][2];
	Float_t secvertices_track_dzerr[M_secverticesmaxcount][2];
	Float_t secvertices_track_dedxharmonic2[M_secverticesmaxcount][2];
	Int_t secvertices_track_charge[M_secverticesmaxcount][2];
	UChar_t secvertices_track_nhits[M_secverticesmaxcount][2];
	UChar_t secvertices_track_nmissinghits[M_secverticesmaxcount][2];
	UChar_t secvertices_track_npixelhits[M_secverticesmaxcount][2];
	UChar_t secvertices_track_npixellayers[M_secverticesmaxcount][2];
	UChar_t secvertices_track_nstriplayers[M_secverticesmaxcount][2];

	UInt_t musecvertices_count;
	Float_t musecvertices_vx[M_musecverticesmaxcount];
	Float_t musecvertices_vy[M_musecverticesmaxcount];
	Float_t musecvertices_vz[M_musecverticesmaxcount];
	Float_t musecvertices_chi2[M_musecverticesmaxcount];
	Float_t musecvertices_ndof[M_musecverticesmaxcount];
	Float_t musecvertices_cov[M_musecverticesmaxcount][6];
	Float_t musecvertices_track_px[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_py[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_pz[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_closestpointx[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_closestpointy[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_closestpointz[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_chi2[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_ndof[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_dxy[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_dxyerr[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_dz[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_dzerr[M_musecverticesmaxcount][2];
	Float_t musecvertices_track_dedxharmonic2[M_musecverticesmaxcount][2];
	Int_t musecvertices_track_charge[M_musecverticesmaxcount][2];
	UChar_t musecvertices_track_nhits[M_musecverticesmaxcount][2];
	UChar_t musecvertices_track_nmissinghits[M_musecverticesmaxcount][2];
	UChar_t musecvertices_track_npixelhits[M_musecverticesmaxcount][2];
	UChar_t musecvertices_track_npixellayers[M_musecverticesmaxcount][2];
	UChar_t musecvertices_track_nstriplayers[M_musecverticesmaxcount][2];
	//Generator Information
	Float_t genweight;
	Float_t genid1;
	Float_t genx1;
	Float_t genid2;
	Float_t genx2;
	Float_t genScale;

	Int_t numpileupinteractionsminus;
	Int_t numpileupinteractions;
	Int_t numpileupinteractionsplus;
	Float_t numtruepileupinteractions;

	Float_t genmetcalo_ex;
	Float_t genmetcalo_ey;
	Float_t genmettrue_ex;
	Float_t genmettrue_ey;

	UInt_t genak4jet_count;
	Float_t genak4jet_e[M_genjetmaxcount];
	Float_t genak4jet_px[M_genjetmaxcount];
	Float_t genak4jet_py[M_genjetmaxcount];
	Float_t genak4jet_pz[M_genjetmaxcount];
	Float_t genak4jet_einvisible[M_genjetmaxcount];
	Int_t genak4jet_flavour[M_genjetmaxcount];
	UInt_t genak4jet_info[M_genjetmaxcount];

	UInt_t genparticles_count;
	Float_t genparticles_e[M_genparticlesmaxcount];
	Float_t genparticles_px[M_genparticlesmaxcount];
	Float_t genparticles_py[M_genparticlesmaxcount];
	Float_t genparticles_pz[M_genparticlesmaxcount];
	Float_t genparticles_vx[M_genparticlesmaxcount];
	Float_t genparticles_vy[M_genparticlesmaxcount];
	Float_t genparticles_vz[M_genparticlesmaxcount];
	Int_t genparticles_pdgid[M_genparticlesmaxcount];
	Int_t genparticles_status[M_genparticlesmaxcount];
	Int_t genparticles_indirectmother[M_genparticlesmaxcount];
	UInt_t genparticles_info[M_genparticlesmaxcount];

	UInt_t genallparticles_count;
	Float_t genallparticles_e[M_genallparticlesmaxcount];
	Float_t genallparticles_px[M_genallparticlesmaxcount];
	Float_t genallparticles_py[M_genallparticlesmaxcount];
	Float_t genallparticles_pz[M_genallparticlesmaxcount];
	Float_t genallparticles_vx[M_genallparticlesmaxcount];
	Float_t genallparticles_vy[M_genallparticlesmaxcount];
	Float_t genallparticles_vz[M_genallparticlesmaxcount];
	Int_t genallparticles_pdgid[M_genallparticlesmaxcount];
	Int_t genallparticles_status[M_genallparticlesmaxcount];
	UInt_t genallparticles_motherbeg[M_genallparticlesmaxcount];
	UInt_t genallparticles_daughterbeg[M_genallparticlesmaxcount];

	UInt_t genallparticlesmother_count;
	UInt_t genallparticles_mothers[M_genmotherdaughtermaxcount];

	UInt_t genallparticlesdaughter_count;
	UInt_t genallparticles_daughters[M_genmotherdaughtermaxcount];

	public:
	Analyse(int argc = 0, char** argv = 0, bool batchmode = false);
	virtual ~Analyse();
	Int_t AddFile(string filename);
	Int_t AddDir(string path);
	void Batch_Prepare(bool simple = false); //will run on a file based splitting. dublicate checking will not work.
//do not use this unless you are running on 2011 MC where the lumi based splitting will not work.
	Int_t Batch_MyId() const {return(batch_myid);} 
	Int_t Batch_NumJobs() const {return(batch_numjobs);} 
	Long64_t GetNumAddedEvents() const {return(filelimits[filelimits.size()-1]);};
	string GetCurrentFileName() const {if(currentfile != 0) {return(currentfile->GetName());} else {return("NO FILE OPENED");}}

	//general event information
	Double_t Number() const {return(event_nr);}
	UInt_t Run() const {return(event_run);}
	UInt_t LumiBlock() const {return(event_luminosityblock);}
	UInt_t TimeUnix() const {return(event_timeunix);}
	UInt_t TimeMicroSec() const {return(event_timemicrosec);}
	Double_t AK4PFRho() const {return(ak4pfjet_rho);}
	Double_t AK4PFSigma() const {return(ak4pfjet_sigma);}

	//RECO-level information
	void LoadBeamSpot(bool select = true);
	BeamSpot GetBeamSpot() const;

	void LoadSecVertices(bool select = true);
	TrackComposedParticle SecVertices(UInt_t n) const;
	UInt_t NumSecVertices() const {return(secvertices_count);}

	void LoadMuons(bool select = true);
	Muon Muons(UInt_t n, int correction = 0) const;
	UInt_t NumMuons() const {return(muon_count);}

	void LoadElectrons(bool select = true);
	Electron Electrons(UInt_t n, int correction = 0) const;
	UInt_t NumElectrons() const {return(electron_count);}

	void LoadPhotons(bool select = true);
	Photon Photons(UInt_t n) const;
	UInt_t NumPhotons() const {return(photon_count);}

	void LoadTaus(bool select = true);
	Tau Taus(UInt_t n) const;
	UInt_t NumTaus() const {return(tau_count);}

	void LoadMET(bool select = true);
	TLorentzVector PFMET() const;
	TLorentzVector PFMETTYPE1() const;
	TLorentzVector PFMETTYPE0TYPE1() const;

	//void LoadAK4CaloJets(bool select = true);
	//Jet AK4CaloJets(UInt_t n) const;
	//UInt_t NumAK4CaloJets() const {return(ak4calojet_count);}

	//void LoadAK4JPTJets(bool select = true);
	//Jet AK4JPTJets(UInt_t n) const;
	//UInt_t NumAK4JPTJets() const {return(ak4jptjet_count);}

	void LoadAK4PFCHSJets(bool select = true);
	Jet AK4PFCHSJets(UInt_t n) const;
	UInt_t NumAK4PFCHSJets() const {return(ak4pfchsjet_count);}

	void LoadAK4PFJets(bool select = true);
	Jet AK4PFJets(UInt_t n) const;
	UInt_t NumAK4PFJets() const {return(ak4pfjet_count);}

	void LoadTracks(bool select = true);
	Track Tracks(UInt_t n) const;
	UInt_t NumTracks() const {return(track_count);}

	void LoadPrimVertices(bool select = true);
	Vertex PrimVertices(UInt_t n) const;
	UInt_t NumPrimVertices() const {return(primvertex_count);}

	void LoadSuperClusters(bool select = true, bool usebasiccluster = false, bool usebasicclusterhit = false);
	SuperCluster SuperClusters(UInt_t n, TVector3 refpoint) const;
	SuperCluster SuperClusters(UInt_t n) const;
	UInt_t NumSuperClusters() const {return(supercluster_count);}

	//generator-level information
	void LoadGenInfo(bool select = true);
	Double_t GenWeight() const {return(genweight);}
	Double_t GenId1() const {return(genid1);}
	Double_t Genx1() const {return(genx1);}
	Double_t GenId2() const {return(genid2);}
	Double_t Genx2() const {return(genx2);}
	Double_t GenScale() const {return(genScale);}

	Int_t NumPileUpInteractionsMinus() const {return(numpileupinteractionsminus);}
	Int_t NumPileUpInteractions() const {return(numpileupinteractions);}
	Int_t NumPileUpInteractionsPlus() const {return(numpileupinteractionsplus);}
	Float_t NumTruePileUpInteractions() const {return(numtruepileupinteractions);}
	void UsePileUpInfo();
	//Double_t GetPileUpWeight(Double_t meaninteractions) const;
	void SetPileUpScale(double puscale) {pileupscale = puscale;}
	Double_t GetPileUpMaxWeight(vector<Double_t>& datadist) const;
	Double_t GetPileUpWeight(vector<Double_t>& datadist) const;
	void UsePrimVertexInfo();
	//Double_t GetPrimVertexWeight(Double_t meaninteractions) const;
	Double_t GetPrimVertexMaxWeight(vector<Double_t>& datadist) const;
	Double_t GetPrimVertexWeight(vector<Double_t>& datadist) const;
	Int_t NumGoodPrimVertices() const;

	void LoadAllGenParticles(bool select = true);
	GenParticle AllGenParticles(UInt_t n) const;
	UInt_t NumAllGenParticles() const {return(genallparticles_count);}

	void LoadGenParticles(bool select = true);
	GenLightParticle GenParticles(UInt_t n) const;
	UInt_t NumGenParticles() const {return(genparticles_count);}

	void LoadGenAK4Jets(bool select = true);
	GenJet GenAK4Jets(UInt_t n) const;
	UInt_t NumGenAK4Jets() const {return(genak4jet_count);}

	TLorentzVector GenMETCalo() const;
	TLorentzVector GenMETTrue() const;

	//virtual dummies:
	//executed for each event in the loop
	virtual Int_t AnalyseEvent(){return(1);};
	//executed once at the beginning of a the loop
	virtual void BeginLoop(){};
	//executed once at the end of a loop
	virtual void EndLoop(){};

	//use this to start your analysis
	Long64_t Loop(Long64_t start = 0, Long64_t end = -1);
	//Number of Processed Events
	Long64_t Processed() const {return(processed);}

	//setting output: -1 no output, the p-th events are printed
	void SetPrintInfo(Int_t p = -1) {printinfo = p;}

	//tool to check for duplicated events
	void EnableDuplicateCheck(bool switchval = true) {duplicatecheck = switchval;}
	UInt_t CheckDuplicate() {return(eventlist[Run()][LumiBlock()][Number()]);}

	//Skiming
	Int_t PrepareSkimming(string filename);
	Int_t SkimEvent();

	//trigger information
	void LoadTrigger(bool select = true);
	TriggerSelection* AddTriggerSelection(string id, vector<string> triggernames, bool useprescaled = false);
	TriggerSelection* GetTriggerSelection(string id);
	bool GetL1Trigger(UInt_t bit) const;
	bool GetL1TriggerBits(UInt_t bit) const;
	bool GetHLTrigger(UInt_t index) const;
	Int_t GetHLTrigger(vector<string> triggernames) const; //looks if one of the tiggers fired. Only unprescaled trigger are considered. returns -1 if non of the trigger is unprescaled, 0 non fired, 1 at least one fired.
	Int_t GetHLTriggerIndex(string triggername) const;
	string GetHLTriggerName(UInt_t index) const; 
	Int_t GetHLTPrescale(UInt_t triggerindex) const;
	Int_t GetNumHLTriggers() const;

	//Lumi calculation
	void AddLumiFile(string filename, string dir = "");
	Int_t IsLumiAvailable() const;
	Double_t GetInstLumi() const;
	Double_t GetAvgPU() const; 
	Double_t GetLumi(Int_t format = 0);
	//Double_t GetLumiBlockLumi(); //Lumi in dataset up to current event.
	void PrintPrescaleInfo(string triggername);
	void PrintPrescaleInfoB(string triggername);
	void PrintLumiOfRuns();
	void PrintLumiOfLumiSectionsInRun(UInt_t runnumber);
	//very special don't use!
	Int_t SetLumi(UInt_t run, UInt_t block, Float_t lumival, Float_t avgpu = -1);
	void ResetLumiValues();
	void WriteLumiFile(string filename);
	//Use JSON filter
	bool LoadJSON(string filename);
	
	bool IsData() const {return(NumTruePileUpInteractions() == -1);}
	bool IsMC() const {return(NumTruePileUpInteractions() != -1);}

};

extern Analyse* GLAN;

Long64_t mem_usage();
#endif
