// -*- C++ -*-
//
// Package:    SiStripConditionAnalysis/SiStripConditionsReader
// Class:      SiStripConditionsReader
// 
/**\class SiStripConditionsReader SiStripConditionsReader.cc SiStripConditionAnalysis/SiStripConditionsReader/plugins/SiStripConditionsReader.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Marco Musich
//         Created:  Wed, 21 Dec 2016 12:50:51 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "CondFormats/SiStripObjects/interface/SiStripApvGain.h"
#include "CondFormats/DataRecord/interface/SiStripApvGainRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripGain.h"
#include "CalibTracker/Records/interface/SiStripGainRcd.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"

#include <sstream>
#include <fstream>

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class SiStripConditionsReader : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit SiStripConditionsReader(const edm::ParameterSet&);
      ~SiStripConditionsReader();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      std::pair<int,int> typeAndLayerFromDetId( const DetId& detId , const TrackerTopology* tTopo) const;

      // ----------member data ---------------------------
      edm::ESWatcher<SiStripApvGainRcd>  G1watcher_;
      edm::ESWatcher<SiStripApvGain2Rcd> G2watcher_;
  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SiStripConditionsReader::SiStripConditionsReader(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");

}


SiStripConditionsReader::~SiStripConditionsReader()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
SiStripConditionsReader::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   //Retrieve tracker topology from geometry
    edm::ESHandle<TrackerTopology> tTopoHandle;
    iSetup.get<TrackerTopologyRcd>().get(tTopoHandle);
    const TrackerTopology* tTopo = tTopoHandle.product();

   if ( G1watcher_.check(iSetup) || G2watcher_.check(iSetup) ) { 
     std::cout<< " G1 or G2 has a new IOV for runs: " << iEvent.id().run() << std::endl;
     
     edm::ESHandle<SiStripGain> SiStripApvGain_;
     iSetup.get<SiStripGainRcd>().get(SiStripApvGain_);

     std::vector<uint32_t> detid;
     SiStripApvGain_->getDetIds(detid);

     for (size_t id=0;id<detid.size();id++){
       SiStripApvGain::Range rangeG1=SiStripApvGain_->getRange(detid[id],0);	
       SiStripApvGain::Range rangeG2=SiStripApvGain_->getRange(detid[id],1);	
	      
       int apv=0;
       for(int it=0;it<rangeG1.second-rangeG1.first;it++){
	 
	 float G1 = SiStripApvGain_->getApvGain(it,rangeG1);
	 float G2 = SiStripApvGain_->getApvGain(it,rangeG2);

	 float G1G2 = G1*G2;

	 std::pair<int,int> packedTopo = this->typeAndLayerFromDetId(detid[id],tTopo);
	 
	 std::cout << "type: " << packedTopo.first;
	 std::cout << "layer:" << packedTopo.second;

	 std::cout << "detid " << detid[id] << " \t " << apv++ << " \t  G1: " << G1  << " \t  G2: " << G2 << " \t G1*G2: " << G1G2 << std::endl;        
       }
     }
   }
}

// ------------ method called once each job just before starting event loop  ------------
void 
SiStripConditionsReader::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SiStripConditionsReader::endJob() 
{
}

std::pair<int,int> SiStripConditionsReader::typeAndLayerFromDetId( const DetId& detId , const TrackerTopology* tTopo) const
{

  int layerNumber = 0;
  unsigned int subdetId = static_cast<unsigned int>(detId.subdetId());
  
  if ( subdetId == StripSubdetector::TIB) { 	   
    layerNumber = tTopo->tibLayer(detId.rawId());
  }
  else if ( subdetId ==  StripSubdetector::TOB ){
    layerNumber = tTopo->tobLayer(detId.rawId());
  }
  else if ( subdetId ==  StripSubdetector::TID) { 
    layerNumber = tTopo->tidWheel(detId.rawId());
  }
  else if ( subdetId ==  StripSubdetector::TEC ){ 
    layerNumber = tTopo->tecWheel(detId.rawId()); 
  }
  else if ( subdetId ==  PixelSubdetector::PixelBarrel ) { 
    layerNumber = tTopo->pxbLayer(detId.rawId());  
  }
  else if ( subdetId ==  PixelSubdetector::PixelEndcap ) { 
    layerNumber = tTopo->pxfDisk(detId.rawId());  
  }
  else
    edm::LogWarning("LogicError") << "Unknown subdetid: " <<  subdetId;

  return std::make_pair( subdetId, layerNumber );
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SiStripConditionsReader::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SiStripConditionsReader);
