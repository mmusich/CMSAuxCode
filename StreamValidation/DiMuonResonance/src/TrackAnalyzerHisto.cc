#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "IOMC/RandomEngine/src/RandomEngineStateProducer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"

#include "CondFormats/HLTObjects/interface/AlCaRecoTriggerBits.h"
#include "CondFormats/DataRecord/interface/AlCaRecoTriggerBitsRcd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
// #include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h" 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TBranch.h"
#include <map>

#define DEBUG 0

using namespace std;

using namespace edm;

class TrackAnalyzerHisto : public edm::EDAnalyzer {
 public:
  TrackAnalyzerHisto(const edm::ParameterSet& pset) {
    TkTag_ = pset.getParameter<string>("TkTag");
  }

  ~TrackAnalyzerHisto(){}

  edm::Service<TFileService> fs;
  
  TTree* AnalysisTree;
//   BRANCH branch;

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
  TH1D * hPhiDT;
  TH1D * hPhiRPCPlus;
  TH1D * hPhiRPCMinus;
  TH1D * hPhiCSCPlus;
  TH1D * hPhiCSCMinus;
  TH1D *hPhiPlus;
  TH1D *hPhiMinus;

  TH1D *hDeltaPhi;
  TH1D *hDeltaEta;
  TH1D *hDeltaR  ;



  TH1D *hvx  ;
  TH1D *hvy  ;
  TH1D *hvz  ;
  TH1D *hd0  ;
  TH1D *hdz  ;
  TH1D *hdxy  ;

  TH2D *hd0vsphi  ;
  TH2D *hd0vseta  ;
  TH2D *hd0vspt   ;

  TH1D *hnhpxb  ;
  TH1D *hnhpxe  ;
  TH1D *hnhTIB  ;
  TH1D *hnhTID  ;
  TH1D *hnhTOB  ;
  TH1D *hnhTEC  ;

  TH1D *hMultCand ;

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
  double invMass;
  bool firstEvent_;
  const TrackerGeometry* trackerGeometry_;

