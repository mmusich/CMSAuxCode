// -*- C++ -*-
//
// Package:    TkAlConditionsAnalysis/TkAlConditionsReader
// Class:      TkAlConditionsReader
// 
/**\class TkAlConditionsReader TkAlConditionsReader.cc TkAlConditionsAnalysis/TkAlConditionsReader/plugins/TkAlConditionsReader.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Marco Musich
//         Created:  Thu, 26 Jan 2017 09:52:39 GMT
//
//

// system include files
#include <memory>

// user include files

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CondFormats/Alignment/interface/AlignmentErrorsExtended.h"
#include "CondFormats/Alignment/interface/Alignments.h"
#include "CondFormats/AlignmentRecord/interface/TrackerAlignmentErrorExtendedRcd.h"
#include "CondFormats/AlignmentRecord/interface/TrackerAlignmentRcd.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/GeometrySurface/interface/Surface.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/PTrackerParametersRcd.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeomBuilderFromGeometricDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

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

namespace trk {

  enum partitions { 
    BPixL1o,          //0  Barrel Pixel Layer 1 outer
    BPixL1i,          //1  Barrel Pixel Layer 1 inner
    BPixL2o,          //2  Barrel Pixel Layer 2 outer
    BPixL2i,          //3  Barrel Pixel Layer 2 inner
    BPixL3o,          //4  Barrel Pixel Layer 3 outer
    BPixL3i,          //5  Barrel Pixel Layer 3 inner
    BPixL4o,          //6  Barrel Pixel Layer 4 outer
    BPixL4i,          //7  Barrel Pixel Layer 4 inner
    FPixmL1,          //8  Forward Pixel Minus side Disk 1
    FPixmL2,          //9 Forward Pixel Minus side Disk 2
    FPixmL3,          //10 Forward Pixel Minus side Disk 3
    FPixpL1,          //11 Forward Pixel Plus side Disk 1
    FPixpL2,          //12 Forward Pixel Plus side Disk 2
    FPixpL3,          //13 Forward Pixel Plus side Disk 3
    TIBL1Ro,          //14 Inner Barrel Layer 1 Rphi outer
    TIBL1Ri,          //15 Inner Barrel Layer 1 Rphi inner
    TIBL1So,          //16 Inner Barrel Layer 1 Stereo outer
    TIBL1Si,          //17 Inner Barrel Layer 1 Stereo inner
    TIBL2Ro,          //18 Inner Barrel Layer 2 Rphi outer
    TIBL2Ri,          //19 Inner Barrel Layer 2 Rphi inner
    TIBL2So,          //20 Inner Barrel Layer 2 Stereo outer
    TIBL2Si,          //21 Inner Barrel Layer 2 Stereo inner
    TIBL3o,           //22 Inner Barrel Layer 3 outer
    TIBL3i,           //23 Inner Barrel Layer 3 inner
    TIBL4o,           //24 Inner Barrel Layer 4 outer
    TIBL4i,           //25 Inner Barrel Layer 4 inner
    TOBL1Ro,          //26 Outer Barrel Layer 1 Rphi outer
    TOBL1Ri,          //27 Outer Barrel Layer 1 Rphi inner
    TOBL1So,          //28 Outer Barrel Layer 1 Stereo outer
    TOBL1Si,          //29 Outer Barrel Layer 1 Stereo inner
    TOBL2Ro,          //30 Outer Barrel Layer 2 Rphi outer
    TOBL2Ri,          //31 Outer Barrel Layer 2 Rphi inner
    TOBL2So,          //32 Outer Barrel Layer 2 Stereo outer
    TOBL2Si,          //33 Outer Barrel Layer 2 Stereo inner
    TOBL3o,           //34 Outer Barrel Layer 3 outer
    TOBL3i,           //35 Outer Barrel Layer 3 inner
    TOBL4o,           //36 Outer Barrel Layer 4 outer
    TOBL4i,           //37 Outer Barrel Layer 4 inner
    TOBL5o,           //38 Outer Barrel Layer 5 outer
    TOBL5i,           //39 Outer Barrel Layer 5 inner
    TOBL6o,           //40 Outer Barrel Layer 6 outer
    TOBL6i,           //41 Outer Barrel Layer 6 inner
    TIDmR1R,          //42 Inner Disk Minus side Ring 1 Rphi
    TIDmR1S,          //43 Inner Disk Minus side Ring 1 Stereo
    TIDmR2R,          //44 Inner Disk Minus side Ring 2 Rphi
    TIDmR2S,          //45 Inner Disk Minus side Ring 2 Stereo
    TIDmR3,           //46 Inner Disk Minus side Ring 3
    TIDpR1R,          //47 Inner Disk Plus side Ring 1 Rphi
    TIDpR1S,          //48 Inner Disk Plus side Ring 1 Stereo
    TIDpR2R,          //49 Inner Disk Plus side Ring 2 Rphi
    TIDpR2S,          //50 Inner Disk Plus side Ring 2 Stereo
    TIDpR3,           //51 Inner Disk Plus side Ring 3
    TECmR1R,          //52 Endcaps Minus side Ring 1 Rphi
    TECmR1S,          //53 Endcaps Minus side Ring 1 Stereo
    TECmR2R,          //54 Encdaps Minus side Ring 2 Rphi
    TECmR2S,          //55 Endcaps Minus side Ring 2 Stereo
    TECmR3,           //56 Endcaps Minus side Ring 3
    TECmR4,           //57 Endcaps Minus side Ring 4
    TECmR5,           //58 Endcaps Minus side Ring 5
    TECmR6,           //59 Endcaps Minus side Ring 6
    TECmR7,           //60 Endcaps Minus side Ring 7
    TECpR1R,          //61 Endcaps Plus side Ring 1 Rphi
    TECpR1S,          //62 Endcaps Plus side Ring 1 Stereo
    TECpR2R,          //63 Encdaps Plus side Ring 2 Rphi
    TECpR2S,          //64 Endcaps Plus side Ring 2 Stereo
    TECpR3,           //65 Endcaps Plus side Ring 3
    TECpR4,           //66 Endcaps Plus side Ring 4
    TECpR5,           //67 Endcaps Plus side Ring 5
    TECpR6,           //68 Endcaps Plus side Ring 6
    TECpR7,           //69 Endcaps Plus side Ring 7                       
    StripDoubleSide,  // 70 -- not to be considered
    NUM_OF_TYPES      // 71 -- default
  };
}

class TkAlConditionsReader : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit TkAlConditionsReader(const edm::ParameterSet&);
      ~TkAlConditionsReader();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      std::string getStringFromEnum(trk::partitions e);


      // ----------member data ---------------------------
  
      struct geometryFromDB
      {
	uint32_t m_rawid;  
	int      m_subdetid;
	int      m_layer;
	int      m_side;
	int      m_ring;
	bool     m_isRphi; 
	bool     m_isDoubleSide;
	int      m_uDirection;
	int      m_vDirection;
	int      m_wDirection;
	void init();
	void fillGeometryInfo(const DetId& detId,  const TrackerTopology* tTopo, const TrackerGeometry* tkGeeom);
	trk::partitions filterThePartition();
	void printAll();
      } geometryFromDB_;

      edm::Service<TFileService> fs;  
      std::auto_ptr<std::ofstream> output_;

      static const int nIOVs_  = 100; 
      static const int nParts_ = trk::NUM_OF_TYPES;

      int IOVcount_;

      edm::ESWatcher<TrackerAlignmentErrorExtendedRcd>  APEwatcher_;
    
      std::map<int,std::map<trk::partitions,TH1D*> > APETrendByPartition_ ; 
      TH1D* h_APEByPart_[nParts_][nIOVs_];

      std::map<trk::partitions , TGraphErrors* > g_APEs_ByPartition_ByIOV;
      std::map<trk::partitions , TGraph* >       g_APEs_RMSByPartition_ByIOV;

};


//
// static data member definitions
//

//
// constructors and destructor
//
TkAlConditionsReader::TkAlConditionsReader(const edm::ParameterSet& iConfig)

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


TkAlConditionsReader::~TkAlConditionsReader()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TkAlConditionsReader::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   bool hasAPEIOV = APEwatcher_.check(iSetup);

   if (hasAPEIOV){

     int run = iEvent.id().run();
     std::cout<< " Tracker APE have a new IOV for runs: " << iEvent.id().run() << std::endl;

     // Retrieve tracker topology from geometry
     edm::ESHandle<TrackerTopology> tTopoHandle;
     iSetup.get<TrackerTopologyRcd>().get(tTopoHandle);
     const TrackerTopology* tTopo = tTopoHandle.product();
     
     // Retrieve tracking geometry

     edm::ESHandle<GeometricDet> geometricDet;
     iSetup.get<IdealGeometryRecord>().get(geometricDet);
     
     edm::ESHandle<PTrackerParameters> ptp;
     iSetup.get<PTrackerParametersRcd>().get(ptp);
     
     TrackerGeomBuilderFromGeometricDet trackerBuilder;
     TrackerGeometry* tkGeom = trackerBuilder.build(&(*geometricDet), *ptp, tTopo);
     
     // assign to each element to the map the corresponding histogram

     for(int i = trk:: BPixL1o; i!=trk::NUM_OF_TYPES; i++){
       trk::partitions part = (trk::partitions) i;           
       APETrendByPartition_[run][part] = h_APEByPart_[part][IOVcount_];
     }

     std::cout << " after loop" << std::endl;

     edm::ESHandle<AlignmentErrorsExtended> alignmentErrors;
     iSetup.get<TrackerAlignmentErrorExtendedRcd>().get(alignmentErrors);
     
     Double_t dxx; //,dxy,dyy,dzx,dzy,dzz;

     std::vector<AlignTransformErrorExtended> alignErrors = alignmentErrors->m_alignError;
     for (std::vector<AlignTransformErrorExtended>::const_iterator i = alignErrors.begin(); i != alignErrors.end(); ++i){
    
       geometryFromDB_.init();

       if(DEBUG)
	 std::cout<< " after init " << std::endl;
       
       geometryFromDB_.m_rawid = i->rawId();

       if(DEBUG)
	 std::cout << " raw ID=" << i->rawId();
       
       DetId detid(geometryFromDB_.m_rawid);
       geometryFromDB_.m_subdetid = detid.subdetId();
       geometryFromDB_.fillGeometryInfo(detid,tTopo,tkGeom);
       
       if(DEBUG)
	 std::cout<< " after fill geometry " << std::endl;

       trk::partitions thePart = geometryFromDB_.filterThePartition();

       if(DEBUG)
	 std::cout<< " after retrieving partition " << this->getStringFromEnum(thePart)<<std::endl;

       CLHEP::HepSymMatrix errMatrix = i->matrix();
       dxx = errMatrix[0][0];
       /*
	 dxy = errMatrix[0][1];
	 dzx = errMatrix[0][2];
	 dyy = errMatrix[1][1];
	 dzy = errMatrix[1][2];
	 dzz = errMatrix[2][2];
       */

       if(thePart == trk::NUM_OF_TYPES) continue;

       if(thePart == trk::TOBL1Ro){
	 std::cout<<"==================="<<std::endl;
	 geometryFromDB_.printAll();
	 std::cout<<"APE(xx): " << dxx << std::endl;
       }

       h_APEByPart_[thePart][IOVcount_]->Fill(std::sqrt(dxx)*10000);
      
      

     } // ends loop on the alignables
     IOVcount_++;
   } // if there is a new IOV
}


