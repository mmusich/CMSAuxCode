
// CMSSW includes
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CondFormats/DataRecord/interface/AlCaRecoTriggerBitsRcd.h"
#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"
#include "CondFormats/HLTObjects/interface/AlCaRecoTriggerBits.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
// #include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h" 
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "IOMC/RandomEngine/src/RandomEngineStateProducer.h"

// ROOT includes
#include "TBranch.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLorentzVector.h"
#include "TTree.h"

// System includes
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <vector>

// RooFit includes
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"
#include "RooVoigtian.h"

#define DEBUG 0

using namespace std;
using namespace edm;

class TrackAnalyzerNewTwoBodyHisto : public edm::EDAnalyzer {
 public:
  TrackAnalyzerNewTwoBodyHisto(const edm::ParameterSet& pset) {
    TkTag_ = pset.getParameter<string>("TkTag");
    theTrackCollectionToken = consumes<reco::TrackCollection>(TkTag_);
     
    InputTag tag("TriggerResults","","RECO"); 
    hltresultsToken = consumes<edm::TriggerResults>(tag);

    InputTag beamSpotTag("offlineBeamSpot");
    beamspotToken = consumes<reco::BeamSpot>(beamSpotTag);
    
    InputTag vertexTag("offlinePrimaryVertices");
    vertexToken   = consumes<reco::VertexCollection>(vertexTag);

    theMinMass = pset.getParameter<double>( "minMass" );
    theMaxMass = pset.getParameter<double>( "maxMass" );
    nBins_     = pset.getUntrackedParameter<int>("nBins",24);
    m_verbose_fit = pset.getUntrackedParameter<bool>("verbose_fit",true);
   }

  ~TrackAnalyzerNewTwoBodyHisto(){}

  //void fitVoigt(TH1F *hist);
  //void makeNicePlotStyle(RooPlot* plot);

  edm::EDGetTokenT<reco::TrackCollection>  theTrackCollectionToken;
  edm::EDGetTokenT<edm::TriggerResults> hltresultsToken;
  edm::EDGetTokenT<reco::BeamSpot> beamspotToken;
  edm::EDGetTokenT<reco::VertexCollection> vertexToken;

  edm::Service<TFileService> fs;
  
  TTree* AnalysisTree;
  //   BRANCH branch;

  TH1D *hInvMassJpsi  ;
  TH1D *hInvMassUpsilon  ;
  TH1D *hInvMassZ ;

  std::map<int,TH1D*> hInvMassZinPhiPlusBins_;
  std::map<int,TH1D*> hInvMassZinPhiMinusBins_;
  std::map<int,TH1D*> hInvMassZinEtaPlusBins_;
  std::map<int,TH1D*> hInvMassZinEtaMinusBins_;
  std::map<int,TH1D*> hInvMassZinDeltaEtaBins_;

  std::map<std::pair<int,int>,TH1D*> hInvMassZinEtaPhiPlusBins_;
  std::map<std::pair<int,int>,TH1D*> hInvMassZinEtaPhiMinusBins_;

  TH1D *hInvMassVsPhiPlus; 
  TH1D *hInvMassVsPhiMinus;
  TH1D *hInvMassVsEtaPlus; 
  TH1D *hInvMassVsEtaMinus;
  TH1D *hInvMassVsDeltaEta;

  TH2D *hInvMassVsEtaPhiPlus;
  TH2D *hInvMassVsEtaPhiMinus;

  vector <pair <string, bool> > triggerInfo;
 
  TH1D *hchi2  ;
  TH1D *hNtrk  ;
  TH1D *hP     ;
  TH1D *hPPlus ;
  TH1D *hPMinus;
  TH1D *hPt    ;
  TH1D *hMinPt ;
  TH1D *hPtPlus;
  TH1D *hPtMinus;
  TH1D *hHit;    
  TH1D *hHitPlus;
  TH1D *hHitMinus;

