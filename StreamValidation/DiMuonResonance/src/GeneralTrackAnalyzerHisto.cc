#include "CondFormats/Common/interface/Time.h"
#include "Alignment/CommonAlignment/interface/Alignable.h"
#include "Alignment/CommonAlignment/interface/AlignableExtras.h"
#include "Alignment/CommonAlignment/interface/AlignableNavigator.h"
#include "Alignment/CommonAlignment/interface/AlignableObjectId.h"
#include "Alignment/CommonAlignment/interface/AlignmentParameters.h"
#include "Alignment/CommonAlignment/interface/SurveyResidual.h"
#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentParameterSelector.h"
#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentParameterStore.h"
#include "Alignment/HIPAlignmentAlgorithm/interface/HIPUserVariables.h"
#include "Alignment/HIPAlignmentAlgorithm/interface/HIPUserVariablesIORoot.h"
#include "Alignment/MuonAlignment/interface/AlignableMuon.h"
#include "Alignment/TrackerAlignment/interface/AlignableTracker.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CondFormats/AlignmentRecord/interface/GlobalPositionRcd.h"
#include "CondFormats/DataRecord/interface/AlCaRecoTriggerBitsRcd.h"
#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"
#include "CondFormats/HLTObjects/interface/AlCaRecoTriggerBits.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripDetId/interface/SiStripDetId.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit1D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiTrackerMultiRecHit.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"  
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ValidityInterval.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "IOMC/RandomEngine/src/RandomEngineStateProducer.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TBranch.h"
#include "TFile.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TLorentzVector.h"
#include "TProfile.h"
#include "TTree.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include <DataFormats/GeometrySurface/interface/LocalError.h>
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <vector>


class MagneticField;

#define DEBUG 0

using namespace std;
using namespace edm;

const int kBPIX = PixelSubdetector::PixelBarrel;
const int kFPIX = PixelSubdetector::PixelEndcap;

class GeneralTrackAnalyzerHisto : public edm::EDAnalyzer {
 public:
  GeneralTrackAnalyzerHisto(const edm::ParameterSet& pset) {
    TkTag_     = pset.getParameter<string>("TkTag");
    isCosmics_ = pset.getParameter<bool>("isCosmics");
  }

  ~GeneralTrackAnalyzerHisto(){}

  template <class OBJECT_TYPE>
  int GetIndex(const std::vector<OBJECT_TYPE*> &vec, const TString &name)
  {
    int result = 0;
    for (typename std::vector<OBJECT_TYPE*>::const_iterator iter = vec.begin(), iterEnd = vec.end();
	 iter != iterEnd; ++iter, ++result) {
      if (*iter && (*iter)->GetName() == name) return result;
    }
    edm::LogError("Alignment") << "@SUB=TrackerOfflineValidation::GetIndex" << " could not find " << name;
    return -1;
  }

  edm::Service<TFileService> fs;
  
  TTree* AnalysisTree;
  //   BRANCH branch;

  TH1D *hchi2  ;
  TH1D *hNtrk  ;
  TH1D *hNtrkZoom;
  TH1I *htrkQuality;
  TH1I *htrkAlgo;
  TH1D *hNhighPurity;
  TH1D *hP     ;
  TH1D *hPPlus ;
  TH1D *hPMinus;
  TH1D *hPt    ;
  TH1D *hMinPt ;
  TH1D *hPtPlus;
  TH1D *hPtMinus;
  TH1D *hHit;
  TH1D *hHit2D; 
  TH1D *hHitPlus;
  TH1D *hHitMinus;

  TH1D *hPhp;    
  TH1D *hPthp;   
  TH1D *hHithp;  
  TH1D *hEtahp; 
  TH1D *hPhihp;  
  TH1D *hchi2hp; 
  TH1D *hchi2Probhp;

  TH1D *hCharge;
  TH1D *hQoverP;
  TH1D *hQoverPZoom;
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
  TH1D *hdz;
  TH1D *hdxy;

  TH2D *hd0PVvsphi ;
  TH2D *hd0PVvseta;
  TH2D *hd0PVvspt;

  TH2D *hd0vsphi;
  TH2D *hd0vseta;
  TH2D *hd0vspt;

  TH1D *hnhpxb;
  TH1D *hnhpxe;
  TH1D *hnhTIB;
  TH1D *hnhTID;
  TH1D *hnhTOB;
  TH1D *hnhTEC;

  TH1D *hHitComposition;

  TProfile* pNBpixHitsVsVx;
  TProfile* pNBpixHitsVsVy;
  TProfile* pNBpixHitsVsVz;

  TH1D *hMultCand ;

  TH1D *hdxyBS;
  TH1D *hd0BS;
  TH1D *hdzBS;
  TH1D *hdxyPV;
  TH1D *hd0PV;
  TH1D *hdzPV;
  TH1D *hrun;
  TH1D *hlumi;

  std::vector<TH1*> vTrackHistos_;
  std::vector<TH1*> vTrackProfiles_;
  std::vector<TH1*> vTrack2DHistos_;

  std::string trigNames;
  int trigCount;
  int ievt;
  int mol;
  float InvMass;
  double invMass;
  bool firstEvent_;
  const TrackerGeometry* trackerGeometry_;