// ------------ method called once each job just before starting event loop  ------------
void 
TkAlConditionsReader::beginJob()
{
  std::vector<TFileDirectory> dirs;
  for (int i=trk::BPixL1o;i!=trk::NUM_OF_TYPES;i++){
    trk::partitions part = (trk::partitions) i;           
    
    dirs.push_back(fs->mkdir(this->getStringFromEnum(part)));
    
    for(int j=0;j<nIOVs_; ++j){
      h_APEByPart_[i][j] = dirs[i].make<TH1D>(Form("APE_%s_IOV%i",this->getStringFromEnum(part).c_str(),j),Form("%s APE for IOV %i; APE d_{xx}; n. modules",this->getStringFromEnum(part).c_str(),j),100,0.,100.);
    } 
  }

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TkAlConditionsReader::endJob() 
{
   std::ostringstream output; 

  std::vector<float>  theBoundaries_;
  std::vector<float>  ex_;
  std::map<trk::partitions, std::vector<float> > the_APE_averages_;
  std::map<trk::partitions, std::vector<float> > the_APE_RMS_;

  output<<" the IOVs are at :" << std::endl;
  for(std::map< int,std::map<trk::partitions,TH1D*> >::iterator it = APETrendByPartition_.begin(); it != APETrendByPartition_.end(); ++it) {
    theBoundaries_.push_back(it->first);
    ex_.push_back(0.);
    output<<" - "<< it->first << std::endl;
  }

  for(unsigned int i=0;i<theBoundaries_.size();i++){
    
    int the_r = theBoundaries_[i]; 

    output<<"|====================================== " << std::endl
	  <<"| run: "<< the_r << std::endl; 
    
    for(int j = trk::BPixL1o; j!=trk::NUM_OF_TYPES; j++){
       trk::partitions part = (trk::partitions) j;           

      auto theMap = APETrendByPartition_[the_r];
      
      the_APE_averages_[part].push_back( theMap[part]->GetMean() );
      the_APE_RMS_[part].push_back( theMap[part]->GetRMS() );
	    
      output<<"| "<< std::setw(10) << this->getStringFromEnum(part) << " <APE(x)>: "<< std::setw(10) << std::setprecision(3) << the_APE_averages_[part][i] << " um | RMS(APE(x)): "<< std::setw(10) << std::setprecision(3) << the_APE_RMS_[part][i] << " um" << std::endl;
    }    
  }
  
  for(int i = trk::BPixL1o; i!=trk::NUM_OF_TYPES; i++){
    trk::partitions part = (trk::partitions) i;    

    g_APEs_ByPartition_ByIOV[part] = fs->make<TGraphErrors>(theBoundaries_.size(),&(theBoundaries_[0]),&(the_APE_averages_[part][0]),&(ex_[0]),&(the_APE_RMS_[part][0]));
    g_APEs_ByPartition_ByIOV[part]->SetName(Form("g_average_APE_%s",this->getStringFromEnum(part).c_str()) );
    g_APEs_ByPartition_ByIOV[part]->SetTitle(Form("average APE in %s",this->getStringFromEnum(part).c_str()));
    g_APEs_ByPartition_ByIOV[part]->GetXaxis()->SetTitle("IOV (run number)");
    g_APEs_ByPartition_ByIOV[part]->GetYaxis()->SetTitle("#LT #sqrt{d_{xx}}#GT #mum");
    
    g_APEs_RMSByPartition_ByIOV[part] = fs->make<TGraph>(theBoundaries_.size(),&(theBoundaries_[0]),&(the_APE_RMS_[part][0]));
    g_APEs_RMSByPartition_ByIOV[part]->SetName(Form("g_RMS_APE_%s",this->getStringFromEnum(part).c_str()) );
    g_APEs_RMSByPartition_ByIOV[part]->SetTitle(Form("RMS of APE in %s",this->getStringFromEnum(part).c_str()));
    g_APEs_RMSByPartition_ByIOV[part]->GetXaxis()->SetTitle("IOV (run number)");
    g_APEs_RMSByPartition_ByIOV[part]->GetYaxis()->SetTitle("RMS(#sqrt{d_{xx}}) #mum");

  }

  /// Final output - either message logger or output file:
  if (output_.get()) *output_ << output.str();
  else edm::LogInfo("") << output.str();
  
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TkAlConditionsReader::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// ------------ method to initialize the geometry struct info ---------------
void 
TkAlConditionsReader::geometryFromDB::init(){
  
  m_rawid         = 0;  
  m_subdetid      = -1;
  m_layer         = -1;
  m_side          = -1;
  m_ring          = -1;
  m_isRphi        = false;
  m_isDoubleSide  = false;
  m_uDirection    = 0;
  m_vDirection    = 0;
  m_wDirection    = 0;

}
  
// ------------ method to fill the geometry struct info ---------------
void 
TkAlConditionsReader::geometryFromDB::fillGeometryInfo(const DetId& detId, 
						       const TrackerTopology* tTopo, 
						       const TrackerGeometry* tkGeom){
  
  unsigned int subdetId = static_cast<unsigned int>(detId.subdetId());
 
  if ( subdetId == StripSubdetector::TIB) { 	   
    m_layer = tTopo->tibLayer(detId.rawId());
    m_side  = tTopo->tibSide(detId.rawId());
    m_isRphi = tTopo->isRPhi(detId.rawId());
    m_isDoubleSide = tTopo->tibIsDoubleSide(detId.rawId());
  }
  else if ( subdetId ==  StripSubdetector::TOB ){
    m_layer = tTopo->tobLayer(detId.rawId());
    m_side  = tTopo->tobSide(detId.rawId());
    m_isRphi = tTopo->isRPhi(detId.rawId());
    m_isDoubleSide = tTopo->tobIsDoubleSide(detId.rawId());
  }
  else if ( subdetId ==  StripSubdetector::TID) { 
    m_layer = tTopo->tidWheel(detId.rawId());
    m_side  = tTopo->tidSide(detId.rawId());
    m_isRphi = tTopo->isRPhi(detId.rawId());
    m_ring = tTopo->tidRing(detId.rawId());
    m_isDoubleSide = tTopo->tidIsDoubleSide(detId.rawId());
  }
  else if ( subdetId ==  StripSubdetector::TEC ){ 
    m_layer = tTopo->tecWheel(detId.rawId());
    m_side  = tTopo->tecSide(detId.rawId()); 
    m_isRphi = tTopo->isRPhi(detId.rawId());
    m_ring = tTopo->tecRing(detId.rawId());
    m_isDoubleSide = tTopo->tecIsDoubleSide(detId.rawId());
  }
  else if ( subdetId ==  PixelSubdetector::PixelBarrel ) { 
    m_layer = tTopo->pxbLayer(detId.rawId());  
  }
  else if ( subdetId ==  PixelSubdetector::PixelEndcap ) { 
    m_layer = tTopo->pxfDisk(detId.rawId()); 
    m_side  = tTopo->pxfSide(detId.rawId());
  }
  else
    edm::LogWarning("LogicError") << "Unknown subdetid: " <<  subdetId;


  const GeomDet& geomDet = *tkGeom->idToDet(*(&detId));
  const Surface& surface = (&geomDet)->surface();
  
  LocalPoint lPModule(0.,0.,0.), lUDirection(1.,0.,0.), lVDirection(0.,1.,0.), lWDirection(0.,0.,1.);
  GlobalPoint gPModule    = surface.toGlobal(lPModule), 
    gUDirection = surface.toGlobal(lUDirection),
    gVDirection = surface.toGlobal(lVDirection),
    gWDirection = surface.toGlobal(lWDirection);
  double dR(999.), dPhi(999.), dZ(999.);
  if(subdetId == PixelSubdetector::PixelBarrel || 
     subdetId == StripSubdetector::TIB ||
     subdetId==StripSubdetector::TOB){
    dR   = gWDirection.perp() - gPModule.perp();
    dPhi = deltaPhi(gUDirection.barePhi(),gPModule.barePhi());
    dZ   = gVDirection.z() - gPModule.z();
    m_uDirection = dPhi>0. ? 1 : -1;
    m_vDirection = dZ>0.   ? 1 : -1;
    m_wDirection = dR>0.   ? 1 : -1;
  } else if(subdetId==PixelSubdetector::PixelEndcap){
    dR   = gUDirection.perp() - gPModule.perp();
    dPhi = deltaPhi(gVDirection.barePhi(),gPModule.barePhi());
    dZ   = gWDirection.z() - gPModule.z();
    m_uDirection = dR>0.   ? 1 : -1;
    m_vDirection = dPhi>0. ? 1 : -1;
    m_wDirection = dZ>0.   ? 1 : -1;
  } else if(subdetId==StripSubdetector::TID || 
	    subdetId==StripSubdetector::TEC){
    dR = gVDirection.perp() - gPModule.perp();
    dPhi = deltaPhi(gUDirection.barePhi(),gPModule.barePhi());
    dZ = gWDirection.z() - gPModule.z();
    m_uDirection = dPhi>0. ? 1 : -1;
    m_vDirection = dR>0.   ? 1 : -1;
    m_wDirection = dZ>0.   ? 1 : -1;
  } else {
    edm::LogWarning("LogicError") << "Unknown subdetid: " <<  subdetId;
  }
}





// ------------ method to assign a partition based on the geometry struct info ---------------
trk::partitions
TkAlConditionsReader::geometryFromDB::filterThePartition(){
  
  trk::partitions ret = trk::NUM_OF_TYPES;

  if(m_isDoubleSide){
    return trk::StripDoubleSide;
  }

  // BPix
  if(m_subdetid==1){
    switch(m_layer)
      {
      case 1:
	m_wDirection > 0 ? ret = trk::BPixL1o  : ret = trk::BPixL1i;
	break;
      case 2:
	m_wDirection > 0 ? ret = trk::BPixL2o  : ret = trk::BPixL2i;
	break;
      case 3:
	m_wDirection > 0 ? ret = trk::BPixL3o  : ret = trk::BPixL3i;
      case 4:
	m_wDirection > 0 ? ret = trk::BPixL4o  : ret = trk::BPixL4i;
	break;
      default:
	edm::LogWarning("LogicError") << "Unknow BPix layer: " <<  m_layer;
	break;
      }
    // FPix
  } else if (m_subdetid==2) {
    switch(m_layer)
      {
      case 1:
	m_side > 1 ? ret = trk::FPixpL1 : ret = trk::FPixmL1; 
	break;
      case 2:
	m_side > 1 ? ret = trk::FPixpL2 : ret = trk::FPixmL2; 
	break;
      case 3:
	m_side > 1 ? ret = trk::FPixpL3 : ret = trk::FPixmL3;
	break;
      default:
	edm::LogWarning("LogicError") << "Unknow FPix disk: " <<  m_layer;
	break;
      }
    // TIB
  } else if (m_subdetid==3) {
    switch(m_layer)
      {
      case 1:
	if(m_isRphi){
	  m_wDirection > 0 ? ret = trk::TIBL1Ro : ret = trk::TIBL1Ri;
	} else {
	  // std::cout<<"TIB L1 Stereo"<<std::endl;
	  // std::cout<<"wDirection: "<< m_wDirection << std::endl;
	  m_wDirection > 0 ? ret = trk::TIBL1So : ret = trk::TIBL1Si;
	}
	break;
      case 2:
	if(m_isRphi){
	  m_wDirection > 0 ? ret = trk::TIBL2Ro  : ret = trk::TIBL2Ri;
	} else {
	  m_wDirection > 0 ? ret = trk::TIBL2So  : ret = trk::TIBL2Si;
	}
	break;
      case 3:
	m_wDirection > 0 ? ret = trk::TIBL3o  : ret = trk::TIBL3i;
	break;
      case 4:
	m_wDirection > 0 ? ret = trk::TIBL4o  : ret = trk::TIBL4i;
	break;
      default:
	edm::LogWarning("LogicError") << "Unknow TIB layer: " <<  m_layer;
	break;
      }
    // TID
  } else if (m_subdetid==4) {
    switch(m_ring)
      {
      case 1:
	if(m_isRphi){
	  m_side > 1 ? ret = trk::TIDpR1R  : ret = trk::TIDmR1R;
	} else {
	  m_side > 1 ? ret = trk::TIDpR1S  : ret = trk::TIDmR1S;
	}
	break;
      case 2:
	if(m_isRphi){
	  m_side > 1 ? ret = trk::TIDpR2R  : ret = trk::TIDmR2R;
	} else {
	  m_side > 1 ? ret = trk::TIDpR2S  : ret = trk::TIDmR2S;
	}
	break;
      case 3:
	m_side > 1 ? ret = trk::TIDpR3 : ret = trk::TIDmR3; 
	break;
      default:
	edm::LogWarning("LogicError") << "Unknow TID wheel: " <<  m_layer;
	break;
      }
    // TOB
  } else if (m_subdetid==5) {
    switch(m_layer)
      {
      case 1:
	if(m_isRphi){
	  m_wDirection > 0 ? ret = trk::TOBL1Ro  : ret = trk::TOBL1Ri;
	} else {
	  m_wDirection > 0 ? ret = trk::TOBL1So  : ret = trk::TOBL1Si;
	}
	break;
      case 2:
	if(m_isRphi){
	  m_wDirection > 0 ? ret = trk::TOBL2Ro  : ret = trk::TOBL2Ri;
	} else {
	  m_wDirection > 0 ? ret = trk::TOBL2So  : ret = trk::TOBL2Si;
	}
	break;
      case 3:
	m_wDirection > 0 ? ret = trk::TOBL3o  : ret = trk::TOBL3i;
	break;
      case 4:
	m_wDirection > 0 ? ret = trk::TOBL4o  : ret = trk::TOBL4i;
	break;
      case 5:
	m_wDirection > 0 ? ret = trk::TOBL5o  : ret = trk::TOBL5i;
	break;
      case 6:
	m_wDirection > 0 ? ret = trk::TOBL6o  : ret = trk::TOBL6i;
	break;
      default:
	edm::LogWarning("LogicError") << "Unknow TOB layer: " <<  m_layer;
	break;
      }
    // TEC
  } else if (m_subdetid==6) {

    switch (m_ring) 
      {
      case 1:
	if (m_isRphi) {
	  m_side > 1 ? ret = trk::TECpR1R : ret = trk::TECmR1R;
	} else {
	  m_side > 1 ? ret = trk::TECpR1S : ret = trk::TECmR1S;
	}
	break;
      case 2:
	if (m_isRphi) {
	  m_side > 1 ? ret = trk::TECpR2R : ret = trk::TECmR2R;
	} else {
	  m_side > 1 ? ret = trk::TECpR2S : ret = trk::TECmR2S;
	}
	break;
      case 3:
	m_side > 1 ? ret = trk::TECpR3 : ret = trk::TECmR3;
	break;
      case 4:
	m_side > 1 ? ret = trk::TECpR4 : ret = trk::TECmR4;
	break;
      case 5:
	m_side > 1 ? ret = trk::TECpR5 : ret = trk::TECmR5;
	break;
      case 6:
	m_side > 1 ? ret = trk::TECpR6 : ret = trk::TECmR6;
	break;
      case 7:
	m_side > 1 ? ret = trk::TECpR7 : ret = trk::TECmR7;
	break;
      default:
	edm::LogWarning("LogicError") << "Unknow TEC ring: " << m_ring;
	break;
      }
  }
  return ret;
}


void
TkAlConditionsReader::geometryFromDB::printAll(){

  std::cout<<" detId:"       <<   m_rawid     
	   <<" subdetid: "   <<	  m_subdetid  
	   <<" layer: "      <<	  m_layer   
	   <<" side: "       <<	  m_side      
	   <<" ring: "       <<	  m_ring      
	   <<" isRphi:"      <<   m_isRphi    
	   <<" isDoubleSide:"<<   m_isDoubleSide
	   <<" wDirection: " <<	  m_uDirection
	   << std::endl;
}

// -------------- method to get the topology from the detID ------------------------------
std::string 
TkAlConditionsReader::getStringFromEnum(trk::partitions e)
{
  switch(e)
    {
    case trk::BPixL1o: return "BPixL1o";            
    case trk::BPixL1i: return "BPixL1i"; 
    case trk::BPixL2o: return "BPixL2o"; 
    case trk::BPixL2i: return "BPixL2i"; 
    case trk::BPixL3o: return "BPixL3o"; 
    case trk::BPixL3i: return "BPixL3i"; 
    case trk::BPixL4o: return "BPixL4o";
    case trk::BPixL4i: return "BPixL4i";
    case trk::FPixmL1: return "FPixmL1"; 
    case trk::FPixmL2: return "FPixmL2"; 
    case trk::FPixmL3: return "FPixmL3"; 
    case trk::FPixpL1: return "FPixpL1"; 
    case trk::FPixpL2: return "FPixpL2"; 
    case trk::FPixpL3: return "FPixpL3";   
    case trk::TIBL1Ro: return "TIBL1Ro"; 
    case trk::TIBL1Ri: return "TIBL1Ri"; 
    case trk::TIBL1So: return "TIBL1So"; 
    case trk::TIBL1Si: return "TIBL1Si"; 
    case trk::TIBL2Ro: return "TIBL2Ro"; 
    case trk::TIBL2Ri: return "TIBL2Ri"; 
    case trk::TIBL2So: return "TIBL2So"; 
    case trk::TIBL2Si: return "TIBL2Si"; 
    case trk::TIBL3o:  return "TIBL3o";  
    case trk::TIBL3i:  return "TIBL3i";  
    case trk::TIBL4o:  return "TIBL4o";  
    case trk::TIBL4i:  return "TIBL4i";  
    case trk::TOBL1Ro: return "TOBL1Ro"; 
    case trk::TOBL1Ri: return "TOBL1Ri"; 
    case trk::TOBL1So: return "TOBL1So"; 
    case trk::TOBL1Si: return "TOBL1Si"; 
    case trk::TOBL2Ro: return "TOBL2Ro"; 
    case trk::TOBL2Ri: return "TOBL2Ri"; 
    case trk::TOBL2So: return "TOBL2So"; 
    case trk::TOBL2Si: return "TOBL2Si"; 
    case trk::TOBL3o:  return "TOBL3o";  
    case trk::TOBL3i:  return "TOBL3i";  
    case trk::TOBL4o:  return "TOBL4o";  
    case trk::TOBL4i:  return "TOBL4i";  
    case trk::TOBL5o:  return "TOBL5o";  
    case trk::TOBL5i:  return "TOBL5i";  
    case trk::TOBL6o:  return "TOBL6o";  
    case trk::TOBL6i:  return "TOBL6i";  
    case trk::TIDmR1R: return "TIDmR1R"; 
    case trk::TIDmR1S: return "TIDmR1S"; 
    case trk::TIDmR2R: return "TIDmR2R"; 
    case trk::TIDmR2S: return "TIDmR2S"; 
    case trk::TIDmR3:  return "TIDmR3";  
    case trk::TIDpR1R: return "TIDpR1R"; 
    case trk::TIDpR1S: return "TIDpR1S"; 
    case trk::TIDpR2R: return "TIDpR2R"; 
    case trk::TIDpR2S: return "TIDpR2S"; 
    case trk::TIDpR3:  return "TIDpR3";  
    case trk::TECmR1R: return "TECmR1R";         	  
    case trk::TECmR1S: return "TECmR1S";         		  
    case trk::TECmR2R: return "TECmR2R";  
    case trk::TECmR2S: return "TECmR2S";
    case trk::TECmR3:  return "TECmR3";         		  
    case trk::TECmR4:  return "TECmR4";          		  
    case trk::TECmR5:  return "TECmR5";           		  
    case trk::TECmR6:  return "TECmR6";           		  
    case trk::TECmR7:  return "TECmR7";           		  
    case trk::TECpR1R: return "TECpR1R";         	  
    case trk::TECpR1S: return "TECpR1S";         		  
    case trk::TECpR2R: return "TECpR2R";         	  
    case trk::TECpR2S: return "TECpR2S";          		  
    case trk::TECpR3:  return "TECpR3";           		  
    case trk::TECpR4:  return "TECpR4";           		  
    case trk::TECpR5:  return "TECpR5";          		  
    case trk::TECpR6:  return "TECpR6";          		  
    case trk::TECpR7:  return "TECpR7";                     
    case trk::StripDoubleSide: return "StripDoubleSide";                     
    default: 
      edm::LogWarning("LogicError") << "Unknown partition: " <<  e;
      return "";
    }
}


//define this as a plug-in
DEFINE_FWK_MODULE(TkAlConditionsReader);