  TH1D *hEta;
  TH1D *hEtaPlus; 
  TH1D *hEtaMinus;
  TH1D *hPhi;
  TH1D *hPhiDT;
  TH1D *hPhiRPCPlus;
  TH1D *hPhiRPCMinus;
  TH1D *hPhiCSCPlus;
  TH1D *hPhiCSCMinus;
  TH1D *hPhiPlus;
  TH1D *hPhiMinus;

  TH1D *hDeltaPhi;
  TH1D *hDeltaEta;
  TH1D *hDeltaR  ;

  TH1D *hvx;
  TH1D *hvy;
  TH1D *hvz;
  TH1D *hd0;
  TH2D *hd0vsphi;
  TH2D *hd0vseta;
  TH2D *hd0vspt;

  TH1D *hnhpxb;
  TH1D *hnhpxe;
  TH1D *hnhTIB;
  TH1D *hnhTID;
  TH1D *hnhTOB;
  TH1D *hnhTEC;

  TH1D *hMultCand ;

  TH1D * hPx ;
  TH1D * hPy ;
  TH1D * hPz ;
  TH1D * hmuQ;
  TH1D * hdxy;
  TH1D * hdz ;

  TH1D * hEtaMother;
  TH1D * hPtMother ;
  TH1D * hPhiMother;

  TH1D * hdxyBS;
  TH1D * hd0BS;
  TH1D * hdzBS;
  TH1D * hdxyPV;
  TH1D * hd0PV;
  TH1D * hdzPV;
  TH1D *hrun;
  TH1D *hlumi;

  std::string trigNames;
  int trigCount;
  int ievt;
  int mol;
  float InvMass;
  bool firstEvent_;
  const TrackerGeometry* trackerGeometry_;

