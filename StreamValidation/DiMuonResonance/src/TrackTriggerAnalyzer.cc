#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

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
using namespace std;

using namespace edm;

class TrackTriggerAnalyzer : public edm::EDAnalyzer {
 public:
  TrackTriggerAnalyzer(const edm::ParameterSet& pset) {
    TkTag_ = pset.getParameter<string>("TkTag");
    theTrackCollectionToken = consumes<reco::TrackCollection>(TkTag_);
     
    InputTag tag("TriggerResults","","HLT"); 
    hltresultsToken = consumes<edm::TriggerResults>(tag);

    InputTag beamSpotTag("offlineBeamSpot");
    beamspotToken = consumes<reco::BeamSpot>(beamSpotTag);
    
    InputTag vertexTag("offlinePrimaryVertices");
    vertexToken   = consumes<reco::VertexCollection>(vertexTag);

  }

  ~TrackTriggerAnalyzer(){}

  edm::Service<TFileService> fs;
 
  //   BRANCH branch;

  TH1D *hchi2;
  TH1D *hNtrk;
  TH1D *hP;
  TH1D *hPPlus;
  TH1D *hPMinus;
  TH1D *hPt;
  TH1D *hPtZoomed;
  TH1D *hMinPt;
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

  TH1D * hdxyBS;
  TH1D * hd0BS;
  TH1D * hdzBS;
  TH1D * hdxyPV;
  TH1D * hd0PV;
  TH1D * hdzPV;
  TH1D *hrun;
  TH1D *hlumi;
  
  TH1D *tksByTrigger_;
  TH1D *evtsByTrigger_;

  std::string trigNames;
  
  int trigCount;
  int ievt;
  int itrks;
  int mol;
  float InvMass;
  double invMass;
  bool firstEvent_;
  const TrackerGeometry* trackerGeometry_;

  std::string TkTag_;
  edm::EDGetTokenT<reco::TrackCollection>  theTrackCollectionToken;
  edm::EDGetTokenT<edm::TriggerResults> hltresultsToken;
  edm::EDGetTokenT<reco::BeamSpot> beamspotToken;
  edm::EDGetTokenT<reco::VertexCollection> vertexToken;

