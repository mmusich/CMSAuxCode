// -*- C++ -*-
//
// Package:    AuxCode/FieldAndStripLatencyAnalyzer
// Class:      FieldAndStripLatencyAnalyzer
// 
/**\class FieldAndStripLatencyAnalyzer FieldAndStripLatencyAnalyzer.cc AuxCode/FieldAndStripLatencyAnalyzer/plugins/FieldAndStripLatencyAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Marco Musich
//         Created:  Thu, 16 Apr 2015 08:03:46 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"
#include "CondFormats/SiStripObjects/interface/SiStripLatency.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"

#include <string>
#include <TTree.h>
#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"

using namespace std;
using namespace edm;

//
// class declaration
//

class FieldAndStripLatencyAnalyzer : public edm::EDAnalyzer {
   public:
     explicit FieldAndStripLatencyAnalyzer(const edm::ParameterSet&);
     ~FieldAndStripLatencyAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------

      std::string TkTag_;

      edm::Service<TFileService> fs; 
  
      TTree *runInfoTree_;
      int ievt;
      int lastRun;
      int itrks;
      int mode;

      int numEventsInRun;
      int numTracksInRun;
  
      TH1D *modeByRun_;
      TH1D *fieldByRun_;

      std::map<int,std::pair<int,float> > conditionsMap_; 
      std::map<int,std::pair<float,float> > magFieldMap_;
      std::map<int,std::pair<int,int> > runInfoMap_;

      struct myRunInfo{
	int runnum;
	int numevents;
	int numtracks;
	int isPeak;
	float current;
	float magfield;
	void init();
      } theRunInfo;
  
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
FieldAndStripLatencyAnalyzer::FieldAndStripLatencyAnalyzer(const edm::ParameterSet& iConfig)
{
  TkTag_     = iConfig.getParameter<string>("TkTag");
  runInfoTree_ = 0;
  ievt = 0;
  itrks = 0;
  mode = 0;
}


FieldAndStripLatencyAnalyzer::~FieldAndStripLatencyAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

// ------------ method called for each event  ------------
void
FieldAndStripLatencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   ievt++;
   
   edm::Handle<reco::TrackCollection> trackCollection;
   iEvent.getByLabel(TkTag_, trackCollection);
   
   // magnetic field setup
   edm::ESHandle<MagneticField> magneticField_;
   iSetup.get<IdealMagneticFieldRecord>().get(magneticField_);
   float B_=magneticField_.product()->inTesla(GlobalPoint(0,0,0)).mag();
     
   // runInfo record
   edm::ESHandle<RunInfo> rInfo;
   iSetup.get<RunInfoRcd>().get(rInfo);
   const RunInfo* summary=rInfo.product();
   //summary->printAllValues();
  
   // iRecord.getRecord<RunInfoRcd>().get(rInfo);
   float current = summary->m_avg_current;
   //std::cout<<"current: "<<current<<", mag field: "<<B_<<" [T]"<<std::endl;

   // SiStrip latency
   edm::ESHandle<SiStripLatency> apvlat;
   iSetup.get<SiStripLatencyRcd>().get(apvlat);
   if(apvlat->singleReadOutMode()==1) mode =  1; // peak mode
   if(apvlat->singleReadOutMode()==0) mode = -1; // deco mode
   
   conditionsMap_[iEvent.run()].first  = mode;
   conditionsMap_[iEvent.run()].second = B_;

   magFieldMap_[iEvent.run()].first  = current;
   magFieldMap_[iEvent.run()].second = B_;

   const reco::TrackCollection tC = *(trackCollection.product());
   itrks+=tC.size();

   numEventsInRun++;
   numTracksInRun+=tC.size();

   runInfoMap_[iEvent.run()].first +=1;
   runInfoMap_[iEvent.run()].second +=tC.size();
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
FieldAndStripLatencyAnalyzer::beginJob()
{
  runInfoTree_= fs->make<TTree>("runInfoTree","run info analyzer ntuple");
  int bufsize = 64000;
  runInfoTree_->Branch("theRunInfoTree", &theRunInfo, 
		       "runnum/I:numevents:numtracks:isPeak:current/F:magfield", bufsize);  
  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
FieldAndStripLatencyAnalyzer::endJob() 
{
  std::cout<< "*******************************" << std::endl;
  std::cout<< "Events run in total: " << ievt << std::endl;
  std::cout<< "n. tracks: "<<itrks<<std::endl;
  std::cout<< "*******************************" << std::endl;

  Int_t nRuns    = conditionsMap_.size();
  
  vector<int> theRuns_;
  for(map<int,std::pair<int,float> >::iterator it = conditionsMap_.begin(); it != conditionsMap_.end(); ++it) {
    theRuns_.push_back(it->first);
  }

  sort(theRuns_.begin(),theRuns_.end());
  Int_t runRange = theRuns_[theRuns_.size()-1] - theRuns_[0];
  
  std::cout<< "*******************************" << std::endl;
  std::cout<<"first run: "<<theRuns_[0]<<std::endl;
  std::cout<<"last run:  "<<theRuns_[theRuns_.size()-1]<<std::endl;
  std::cout<<"considered runs: "<<nRuns<<std::endl;
  std::cout<< "*******************************" << std::endl;
  
  modeByRun_ = fs->make<TH1D>("modeByRun","Strip APV mode by run number;;APV mode (-1=deco,+1=peak)",runRange,theRuns_[0]-0.5,theRuns_[theRuns_.size()-1]-0.5);
  fieldByRun_= fs->make<TH1D>("fieldByRun","CMS B-field intensity by run number;;B-field intensity [T]",runRange,theRuns_[0]-0.5,theRuns_[theRuns_.size()-1]-0.5);
  
  for(Int_t the_r=theRuns_[0];the_r<=theRuns_[theRuns_.size()-1];the_r++){
    
    if(conditionsMap_.find(the_r)->second.first!=0){ 
      std::cout<<"run:"<<the_r<<" | isPeak: "<< std::setw(4) << conditionsMap_.find(the_r)->second.first
	       <<"| B-field: " << std::setw(8) <<conditionsMap_.find(the_r)->second.second<<" [T]"
	       <<"| current: " << std::setw(8) <<magFieldMap_.find(the_r)->second.first<<" "
	       <<"| events: " <<setw(10)<<runInfoMap_.find(the_r)->second.first << ", tracks "<< setw(10) << runInfoMap_.find(the_r)->second.second <<std::endl;
    }
    
    modeByRun_->SetBinContent(the_r-theRuns_[0],conditionsMap_.find(the_r)->second.first);
    fieldByRun_->SetBinContent(the_r-theRuns_[0],conditionsMap_.find(the_r)->second.second);
    // modeByRun_->GetXaxis()->SetBinLabel(the_r-theRuns_[0]+1,(const char*)the_r);
  }
}

// ------------ method called when starting to processes a run  ------------
void 
FieldAndStripLatencyAnalyzer::beginRun(edm::Run const& run, edm::EventSetup const& iSetup)
{
  numEventsInRun=0;
  numTracksInRun=0;

  theRunInfo.init();

}

// ------------ method called when ending the processing of a run  ------------
void 
FieldAndStripLatencyAnalyzer::endRun(edm::Run const& run, edm::EventSetup const& iSetup)
{
  lastRun=run.run();

  theRunInfo.runnum = lastRun;
  theRunInfo.numevents = numEventsInRun;
  theRunInfo.numtracks = numTracksInRun;
  //myRunInfo.isPeak =

  std::cout<<" ======> theRunInfo.runnum: " <<  theRunInfo.runnum 
	   <<" theRunInfo.numevents: " << theRunInfo.numevents 
	   <<" theRunInfo.numtracks: " << theRunInfo.numtracks 
	   <<" theRunInfo.isPeak: " << theRunInfo.isPeak 
	   <<" theRunInfo.current: " << theRunInfo.current 
	   <<" theRunInfo.magfield: "<< theRunInfo.magfield << std::endl;
  

  runInfoTree_->Fill();
  
}

void 
FieldAndStripLatencyAnalyzer::myRunInfo::init()
{
  int dummy_int = 9999;
  float dummy_float = 9999.;
  runnum    = dummy_int;
  numevents = dummy_int;
  numtracks = dummy_int;
  isPeak    = dummy_int;
  current   = dummy_float;
  magfield  = dummy_float;
}


// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
FieldAndStripLatencyAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
FieldAndStripLatencyAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FieldAndStripLatencyAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(FieldAndStripLatencyAnalyzer);