  std::string TkTag_;
  double theMinMass, theMaxMass;
  int nBins_;
  bool  m_verbose_fit;

  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup){
    //    double pigreco = 3.141592;
    
    if (DEBUG) cout << __LINE__ << endl;

    using namespace std;
    vector <double> invMass;
    ievt++;
    if (DEBUG) cout << __LINE__ << endl;

    edm::Handle<reco::TrackCollection> trackCollection;
    event.getByToken(theTrackCollectionToken, trackCollection);

    if (DEBUG) cout << __LINE__ << endl;
    
    const reco::TrackCollection tC = *(trackCollection.product());
    
    int MultCand = 0 ;
    if (tC.size() > 2 ) MultCand =1 ; 
    if (DEBUG) cout << __LINE__ << endl;
    hNtrk->Fill(tC.size());
    if (MultCand==1) std::cout << "event#" << ievt << " Event ID = "<< event.id() << std::endl ;
    if (MultCand==1) std::cout << "Reconstructed "<< tC.size() << " tracks" << std::endl ;
    TLorentzVector mother(0.,0.,0.,0.);
    
    if (DEBUG) cout << __LINE__ << endl;
    int iCounter=0;
    if (DEBUG) cout << __LINE__ << endl;
    hrun->Fill(event.run());
    hlumi->Fill(event.luminosityBlock());

    TAxis *phiaxis = new TAxis(nBins_,-M_PI,M_PI);
    TAxis *etaaxis = new TAxis(nBins_,-2.5,2.5);
    TAxis *deltaEtaaxis = new TAxis(nBins_,-3,3);

    invMass.clear();
    for (reco::TrackCollection::const_iterator track=tC.begin(); track!=tC.end(); track++){
      TLorentzVector track0(track->px(),track->py(),track->pz(),sqrt((track->p()*track->p())+(0.105*0.105))); //old 106
      InvMass=0; 
      for (reco::TrackCollection::const_iterator track1=track+1; track1!=tC.end(); track1++){
	TLorentzVector track01(track1->px(),track1->py(),track1->pz(),sqrt((track1->p()*track1->p())+(0.105*0.105)));
	mother=track0+track01;
	InvMass=mother.M();
	invMass.push_back(InvMass);
	if(theMaxMass<5.   && theMinMass>0.)  hInvMassJpsi->Fill(InvMass);
	if(theMaxMass<15.  && theMinMass>5.)  hInvMassUpsilon->Fill(InvMass);
	if(theMaxMass<150. && theMinMass>50.) hInvMassZ->Fill(InvMass);
	hEtaMother->Fill(mother.Eta());
	hPtMother->Fill(mother.Pt());
	hPhiMother->Fill(mother.Phi());
	
	double etaMu1 = track->eta();
	double phiMu1 = track->phi();
	//double ptMu1 = track->pt();
	int charge1  = track->charge();
	
	int phi1bin = phiaxis->FindBin(phiMu1);
	int eta1bin = etaaxis->FindBin(etaMu1);

	double etaMu2 = track1->eta();
	double phiMu2 = track1->phi();
	//double ptMu2 = track1->pt();
	//int charge2   = track1->charge();

	int phi2bin = phiaxis->FindBin(phiMu2);
	int eta2bin = etaaxis->FindBin(etaMu2);
	
	auto etaphi1 = std::make_pair(eta1bin,phi1bin);
	auto etaphi2 = std::make_pair(eta2bin,phi2bin);

	//std::cout<< "phi1bin:" << phi1bin << " phi2bin:"<< phi2bin << " eta1bin:"<<eta1bin << " eta2bin:" << eta2bin << std::endl;
	if(charge1>0){
	  hInvMassZinPhiPlusBins_[phi1bin]->Fill(InvMass); 
	  hInvMassZinPhiMinusBins_[phi2bin]->Fill(InvMass);
	  hInvMassZinEtaPlusBins_[eta1bin]->Fill(InvMass); 
	  hInvMassZinEtaMinusBins_[eta2bin]->Fill(InvMass);
	  hInvMassZinEtaPhiPlusBins_[etaphi1]->Fill(InvMass);
	  hInvMassZinEtaPhiMinusBins_[etaphi2]->Fill(InvMass);

	} else {
	  hInvMassZinPhiPlusBins_[phi2bin]->Fill(InvMass); 
	  hInvMassZinPhiMinusBins_[phi1bin]->Fill(InvMass);
	  hInvMassZinEtaPlusBins_[eta2bin]->Fill(InvMass); 
	  hInvMassZinEtaMinusBins_[eta1bin]->Fill(InvMass);
	  hInvMassZinEtaPhiPlusBins_[etaphi2]->Fill(InvMass);
	  hInvMassZinEtaPhiMinusBins_[etaphi1]->Fill(InvMass);
	}
       
	double deltaEta = etaMu1 - etaMu2;
	int deltaEtaBin = deltaEtaaxis->FindBin(deltaEta);
	hInvMassZinDeltaEtaBins_[deltaEtaBin]->Fill(InvMass);

	double deltaPhi = phiMu1 - phiMu2;
	if (deltaPhi < - M_PI) deltaPhi = 2*M_PI + deltaPhi;
	if (deltaPhi >   M_PI) deltaPhi =-2*M_PI + deltaPhi;
	double R  = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi);
	hDeltaPhi -> Fill (deltaPhi);
	hDeltaEta -> Fill (deltaEta);
	hDeltaR   -> Fill (R) ;
	
      }//TRACK1
      if (invMass.size()==0) continue;
      
      hHit->Fill(track->numberOfValidHits());
      hnhpxb->Fill( track->hitPattern().numberOfValidPixelBarrelHits());
      hnhpxe->Fill(track->hitPattern().numberOfValidPixelEndcapHits());
      hnhTIB->Fill(track->hitPattern().numberOfValidStripTIBHits());
      hnhTID->Fill(track->hitPattern().numberOfValidStripTIDHits());
      hnhTOB->Fill(track->hitPattern().numberOfValidStripTOBHits());
      hnhTEC->Fill(track->hitPattern().numberOfValidStripTECHits());
      hPt->Fill(track->pt());
      hP->Fill(track->p());
      hchi2->Fill(track->normalizedChi2());
      hEta->Fill(track->eta());
      hPhi->Fill(track->phi());
      if(fabs(track->eta() ) < 0.8 )                hPhiDT->Fill(track->phi());
      if(track->eta()>0.8 && track->eta()  < 1.4 )  hPhiRPCPlus->Fill(track->phi());
      if(track->eta()<-0.8 && track->eta() >- 1.4 ) hPhiRPCMinus->Fill(track->phi());
      if(track->eta()>1.4)                          hPhiCSCPlus->Fill(track->phi());
      if(track->eta()<-1.4)                         hPhiCSCMinus->Fill(track->phi());
      hd0->Fill(track->d0());
      hvx->Fill(track->vx());
      hvy->Fill(track->vy());
      hvz->Fill(track->vz());
      hd0vsphi ->Fill(track->phi(),track->d0());
      hd0vseta ->Fill(track->eta(), track->d0());
      hd0vspt  ->Fill(track->pt(), track->d0());
      hPx->Fill(track->px());
      hPy->Fill(track->py());
      hPz->Fill(track->pz());
      hmuQ->Fill(track ->charge());     
      hdxy->Fill(track->dxy());
      hdz->Fill(track->dz());

      //dxy with respect to the beamspot
      reco::BeamSpot beamSpot;
      edm::Handle<reco::BeamSpot> beamSpotHandle;
      event.getByToken(beamspotToken,beamSpotHandle);
      if(beamSpotHandle.isValid()){ 
	beamSpot = *beamSpotHandle;
	math::XYZPoint point(beamSpot.x0(),beamSpot.y0(), beamSpot.z0());
	double dxy = track->dxy(point);
	double dz = track->dz(point);
	hdxyBS->Fill(dxy);
	hd0BS->Fill(-dxy);
	hdzBS->Fill(dz);
      }
      
      //dxy with respect to the primary vertex
      edm::Handle<reco::VertexCollection> pVert;
      event.getByToken(vertexToken,pVert);
      if (!pVert.isValid()) {
	cout << "PrimaryVertex not found" << endl;
	return;
      }
      const reco::Vertex * myVertex = 0;
      double mindxy = 100.;
      for(reco::VertexCollection::const_iterator iv = pVert->begin(); iv!=pVert->end(); ++iv) {
	math::XYZPoint mypoint( iv->x(), iv->y(), iv->z());
	double mydxy = abs(track->dxy(mypoint));
	if (mydxy < mindxy){
	  mindxy=mydxy;
	  myVertex = &*iv;
	}
      }
      math::XYZPoint mypoint(myVertex->x(), myVertex->y(), myVertex->z());
      hdxyPV->Fill(track->dxy(mypoint));
      hd0PV->Fill(-track->dxy(mypoint));
      hdzPV->Fill(-track->dz(mypoint));
							       
      iCounter++;

      

    }//return; 

    delete phiaxis;
    delete etaaxis;
  }

  virtual void beginJob() {
    ievt=0;
    hrun = fs->make<TH1D>("hrun","run",20000,180000.5,200000.5);//1000000,0,1000000);
    hlumi = fs->make<TH1D>("hlumi","lumi",1000,0,1000);
    hInvMassJpsi = fs->make<TH1D>("hInvMassJpsi","hInvMassJpsi",400,2,4);
    hInvMassUpsilon = fs->make<TH1D>("hInvMassUpsilon","hInvMassUpsilon",600,8,11);
    hInvMassZ = fs->make<TH1D>("hInvMassZ","hInvMassZ",600,60,120);

    hchi2 = fs->make<TH1D>("hchi2","#chi^{2}/ndf",100,0,5.);
    hNtrk=fs->make<TH1D>("hNtrk","Number of Tracks",200,0.,200.);
    hP=fs->make<TH1D>("hP","Momentum",200,0.,200.);
    hPt=fs->make<TH1D>("hPt","Transverse Momentum",200,0.,200.);
    hHit=fs->make<TH1D>("hHit","Number of hit",30,0,30);
    hEta=fs->make<TH1D>("hEta","Eta",100,-4.,4.);
    hPhi=fs->make<TH1D>("hPhi","Phi",100,-4.,4.);
    hPhiDT=fs->make<TH1D>("hPhiDT","hPhiDT",100,-4.,4.);
    hPhiRPCPlus=fs->make<TH1D>("hPhiRPCPlus","hPhiRPCPlus",100,-4.,4.);
    hPhiRPCMinus=fs->make<TH1D>("hPhiRPCMinus","hPhiRPCMinus",100,-4.,4.);
    hPhiCSCPlus=fs->make<TH1D>("hPhiCSCPlus","hPhiCSCPlus",100,-4.,4.);
    hPhiCSCMinus=fs->make<TH1D>("hPhiCSCMinus","hPhiCSCMinus",100,-4.,4.);
    hvx  =fs->make<TH1D>("hvx"  ,"hvx"  ,100,0.,.5);
    hvy  =fs->make<TH1D>("hvy"  ,"hvy"  ,100,0.,.5);
    hvz  =fs->make<TH1D>("hvz"  ,"hvz"  ,100,-10.,10.);
    hd0  =fs->make<TH1D>("hd0"  ,"hd0"  ,100,-1.,1.);
    hd0vsphi  =fs->make<TH2D>("hd0vsphi"  ,"hd0vsphi"  ,160,-3.20,3.20,100,-1.,1.);
    hd0vseta  =fs->make<TH2D>("hd0vseta"  ,"hd0vseta"  ,160,-3.20,3.20,100,-1.,1.);
    hd0vspt   =fs->make<TH2D>("hd0vspt"   ,"hd0vspt"   ,50,0.,100.,100,-0.25,0.25);
    if (DEBUG) cout << __LINE__ << endl;

    hnhpxb  =fs->make<TH1D>("nhpxb"  ,"nhpxb"  ,10,0.,10.);
    hnhpxe  =fs->make<TH1D>("nhpxe"  ,"nhpxe"  ,10,0.,10.);
    hnhTIB  =fs->make<TH1D>("nhTIB"  ,"nhTIB"  ,20,0.,20.);
    hnhTID  =fs->make<TH1D>("nhTID"  ,"nhTID"  ,20,0.,20.);
    hnhTOB  =fs->make<TH1D>("nhTOB"  ,"nhTOB"  ,20,0.,20.);
    hnhTEC  =fs->make<TH1D>("nhTEC"  ,"nhTEC"  ,20,0.,20.);
    if (DEBUG) cout << __LINE__ << endl;
      

    hPx=fs->make<TH1D>("hPx","hPx",200,-100.,100.);
    hPy=fs->make<TH1D>("hPy","hPy",200,-100.,100.);
    hPz=fs->make<TH1D>("hPz","hPz",200,-100.,100.);
    
    hmuQ=fs->make<TH1D>("hmuQ","hmuQ",5,-2.5,2.5);
    
    hdxy=fs->make<TH1D>("hdxy","hdxy",200,-0.1,0.1);
    hdz =fs->make<TH1D>("hdz","hdz",200,-0.5,0.5);


    hEtaMother=fs->make<TH1D>("hEtaMother","EtaMother",100,-4.,4.);
    hPtMother=fs->make<TH1D>("hPtMother","Transverse Momentum Mother",200,0.,200.);
    hPhiMother=fs->make<TH1D>("hPhiMother","PhiMother",100,-4.,4.);

    hDeltaPhi= fs->make<TH1D>("hDeltaPhi","DeltaPhi",100,-10.,10.);
    hDeltaEta= fs->make<TH1D>("hDeltaEta","DeltaEta",100,-10.,10.);
    hDeltaR  = fs->make<TH1D>("R"  ,"R"  ,100,-1.,10.);

    hdxyBS=fs->make<TH1D>("hdxyBS","hdxyBS",200,-0.02,0.02);
    hd0BS=fs->make<TH1D>("hd0BS","hd0BS",600,-0.06,0.06);
    hdzBS=fs->make<TH1D>("hdzBS","hdzBS",1200,-12,12);
    hdxyPV=fs->make<TH1D>("hdxyPV","hdxyPV",200,-0.02,0.02);
    hd0PV=fs->make<TH1D>("hd0PV","hd0PV",200,-0.02,0.02);
    hdzPV=fs->make<TH1D>("hdzPV","hdzPV",200,-0.02,0.02);

    // book the binned distributions of invariant mass
    
    TFileDirectory BinnedDistributions = fs->mkdir("BinnedDistributions");
    TFileDirectory phiPlus     = BinnedDistributions.mkdir("phiPlus");
    TFileDirectory phiMinus    = BinnedDistributions.mkdir("phiMinus");
    TFileDirectory etaPlus     = BinnedDistributions.mkdir("etaPlus");
    TFileDirectory etaMinus    = BinnedDistributions.mkdir("etaMinus");
    TFileDirectory etaPhiMinus = BinnedDistributions.mkdir("etaPhiMinus");
    TFileDirectory etaPhiPlus  = BinnedDistributions.mkdir("etaPhiPlus");
    TFileDirectory deltaEta    = BinnedDistributions.mkdir("deltaEta");

    for(int i=0;i<=nBins_+1;i++){
      hInvMassZinPhiPlusBins_[i]  = phiPlus.make<TH1D>(Form("hInvMassZ_vsPhiPlus_bin%i",i),   Form("di-#mu invariant mass (#phi_{#mu^{+}} bin %i);di-#mu invariant mass [GeV];n. events",i),120,60,120); 
      hInvMassZinPhiMinusBins_[i] = phiMinus.make<TH1D>(Form("hInvMassZ_vsPhiMinus_bin%i",i), Form("di-#mu invariant mass (#phi_{#mu^{-}} bin %i);di-#mu invariant mass [GeV];n. events",i),120,60,120);
      hInvMassZinEtaPlusBins_[i]  = etaPlus.make<TH1D>(Form("hInvMassZ_vsEtaPlus_bin_%i",i),  Form("di-#mu invariant mass (#eta_{#mu^{+}} bin %i);di-#mu invariant mass [GeV];n. events",i),120,60,120);  
      hInvMassZinEtaMinusBins_[i] = etaMinus.make<TH1D>(Form("hInvMassZ_vsEtaMinus_bin_%i",i),Form("di-#mu invariant mass (#eta_{#mu^{+}} bin %i);di-#mu invariant mass [GeV];n. events",i),120,60,120);
      hInvMassZinDeltaEtaBins_[i] = deltaEta.make<TH1D>(Form("hInvMassZ_vsDeltaEta_bin_%i",i),Form("di-#mu invariant mass (#eta_{#mu^{+}} bin %i);di-#mu invariant mass [GeV];n. events",i),120,60,120);;

      // 2D maps
      for(int j=0;j<=nBins_+1;j++){
	auto pair = std::make_pair(i,j);
	hInvMassZinEtaPhiPlusBins_[pair]  = etaPhiPlus.make<TH1D> (Form("hInvMassZ_vsEta_bin_%i_Phi_bin_%i_Plus",i,j),  Form("di-#mu invariant mass (#eta_{#mu^{+}} bin %i,#phi_{#mu^{+}} bin %i);di-#mu invariant mass [GeV];n. events",i,j),120,60,120);  
	hInvMassZinEtaPhiMinusBins_[pair] = etaPhiMinus.make<TH1D>(Form("hInvMassZ_vsEta_bin_%i_Phi_bin_%i_Minus",i,j), Form("di-#mu invariant mass (#eta_{#mu^{-}} bin %i,#phi_{#mu^{-}} bin %i);di-#mu invariant mass [GeV];n. events",i,j),120,60,120);
      }
    }

    TFileDirectory MassTrends = fs->mkdir("Mass Trends");
    hInvMassVsPhiPlus  = MassTrends.make<TH1D>("hInvMassVsPhiPlus", "di-#mu invariant mass vs #phi_{#mu^{+}};#phi_{#mu^{+}} [rad];di-#mu invariant mass [GeV]",nBins_,-0.5,nBins_-0.5); 
    hInvMassVsPhiMinus = MassTrends.make<TH1D>("hInvMassVsPhiMinus","di-#mu invariant mass vs #phi_{#mu^{-}};#phi_{#mu^{-}} [rad];di-#mu invariant mass [GeV]",nBins_,-0.5,nBins_-0.5);
    hInvMassVsEtaPlus  = MassTrends.make<TH1D>("hInvMassVsEtaPlus", "di-#mu invariant mass vs #eta_{#mu^{+}};#eta_{#mu^{+}};di-#mu invariant mass [GeV]",nBins_,-0.5,nBins_-0.5); 
    hInvMassVsEtaMinus = MassTrends.make<TH1D>("hInvMassVsEtaMinus","di-#mu invariant mass vs #eta_{#mu^{-}};#eta_{#mu^{-}};di-#mu invariant mass [GeV]",nBins_,-0.5,nBins_-0.5);

    hInvMassVsEtaPhiPlus  = MassTrends.make<TH2D>("hInvMassVsEtaPhiPlus", "di-#mu invariant mass vs (#eta_{#mu^{+}},#phi_{#mu^{+}});#eta_{#mu^{+}};#phi_{#mu^{+}} [rad];di-#mu invariant mass [GeV]",nBins_,-0.5,nBins_-0.5,nBins_,-0.5,nBins_-0.5); 
    hInvMassVsEtaPhiMinus = MassTrends.make<TH2D>("hInvMassVsEtaPhiMinus","di-#mu invariant mass vs (#eta_{#mu^{-}},#phi_{#mu^{-}});#eta_{#mu^{+}};#phi_{#mu^{-}} [rad];di-#mu invariant mass [GeV]",nBins_,-0.5,nBins_-0.5,nBins_,-0.5,nBins_-0.5);

    hInvMassVsDeltaEta = MassTrends.make<TH1D>("hInvMassVsDeltaEta", "di-#mu invariant mass vs #Delta#eta_{#mu^{+},#mu_{-}};#Delta#eta_{#mu^{+},#mu^{-}};di-#mu invariant mass [GeV]",nBins_,-0.5,nBins_-0.5);

    firstEvent_=true;

  }//beginJob

  
