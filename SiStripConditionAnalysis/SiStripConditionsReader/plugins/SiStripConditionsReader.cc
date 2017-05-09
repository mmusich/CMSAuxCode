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

#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/SiStripObjects/interface/SiStripApvGain.h"
#include "CondFormats/DataRecord/interface/SiStripApvGainRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripGain.h"
#include "CalibTracker/Records/interface/SiStripGainRcd.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripQuality.h"
#include "CalibTracker/Records/interface/SiStripQualityRcd.h"
#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"

// ROOT includes

#include "TFile.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"

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

// add new entries before NUM_OF_TYPES
// otherwise will screw up the counts

namespace partitions {
  enum layers {
    TIB,
    TIBL1,
    TIBL2,
    TIBL3,
    TIBL4,
    TOB,
    TOBL1,
    TOBL2,
    TOBL3,
    TOBL4,
    TOBL5,
    TOBL6,
    TIDP,
    TIDM,
    TIDPD1,
    TIDPD2,
    TIDPD3,
    TIDMD1,
    TIDMD2,
    TIDMD3,
    TECP,
    TECM,
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
      std::pair<int,std::pair<int,int> > typeAndLayerFromDetId( const DetId& detId , const TrackerTopology* tTopo) const;
  
      // ----------member data ---------------------------

      /*!< Description of disabled detector channels. */
      SiStripQuality*  siStripQuality_; 

      edm::Service<TFileService> fs;
      std::auto_ptr<std::ofstream> output_;

      static const int nIOVs_ = 100; 
      static const int nParts_ = partitions::NUM_OF_TYPES;

      int IOVcount_;

      edm::ESWatcher<SiStripApvGainRcd>  G1watcher_;
      edm::ESWatcher<SiStripApvGain2Rcd> G2watcher_;


      // Product G1*G2
      std::map<int,std::map<partitions::layers,TH1D*> > gainTrendByPartition_ ; 
      TH1D* h_gainByPart_[nParts_][nIOVs_];

      std::map<partitions::layers , TGraphErrors* > g_GainsByPartition_ByIOV;
      std::map<partitions::layers , TGraph* >       g_GainsRMSByPartition_ByIOV;

      std::map<partitions::layers , TH1F* > h_GainsByPartition_ByIOV;
      std::map<partitions::layers , TH1F* > h_GainsRMSByPartition_ByIOV;
  
      int lastRun_;
      bool applyQuality_;
  
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
  
  siStripQuality_ = 0;
  siStripQuality_ = new SiStripQuality();
 
  usesResource("TFileService");
  IOVcount_=0;
  
  lastRun_=0;

  std::string fileName(iConfig.getUntrackedParameter<std::string>("rawFileName"));
  applyQuality_ = iConfig.getUntrackedParameter<bool>("applySiStripQuality",true);

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
  //if (siStripQuality_!=0) delete siStripQuality_; 
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

   //Retrieve the strip quality from conditions
   //edm::ESHandle<SiStripQuality> siStripQuality_;
   //iSetup.get<SiStripQualityRcd>().get(siStripQuality_);

   int run = iEvent.id().run();

   if(run>lastRun_) lastRun_= run;
      
   bool hasG1IOV = G1watcher_.check(iSetup);
   bool hasG2IOV = G2watcher_.check(iSetup);

