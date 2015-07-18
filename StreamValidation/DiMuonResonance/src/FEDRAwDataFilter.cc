// -*- C++ -*-
//
// Package:    FEDRawDataFilter
// Class:      FEDRawDataFilter
// 
/**\class FEDRawDataFilter FEDRawDataFilter.cc StreamValidation/DimuonResonance/src/FEDRawDataFilter.cc

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
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelNameUpgrade.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapNameUpgrade.h"

//
// class declaration
//

class FEDRawDataFilter : public edm::EDFilter {
   public:
      explicit FEDRawDataFilter(const edm::ParameterSet&);
      ~FEDRawDataFilter();

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
  
      
      // ----------member data ---------------------------
      edm::ParameterSet conf_;
      edm::EDGetTokenT<FEDRawDataCollection> rawin_;
      bool selectFPix_;
      bool selectBPix_;
};

FEDRawDataFilter::FEDRawDataFilter(const edm::ParameterSet& iConfig) :
  conf_(iConfig),
  rawin_( consumes<FEDRawDataCollection>( conf_.getParameter<edm::InputTag>( "RawInput" ) ) )
{
  selectFPix_ = iConfig.getParameter<bool>("selectFPix");
  selectBPix_ = iConfig.getParameter<bool>("selectBPix");
}


FEDRawDataFilter::~FEDRawDataFilter()
{
}

bool
FEDRawDataFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 bool result = false; 
 bool containsFPix = false;
 bool containsBPix = false;

 edm::Handle< FEDRawDataCollection >  rawinput;
 iEvent.getByToken( rawin_, rawinput );

 int fedId;
   
 for(fedId = 0; fedId <= 39; fedId++) {
   //get event data for this fed
   const FEDRawData& fedRawData = rawinput->FEDData(fedId);
   if (fedRawData.size() != 0){
     if (fedId<32){
       containsBPix=true;
     } else {
       containsFPix=true;
       std::cout<<"======================> contains FPix raw data!"<<std::endl;
     }
   }
 } 

 bool isBPixOk=false;
 bool isFPixOk=false;

 if(selectBPix_){ 
   isBPixOk=containsBPix;
 } if(selectFPix_){  
   isFPixOk=containsFPix;
 }

 result = (isBPixOk || isFPixOk);
 
 return result;
}


//define this as a plug-in
DEFINE_FWK_MODULE(FEDRawDataFilter);
