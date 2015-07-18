// -*- C++ -*-
//
// Package:    FieldAndStripLatencyFilter
// Class:      FieldAndStripLatencyFilter
// 
/**\class FieldAndStripLatencyFilter FieldAndStripLatencyFilter.cc StreamValidation/DimuonResonance/src/FieldAndStripLatencyFilter.cc

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
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"
#include "CondFormats/SiStripObjects/interface/SiStripLatency.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"


//
// class declaration
//

class FieldAndStripLatencyFilter : public edm::EDFilter {
   public:
      explicit FieldAndStripLatencyFilter(const edm::ParameterSet&);
      ~FieldAndStripLatencyFilter();

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
  
   // ----------member data ---------------------------
      bool isPeakMode_;
      bool isZeroTesla_;
};

FieldAndStripLatencyFilter::FieldAndStripLatencyFilter(const edm::ParameterSet& iConfig)
{
  isPeakMode_  = iConfig.getParameter<bool>("isPeakMode");
  isZeroTesla_ = iConfig.getParameter<bool>("isZeroTesla");
}


FieldAndStripLatencyFilter::~FieldAndStripLatencyFilter()
{
}

bool
FieldAndStripLatencyFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 bool result = false; 
 bool isPeak = false;
 bool is0T   = false;

 edm::ESHandle<SiStripLatency> apvlat;
 iSetup.get<SiStripLatencyRcd>().get(apvlat);
 if(apvlat->singleReadOutMode()==1) isPeak = true; // peak mode
 
 edm::ESHandle<MagneticField> magfield;
 iSetup.get<IdealMagneticFieldRecord>().get(magfield);
 float B_=magfield.product()->inTesla(GlobalPoint(0,0,0)).mag();
 if(B_<0.1) is0T = true;

 bool latencyOk = false;
 bool fieldOk   = false;

 if(isPeakMode_==isPeak) 
   latencyOk=true;
 if(isZeroTesla_==is0T)  
   fieldOk=true;

 result = (latencyOk && fieldOk);

 //std::cout<<"isPeakMode_:  "<<isPeakMode_ <<" isPeak: "<<isPeak <<"| latencyOk: "<<latencyOk<<std::endl;
 //std::cout<<"isZeroTesla_: "<<isZeroTesla_<<" is0T:   "<<is0T   <<"| fieldOk:   "<<fieldOk<<std::endl;

 return result;
}


//define this as a plug-in
DEFINE_FWK_MODULE(FieldAndStripLatencyFilter);