//****************************************************************/
  std::pair<Measurement1D, Measurement1D>  fitVoigt(TH1 *hist)
//****************************************************************/
  {
    std::cout << "## Fitting TH1 histograms ##" << std::endl;
    if (!m_verbose_fit) {
      RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
    }

    TCanvas *c1 = new TCanvas();
    // Define common things for the different fits
  
    c1->Clear();

    c1->SetLeftMargin(0.15);
    c1->SetRightMargin(0.10);

    Double_t xmin = 65;
    Double_t xmax = 115;
    RooRealVar InvMass("InVMass", "di-muon mass", xmin, xmax);
    RooPlot* frame = InvMass.frame();
    RooDataHist datahist("datahist", "datahist",InvMass, RooFit::Import(*hist));
    datahist.plotOn(frame);
  
    RooRealVar mean("mean","mean",95.0, 60.0, 120.0);
    RooRealVar width("width","width",5.0, 0.0, 120.0);
    RooRealVar sigma("sigma","sigma",5.0, 0.0, 120.0);
    //RooBreitWigner gauss("gauss","gauss",x,mean,sigma);
    RooVoigtian voigt("voigt","voigt",InvMass,mean,width,sigma);

    RooFitResult* filters = voigt.fitTo(datahist,"qr");
    voigt.plotOn(frame,RooFit::LineColor(4));//this will show fit overlay on canvas 
    voigt.paramOn(frame); //this will display the fit parameters on canvas
  
    makeNicePlotStyle(frame);

  // Redraw data on top and print / store everything
    datahist.plotOn(frame);
    frame->GetYaxis()->SetTitle("n. of events");
    TString histName = hist->GetName();
    frame->SetName("frame"+histName);
    frame->SetTitle(hist->GetTitle());
    frame->Draw();
    //    m_output->cd();
    //frame->Write();
    
    c1->Print("fit_debug"+histName+".pdf");
    delete c1;

    //return std::make_tuple(mean.getVal(),width.getVal(),sigma.getVal());

    float mass_mean  = mean.getVal();
    float mass_sigma = sigma.getVal();
  
    float mass_mean_err  = mean.getError();
    float mass_sigma_err = sigma.getError();

    Measurement1D resultM(mass_mean,mass_mean_err);
    Measurement1D resultW(mass_sigma,mass_sigma_err);

    std::pair<Measurement1D, Measurement1D> result;
  
    result = std::make_pair(resultM,resultW);
    return result;

}