  std::string TkTag_;

  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup){

   ievt++;
    std::cout << "event#" << ievt << " Event ID = "<< event.id() << std::endl ;


    edm::Handle<reco::TrackCollection> trackCollection;
    event.getByLabel(TkTag_, trackCollection);

    
    const reco::TrackCollection tC = *(trackCollection.product());

    std::cout << "Reconstructed "<< tC.size() << " tracks" << std::endl ;
    TLorentzVector mother(0.,0.,0.,0.);
    
    //int iCounter=0;

    edm::Handle<edm::TriggerResults> hltresults;
    InputTag tag("TriggerResults","","HLT");//InputTag tag("TriggerResults");
    event.getByLabel(tag,hltresults);
    edm::TriggerNames triggerNames_ = event.triggerNames(*hltresults);
    //int ntrigs=hltresults->size();
    vector<string> triggernames = triggerNames_.triggerNames();
    hrun->Fill(event.run());
    hlumi->Fill(event.luminosityBlock());

    for (reco::TrackCollection::const_iterator track=tC.begin(); track!=tC.end(); track++){

      hHit->Fill(track->numberOfValidHits());
      std::cout<<"n. hits:"<<track->numberOfValidHits()<<std::endl;

      const reco::HitPattern& p = track->hitPattern();  
      //std::cout<<"n. hits (from hit pattern):" << p.numberOfHits(reco::HitPattern::TRACK_HITS)<<std::endl;
      std::cout<<"n. layers:" << p.trackerLayersWithMeasurement()<<std::endl;

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
      if (fabs(track->eta() ) < 0.8 )  hPhiDT->Fill(track->phi());
      if (track->eta()>0.8 && track->eta()  < 1.4 )  hPhiRPCPlus->Fill(track->phi());
      if (track->eta()<-0.8 && track->eta() >- 1.4 )  hPhiRPCMinus->Fill(track->phi());
      if (track->eta()>1.4) hPhiCSCPlus->Fill(track->phi());
      if (track->eta()<-1.4) hPhiCSCMinus->Fill(track->phi());
     
      hd0->Fill(track->d0());
      hdz->Fill(track->dz());
      hdxy->Fill(track->dxy());
      hvx->Fill(track->vx());
      hvy->Fill(track->vy());
      hvz->Fill(track->vz());
      hd0vsphi ->Fill(track->phi(),track->d0());
      hd0vseta ->Fill(track->eta(), track->d0());
      hd0vspt  ->Fill(track->pt(), track->d0());


      //dxy with respect to the beamspot
      reco::BeamSpot beamSpot;
      edm::Handle<reco::BeamSpot> beamSpotHandle;
      event.getByLabel("offlineBeamSpot", beamSpotHandle);
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
      reco::Vertex pvtx;
      edm::Handle<reco::VertexCollection> vertexHandle;
      reco::VertexCollection vertexCollection;
      event.getByLabel("offlinePrimaryVertices", "WithBS", vertexHandle);
      double mindxy = 100.;
      double dz = 100;
      if(vertexHandle.isValid()) {
	for(reco::VertexCollection::const_iterator pvtx = vertexHandle->begin(); pvtx!=vertexHandle->end(); ++pvtx) {
	  math::XYZPoint mypoint( pvtx->x(), pvtx->y(), pvtx->z());
	  if(abs(mindxy)>abs(track->dxy(mypoint))){
	    mindxy = track->dxy(mypoint);
	    dz=track->dz(mypoint);
	  }
	}
	hdxyPV->Fill(mindxy);
	hd0PV->Fill(-mindxy);
	hdzPV->Fill(dz);

      }
      else{
	hdxyPV->Fill(100);
	hd0PV->Fill(100);
	hdzPV->Fill(100);
      }

    }

    hNtrk->Fill(tC.size());
  }

  virtual void beginJob() {
    if (DEBUG) cout << __LINE__ << endl;
    ievt=0;
    hrun = fs->make<TH1D>("hrun","run",20000,180000.5,200000.5);//1000000,0,1000000);
    hlumi = fs->make<TH1D>("hlumi","lumi",1000,0,1000);

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
    hdxy=fs->make<TH1D>("hdxy","hdxy",200,-0.1,0.1);
    hdz =fs->make<TH1D>("hdz","hdz",200,-0.5,0.5);

    
    hd0vsphi  =fs->make<TH2D>("hd0vsphi"  ,"hd0vsphi"  ,160,-3.20,3.20,100,-1.,1.);
    hd0vseta  =fs->make<TH2D>("hd0vseta"  ,"hd0vseta"  ,160,-3.20,3.20,100,-1.,1.);
    hd0vspt   =fs->make<TH2D>("hd0vspt"   ,"hd0vspt"   ,50,0.,100.,100,-0.25,0.25);

    hnhpxb  =fs->make<TH1D>("nhpxb"  ,"nhpxb"  ,10,0.,10.);
    hnhpxe  =fs->make<TH1D>("nhpxe"  ,"nhpxe"  ,10,0.,10.);
    hnhTIB  =fs->make<TH1D>("nhTIB"  ,"nhTIB"  ,20,0.,20.);
    hnhTID  =fs->make<TH1D>("nhTID"  ,"nhTID"  ,20,0.,20.);
    hnhTOB  =fs->make<TH1D>("nhTOB"  ,"nhTOB"  ,20,0.,20.);
    hnhTEC  =fs->make<TH1D>("nhTEC"  ,"nhTEC"  ,20,0.,20.);

    hdxyBS=fs->make<TH1D>("hdxyBS","hdxyBS",600,-0.06,0.06);
    hd0BS=fs->make<TH1D>("hd0BS","hd0BS",600,-0.06,0.06);
    hdzBS=fs->make<TH1D>("hdzBS","hdzBS",1200,-12,12);
    hdxyPV=fs->make<TH1D>("hdxyPV","hdxyPV",200,-0.02,0.02);
    hd0PV=fs->make<TH1D>("hd0PV","hd0PV",200,-0.02,0.02);
    hdzPV=fs->make<TH1D>("hdzPV","hdzPV",200,-0.02,0.02);


    
    firstEvent_=true;

  }//beginJob

  virtual void endJob()
  { 

  }

};


DEFINE_FWK_MODULE(TrackAnalyzerHisto);