   if ( hasG1IOV || hasG2IOV  ) { 

     if(hasG1IOV)
       std::cout<< " G1 has a new IOV for runs: " << iEvent.id().run() << std::endl;
     if(hasG2IOV) 
       std::cout<< " G2 has a new IOV for runs: " << iEvent.id().run() << std::endl;
     
     // assign to each element to the map the corresponding histogram

     for(int i = partitions::TIB; i!=partitions::NUM_OF_TYPES; i++){
       partitions::layers part = (partitions::layers) i;           
       gainTrendByPartition_[run][part] = h_gainByPart_[part][IOVcount_];
     }
     
     edm::ESHandle<SiStripGain> SiStripApvGain_;
     iSetup.get<SiStripGainRcd>().get(SiStripApvGain_);

     std::vector<uint32_t> detid;
     SiStripApvGain_->getDetIds(detid);

     // Retrieve the SiStripDetCabling description
     edm::ESHandle<SiStripDetCabling> detCabling_;     
     iSetup.get<SiStripDetCablingRcd>().get( detCabling_ );
     
     //  Retrieve the RunInfo object
     edm::ESHandle<RunInfo> runInfo;
     iSetup.get<RunInfoRcd>().get( runInfo );
     
     siStripQuality_->add( detCabling_.product() );
     siStripQuality_->add( runInfo.product() );
     siStripQuality_->cleanUp();
     siStripQuality_->fillBadComponents();
     
     for (size_t id=0;id<detid.size();id++){
       SiStripApvGain::Range rangeG1=SiStripApvGain_->getRange(detid[id],0);	
       SiStripApvGain::Range rangeG2=SiStripApvGain_->getRange(detid[id],1);	
	      
       int apv=0,nAPV=0;
       for(int it=0;it<rangeG1.second-rangeG1.first;it++){
	 nAPV++;

	 //std::cout<<"nAPV: "<<nAPV<<std::endl;

	 //check if the quality was OK
	 if(siStripQuality_->IsApvBad(detid[id],nAPV) && applyQuality_ ){
	   //std::cout<<"In IOV"<< run << " | DetId: "<<detid[id]<<" APV"<<nAPV<<" is bad!"<<std::endl;
	   continue;
	 }	    

	 float G1 = SiStripApvGain_->getApvGain(it,rangeG1);
	 float G2 = SiStripApvGain_->getApvGain(it,rangeG2);

	 float G1G2 = G1*G2;

	 std::pair<int,std::pair<int,int> > packedTopo = this->typeAndLayerFromDetId(detid[id],tTopo);
	 
	 // here starts TIB
	 if(packedTopo.first == 3){

	   h_gainByPart_[partitions::TIB][IOVcount_]->Fill(G1G2); 

	   switch (packedTopo.second.first)
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
	       edm::LogWarning("LogicError") << "Unknown TIB layer: " <<  packedTopo.second.first;
	     } 
	 }
	 
	 // here starts TOB
	 if(packedTopo.first == 5){

	   h_gainByPart_[partitions::TOB][IOVcount_]->Fill(G1G2); 

	   switch (packedTopo.second.first) 
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
	       edm::LogWarning("LogicError") << "Unknown TOB layer: " <<  packedTopo.second.first;
	     }
	 }

	 // here starts TID
	 if(packedTopo.first == 4){
	   switch (packedTopo.second.second)
	     {
	     case 1:
	       h_gainByPart_[partitions::TIDM][IOVcount_]->Fill(G1G2); 
	       if(packedTopo.second.first==1){
		 h_gainByPart_[partitions::TIDMD1][IOVcount_]->Fill(G1G2); 
	       } else if(packedTopo.second.first==2){
		 h_gainByPart_[partitions::TIDMD2][IOVcount_]->Fill(G1G2); 
	       } else if(packedTopo.second.first==3){
		 h_gainByPart_[partitions::TIDMD3][IOVcount_]->Fill(G1G2); 
	       } else{
		 edm::LogWarning("LogicError") << "Unknown TID disk: " <<  packedTopo.second.first;
	       }	       		 
	       break;
	       
	     case 2:
	       h_gainByPart_[partitions::TIDP][IOVcount_]->Fill(G1G2); 	       
	       if(packedTopo.second.first==1){
		 h_gainByPart_[partitions::TIDPD1][IOVcount_]->Fill(G1G2); 
	       } else if(packedTopo.second.first==2){
		 h_gainByPart_[partitions::TIDPD2][IOVcount_]->Fill(G1G2); 
	       } else if(packedTopo.second.first==3){
		 h_gainByPart_[partitions::TIDPD3][IOVcount_]->Fill(G1G2); 
	       } else{
		 edm::LogWarning("LogicError") << "Unknown TID disk: " <<  packedTopo.second.first;
	       }
	       break;
	       
	     default:
	       edm::LogWarning("LogicError") << "Unknown TID side: " <<  packedTopo.second.second;
	     }
	 }  

	 // here starts TEC
	 if(packedTopo.first == 6){
	   switch (packedTopo.second.second)
	     {
	     case 1:
	       h_gainByPart_[partitions::TECM][IOVcount_]->Fill(G1G2); 
	       break;
	       
	     case 2:
	       h_gainByPart_[partitions::TECP][IOVcount_]->Fill(G1G2); 	       
	       break;
	       
	     default:
	       edm::LogWarning("LogicError") << "Unknown TEC side: " <<  packedTopo.second.second;
	     }
	 }  


	 if(DEBUG){
	   std::cout << "type: " << packedTopo.first;
	   std::cout << "layer:" << packedTopo.second.first;
	   std::cout << "side: " << packedTopo.second.second;
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
  for (int i=partitions::TIB;i!=partitions::NUM_OF_TYPES;i++){
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
  std::vector<float>  ex_;
  std::map<partitions::layers, std::vector<float> > the_gain_averages_;
  std::map<partitions::layers, std::vector<float> > the_gain_RMS_;

  output<<" the IOVs are at :" << std::endl;
  for(std::map< int,std::map<partitions::layers,TH1D*> >::iterator it = gainTrendByPartition_.begin(); it != gainTrendByPartition_.end(); ++it) {
    theBoundaries_.push_back(it->first);
    ex_.push_back(0.);
    output<<" - "<< it->first << std::endl;
  }

  for(unsigned int i=0;i<theBoundaries_.size();i++){
    
    int the_r = theBoundaries_[i]; 

    output<<"|====================================== " << std::endl
	  <<"| run: "<< the_r << std::endl; 
    
    for(int j = partitions::TIB; j!=partitions::NUM_OF_TYPES; j++){
       partitions::layers part = (partitions::layers) j;           

      auto theMap = gainTrendByPartition_[the_r];
      
      theMap[part]->SetTitle(Form("%s Gain for IOV %i (%.0f,%.0f)",this->getStringFromEnum(part).c_str(),i,theBoundaries_[i],theBoundaries_[i+1]));

      the_gain_averages_[part].push_back( theMap[part]->GetMean() );
      the_gain_RMS_[part].push_back( theMap[part]->GetRMS() );
	    
      output<<"| "<< this->getStringFromEnum(part) << " <G>: "<< std::setw(4) << the_gain_averages_[part][i] << " RMS(G): "<< std::setw(4) << the_gain_RMS_[part][i] << std::endl;
    }    
  }

  std::vector<float> binStops_(theBoundaries_);
  binStops_.push_back(lastRun_);
  const unsigned int nPoints = binStops_.size();

  std::vector<float> midPoints;
  std::vector<float> binWidths;

  for(unsigned int i=0;i<theBoundaries_.size();i++){
    float halfwidth = (binStops_[i+1]-binStops_[i])/2.;

    midPoints.push_back(binStops_[i]+halfwidth);
    binWidths.push_back(halfwidth);

    std::cout<<"i: "<<i<<"| midPoints["<<i<<"]:"<<std::setw(8)<< midPoints[i] << " binWidths["<<i<<"]:" << std::setw(8) << binWidths[i] 
	     <<" left: "<< std::setw(8) << midPoints[i]-binWidths[i] << " right: "<< std::setw(8) << midPoints[i]+binWidths[i] << std::endl; 

  }

  for(int i = partitions::TIB; i!=partitions::NUM_OF_TYPES; i++){
    partitions::layers part = (partitions::layers) i;    

    //g_GainsByPartition_ByIOV[part] = fs->make<TGraphErrors>(theBoundaries_.size(),&(theBoundaries_[0]),&(the_gain_averages_[part][0]),&(ex_[0]),&(the_gain_RMS_[part][0]));

    g_GainsByPartition_ByIOV[part] = fs->make<TGraphErrors>(theBoundaries_.size(),&(midPoints[0]),&(the_gain_averages_[part][0]),&(binWidths[0]),&(ex_[0])); //&(the_gain_RMS_[part][0]));
    g_GainsByPartition_ByIOV[part]->SetName(Form("g_average_Gain_%s",this->getStringFromEnum(part).c_str()) );
    g_GainsByPartition_ByIOV[part]->SetTitle(Form("average gain in %s",this->getStringFromEnum(part).c_str()));
    g_GainsByPartition_ByIOV[part]->GetXaxis()->SetTitle("IOV (run number)");
    g_GainsByPartition_ByIOV[part]->GetYaxis()->SetTitle("#LT G1*G2 #GT");
    
    //g_GainsRMSByPartition_ByIOV[part] = fs->make<TGraph>(theBoundaries_.size(),&(theBoundaries_[0]),&(the_gain_RMS_[part][0]));

    g_GainsRMSByPartition_ByIOV[part] = fs->make<TGraph>(theBoundaries_.size(),&(theBoundaries_[0]),&(the_gain_RMS_[part][0]));
    g_GainsRMSByPartition_ByIOV[part]->SetName(Form("g_RMS_Gain_%s",this->getStringFromEnum(part).c_str()) );
    g_GainsRMSByPartition_ByIOV[part]->SetTitle(Form("RMS of gain in %s",this->getStringFromEnum(part).c_str()));
    g_GainsRMSByPartition_ByIOV[part]->GetXaxis()->SetTitle("IOV (run number)");
    g_GainsRMSByPartition_ByIOV[part]->GetYaxis()->SetTitle("RMS(G1*G2)");
    
    h_GainsByPartition_ByIOV[part] = fs->make<TH1F>(Form("h_average_Gain_%s",this->getStringFromEnum(part).c_str()),
    						    Form("h_average_Gain_%s; IOV (run number); #LT G1*G2 #GT",this->getStringFromEnum(part).c_str()),
    						    nPoints-1,&(binStops_[0]));
    
    h_GainsRMSByPartition_ByIOV[part] = fs->make<TH1F>(Form("h_RMS_Gain_%s",this->getStringFromEnum(part).c_str()),
    						       Form("h_RMS_Gain_%s; IOV (run number); RMS(G1*G2)",this->getStringFromEnum(part).c_str()),
    						       nPoints-1,&(binStops_[0]));
    
    for(unsigned int j=0;j<nPoints-1;j++){
      
      //std::cout<<"index: "<<j<<" binStops_["<<j<<"]="<<binStops_[j] 
      //       <<" bin: "<<j+1<<"| avg: "<< the_gain_averages_[part][j] << " | rms:" << the_gain_RMS_[part][j] <<std::endl;
      
      h_GainsByPartition_ByIOV[part]->SetBinContent(j+1,the_gain_averages_[part][j]);
      h_GainsRMSByPartition_ByIOV[part]->SetBinContent(j+1,the_gain_RMS_[part][j]);

      h_GainsByPartition_ByIOV[part]->SetBinError(j+1,0.);
      h_GainsRMSByPartition_ByIOV[part]->SetBinError(j+1,0.);

      //h_GainsByPartition_ByIOV[part]->GetXaxis()->SetBinLabel(j+1,Form("%f",binStops_[j]));
      //h_GainsRMSByPartition_ByIOV[part]->GetXaxis()->SetBinLabel(j+1,Form("%f",binStops_[j]));
    }
  

  }

  /// Final output - either message logger or output file:
  if (output_.get()) *output_ << output.str();
  else edm::LogInfo("") << output.str();

}

// -------------- method to get the topology from the detID ------------------------------
std::pair<int,std::pair<int,int> > 
SiStripConditionsReader::typeAndLayerFromDetId( const DetId& detId , const TrackerTopology* tTopo) const
{

  int layerNumber = 0;
  int sideNumber  = 0;
  unsigned int subdetId = static_cast<unsigned int>(detId.subdetId());
  
  if ( subdetId == StripSubdetector::TIB) { 	   
    layerNumber = tTopo->tibLayer(detId.rawId());
    sideNumber  = tTopo->tibSide(detId.rawId());
  }
  else if ( subdetId ==  StripSubdetector::TOB ){
    layerNumber = tTopo->tobLayer(detId.rawId());
    sideNumber  = tTopo->tobSide(detId.rawId());
  }
  else if ( subdetId ==  StripSubdetector::TID) { 
    layerNumber = tTopo->tidWheel(detId.rawId());
    sideNumber  = tTopo->tidSide(detId.rawId());
  }
  else if ( subdetId ==  StripSubdetector::TEC ){ 
    layerNumber = tTopo->tecWheel(detId.rawId());
    sideNumber  = tTopo->tecSide(detId.rawId()); 
  }
  else if ( subdetId ==  PixelSubdetector::PixelBarrel ) { 
    layerNumber = tTopo->pxbLayer(detId.rawId());  
  }
  else if ( subdetId ==  PixelSubdetector::PixelEndcap ) { 
    layerNumber = tTopo->pxfDisk(detId.rawId()); 
    sideNumber  = tTopo->pxfSide(detId.rawId());
  }
  else
    edm::LogWarning("LogicError") << "Unknown subdetid: " <<  subdetId;

  return std::make_pair( subdetId, std::make_pair(layerNumber,sideNumber) );
}

// -------------- method to get the topology from the detID ------------------------------
std::string 
SiStripConditionsReader::getStringFromEnum(partitions::layers e)
{
  switch(e)
    {
    case partitions::TIB  :  return "TIB";
    case partitions::TIBL1:  return "TIB L1";
    case partitions::TIBL2:  return "TIB L2";
    case partitions::TIBL3:  return "TIB L3";
    case partitions::TIBL4:  return "TIB L4";
    case partitions::TOB  :  return "TOB";
    case partitions::TOBL1:  return "TOB L1";
    case partitions::TOBL2:  return "TOB L2";
    case partitions::TOBL3:  return "TOB L3";
    case partitions::TOBL4:  return "TOB L4";
    case partitions::TOBL5:  return "TOB L5";
    case partitions::TOBL6:  return "TOB L6";
    case partitions::TIDP:   return "TIDplus";
    case partitions::TIDM:   return "TIDminus";
    case partitions::TIDPD1: return "TID plus Disk 1";
    case partitions::TIDPD2: return "TID plus Disk 2";
    case partitions::TIDPD3: return "TID plus Disk 3";
    case partitions::TIDMD1: return "TID minus Disk 1";
    case partitions::TIDMD2: return "TID minus Disk 2";
    case partitions::TIDMD3: return "TID minus Disk 3";  
    case partitions::TECP:   return "TECplus";
    case partitions::TECM:   return "TECminus";
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