  std::map<std::string,std::pair<int,int> > triggerMap_;

  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup){
    
    // extract tracker geometry
    //
    //     edm::ESHandle<TrackerGeometry> theG;
    //     setup.get<TrackerDigiGeometryRecord>().get(theG);
    
    //     if(firstEvent_){
    //      edm::ESHandle<TrackerGeometry> tmpTkGeometry;
    //      setup.get<TrackerDigiGeometryRecord>().get(tmpTkGeometry);
    //      trackerGeometry_=&(*tmpTkGeometry);
    //      firstEvent_=false;
    //     }
    
    ievt++;
    //std::cout << "event#" << ievt << " Event ID = "<< event.id() << std::endl ;
      
    edm::Handle<reco::TrackCollection>  trackCollection;
    event.getByToken(theTrackCollectionToken, trackCollection);
 
    //     event.getByType(trackCollection); 
    //     edm::Handle<reco::TrackCollection> globalMuons;
    //     event.getByLabel("globalMuons", globalMuons);    
    //     const reco::TrackCollection gMu= *(globalMuons.product());
        
    const reco::TrackCollection tC = *(trackCollection.product());
   
    itrks+=tC.size();
 
    // std::cout << "Reconstructed "<< tC.size() << " tracks" << std::endl ;
    TLorentzVector mother(0.,0.,0.,0.);
      
    edm::Handle<edm::TriggerResults> hltresults;
    event.getByToken(hltresultsToken,hltresults);

    edm::TriggerNames triggerNames_ = event.triggerNames(*hltresults);
    int ntrigs=hltresults->size();
    vector<string> triggernames = triggerNames_.triggerNames();
    
    for (int itrig = 0; itrig != ntrigs; ++itrig){
      string trigName=triggerNames_.triggerName(itrig);
      bool accept = hltresults->accept(itrig);
      if (accept== 1){
	// cout << trigName << " " << accept << " ,track size: " << tC.size() << endl;
	triggerMap_[trigName].first+=1;
	triggerMap_[trigName].second+=tC.size();
	// triggerInfo.push_back(pair <string, int> (trigName, accept));
      }
    }

    for (reco::TrackCollection::const_iterator track=tC.begin(); track!=tC.end(); track++){

      hHit->Fill(track->numberOfValidHits());
      hnhpxb->Fill( track->hitPattern().numberOfValidPixelBarrelHits());
      hnhpxe->Fill(track->hitPattern().numberOfValidPixelEndcapHits());
      hnhTIB->Fill(track->hitPattern().numberOfValidStripTIBHits());
      hnhTID->Fill(track->hitPattern().numberOfValidStripTIDHits());
      hnhTOB->Fill(track->hitPattern().numberOfValidStripTOBHits());
      hnhTEC->Fill(track->hitPattern().numberOfValidStripTECHits());
      hPt->Fill(track->pt());
      hPtZoomed->Fill(track->pt());
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
      hvx->Fill(track->vx());
      hvy->Fill(track->vy());
      hvz->Fill(track->vz());
      hd0vsphi ->Fill(track->phi(),track->d0());
      hd0vseta ->Fill(track->eta(), track->d0());
      hd0vspt  ->Fill(track->pt(), track->d0());
 
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
      reco::Vertex pvtx;
      edm::Handle<reco::VertexCollection> vertexHandle;
      reco::VertexCollection vertexCollection;
      event.getByToken(vertexToken,vertexHandle);
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

    hNtrk -> Fill (tC.size());

  }

  virtual void beginJob() {
    ievt=0;
    itrks=0;
    
    hchi2 = fs->make<TH1D>("hchi2","#chi^{2}/ndf",100,0,5.);
    hNtrk=fs->make<TH1D>("hNtrk","Number of Tracks",200,0.,200.);
    hP=fs->make<TH1D>("hP","Momentum",200,0.,200.);
    hPt=fs->make<TH1D>("hPt","Transverse Momentum",200,0.,200.);
    hPtZoomed=fs->make<TH1D>("hPtZoomed","Transverse Momentum",200,0.,20.);
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

    hnhpxb  =fs->make<TH1D>("nhpxb"  ,"nhpxb"  ,10,0.,10.);
    hnhpxe  =fs->make<TH1D>("nhpxe"  ,"nhpxe"  ,10,0.,10.);
    hnhTIB  =fs->make<TH1D>("nhTIB"  ,"nhTIB"  ,20,0.,20.);
    hnhTID  =fs->make<TH1D>("nhTID"  ,"nhTID"  ,20,0.,20.);
    hnhTOB  =fs->make<TH1D>("nhTOB"  ,"nhTOB"  ,20,0.,20.);
    hnhTEC  =fs->make<TH1D>("nhTEC"  ,"nhTEC"  ,20,0.,20.);

    hdxyBS=fs->make<TH1D>("hdxyBS","Track d_{xy}(BS);d_{xy}(BS) [cm];tracks",100,-100.,100.);
    hd0BS=fs->make<TH1D>("hd0BS","Track d_{0}(BS);d_{0}(BS) [cm];tracks",100,-100.,100.);
    hdzBS=fs->make<TH1D>("hdzBS","Track d_{z}(BS);d_{z}(BS) [cm];tracks",100,-100.,100.);
    hdxyPV=fs->make<TH1D>("hdxyPV","Track d_{xy}(PV); d_{xy}(PV) [cm];tracks",100,-100.,100.);
    hd0PV=fs->make<TH1D>("hd0PV","Track d_{0}(PV); d_{0}(PV) [cm];tracks",100,-100.,100.);
    hdzPV=fs->make<TH1D>("hdzPV","Track d_{z}(PV); d_{z}(PV) [cm];tracks",100,-100.,100.);

    firstEvent_=true;
    
    // hPPlus=fs->make<TH1D>("hPPlus","Momentum, #mu^{+}",200,0.,200.);
    // hPMinus=fs->make<TH1D>("hPMinus","Momentum, #mu^{-}",200,0.,200.);
    // MinPt=fs->make<TH1D>("hMinPt","Transverse Momentum, lowest of the muon pair",200,0.,200.);
    // PtPlus=fs->make<TH1D>("hPtPPlus","Transverse Momentum, #mu^{+}",200,0.,200.);
    // PtMinus=fs->make<TH1D>("hPtMinus","Transverse Momentum, #mu^{-}",200,0.,200.);
    // HitPlus=fs->make<TH1D>("hHitPlus","Number of hit, #mu^{+}",30,0,30);
    // HitMinus=fs->make<TH1D>("hHitMinus","Number of hit, #mu^{-}",30,0,30);
    // PhiPlus=fs->make<TH1D>("hPhiPlus","#phi, #mu^{+}",100,-4.,4.);
    // PhiMinus=fs->make<TH1D>("hPhiMinus","#phi, #mu^{-}",100,-4.,4.);
    // DeltaPhi=fs->make<TH1D>("hDeltaPhi","DeltaPhi",100,-10.,10.);
    // DeltaEta=fs->make<TH1D>("hDeltaEta","DeltaEta",100,-10.,10.);
    // DeltaR  =fs->make<TH1D>("R"  ,"R"  ,100,-1.,10.);
    // EtaPlus=fs->make<TH1D>("hEtaPlus","#eta, #mu^{+}",100,-4.,4.);
    // EtaMinus=fs->make<TH1D>("hEtaMinus","#eta, #mu^{-}",100,-4.,4.);

  }//beginJob

  virtual void endJob()
  { 
  
    std::cout<<"******************************"<<std::endl;
    std::cout<<"n. events: "<<ievt<<std::endl;
    std::cout<<"n. tracks: "<<itrks<<std::endl;
    std::cout<<"******************************"<<std::endl;

    Int_t nFiringTriggers = triggerMap_.size();
    std::cout<<"firing triggers: "<<nFiringTriggers<<std::endl;

    tksByTrigger_= fs->make<TH1D>("tksByTrigger","tracks by HLT path;;% of # traks",nFiringTriggers,-0.5,nFiringTriggers-0.5);
    evtsByTrigger_= fs->make<TH1D>("evtsByTrigge","events by HLT path;;% of # events",nFiringTriggers,-0.5,nFiringTriggers-0.5);
  
    Int_t i =0;
    
    for(std::map<std::string,std::pair<int,int> >::iterator it=triggerMap_.begin(); it!=triggerMap_.end(); ++it){
      i++;

      Double_t trkpercent = ((it->second).second)*100./Double_t(itrks);
      Double_t evtpercent = ((it->second).first)*100./Double_t(ievt);

      std::cout.precision(4);

      std::cout<<"HLT path: "<<std::setw(50) << left << it->first << " | events firing: " << right << std::setw(8)<<(it->second).first << " ("<< setw(8) << fixed << evtpercent <<"%)"
	       <<" | tracks collected: " <<std::setw(10)<< (it->second).second << " ("<< setw(8) << fixed << trkpercent <<"%)"<<  '\n';
      
      tksByTrigger_->SetBinContent(i,trkpercent);
      tksByTrigger_->GetXaxis()->SetBinLabel(i,(it->first).c_str());
      
      evtsByTrigger_->SetBinContent(i,evtpercent);
      evtsByTrigger_->GetXaxis()->SetBinLabel(i,(it->first).c_str());

    }
  }

};

DEFINE_FWK_MODULE(TrackTriggerAnalyzer);

