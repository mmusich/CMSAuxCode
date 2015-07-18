// -*- C++ -*-
//
// Package:    FEDRawDataFilter
// Class:      PixelDigiFilter
// 
/**\class PixelDigiFilter PixelDigiFilter.cc StreamValidation/DimuonResonance/src/PixelDigiFilter.cc

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

class PixelDigiFilter : public edm::EDFilter {
   public:
      explicit PixelDigiFilter(const edm::ParameterSet&);
      ~PixelDigiFilter();

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
  
      
      // ----------member data ---------------------------
      edm::ParameterSet conf_;
      edm::InputTag src_;
      bool selectFPix_;
      bool selectBPix_;
};

PixelDigiFilter::PixelDigiFilter(const edm::ParameterSet& iConfig) :
  conf_(iConfig),
  src_( conf_.getParameter<edm::InputTag>( "src" ) )
{
  selectFPix_ = iConfig.getParameter<bool>("selectFPix");
  selectBPix_ = iConfig.getParameter<bool>("selectBPix");
}

PixelDigiFilter::~PixelDigiFilter()
{
}

bool
PixelDigiFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 bool result = false; 
 bool containsFPix = false;
 bool containsBPix = false;

 //Retrieve tracker topology from geometry
 //edm::ESHandle<TrackerTopology> tTopoHandle;
 //es.get<IdealGeometryRecord>().get(tTopoHandle);
 //const TrackerTopology* const tTopo = tTopoHandle.product();
 
 // geometry setup
 //edm::ESHandle<TrackerGeometry> geometry;
 //iSetup.get<TrackerDigiGeometryRecord>().get(geometry);
 //const TrackerGeometry* theGeometry = &(*geometry);
 
 edm::Handle<SiPixelRecHitCollection> recHitColl;
 iEvent.getByLabel( src_, recHitColl);
    
 if((recHitColl.product())->dataSize() > 0) {
   SiPixelRecHitCollection::const_iterator recHitIdIterator = (recHitColl.product())->begin();
   SiPixelRecHitCollection::const_iterator recHitIdIteratorEnd = (recHitColl.product())->end();
   std::string detname ;
  
   for ( ; recHitIdIterator != recHitIdIteratorEnd; recHitIdIterator++) {
     SiPixelRecHitCollection::DetSet detset = *recHitIdIterator;
     if( detset.empty() ) continue;
     DetId detId = DetId(detset.detId()); // Get the Detid object
     //const GeomDet* geomDet( theGeometry->idToDet(detId) );
     // Loop over rechits for this detid
     SiPixelRecHitCollection::DetSet::const_iterator rechitRangeIteratorBegin = detset.begin();
     SiPixelRecHitCollection::DetSet::const_iterator rechitRangeIteratorEnd = detset.end();
     SiPixelRecHitCollection::DetSet::const_iterator iterRecHit;
 
     for ( iterRecHit = rechitRangeIteratorBegin;
	   iterRecHit != rechitRangeIteratorEnd; ++iterRecHit) {
       
       unsigned int subid = detId.subdetId();
       //int detid_db = detId.rawId();
       // int layer_num = -99,ladder_num=-99,module_num=-99,disk_num=-99,blade_num=-99,panel_num=-99,side_num=-99;
       if ( ( subid == PixelSubdetector::PixelBarrel ) || ( subid == PixelSubdetector::PixelEndcap ) ) {
	 // 1 = PXB, 2 = PXF
	 if ( subid == PixelSubdetector::PixelBarrel ) {
	   //layer_num = tTopo->pxbLayer(detId.rawId());
	   //ladder_num = tTopo->pxbLadder(detId.rawId());
	   //module_num = tTopo->pxbModule(detId.rawId());
	   
	   containsBPix=true;

	   // std::cout <<"\ndetId = "<<subid<<" : "<<tTopo->pxbLayer(detId.rawId())<<" , "<<tTopo->pxbLadder(detId.rawId())<<" , "<< tTopo->pxbModule(detId.rawId());
	 } else if ( subid == PixelSubdetector::PixelEndcap ) {
	   //module_num = tTopo->pxfModule(detId());
	   //disk_num = tTopo->pxfDisk(detId());
	   //blade_num = tTopo->pxfBlade(detId());
	   //panel_num = tTopo->pxfPanel(detId());
	   //side_num = tTopo->pxfSide(detId());

	   containsFPix=true; 
	 } // if Fpix
       } // if Pixel
     } // loop over rechits on a given detId
   } // loop over detIds
 } // if rechitCollection is not empty

 bool isBPixOk=false;
 bool isFPixOk=false;

 if(selectBPix_){ 
   isBPixOk=containsBPix;
 } 

 if(selectFPix_){  
   isFPixOk=containsFPix;
 }

 result = (isBPixOk || isFPixOk);
 
 return result;
}


//define this as a plug-in
DEFINE_FWK_MODULE(PixelDigiFilter);