/*--------------------------------------------------------------------*/
void makeNicePlotStyle(RooPlot* plot)
/*--------------------------------------------------------------------*/
{ 
  plot->GetXaxis()->CenterTitle(true);
  plot->GetYaxis()->CenterTitle(true);
  plot->GetXaxis()->SetTitleFont(42); 
  plot->GetYaxis()->SetTitleFont(42);  
  plot->GetXaxis()->SetTitleSize(0.05);
  plot->GetYaxis()->SetTitleSize(0.05);
  plot->GetXaxis()->SetTitleOffset(0.9);
  plot->GetYaxis()->SetTitleOffset(1.3);
  plot->GetXaxis()->SetLabelFont(42);
  plot->GetYaxis()->SetLabelFont(42);
  plot->GetYaxis()->SetLabelSize(.05);
  plot->GetXaxis()->SetLabelSize(.05);
}


  virtual void endJob()
  { 
  
    for(int i=1;i<=nBins_;i++){
      auto phiplusparams  = fitVoigt(hInvMassZinPhiPlusBins_[i]);
      hInvMassVsPhiPlus->SetBinContent(i,phiplusparams.first.value()); 
      hInvMassVsPhiPlus->SetBinError(i,phiplusparams.first.error()); 

      auto phiminusparams = fitVoigt(hInvMassZinPhiMinusBins_[i]);
      hInvMassVsPhiMinus->SetBinContent(i,phiminusparams.first.value());
      hInvMassVsPhiMinus->SetBinError(i,phiminusparams.first.error());

      auto etaplusparams  = fitVoigt(hInvMassZinEtaPlusBins_[i]);
      hInvMassVsEtaPlus->SetBinContent(i,etaplusparams.first.value()); 
      hInvMassVsEtaPlus->SetBinError(i,etaplusparams.first.error()); 

      auto etaminusparams = fitVoigt(hInvMassZinEtaMinusBins_[i]);
      hInvMassVsEtaMinus->SetBinContent(i,etaminusparams.first.value());
      hInvMassVsEtaMinus->SetBinError(i,etaminusparams.first.error());

      auto deltaEtaparams = fitVoigt(hInvMassZinDeltaEtaBins_[i]);
      hInvMassVsDeltaEta->SetBinContent(i,deltaEtaparams.first.value());
      hInvMassVsDeltaEta->SetBinError(i,deltaEtaparams.first.error());

      for(int j=1;j<=nBins_;j++){
	auto pair = std::make_pair(i,j);
	auto etaphiplusparams  = fitVoigt(hInvMassZinEtaPhiPlusBins_[pair]);
	hInvMassVsEtaPhiPlus->SetBinContent(i,j,etaphiplusparams.first.value()); 
	hInvMassVsEtaPhiPlus->SetBinError(i,j,etaphiplusparams.first.error()); 

	auto etaphiminusparams = fitVoigt(hInvMassZinEtaPhiMinusBins_[pair]);
	hInvMassVsEtaPhiMinus->SetBinContent(i,j,etaphiminusparams.first.value());
	hInvMassVsEtaPhiMinus->SetBinError(i,j,etaphiminusparams.first.error());
      }
    }
  }

};


DEFINE_FWK_MODULE(TrackAnalyzerNewTwoBodyHisto);

