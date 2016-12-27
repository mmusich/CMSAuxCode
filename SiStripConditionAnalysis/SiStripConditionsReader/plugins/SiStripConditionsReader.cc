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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "CondFormats/SiStripObjects/interface/SiStripApvGain.h"
#include "CondFormats/DataRecord/interface/SiStripApvGainRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripGain.h"
#include "CalibTracker/Records/interface/SiStripGainRcd.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"

// ROOT includes

#include "TFile.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TGraph.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <vector>

#define DEBUG 0

//
// constants, enums and typedefs
//

// add new entries bef

namespace partitions {
  enum layers {
    TIBL1,
    TIBL2,
    TIBL3,
    TIBL4,
    TOBL1,
    TOBL2,
    TOBL3,
    TOBL4,
    TOBL5,
    TOBL6,
    NUM_OF_TYPES
  };
}

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
      std::string getStringFromEnum(partitions::layers e);
      std::pair<int,int> typeAndLayerFromDetId( const DetId& detId , const TrackerTopology* tTopo) const;
  
      // ----------member data ---------------------------
    
      edm::Service<TFileService> fs;
      std::auto_ptr<std::ofstream> output_;

      // std::vector<std::string> partitions {"TIBL1","TIBL2","TIBL3","TIBL4","TOBL1","TOBL2","TOBL3","TOBL4","TOBL5","TOBL6"};

      static const int nIOVs_ = 100; 
      static const int nParts_ = 10;

      int IOVcount_;

      edm::ESWatcher<SiStripApvGainRcd>  G1watcher_;
      edm::ESWatcher<SiStripApvGain2Rcd> G2watcher_;
  
      std::map<int,std::map<partitions::layers,TH1D*> > gainTrendByPartition_ ; 
      TH1D* h_gainByPart_[nParts_][nIOVs_];

      std::map<partitions::layers , TGraph* > g_GainsByPartition_ByIOV;

};

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
   IOVcount_=0;
   
   std::string fileName(iConfig.getUntrackedParameter<std::string>("rawFileName"));
   if (fileName.size()) {
     output_.reset(new std::ofstream(fileName.c_str()));
     if (!output_->good()) {
       edm::LogError("IOproblem") << "Could not open output file " << fileName << ".";
       output_.reset();
     }
   }
  
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

   bool hasG1IOV = G1watcher_.check(iSetup);
   bool hasG2IOV = G2watcher_.check(iSetup);

   if ( hasG1IOV || hasG2IOV  ) { 

     int run = iEvent.id().run();
     if(hasG1IOV)
       std::cout<< " G1 has a new IOV for runs: " << iEvent.id().run() << std::endl;
     if(hasG2IOV) 
       std::cout<< " G2 has a new IOV for runs: " << iEvent.id().run() << std::endl;
     
     // assign to each element to the map the corresponding histogram

     for(int i = partitions::TIBL1; i!=partitions::NUM_OF_TYPES; i++){
       partitions::layers part = (partitions::layers) i;           
       gainTrendByPartition_[run][part] = h_gainByPart_[part][IOVcount_];
     }
     
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
	 
	 // here starts TIB
	 if(packedTopo.first == 3){
	
	   switch (packedTopo.second)
	     {
	     case 1:
	       h_gainByPart_[partitions::TIBL1][IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 2:
	       h_gainByPart_[partitions::TIBL2][IOVcount_]->Fill(G1G2); 	       
	       break;
	       
	     case 3:
	       h_gainByPart_[partitions::TIBL3][IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 4:
	       h_gainByPart_[partitions::TIBL4][IOVcount_]->Fill(G1G2); 
	       break;
	       
	     default:
	       edm::LogWarning("LogicError") << "Unknown layer: " <<  packedTopo.second;
	     } 
	 }
	 
	 // here starts TOB
	 if(packedTopo.first == 5){
	   switch (packedTopo.second) 
	     {
	     case 1:
	       h_gainByPart_[partitions::TOBL1][IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 2:
	       h_gainByPart_[partitions::TOBL2][IOVcount_]->Fill(G1G2); 	       
	       break;
	       
	     case 3:
	       h_gainByPart_[partitions::TOBL3][IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 4:
	       h_gainByPart_[partitions::TOBL4][IOVcount_]->Fill(G1G2); 
	       break;

	     case 5:
	       h_gainByPart_[partitions::TOBL5][IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 6:
	       h_gainByPart_[partitions::TOBL6][IOVcount_]->Fill(G1G2); 
	       break;

	     default:
	       edm::LogWarning("LogicError") << "Unknown layer: " <<  packedTopo.second;
	     }
	 }

	 if(DEBUG){
	   std::cout << "type: " << packedTopo.first;
	   std::cout << "layer:" << packedTopo.second;
	   std::cout << "detid " << detid[id] << " \t " << apv++ << " \t  G1: " << G1  << " \t  G2: " << G2 << " \t G1*G2: " << G1G2 << std::endl;       
	 }

       } // ends loop on APVs
     } // ends loop on detID

     // increase the IOV counter
     IOVcount_++;
     
   } // if there is a new IOV
}

// ------------ method called once each job just before starting event loop  ------------
void 
SiStripConditionsReader::beginJob()
{
  
  std::vector<TFileDirectory> dirs;
  for (int i=partitions::TIBL1;i!=partitions::NUM_OF_TYPES;i++){
    partitions::layers part = (partitions::layers) i;           
    
    dirs.push_back(fs->mkdir(this->getStringFromEnum(part)));
    
    for(int j=0;j<nIOVs_; ++j){
      h_gainByPart_[i][j] = dirs[i].make<TH1D>(Form("G_%s_IOV%i",this->getStringFromEnum(part).c_str(),j),Form("%s Gain for IOV %i; Gain; n. APV",this->getStringFromEnum(part).c_str(),j),100,0.,2.);
    } 
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SiStripConditionsReader::endJob() 
{
  
  std::ostringstream output; 

  std::vector<float>  theBoundaries_;
  std::map<partitions::layers, std::vector<float> > the_gain_averages_;
  
  output<<" the IOVs are at :" << std::endl;
  for(std::map< int,std::map<partitions::layers,TH1D*> >::iterator it = gainTrendByPartition_.begin(); it != gainTrendByPartition_.end(); ++it) {
    theBoundaries_.push_back(it->first);
    output<<" - "<< it->first << std::endl;
  }

  for(unsigned int i=0;i<theBoundaries_.size();i++){
    
    int the_r = theBoundaries_[i]; 

    output<<"|====================================== " << std::endl
	  <<"| run: "<< the_r << std::endl; 
    
    for(int j = partitions::TIBL1; j!=partitions::NUM_OF_TYPES; j++){
       partitions::layers part = (partitions::layers) j;           

      auto theMap = gainTrendByPartition_[the_r];
      
      the_gain_averages_[part].push_back( theMap[part]->GetMean() );
      output<<"| "<< this->getStringFromEnum(part) << " <G>: "<< std::setw(4) << the_gain_averages_[part][i] << std::endl;
    }    
  }
  
  for(int i = partitions::TIBL1; i!=partitions::NUM_OF_TYPES; i++){
    partitions::layers part = (partitions::layers) i;    
    g_GainsByPartition_ByIOV[part] = fs->make<TGraph>(theBoundaries_.size(),&(theBoundaries_[0]),&(the_gain_averages_[part][0]));
    g_GainsByPartition_ByIOV[part]->SetName(Form("g_average_Gain_%s",this->getStringFromEnum(part).c_str()) );
    g_GainsByPartition_ByIOV[part]->SetTitle(Form("average gain in %s",this->getStringFromEnum(part).c_str()));
    g_GainsByPartition_ByIOV[part]->GetXaxis()->SetTitle("IOV (run number)");
    g_GainsByPartition_ByIOV[part]->GetYaxis()->SetTitle("#LT G1*G2 #GT");
  }

  /// Final output - either message logger or output file:
  if (output_.get()) *output_ << output.str();
  else edm::LogInfo("") << output.str();

}

// -------------- method to get the topology from the detID ------------------------------
std::pair<int,int> 
SiStripConditionsReader::typeAndLayerFromDetId( const DetId& detId , const TrackerTopology* tTopo) const
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

// -------------- method to get the topology from the detID ------------------------------
std::string 
SiStripConditionsReader::getStringFromEnum(partitions::layers e)
{
  switch(e)
    {
    case partitions::TIBL1: return "TIB L1";
    case partitions::TIBL2: return "TIB L2";
    case partitions::TIBL3: return "TIB L3";
    case partitions::TIBL4: return "TIB L4";
    case partitions::TOBL1: return "TOB L1";
    case partitions::TOBL2: return "TOB L2";
    case partitions::TOBL3: return "TOB L3";
    case partitions::TOBL4: return "TOB L4";
    case partitions::TOBL5: return "TOB L5";
    case partitions::TOBL6: return "TOB L6";
    default: 
      edm::LogWarning("LogicError") << "Unknown partition: " <<  e;
      return "";
    }
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
