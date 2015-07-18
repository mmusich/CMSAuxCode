// -*- C++ -*-
//
// Package:    StreamValidation
// Class:      AlCaTrackFilter
// 
/**\class AlCaTrackFilter AlCaTrackFilter.cc StreamValidation/DimuonResonance/src/AlCaTrackFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

// DataFormats
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelNameUpgrade.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapNameUpgrade.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

// Geometry
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

//
// class declaration
//

class AlCaTrackFilter : public edm::EDFilter {
   public:
      explicit AlCaTrackFilter(const edm::ParameterSet&);
      ~AlCaTrackFilter();

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob();
      virtual void beginJob();

      // ----------member data ---------------------------
      edm::ParameterSet conf_;
      edm::InputTag src_;
      bool selectFPix_;
      bool selectBPix_;
  
      int ievt;
      int pass;
      int passBPix;
      int passFPix;

};

AlCaTrackFilter::AlCaTrackFilter(const edm::ParameterSet& iConfig) :
  conf_(iConfig),
  src_( conf_.getParameter<edm::InputTag>( "src" ) )
{
  selectFPix_ = iConfig.getParameter<bool>("selectFPix");
  selectBPix_ = iConfig.getParameter<bool>("selectBPix");
}

AlCaTrackFilter::~AlCaTrackFilter()
{
}

void 
AlCaTrackFilter::beginJob(){
  ievt=0;
  pass=0;
  passBPix=0;
  passFPix=0;
}

bool
AlCaTrackFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 bool result = false; 
 bool containsTracks = false;
 bool containsFPix = false;
 bool containsBPix = false;

 ievt++;

 edm::Handle<reco::TrackCollection> trackCollection;
 iEvent.getByLabel(src_, trackCollection);
 
 const reco::TrackCollection tC = *(trackCollection.product());
 
 if(tC.size()>0) {
   containsTracks=true;
 }

 for (reco::TrackCollection::const_iterator track=tC.begin(); track!=tC.end(); track++){
   if(track->hitPattern().numberOfValidPixelBarrelHits()!=0){
     containsBPix=true;
   }
   if(track->hitPattern().numberOfValidPixelEndcapHits()!=0){
     containsFPix=true;
   }
 }
 
 if(selectBPix_ || selectFPix_){

   if(selectBPix_){ 
     result=containsBPix;
   } 
   
   if(selectFPix_){  
     result=containsFPix;
   } 

 } else {
   result=containsTracks;
 }
 
 if(containsBPix){
   passBPix++;
 }

 if(containsFPix){
   passFPix++;
 } 
 
 if(result){
   pass++;
 }

 return result;
}

void 
AlCaTrackFilter::endJob()
{ 
  std::cout<< "*******************************" << std::endl;
  std::cout<< "Events run in total: " << ievt << std::endl;
  std::cout<< "Passes:              " << pass << std::endl;
  std::cout<< "BPix passes:         " << passBPix <<std::endl;
  std::cout<< "FPix passes:         " << passFPix <<std::endl;
  std::cout<< "*******************************" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(AlCaTrackFilter);