  std::string TkTag_;
  bool isCosmics_;

  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup){

    //typedef cond::Time_t Time_t;
    // const unsigned int MAX_VAL(std::numeric_limits<unsigned int>::max());

    // std::cout<<"====> MAXVAL: "<<MAX_VAL<<std::endl;

    // edm::ESHandle<Alignments> globalPositionRcd;
    // edm::ValidityInterval iov(setup.get<GlobalPositionRcd>().validityInterval() );
    // if (iov.first().eventID().run()!=1 || iov.last().eventID().run()!=MAX_VAL) {
    //   throw cms::Exception("DatabaseError")
    // 	<< "@SUB=AlignmentProducer::applyDB"
    // 	<< "\nTrying to apply "<< setup.get<GlobalPositionRcd>().key().name()
    // 	<< " with multiple IOVs in tag.\n"
    // 	<< "Validity range is "
    // 	<< iov.first().eventID().run() << " - " << iov.last().eventID().run();
    // }
    
    ievt++;
    
    edm::Handle<reco::TrackCollection> trackCollection;
    event.getByLabel(TkTag_, trackCollection);
    
    edm::ESHandle<MagneticField> magneticField_;
    setup.get<IdealMagneticFieldRecord>().get(magneticField_);

    GlobalPoint zeroPoint(0,0,0);
    //std::cout << "event#" << ievt << " Event ID = "<< event.id() 
    /// << " magnetic field: " << magneticField_->inTesla(zeroPoint) << std::endl ;
    
    const reco::TrackCollection tC = *(trackCollection.product());

    //std::cout << "Reconstructed "<< tC.size() << " tracks" << std::endl ;
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

    Int_t nHighPurityTracks = 0;

    for (reco::TrackCollection::const_iterator track=tC.begin(); track!=tC.end(); track++){

      unsigned int nHit2D = 0;
      for (trackingRecHit_iterator iHit = track->recHitsBegin(); iHit != track->recHitsEnd(); ++iHit) {
	if(this->isHit2D(**iHit)) ++nHit2D;
      }
      hHit2D->Fill(nHit2D);

      // std::cout << "nHit2D: "<<nHit2D<<std::endl;

      hHit->Fill(track->numberOfValidHits());
      hnhpxb->Fill(track->hitPattern().numberOfValidPixelBarrelHits());
      hnhpxe->Fill(track->hitPattern().numberOfValidPixelEndcapHits());
      hnhTIB->Fill(track->hitPattern().numberOfValidStripTIBHits());
      hnhTID->Fill(track->hitPattern().numberOfValidStripTIDHits());
      hnhTOB->Fill(track->hitPattern().numberOfValidStripTOBHits());
      hnhTEC->Fill(track->hitPattern().numberOfValidStripTECHits());
      
       // fill hit composition histogram
      if(track->hitPattern().numberOfValidPixelBarrelHits()!=0){
	hHitComposition->Fill(0.,track->hitPattern().numberOfValidPixelBarrelHits());

	pNBpixHitsVsVx->Fill(track->vx(),track->hitPattern().numberOfValidPixelBarrelHits());
	pNBpixHitsVsVy->Fill(track->vy(),track->hitPattern().numberOfValidPixelBarrelHits());
	pNBpixHitsVsVz->Fill(track->vz(),track->hitPattern().numberOfValidPixelBarrelHits());

      }
      if(track->hitPattern().numberOfValidPixelEndcapHits()!=0){
	hHitComposition->Fill(1.,track->hitPattern().numberOfValidPixelEndcapHits());
      }
      if(track->hitPattern().numberOfValidStripTIBHits()!=0){
	hHitComposition->Fill(2.,track->hitPattern().numberOfValidStripTIBHits());
      }
      if(track->hitPattern().numberOfValidStripTIDHits()!=0){
	hHitComposition->Fill(3.,track->hitPattern().numberOfValidStripTIDHits());
      }
      if(track->hitPattern().numberOfValidStripTOBHits()!=0){
	hHitComposition->Fill(4.,track->hitPattern().numberOfValidStripTOBHits());
      }
      if(track->hitPattern().numberOfValidStripTECHits()!=0){
	hHitComposition->Fill(5.,track->hitPattern().numberOfValidStripTECHits());
      }

      hCharge->Fill(track->charge());
      hQoverP->Fill(track->qoverp());
      hQoverPZoom->Fill(track->qoverp());
      hPt->Fill(track->pt());
      hP->Fill(track->p());
      hchi2->Fill(track->normalizedChi2());
      hEta->Fill(track->eta());
      hPhi->Fill(track->phi());

      //std::cout << "nHit2D: "<<nHit2D<<std::endl;
      
      if (fabs(track->eta() ) < 0.8 )                hPhiDT->Fill(track->phi());
      if (track->eta()>0.8 && track->eta()  < 1.4 )  hPhiRPCPlus->Fill(track->phi());
      if (track->eta()<-0.8 && track->eta() >- 1.4 ) hPhiRPCMinus->Fill(track->phi());
      if (track->eta()>1.4)                          hPhiCSCPlus->Fill(track->phi());
      if (track->eta()<-1.4)                         hPhiCSCMinus->Fill(track->phi());
     
      hd0->Fill(track->d0());
      hdz->Fill(track->dz());
      hdxy->Fill(track->dxy());
      hvx->Fill(track->vx());
      hvy->Fill(track->vy());
      hvz->Fill(track->vz());
    
      //std::cout << "nHit2D: "<<nHit2D<<std::endl;

      // int myalgo=-88;
      // if(track->algo()==reco::TrackBase::undefAlgorithm)myalgo=0;
      // if(track->algo()==reco::TrackBase::ctf)myalgo=1;
      // if(track->algo()==reco::TrackBase::iter0)myalgo=4;
      // if(track->algo()==reco::TrackBase::iter1)myalgo=5;
      // if(track->algo()==reco::TrackBase::iter2)myalgo=6;
      // if(track->algo()==reco::TrackBase::iter3)myalgo=7;
      // if(track->algo()==reco::TrackBase::iter4)myalgo=8;
      // if(track->algo()==reco::TrackBase::iter5)myalgo=9;
      // if(track->algo()==reco::TrackBase::iter6)myalgo=10;
      // if(track->algo()==reco::TrackBase::iter7)myalgo=11;
      // htrkAlgo->Fill(myalgo);
      
      int myquality=-99;
      if(track->quality(reco::TrackBase::undefQuality)){
	myquality=-1;
	htrkQuality->Fill(myquality);
      } 
      if(track->quality(reco::TrackBase::loose)){
	myquality=0;
	htrkQuality->Fill(myquality);
      }
      if(track->quality(reco::TrackBase::tight)){ 
	myquality=1;
	htrkQuality->Fill(myquality);
      }
      if(track->quality(reco::TrackBase::highPurity)){
	myquality=2;
	htrkQuality->Fill(myquality);
	hPhp->Fill(track->p());
	hPthp->Fill(track->pt());
	hHithp->Fill(track->numberOfValidHits()); 
	hEtahp->Fill(track->eta());
	hPhihp->Fill(track->phi());
	hchi2hp->Fill(track->normalizedChi2());
	hchi2Probhp->Fill(TMath::Prob(track->chi2(),track->ndof()));
	nHighPurityTracks++;
      }
      if(track->quality(reco::TrackBase::confirmed)){
	myquality=3;
	htrkQuality->Fill(myquality);
      }
      if(track->quality(reco::TrackBase::goodIterative)){
	myquality=4;
	htrkQuality->Fill(myquality);
      }
   
      // Fill 1D track histos
      static const int etaindex = this->GetIndex(vTrackHistos_,"h_tracketa");
      vTrackHistos_[etaindex]->Fill(track->eta());
      static const int phiindex = this->GetIndex(vTrackHistos_,"h_trackphi");
      vTrackHistos_[phiindex]->Fill(track->phi());
      static const int numOfValidHitsindex = this->GetIndex(vTrackHistos_,"h_trackNumberOfValidHits");
      vTrackHistos_[numOfValidHitsindex]->Fill(track->numberOfValidHits());
      static const int numOfLostHitsindex = this->GetIndex(vTrackHistos_,"h_trackNumberOfLostHits");
      vTrackHistos_[numOfLostHitsindex]->Fill(track->numberOfLostHits());

      GlobalPoint gPoint(track->vx(), track->vy(), track->vz());
      double theLocalMagFieldInInverseGeV = magneticField_->inInverseGeV(gPoint).z();
      double kappa =  -track->charge()*theLocalMagFieldInInverseGeV/track->pt();

      static const int kappaindex = this->GetIndex(vTrackHistos_,"h_curvature");
      vTrackHistos_[kappaindex]->Fill(kappa);
      static const int kappaposindex = this->GetIndex(vTrackHistos_,"h_curvature_pos");
      if (track->charge() > 0)
      	vTrackHistos_[kappaposindex]->Fill(fabs(kappa));
      static const int kappanegindex = this->GetIndex(vTrackHistos_,"h_curvature_neg");
      if (track->charge() < 0)
      	vTrackHistos_[kappanegindex]->Fill(fabs(kappa));

      double chi2Prob= TMath::Prob(track->chi2(),track->ndof());
      double normchi2 = track->normalizedChi2();

      static const int normchi2index = this->GetIndex(vTrackHistos_,"h_normchi2");
      vTrackHistos_[normchi2index]->Fill(normchi2);
      static const int chi2index = this->GetIndex(vTrackHistos_,"h_chi2");
      vTrackHistos_[chi2index]->Fill(track->chi2());
      static const int chi2Probindex = this->GetIndex(vTrackHistos_,"h_chi2Prob");
      vTrackHistos_[chi2Probindex]->Fill(chi2Prob);
      static const int ptindex = this->GetIndex(vTrackHistos_,"h_pt");
      static const int pt2index = this->GetIndex(vTrackHistos_,"h_ptrebin");
      vTrackHistos_[ptindex]->Fill(track->pt());
      vTrackHistos_[pt2index]->Fill(track->pt());
      if (track->ptError() != 0.) {
      	static const int ptResolutionindex = this->GetIndex(vTrackHistos_,"h_ptResolution");
      	vTrackHistos_[ptResolutionindex]->Fill(track->ptError()/track->pt());
      }
      // Fill track profiles
      static const int d0phiindex = this->GetIndex(vTrackProfiles_,"p_d0_vs_phi");
      vTrackProfiles_[d0phiindex]->Fill(track->phi(),track->d0());
      static const int dzphiindex = this->GetIndex(vTrackProfiles_,"p_dz_vs_phi");
      vTrackProfiles_[dzphiindex]->Fill(track->phi(),track->dz());
      static const int d0etaindex = this->GetIndex(vTrackProfiles_,"p_d0_vs_eta");
      vTrackProfiles_[d0etaindex]->Fill(track->eta(),track->d0());
      static const int dzetaindex = this->GetIndex(vTrackProfiles_,"p_dz_vs_eta");
      vTrackProfiles_[dzetaindex]->Fill(track->eta(),track->dz());
      static const int chiProbphiindex = this->GetIndex(vTrackProfiles_,"p_chi2Prob_vs_phi");
      vTrackProfiles_[chiProbphiindex]->Fill(track->phi(),chi2Prob);
      static const int chiProbabsd0index = this->GetIndex(vTrackProfiles_,"p_chi2Prob_vs_d0");
      vTrackProfiles_[chiProbabsd0index]->Fill(fabs(track->d0()),chi2Prob);
      static const int chiProbabsdzindex = this->GetIndex(vTrackProfiles_,"p_chi2Prob_vs_dz");
      vTrackProfiles_[chiProbabsdzindex]->Fill(track->dz(),chi2Prob);
      static const int chiphiindex = this->GetIndex(vTrackProfiles_,"p_chi2_vs_phi");
      vTrackProfiles_[chiphiindex]->Fill(track->phi(),track->chi2());
      static const int normchiphiindex = this->GetIndex(vTrackProfiles_,"p_normchi2_vs_phi");
      vTrackProfiles_[normchiphiindex]->Fill(track->phi(),normchi2);
      static const int chietaindex = this->GetIndex(vTrackProfiles_,"p_chi2_vs_eta");
      vTrackProfiles_[chietaindex]->Fill(track->eta(),track->chi2());
      static const int normchiptindex = this->GetIndex(vTrackProfiles_,"p_normchi2_vs_pt");
      vTrackProfiles_[normchiptindex]->Fill(track->pt(),normchi2);
      static const int normchipindex = this->GetIndex(vTrackProfiles_,"p_normchi2_vs_p");
      vTrackProfiles_[normchipindex]->Fill(track->p(),normchi2);
      static const int chiProbetaindex = this->GetIndex(vTrackProfiles_,"p_chi2Prob_vs_eta");
      vTrackProfiles_[chiProbetaindex]->Fill(track->eta(),chi2Prob);
      static const int normchietaindex = this->GetIndex(vTrackProfiles_,"p_normchi2_vs_eta");
      vTrackProfiles_[normchietaindex]->Fill(track->eta(),normchi2);
      static const int kappaphiindex = this->GetIndex(vTrackProfiles_,"p_kappa_vs_phi");
      vTrackProfiles_[kappaphiindex]->Fill(track->phi(),kappa);
      static const int kappaetaindex = this->GetIndex(vTrackProfiles_,"p_kappa_vs_eta");
      vTrackProfiles_[kappaetaindex]->Fill(track->eta(),kappa);
      static const int ptResphiindex = this->GetIndex(vTrackProfiles_,"p_ptResolution_vs_phi");
      vTrackProfiles_[ptResphiindex]->Fill(track->phi(),track->ptError()/track->pt());
      static const int ptResetaindex = this->GetIndex(vTrackProfiles_,"p_ptResolution_vs_eta");
      vTrackProfiles_[ptResetaindex]->Fill(track->eta(),track->ptError()/track->pt());
      
      // Fill 2D track histos
      static const int d0phiindex_2d = this->GetIndex(vTrack2DHistos_,"h2_d0_vs_phi");
      vTrack2DHistos_[d0phiindex_2d]->Fill(track->phi(),track->d0());
      static const int dzphiindex_2d = this->GetIndex(vTrack2DHistos_,"h2_dz_vs_phi");
      vTrack2DHistos_[dzphiindex_2d]->Fill(track->phi(),track->dz());
      static const int d0etaindex_2d = this->GetIndex(vTrack2DHistos_,"h2_d0_vs_eta");
      vTrack2DHistos_[d0etaindex_2d]->Fill(track->eta(),track->d0());
      static const int dzetaindex_2d = this->GetIndex(vTrack2DHistos_,"h2_dz_vs_eta");
      vTrack2DHistos_[dzetaindex_2d]->Fill(track->eta(),track->dz());
      static const int chiphiindex_2d = this->GetIndex(vTrack2DHistos_,"h2_chi2_vs_phi");
      vTrack2DHistos_[chiphiindex_2d]->Fill(track->phi(),track->chi2());
      static const int chiProbphiindex_2d = this->GetIndex(vTrack2DHistos_,"h2_chi2Prob_vs_phi");
      vTrack2DHistos_[chiProbphiindex_2d]->Fill(track->phi(),chi2Prob);
      static const int chiProbabsd0index_2d = this->GetIndex(vTrack2DHistos_,"h2_chi2Prob_vs_d0");
      vTrack2DHistos_[chiProbabsd0index_2d]->Fill(fabs(track->d0()),chi2Prob);
      static const int normchiphiindex_2d = this->GetIndex(vTrack2DHistos_,"h2_normchi2_vs_phi");
      vTrack2DHistos_[normchiphiindex_2d]->Fill(track->phi(),normchi2);
      static const int chietaindex_2d = this->GetIndex(vTrack2DHistos_,"h2_chi2_vs_eta");
      vTrack2DHistos_[chietaindex_2d]->Fill(track->eta(),track->chi2());
      static const int chiProbetaindex_2d = this->GetIndex(vTrack2DHistos_,"h2_chi2Prob_vs_eta");
      vTrack2DHistos_[chiProbetaindex_2d]->Fill(track->eta(),chi2Prob);
      static const int normchietaindex_2d = this->GetIndex(vTrack2DHistos_,"h2_normchi2_vs_eta");
      vTrack2DHistos_[normchietaindex_2d]->Fill(track->eta(),normchi2);
      static const int kappaphiindex_2d = this->GetIndex(vTrack2DHistos_,"h2_kappa_vs_phi");
      vTrack2DHistos_[kappaphiindex_2d]->Fill(track->phi(),kappa);
      static const int kappaetaindex_2d = this->GetIndex(vTrack2DHistos_,"h2_kappa_vs_eta");
      vTrack2DHistos_[kappaetaindex_2d]->Fill(track->eta(),kappa);
      static const int normchi2kappa_2d = this->GetIndex(vTrack2DHistos_,"h2_normchi2_vs_kappa");
      vTrack2DHistos_[normchi2kappa_2d]->Fill(normchi2,kappa);
      
      //std::cout << "filling histos"<<std::endl;

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
      
      // //dxy with respect to the primary vertex
      // reco::Vertex pvtx;
      // edm::Handle<reco::VertexCollection> vertexHandle;
      // reco::VertexCollection vertexCollection;
      // event.getByLabel("offlinePrimaryVertices",vertexHandle);
      // double mindxy = 100.;
      // double dz = 100;
      // if(vertexHandle.isValid()) {
      // 	for(reco::VertexCollection::const_iterator pvtx = vertexHandle->begin(); pvtx!=vertexHandle->end(); ++pvtx) {
      // 	  math::XYZPoint mypoint( pvtx->x(), pvtx->y(), pvtx->z());
      // 	  if(abs(mindxy)>abs(track->dxy(mypoint))){
      // 	    mindxy = track->dxy(mypoint);
      // 	    dz=track->dz(mypoint);
      // 	    std::cout<<"dxy: "<<mindxy<<"dz: "<<dz<<std::endl;
      // 	  }
      // 	}

      // 	hdxyPV->Fill(mindxy);
      // 	hd0PV->Fill(-mindxy);
      // 	hdzPV->Fill(dz);
	
      // 	hd0PVvsphi->Fill(track->phi(),-mindxy);
      // 	hd0PVvseta->Fill(track->eta(),-mindxy);
      // 	hd0PVvspt->Fill(track->pt(),-mindxy);

      // } else {
      // 	hdxyPV->Fill(100);
      // 	hd0PV->Fill(100);
      // 	hdzPV->Fill(100);
      // }

      // std::cout<<"end of track loop"<<std::endl;

    }

    // std::cout<<"end of analysis"<<std::endl;

    hNtrk->Fill(tC.size());
    hNtrkZoom->Fill(tC.size());
    hNhighPurity->Fill(nHighPurityTracks);
    
    // std::cout<<"end of analysis"<<std::endl;
    
  }

  virtual void beginJob() {
    if (DEBUG) cout << __LINE__ << endl;

    TH1D::SetDefaultSumw2(kTRUE);

    ievt=0;
    hrun  = fs->make<TH1D>("hrun","run",1000000,0,1000000);
    hlumi = fs->make<TH1D>("hlumi","lumi",1000,0,1000);
    
    hchi2 = fs->make<TH1D>("hchi2","chi2/ndf;#chi^{2}/ndf;tracks",100,0,5.);
    hCharge = fs->make<TH1D>("hcharge","charge;Charge of the track;tracks",5,-2.5,2.5);
    hNtrk = fs->make<TH1D>("hNtrk","ntracks;Number of Tracks;events",200,0.,200.);
    hNtrkZoom=fs->make<TH1D>("hNtrkZoom","Number of tracks; number of tracks;events",10,0.,10.);
    hNhighPurity = fs->make<TH1D>("hNhighPurity","n. high purity tracks;Number of high purity tracks;events",200,0.,200.);

    htrkAlgo     = fs->make<TH1I>("htrkAlgo","tracking step;iterative tracking step;tracks",12,-0.5,11.5);
    TString algos[12] = {"undef","ctf","rs","cosmic","iter0","iter1","iter2","iter3","iter4","iter5","iter6","iter7"};
    for(int nbin=1;nbin<=htrkAlgo->GetNbinsX();nbin++){
      htrkAlgo->GetXaxis()->SetBinLabel(nbin,algos[nbin-1]);
    }

    htrkQuality  = fs->make<TH1I>("htrkQuality","track quality;track quality;tracks",6,-1,5);
    TString qualities[7] = {"undef","loose","tight","highPurity","confirmed","goodIterative"};
    for(int nbin=1;nbin<=htrkQuality->GetNbinsX();nbin++){
      htrkQuality->GetXaxis()->SetBinLabel(nbin,qualities[nbin-1]);
    }
    
    hP      = fs->make<TH1D>("hP","Momentum;track momentum [GeV];tracks",100,0.,100.);
    hQoverP = fs->make<TH1D>("hqoverp","Track q/p; track q/p [GeV^{-1}];tracks",100,-1.,1.);
    hQoverPZoom = fs->make<TH1D>("hqoverpZoom","Track q/p; track q/p [GeV^{-1}];tracks",100,-0.1,0.1); 
    hPt     = fs->make<TH1D>("hPt","Transverse Momentum;track p_{T} [GeV];tracks",100,0.,100.);
    hHit    = fs->make<TH1D>("hHit","Number of hits;track n. hits;tracks",50,-0.5,49.5);
    hHit2D  = fs->make<TH1D>("hHit2D","Number of 2D hits; number of 2D hits;tracks",20,0,20);
    hEta    = fs->make<TH1D>("hEta","Track pseudorapidity; track #eta;tracks",100,-TMath::Pi(),TMath::Pi());
    hPhi    = fs->make<TH1D>("hPhi","Track azimuth; track #phi;tracks",100,-TMath::Pi(),TMath::Pi());
    
    hPhp    = fs->make<TH1D>("hPhp","Momentum (high purity);track momentum [GeV];tracks",100,0.,100.);
    hPthp   = fs->make<TH1D>("hPthp","Transverse Momentum (high purity);track p_{T} [GeV];tracks",100,0.,100.);
    hHithp  = fs->make<TH1D>("hHithp","Number of hits (high purity);track n. hits;tracks",30,0,30);
    hEtahp  = fs->make<TH1D>("hEtahp","Track pseudorapidity (high purity); track #eta;tracks",100,-TMath::Pi(),TMath::Pi());
    hPhihp  = fs->make<TH1D>("hPhihp","Track azimuth (high purity); track #phi;tracks",100,-TMath::Pi(),TMath::Pi());
    hchi2hp = fs->make<TH1D>("hchi2","chi2/ndf (high purity);#chi^{2}/ndf;tracks",100,0,5.);
    hchi2Probhp = fs->make<TH1D>("hchiProbhp","#chi^{2} probability (high purity);#chi^{2}prob_{Track};Number of Tracks",100,0.0,1.);

    hPhiDT       = fs->make<TH1D>("hPhiDT","hPhiDT",100,-TMath::Pi(),TMath::Pi());
    hPhiRPCPlus  = fs->make<TH1D>("hPhiRPCPlus","hPhiRPCPlus;track #phi;tracks",100,-TMath::Pi(),TMath::Pi());
    hPhiRPCMinus = fs->make<TH1D>("hPhiRPCMinus","hPhiRPCMinus;track #phi;tracks",100,-TMath::Pi(),TMath::Pi());
    hPhiCSCPlus  = fs->make<TH1D>("hPhiCSCPlus","hPhiCSCPlus;track #phi;track",100,-TMath::Pi(),TMath::Pi());
    hPhiCSCMinus = fs->make<TH1D>("hPhiCSCMinus","hPhiCSCMinus;track #phi;tracks",100,-TMath::Pi(),TMath::Pi());
    
    if(!isCosmics_){
      hvx  = fs->make<TH1D>("hvx" ,"hvx; track v_{x} [cm];tracks",100,-1.5,1.5);
      hvy  = fs->make<TH1D>("hvy" ,"hvy; track v_{y} [cm];tracks",100,-1.5,1.5);
      hvz  = fs->make<TH1D>("hvz" ,"hvz; track v_{z} [cm];tracks",100,-20.,20.);
      hd0  = fs->make<TH1D>("hd0" ,"hd0; track d_{0} [cm];tracks",100,-1. ,1.);
      hdxy = fs->make<TH1D>("hdxy","hdxy;track d_{xy} [cm]; tracks",100,-0.5,0.5);
      hdz  = fs->make<TH1D>("hdz" ,"hdz; track d_{z} [cm]; tracks" ,100,-20,20);

      hd0PVvsphi = fs->make<TH2D>("hd0PVvsphi","hd0PVvsphi;track #phi;track d_{0}(PV) [cm]",160,-TMath::Pi(),TMath::Pi(),100,-1.,1.);
      hd0PVvseta = fs->make<TH2D>("hd0PVvseta","hdPV0vseta;track #eta;track d_{0}(PV) [cm]",160,-2.5,2.5,100,-1.,1.);
      hd0PVvspt  = fs->make<TH2D>("hd0PVvspt" ,"hdPV0vspt;track p_{T};d_{0}(PV) [cm]"      ,50,0.,100.,100,-1,1.);
     
      hdxyBS = fs->make<TH1D>("hdxyBS","hdxyBS; track d_{xy}(BS) [cm];tracks",100,-0.1,0.1);
      hd0BS  = fs->make<TH1D>("hd0BS" ,"hd0BS ; track d_{0}(BS) [cm];tracks" ,100,-0.1,0.1);
      hdzBS  = fs->make<TH1D>("hdzBS" ,"hdzBS ; track d_{z}(BS) [cm];tracks" ,100,-12,12);
      hdxyPV = fs->make<TH1D>("hdxyPV","hdxyPV; track d_{xy}(PV) [cm];tracks",100,-0.1,0.1);
      hd0PV  = fs->make<TH1D>("hd0PV" ,"hd0PV ; track d_{0}(PV) [cm];tracks" ,100,-0.15,0.15);
      hdzPV  = fs->make<TH1D>("hdzPV" ,"hdzPV ; track d_{z}(PV) [cm];tracks" ,100,-0.1,0.1);
      
      hnhTIB = fs->make<TH1D>("nhTIB"  ,"nhTIB;# hits in TIB; tracks"  ,20,0.,20.);
      hnhTID = fs->make<TH1D>("nhTID"  ,"nhTID;# hits in TID; tracks"  ,20,0.,20.);
      hnhTOB = fs->make<TH1D>("nhTOB"  ,"nhTOB;# hits in TOB; tracks"  ,20,0.,20.);
      hnhTEC = fs->make<TH1D>("nhTEC"  ,"nhTEC;# hits in TEC; tracks"  ,20,0.,20.);
      
    } else {
      
      hvx=fs->make<TH1D>("hvx","Track v_{x};track v_{x} [cm];tracks",100,-100.,100.);
      hvy=fs->make<TH1D>("hvy","Track v_{y};track v_{y} [cm];tracks",100,-100.,100.);
      hvz=fs->make<TH1D>("hvz","Track v_{z};track v_{z} [cm];track",100,-100.,100.);
      hd0=fs->make<TH1D>("hd0","Track d_{0};track d_{0} [cm];track",100,-100.,100.);
      hdxy=fs->make<TH1D>("hdxy","Track d_{xy};track d_{xy} [cm];tracks",100,-100,100);
      hdz=fs->make<TH1D>("hdz","Track d_{z};track d_{z} [cm];tracks",100,-200,200);

      hd0vsphi=fs->make<TH2D>("hd0vsphi","Track d_{0} vs #phi; track #phi;track d_{0} [cm]",160,-3.20,3.20,100,-100.,100.);
      hd0vseta=fs->make<TH2D>("hd0vseta","Track d_{0} vs #eta; track #eta;track d_{0} [cm]",160,-3.20,3.20,100,-100.,100.);
      hd0vspt=fs->make<TH2D>("hd0vspt","Track d_{0} vs p_{T};track p_{T};track d_{0} [cm]",50,0.,100.,100,-100,100);
      
      hdxyBS=fs->make<TH1D>("hdxyBS","Track d_{xy}(BS);d_{xy}(BS) [cm];tracks",100,-100.,100.);
      hd0BS=fs->make<TH1D>("hd0BS","Track d_{0}(BS);d_{0}(BS) [cm];tracks",100,-100.,100.);
      hdzBS=fs->make<TH1D>("hdzBS","Track d_{z}(BS);d_{z}(BS) [cm];tracks",100,-100.,100.);
      hdxyPV=fs->make<TH1D>("hdxyPV","Track d_{xy}(PV); d_{xy}(PV) [cm];tracks",100,-100.,100.);
      hd0PV=fs->make<TH1D>("hd0PV","Track d_{0}(PV); d_{0}(PV) [cm];tracks",100,-100.,100.);
      hdzPV=fs->make<TH1D>("hdzPV","Track d_{z}(PV); d_{z}(PV) [cm];tracks",100,-100.,100.);

      hnhTIB = fs->make<TH1D>("nhTIB"  ,"nhTIB;# hits in TIB; tracks"  ,30,0.,30.);
      hnhTID = fs->make<TH1D>("nhTID"  ,"nhTID;# hits in TID; tracks"  ,30,0.,30.);
      hnhTOB = fs->make<TH1D>("nhTOB"  ,"nhTOB;# hits in TOB; tracks"  ,30,0.,30.);
      hnhTEC = fs->make<TH1D>("nhTEC"  ,"nhTEC;# hits in TEC; tracks"  ,30,0.,30.);
       
    }

    hnhpxb = fs->make<TH1D>("nhpxb"  ,"nhpxb;# hits in Pixel Barrel; tracks"  ,10,0.,10.);
    hnhpxe = fs->make<TH1D>("nhpxe"  ,"nhpxe;# hits in Pixel Endcap; tracks"  ,10,0.,10.);
    
    hHitComposition =  fs->make<TH1D>("h_hitcomposition","track hit composition;;# hits",6,-0.5,5.5);
    
    pNBpixHitsVsVx = fs->make<TProfile>("p_NpixHits_vs_Vx",
					"n. Barrel Pixel hits vs. v_{x};v_{x} (cm);n. BPix hits",
					20,-20,20);

    pNBpixHitsVsVy = fs->make<TProfile>("p_NpixHits_vs_Vy",
					"n. Barrel Pixel hits vs. v_{y};v_{y} (cm);n. BPix hits",
					20,-20,20);

    pNBpixHitsVsVz = fs->make<TProfile>("p_NpixHits_vs_Vz",
					"n. Barrel Pixel hits vs. v_{z};v_{z} (cm);n. BPix hits",
					20,-100,100);

    TString dets[6] = {"PXB","PXF","TIB","TID","TOB","TEC"};
    
    for(Int_t i=1;i<=hHitComposition->GetNbinsX();i++){
      hHitComposition->GetXaxis()->SetBinLabel(i,dets[i-1]);
    }

    vTrackHistos_.push_back(fs->make<TH1F>("h_tracketa",
					   "Track #eta;#eta_{Track};Number of Tracks",
					   90,-3.,3.));
    vTrackHistos_.push_back(fs->make<TH1F>("h_trackphi",
					   "Track #phi;#phi_{Track};Number of Tracks",
					   90,-3.15,3.15));
    vTrackHistos_.push_back(fs->make<TH1F>("h_trackNumberOfValidHits",
					   "Track # of valid hits;# of valid hits _{Track};Number of Tracks",
					   40,0.,40.));
    vTrackHistos_.push_back(fs->make<TH1F>("h_trackNumberOfLostHits",
					   "Track # of lost hits;# of lost hits _{Track};Number of Tracks",
					   10,0.,10.));
    vTrackHistos_.push_back(fs->make<TH1F>("h_curvature",
					   "Curvature #kappa;#kappa_{Track};Number of Tracks",
					   100,-.05,.05));
    vTrackHistos_.push_back(fs->make<TH1F>("h_curvature_pos",
					   "Curvature |#kappa| Positive Tracks;|#kappa_{pos Track}|;Number of Tracks",
					   100,.0,.05));
    vTrackHistos_.push_back(fs->make<TH1F>("h_curvature_neg",
					   "Curvature |#kappa| Negative Tracks;|#kappa_{neg Track}|;Number of Tracks",
					   100,.0,.05));
    vTrackHistos_.push_back(fs->make<TH1F>("h_diff_curvature",
					   "Curvature |#kappa| Tracks Difference;|#kappa_{Track}|;# Pos Tracks - # Neg Tracks",
					   100,.0,.05));
    vTrackHistos_.push_back(fs->make<TH1F>("h_chi2",
					   "#chi^{2};#chi^{2}_{Track};Number of Tracks",
					   500,-0.01,500.));
    vTrackHistos_.push_back(fs->make<TH1F>("h_chi2Prob",
					   "#chi^{2} probability;#chi^{2}prob_{Track};Number of Tracks",
					   100,0.0,1.));	
    vTrackHistos_.push_back(fs->make<TH1F>("h_normchi2",
					   "#chi^{2}/ndof;#chi^{2}/ndof;Number of Tracks",
					   100,-0.01,10.));
    //variable binning for chi2/ndof vs. pT
    double xBins[19]={0.,0.15,0.5,1.,1.5,2.,2.5,3.,3.5,4.,4.5,5.,7.,10.,15.,25.,40.,100.,200.};
    vTrackHistos_.push_back(fs->make<TH1F>("h_pt",
					   "p_{T}^{track};p_{T}^{track} [GeV];Number of Tracks",
					   250,0.,250));
    vTrackHistos_.push_back(fs->make<TH1F>("h_ptrebin",
					   "p_{T}^{track};p_{T}^{track} [GeV];Number of Tracks",
					   18,xBins));
    vTrackHistos_.push_back(fs->make<TH1F>("h_ptResolution",
					   "#delta_{p_{T}}/p_{T}^{track};#delta_{p_{T}}/p_{T}^{track};Number of Tracks",
					   100,0.,0.5));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_d0_vs_phi",
						 "Transverse Impact Parameter vs. #phi;#phi_{Track};#LT d_{0} #GT [cm]",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_dz_vs_phi",
						 "Longitudinal Impact Parameter vs. #phi;#phi_{Track};#LT d_{z} #GT [cm]",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_d0_vs_eta",
						 "Transverse Impact Parameter vs. #eta;#eta_{Track};#LT d_{0} #GT [cm]",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_dz_vs_eta",
						 "Longitudinal Impact Parameter vs. #eta;#eta_{Track};#LT d_{z} #GT [cm]",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_chi2_vs_phi",
						 "#chi^{2} vs. #phi;#phi_{Track};#LT #chi^{2} #GT",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_chi2Prob_vs_phi",
						 "#chi^{2} probablility vs. #phi;#phi_{Track};#LT #chi^{2} probability#GT",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_chi2Prob_vs_d0",
						 "#chi^{2} probablility vs. |d_{0}|;|d_{0}|[cm];#LT #chi^{2} probability#GT",
						 100,0,80));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_chi2Prob_vs_dz",
						 "#chi^{2} probablility vs. dz;d_{z} [cm];#LT #chi^{2} probability#GT",
						 100,-30,30));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_normchi2_vs_phi",
						 "#chi^{2}/ndof vs. #phi;#phi_{Track};#LT #chi^{2}/ndof #GT",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_chi2_vs_eta",
						 "#chi^{2} vs. #eta;#eta_{Track};#LT #chi^{2} #GT",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_normchi2_vs_pt",
						 "norm #chi^{2} vs. p_{T}_{Track}; p_{T}_{Track};#LT #chi^{2}/ndof #GT",
						 18,xBins));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_normchi2_vs_p",
						 "#chi^{2}/ndof vs. p_{Track};p_{Track};#LT #chi^{2}/ndof #GT",
						 18,xBins));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_chi2Prob_vs_eta",
						 "#chi^{2} probability vs. #eta;#eta_{Track};#LT #chi^{2} probability #GT",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_normchi2_vs_eta",
						 "#chi^{2}/ndof vs. #eta;#eta_{Track};#LT #chi^{2}/ndof #GT",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_kappa_vs_phi",
						 "#kappa vs. #phi;#phi_{Track};#kappa",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_kappa_vs_eta",
						 "#kappa vs. #eta;#eta_{Track};#kappa",
						 100,-3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_ptResolution_vs_phi",
						 "#delta_{p_{T}}/p_{T}^{track};#phi^{track};#delta_{p_{T}}/p_{T}^{track}",
						 100, -3.15,3.15));
    vTrackProfiles_.push_back(fs->make<TProfile>("p_ptResolution_vs_eta",
						 "#delta_{p_{T}}/p_{T}^{track};#eta^{track};#delta_{p_{T}}/p_{T}^{track}",
						 100, -3.15,3.15));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_d0_vs_phi",
					     "Transverse Impact Parameter vs. #phi;#phi_{Track};d_{0} [cm]",
					     100, -3.15, 3.15, 100,-1.,1.) );
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_dz_vs_phi",
					     "Longitudinal Impact Parameter vs. #phi;#phi_{Track};d_{z} [cm]",
					     100, -3.15, 3.15, 100,-100.,100.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_d0_vs_eta",
					     "Transverse Impact Parameter vs. #eta;#eta_{Track};d_{0} [cm]",
					     100, -3.15, 3.15, 100,-1.,1.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_dz_vs_eta",
					     "Longitudinal Impact Parameter vs. #eta;#eta_{Track};d_{z} [cm]",
					     100, -3.15, 3.15, 100,-100.,100.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_chi2_vs_phi",
					     "#chi^{2} vs. #phi;#phi_{Track};#chi^{2}",
					     100, -3.15, 3.15, 500, 0., 500.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_chi2Prob_vs_phi",
					     "#chi^{2} probability vs. #phi;#phi_{Track};#chi^{2} probability",
					     100, -3.15, 3.15, 100, 0., 1.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_chi2Prob_vs_d0",
					     "#chi^{2} probability vs. |d_{0}|;|d_{0}| [cm];#chi^{2} probability",
					     100, 0, 80, 100, 0., 1.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_normchi2_vs_phi",
					     "#chi^{2}/ndof vs. #phi;#phi_{Track};#chi^{2}/ndof",
					     100, -3.15, 3.15, 100, 0., 10.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_chi2_vs_eta",
					     "#chi^{2} vs. #eta;#eta_{Track};#chi^{2}",
					     100, -3.15, 3.15, 500, 0., 500.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_chi2Prob_vs_eta",
					     "#chi^{2} probaility vs. #eta;#eta_{Track};#chi^{2} probability",
					     100, -3.15, 3.15, 100, 0., 1.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_normchi2_vs_eta",
					     "#chi^{2}/ndof vs. #eta;#eta_{Track};#chi^{2}/ndof",
					     100,-3.15,3.15, 100, 0., 10.));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_kappa_vs_phi",
					     "#kappa vs. #phi;#phi_{Track};#kappa",
					     100,-3.15,3.15, 100, .0,.05));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_kappa_vs_eta",
					     "#kappa vs. #eta;#eta_{Track};#kappa",
					     100,-3.15,3.15, 100, .0,.05));
    vTrack2DHistos_.push_back(fs->make<TH2F>("h2_normchi2_vs_kappa",
					     "#kappa vs. #chi^{2}/ndof;#chi^{2}/ndof;#kappa",
					     100,0.,10, 100,-.03,.03));
    
    firstEvent_=true;

  }//beginJob

  virtual void endJob()
  { 
    std::cout<< "*******************************" << std::endl;
    std::cout<< "Events run in total: " << ievt << std::endl;
    std::cout<< "*******************************" << std::endl;
  }

  bool isHit2D(const TrackingRecHit &hit) 
  {
    bool countStereoHitAs2D_ = true;
    // we count SiStrip stereo modules as 2D if selected via countStereoHitAs2D_
    // (since they provide theta information)
    if (!hit.isValid() ||
	(hit.dimension() < 2 && !countStereoHitAs2D_ && !dynamic_cast<const SiStripRecHit1D*>(&hit))){
      return false; // real RecHit1D - but SiStripRecHit1D depends on countStereoHitAs2D_
    } else {
      const DetId detId(hit.geographicalId());
      if (detId.det() == DetId::Tracker) {
	if (detId.subdetId() == kBPIX || detId.subdetId() == kFPIX) {
	  return true; // pixel is always 2D
	} else { // should be SiStrip now
	  const SiStripDetId stripId(detId);
	  if (stripId.stereo()) return countStereoHitAs2D_; // stereo modules
	  else if (dynamic_cast<const SiStripRecHit1D*>(&hit)
		   || dynamic_cast<const SiStripRecHit2D*>(&hit)) return false; // rphi modules hit
	  //the following two are not used any more since ages...
	  else if (dynamic_cast<const SiStripMatchedRecHit2D*>(&hit)) return true; // matched is 2D
	  else if (dynamic_cast<const ProjectedSiStripRecHit2D*>(&hit)) {
	    const ProjectedSiStripRecHit2D* pH = static_cast<const ProjectedSiStripRecHit2D*>(&hit);
	    return (countStereoHitAs2D_ && this->isHit2D(pH->originalHit())); // depends on original...
	  } else {
	    edm::LogError("UnkownType") << "@SUB=AlignmentTrackSelector::isHit2D"
					<< "Tracker hit not in pixel, neither SiStripRecHit[12]D nor "
					<< "SiStripMatchedRecHit2D nor ProjectedSiStripRecHit2D.";
	    return false;
	  }
	}
      } else { // not tracker??
	edm::LogWarning("DetectorMismatch") << "@SUB=AlignmentTrackSelector::isHit2D"
					    << "Hit not in tracker with 'official' dimension >=2.";
	return true; // dimension() >= 2 so accept that...
      }
    }
    // never reached...
  }

};


DEFINE_FWK_MODULE(GeneralTrackAnalyzerHisto);

