// -*- C++ -*-
//
// Package:    SiStripConditionAnalysis/SiStripConditionsReader
// Class:      SiStripGainsAnalyzer
// 
/**\class SiStripGainsAnalyzer SiStripGainsAnalyzer.cc SiStripConditionAnalysis/SiStripConditionsReader/plugins/SiStripGainsAnalyzer.cc

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
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class SiStripGainsAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit SiStripGainsAnalyzer(const edm::ParameterSet&);
      ~SiStripGainsAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      std::pair<int,int> typeAndLayerFromDetId( const DetId& detId , const TrackerTopology* tTopo) const;

      // ----------member data ---------------------------
    
      edm::Service<TFileService> fs;
      std::auto_ptr<std::ofstream> output_;

      std::vector<std::string> partitions {"TIBL1","TIBL2","TIBL3","TIBL4","TOBL1","TOBL2","TOBL3","TOBL4","TOBL5","TOBL6"};

      static const int nIOVs_ = 100; 
      int IOVcount_;

      edm::ESWatcher<SiStripApvGainRcd>  G1watcher_;
      edm::ESWatcher<SiStripApvGain2Rcd> G2watcher_;
  
      std::map<int,std::map<std::string,TH1D*> > gainTrendByPartition_ ; 

      TH1D* gainTIBL1_[nIOVs_]; 
      TH1D* gainTIBL2_[nIOVs_];
      TH1D* gainTIBL3_[nIOVs_];
      TH1D* gainTIBL4_[nIOVs_];
                           
      TH1D* gainTOBL1_[nIOVs_];
      TH1D* gainTOBL2_[nIOVs_];
      TH1D* gainTOBL3_[nIOVs_];
      TH1D* gainTOBL4_[nIOVs_];
      TH1D* gainTOBL5_[nIOVs_];
      TH1D* gainTOBL6_[nIOVs_];
     
      std::map<std::string , TGraph* > g_GainsByPartition_ByIOV;

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
SiStripGainsAnalyzer::SiStripGainsAnalyzer(const edm::ParameterSet& iConfig)

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


SiStripGainsAnalyzer::~SiStripGainsAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
SiStripGainsAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
     
     for(std::vector<std::string>::iterator it = partitions.begin() ; it != partitions.end(); ++it){

       if((*it)=="TIBL1")
	 gainTrendByPartition_[run][(*it)] = gainTIBL1_[IOVcount_];
       else if((*it)=="TIBL2")
	 gainTrendByPartition_[run][(*it)] = gainTIBL2_[IOVcount_];
       else if((*it)=="TIBL3")
	 gainTrendByPartition_[run][(*it)] = gainTIBL3_[IOVcount_];
       else if((*it)=="TIBL4")
	 gainTrendByPartition_[run][(*it)] = gainTIBL4_[IOVcount_];
       else if((*it)=="TOBL1")                     	                      
	 gainTrendByPartition_[run][(*it)] = gainTOBL1_[IOVcount_];
       else if((*it)=="TOBL2")
	 gainTrendByPartition_[run][(*it)] = gainTOBL2_[IOVcount_];
       else if((*it)=="TOBL3")
	 gainTrendByPartition_[run][(*it)] = gainTOBL3_[IOVcount_];
       else if((*it)=="TOBL4")
	 gainTrendByPartition_[run][(*it)] = gainTOBL4_[IOVcount_];
       else if((*it)=="TOBL5")
	 gainTrendByPartition_[run][(*it)] = gainTOBL5_[IOVcount_];
       else if((*it)=="TOBL6")
	 gainTrendByPartition_[run][(*it)] = gainTOBL6_[IOVcount_];
       else {
	 std::cout<< "logical error... you should not be here!"<< std::endl;
       }
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
	       gainTIBL1_[IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 2:
	       gainTIBL2_[IOVcount_]->Fill(G1G2); 	       
	       break;
	       
	     case 3:
	       gainTIBL3_[IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 4:
	       gainTIBL4_[IOVcount_]->Fill(G1G2); 
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
	       gainTOBL1_[IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 2:
	       gainTOBL2_[IOVcount_]->Fill(G1G2); 	       
	       break;
	       
	     case 3:
	       gainTOBL3_[IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 4:
	       gainTOBL4_[IOVcount_]->Fill(G1G2); 
	       break;

	     case 5:
	       gainTOBL5_[IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 6:
	       gainTOBL6_[IOVcount_]->Fill(G1G2); 
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
SiStripGainsAnalyzer::beginJob()
{

  TFileDirectory TIBL1 = fs->mkdir("TIB_L1");
  TFileDirectory TIBL2 = fs->mkdir("TIB_L1");
  TFileDirectory TIBL3 = fs->mkdir("TIB_L1");
  TFileDirectory TIBL4 = fs->mkdir("TIB_L1");
  TFileDirectory TOBL1 = fs->mkdir("TOB_L1");
  TFileDirectory TOBL2 = fs->mkdir("TOB_L2");
  TFileDirectory TOBL3 = fs->mkdir("TOB_L3");
  TFileDirectory TOBL4 = fs->mkdir("TOB_L4");
  TFileDirectory TOBL5 = fs->mkdir("TOB_L5");
  TFileDirectory TOBL6 = fs->mkdir("TOB_L6");
  
  for ( int i=0; i<nIOVs_; ++i ) {
    gainTIBL1_[i] = TIBL1.make<TH1D>(Form("G_TIBL1_IOV%i",i),Form("TIB L1 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
    gainTIBL2_[i] = TIBL2.make<TH1D>(Form("G_TIBL2_IOV%i",i),Form("TIB L2 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
    gainTIBL3_[i] = TIBL3.make<TH1D>(Form("G_TIBL3_IOV%i",i),Form("TIB L3 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
    gainTIBL4_[i] = TIBL4.make<TH1D>(Form("G_TIBL4_IOV%i",i),Form("TIB L4 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
              
    gainTOBL1_[i] = TOBL1.make<TH1D>(Form("G_TOBL1_IOV%i",i),Form("TOB L1 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
    gainTOBL2_[i] = TOBL2.make<TH1D>(Form("G_TOBL2_IOV%i",i),Form("TOB L2 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
    gainTOBL3_[i] = TOBL3.make<TH1D>(Form("G_TOBL3_IOV%i",i),Form("TOB L3 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
    gainTOBL4_[i] = TOBL4.make<TH1D>(Form("G_TOBL4_IOV%i",i),Form("TOB L4 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
    gainTOBL5_[i] = TOBL5.make<TH1D>(Form("G_TOBL5_IOV%i",i),Form("TOB L5 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
    gainTOBL6_[i] = TOBL6.make<TH1D>(Form("G_TOBL6_IOV%i",i),Form("TPB L6 Gain for IOV %i; Gain; n. APV",i),100,0.,2.);
  }
  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SiStripGainsAnalyzer::endJob() 
{
  
  std::ostringstream output; 

  std::vector<float>  theBoundaries_;
  std::map<std::string, std::vector<float> > the_gain_averages_;
  
  output<<" the IOVs are at :" << std::endl;
  for(std::map< int,std::map<std::string,TH1D*> >::iterator it = gainTrendByPartition_.begin(); it != gainTrendByPartition_.end(); ++it) {
    theBoundaries_.push_back(it->first);
    output<<" - "<< it->first << std::endl;
  }

  for(unsigned int i=0;i<theBoundaries_.size();i++){
    
    int the_r = theBoundaries_[i]; 

    output<<"|====================================== " << std::endl
	  <<"| run: "<< the_r << std::endl; 

    for(std::vector<std::string>::iterator it = partitions.begin() ; it != partitions.end(); ++it){

      auto theMap = gainTrendByPartition_[the_r];
      
      the_gain_averages_[(*it)].push_back( theMap[(*it)]->GetMean() );
      output<<"| "<< (*it) << " <G>: "<< std::setw(4) << the_gain_averages_[(*it)][i] << std::endl;
    }    
  }
  
  for (std::vector<std::string>::iterator it = partitions.begin() ; it != partitions.end(); ++it){
    g_GainsByPartition_ByIOV[(*it)] = fs->make<TGraph>(theBoundaries_.size(),&(theBoundaries_[0]),&(the_gain_averages_[(*it)][0]));
    g_GainsByPartition_ByIOV[(*it)]->SetName(Form("g_average_Gain_%s",(*it).c_str()) );
    g_GainsByPartition_ByIOV[(*it)]->SetTitle(Form("average gain in %s",(*it).c_str()));
    g_GainsByPartition_ByIOV[(*it)]->GetXaxis()->SetTitle("IOV (run number)");
    g_GainsByPartition_ByIOV[(*it)]->GetYaxis()->SetTitle("#LT G1*G2 #GT");
  }

  /// Final output - either message logger or output file:
  if (output_.get()) *output_ << output.str();
  else edm::LogInfo("") << output.str();

}

// -------------- method to get the topology from the detID ------------------------------
std::pair<int,int> 
SiStripGainsAnalyzer::typeAndLayerFromDetId( const DetId& detId , const TrackerTopology* tTopo) const
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
SiStripGainsAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SiStripGainsAnalyzer);
