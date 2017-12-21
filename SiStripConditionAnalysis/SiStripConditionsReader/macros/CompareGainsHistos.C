#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TPad.h"
#include "TColor.h"
#include "TObjArray.h"
#include "iostream"
#include "TPaveText.h"
#include <algorithm>

/*--------------------------------------------------------------------*/
std::pair<TH2F*,TH2F*> createRebinnedHistograms(TH2F* a,TH2F* b){
/*--------------------------------------------------------------------*/
  
  std::vector<double> binsA, binsB;
  for (int i = 1; i <= a->GetNbinsX()+1; i++) binsA.push_back(a->GetXaxis()->GetBinLowEdge(i));
  for (int j = 1; j <= b->GetNbinsX()+1; j++) binsB.push_back(b->GetXaxis()->GetBinLowEdge(j));

  std::cout<< "A" << std::endl;
  for(const auto bin : binsA) std::cout<< bin <<","; 
  std::cout << std::endl;

  std::cout<< "B" << std::endl;
  for(const auto bin : binsB) std::cout<< bin <<","; 
  std::cout << std::endl;

  std::vector<double> unified;
 
  std::set_union(binsA.begin(), binsA.end(),
		 binsB.begin(), binsB.end(),                  
		 std::back_inserter(unified));
  
  std::cout<< "common" << std::endl;
  for(const auto bin : unified) std::cout<< bin <<","; 
  std::cout << std::endl;

  TH2F* a_rebinned = new TH2F(((TString)a->GetName()+"_A_rebin").Data(),((TString)a->GetTitle()+";"+(TString)a->GetXaxis()->GetTitle()+";"+(TString)a->GetYaxis()->GetTitle()).Data(),unified.size()-1,&(unified[0]),a->GetNbinsY(),a->GetYaxis()->GetXbins()->GetArray());
  TH2F* b_rebinned = new TH2F(((TString)b->GetName()+"_B_rebin").Data(),((TString)b->GetTitle()+";"+(TString)b->GetXaxis()->GetTitle()+";"+(TString)b->GetXaxis()->GetTitle()).Data(),unified.size()-1,&(unified[0]),b->GetNbinsY(),b->GetYaxis()->GetXbins()->GetArray());

  // fill the rebinned A histogram
  for (int i = 1; i <= a_rebinned->GetNbinsX(); i++){
    for (int j = 1; j <= a_rebinned->GetNbinsY(); j++){
      auto x = a_rebinned->GetXaxis()->GetBinCenter(i);
      auto y = a_rebinned->GetYaxis()->GetBinCenter(j);
      a_rebinned->SetBinContent(i,j,a->GetBinContent(a->FindBin(x,y)));
    }
  }

  // fill the rebinned B histogram
  for (int i = 1; i <= b_rebinned->GetNbinsX(); i++){
    for (int j = 1; j <= b_rebinned->GetNbinsY(); j++){
      auto x = b_rebinned->GetXaxis()->GetBinCenter(i);
      auto y = b_rebinned->GetYaxis()->GetBinCenter(j);
      b_rebinned->SetBinContent(i,j,b->GetBinContent(b->FindBin(x,y)));
    }
  }

  return std::make_pair(a_rebinned,b_rebinned);

}


enum palette {HALFGRAY,GRAY,BLUES,REDS,ANTIGRAY,FIRE,ANTIFIRE,LOGREDBLUE,LOGBLUERED,DEFAULT};

/*--------------------------------------------------------------------*/
void setPaletteStyle(palette palette) 
/*--------------------------------------------------------------------*/
{  
  const int NRGBs = 5;
  const int NCont = 255;
  
  switch(palette){
    
  case HALFGRAY:
    {
      double stops[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
      double red[NRGBs]   = {1.00, 0.91, 0.80, 0.67, 1.00};
      double green[NRGBs] = {1.00, 0.91, 0.80, 0.67, 1.00};
      double blue[NRGBs]  = {1.00, 0.91, 0.80, 0.67, 1.00};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    }
    break;
    
  case GRAY:
    {
      double stops[NRGBs] = {0.00, 0.01, 0.05, 0.09, 0.1};
      double red[NRGBs]   = {1.00, 0.84, 0.61, 0.34, 0.00};
      double green[NRGBs] = {1.00, 0.84, 0.61, 0.34, 0.00};
      double blue[NRGBs]  = {1.00, 0.84, 0.61, 0.34, 0.00};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    }
    break;
      
  case BLUES:
    {
      double stops[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
      double red[NRGBs]   = {1.00, 0.84, 0.61, 0.34, 0.00};
      double green[NRGBs] = {1.00, 0.84, 0.61, 0.34, 0.00};
      double blue[NRGBs]  = {1.00, 1.00, 1.00, 1.00, 1.00};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    }
    break;
      
  case REDS:
    {
      double stops[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
      double red[NRGBs]   = {1.00, 1.00, 1.00, 1.00, 1.00};
      double green[NRGBs] = {1.00, 0.84, 0.61, 0.34, 0.00};
      double blue[NRGBs]  = {1.00, 0.84, 0.61, 0.34, 0.00};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);	
    }
    break;
    
  case ANTIGRAY:
    {
      double stops[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
      double red[NRGBs]   = {0.00, 0.34, 0.61, 0.84, 1.00};
      double green[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
      double blue[NRGBs]  = {0.00, 0.34, 0.61, 0.84, 1.00};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);		
    }      
    break;
      
  case FIRE:
    {
      double stops[NRGBs] = {0.00, 0.20, 0.80, 1.00};
      double red[NRGBs]   = {1.00, 1.00, 1.00, 0.50};
      double green[NRGBs] = {1.00, 1.00, 0.00, 0.00};
      double blue[NRGBs]  = {0.20, 0.00, 0.00, 0.00};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);	
    }
    break;
    
  case ANTIFIRE:
    {
      double stops[NRGBs] = {0.00, 0.20, 0.80, 1.00};
      double red[NRGBs]   = {0.50, 1.00, 1.00, 1.00};
      double green[NRGBs] = {0.00, 0.00, 1.00, 1.00};
      double blue[NRGBs]  = {0.00, 0.00, 0.00, 0.20};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);		
    }
    break;
    
  case LOGREDBLUE:
    {
      double stops[NRGBs] = {0.0001, 0.0010, 0.0100, 0.1000,  1.0000};
      double red[NRGBs]   = {1.00,   0.75,   0.50,   0.25,    0.00};
      double green[NRGBs] = {0.00,   0.00,   0.00,   0.00,    0.00};
      double blue[NRGBs]  = {0.00,   0.25,   0.50,   0.75,    1.00};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);		
    }
    break;
      
  case LOGBLUERED:
    {
      double stops[NRGBs] = {0.0001, 0.0010, 0.0100, 0.1000,  1.0000};
      double red[NRGBs]   = {0.00,   0.25,   0.50,   0.75,    1.00};
      double green[NRGBs] = {0.00,   0.00,   0.00,   0.00,    0.00};
      double blue[NRGBs]  = {1.00,   0.75,   0.50,   0.25,    0.00};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);			
    } 
    break;
    
  case DEFAULT:
    {
      double stops[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
      double red[NRGBs]   = {0.00, 0.00, 0.87, 1.00, 0.51};
      double green[NRGBs] = {0.00, 0.81, 1.00, 0.20, 0.00};
      double blue[NRGBs]  = {0.51, 1.00, 0.12, 0.00, 0.00};
      TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);	
    }
    break;
  default:
    std::cout<<"should nevere be here" << std::endl;
    break;
  }  
  gStyle->SetNumberContours(NCont);
}

/*--------------------------------------------------------------------*/
void beautify(TH1 *g){
/*--------------------------------------------------------------------*/
  g->GetXaxis()->SetLabelFont(42);
  g->GetYaxis()->SetLabelFont(42);
  g->GetYaxis()->SetLabelSize(.05);
  g->GetXaxis()->SetLabelSize(.05);
  g->GetYaxis()->SetTitleSize(.05);
  g->GetXaxis()->SetTitleSize(.05);
  g->GetXaxis()->CenterTitle(true);
  g->GetYaxis()->CenterTitle(true);
  g->GetXaxis()->SetTitleOffset(1.1);
  g->GetYaxis()->SetTitleOffset(1.2);
  g->GetXaxis()->SetTitleFont(42);
  g->GetYaxis()->SetTitleFont(42);
}

/*--------------------------------------------------------------------*/
std::pair<Double_t,Double_t> getExtrema(TObjArray *array){
/*--------------------------------------------------------------------*/

  Double_t theMaximum = -999.;
  Double_t theMinimum =  999.;
  
  for(Int_t i = 0; i< array->GetSize(); i++){

    Double_t tempMax = ((TH1F*)array->At(i))->GetMaximum();
    Double_t tempMin = ((TH1F*)array->At(i))->GetMinimum();

    if( tempMax > theMaximum){
      theMaximum = tempMax;
    }

    if( tempMin < theMinimum){
      theMinimum = tempMin;
    }
  }
  
  return std::make_pair(0.95*theMinimum,1.05*theMaximum);
}


/*--------------------------------------------------------------------*/
void setStyle(){
/*--------------------------------------------------------------------*/

  /*
    bool writeExtraText = true;       // if extra text
    TString lumi_13TeV     = "p-p collisions";
    TString extraText = "Internal";
  */

  TGaxis::SetMaxDigits(6);
  
  TH1::StatOverflows(kTRUE);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(0);
  gStyle->SetPadTopMargin(0.09);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBorderMode(0);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFontSize(0.06);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetStatColor(kWhite);
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.05);///---> gStyle->SetStatFontSize(0.025);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  gStyle->SetPadTickY(1);
  gStyle->SetPadBorderMode(0);
  gStyle->SetOptFit(1);
  gStyle->SetNdivisions(510);

}

void CompareGainsTrends(TString file1, TString file2,int theIOVNumber){
  
  setStyle();
  setPaletteStyle(GRAY);
  
  TFile *f1 = TFile::Open(file1);
  TFile *f2 = TFile::Open(file2);

  TString label1 = file1.ReplaceAll("GainTrend_","").ReplaceAll(".root","");
  TString label2 = file2.ReplaceAll("GainTrend_","").ReplaceAll(".root","");

  TString listOfGraphs[48] = {"h_average_Gain_TIB",
			      "h_average_Gain_TIB L1",
			      "h_average_Gain_TIB L2",
			      "h_average_Gain_TIB L3",
			      "h_average_Gain_TIB L4",
			      "h_average_Gain_TOB L1",
			      "h_average_Gain_TOB",
			      "h_average_Gain_TOB L2",
			      "h_average_Gain_TOB L3",
			      "h_average_Gain_TOB L4",
			      "h_average_Gain_TOB L5",
			      "h_average_Gain_TOB L6",
			      "h_average_Gain_TIDplus",
			      "h_average_Gain_TIDminus",
			      "h_average_Gain_TECplus",
			      "h_average_Gain_TECminus",
			      
			      "h_RMS_Gain_TIB",
			      "h_RMS_Gain_TIB L1",
			      "h_RMS_Gain_TIB L2",
			      "h_RMS_Gain_TIB L3",
			      "h_RMS_Gain_TIB L4",
			      "h_RMS_Gain_TOB",
			      "h_RMS_Gain_TOB L1",
			      "h_RMS_Gain_TOB L2",
			      "h_RMS_Gain_TOB L3",
			      "h_RMS_Gain_TOB L4",
			      "h_RMS_Gain_TOB L5",
			      "h_RMS_Gain_TOB L6",
			      "h_RMS_Gain_TIDplus",
			      "h_RMS_Gain_TIDminus",
			      "h_RMS_Gain_TECplus",
			      "h_RMS_Gain_TECminus",

			      "h2_Gain_TIB",
			      "h2_Gain_TIB L1",
			      "h2_Gain_TIB L2",
			      "h2_Gain_TIB L3",
			      "h2_Gain_TIB L4",
			      "h2_Gain_TOB L1",
			      "h2_Gain_TOB",
			      "h2_Gain_TOB L2",
			      "h2_Gain_TOB L3",
			      "h2_Gain_TOB L4",
			      "h2_Gain_TOB L5",
			      "h2_Gain_TOB L6",
			      "h2_Gain_TIDplus",
			      "h2_Gain_TIDminus",
			      "h2_Gain_TECplus",
			      "h2_Gain_TECminus",

			      };
  
  TCanvas *c1[48]; 

  for(int iGraph=0;iGraph<48;iGraph++){
    
    std::cout<<listOfGraphs[iGraph]<<std::endl;
    c1[iGraph] = new TCanvas(listOfGraphs[iGraph],listOfGraphs[iGraph],1200,800);
    TH1F  *a = (TH1F*)f1->Get("SiStripGainAverage/"+listOfGraphs[iGraph]);
    TH1F  *b = (TH1F*)f2->Get("SiStripGainAverage/"+listOfGraphs[iGraph]);

    c1[iGraph]->cd();

    a->SetLineColor(kRed);
    b->SetLineColor(kBlue);
    
    
    /*
      a->SetFillColor(kRed);
      b->SetFillColor(kBlue);
    */

    a->SetMarkerColor(kRed);
    b->SetMarkerColor(kBlue);

    TLegend *lego = new TLegend(0.15,0.80,0.40,0.90);
    lego->SetLineWidth(0);
    lego->SetBorderSize(0);
    lego->SetFillColor(0);
    
    if(!((TString)a->GetName()).Contains("h2")){
      lego->AddEntry(a,label1,"PL");
      lego->AddEntry(b,label2,"PL");
    } else {

      a->SetFillColorAlpha(kRed,  0.30);
      b->SetFillColorAlpha(kBlue, 0.30);

      a->SetFillStyle(3005);
      b->SetFillStyle(3004);

      lego->AddEntry(a,label1,"F");
      lego->AddEntry(b,label2,"F");
    }

    TString title = a->GetName();

    TObjArray *array = new TObjArray(2); 
    array->Add(a);
    array->Add(b);

    std::pair<Double_t,Double_t> extrema = getExtrema(array);

    delete array;

    a->GetYaxis()->SetRangeUser(extrema.first,extrema.second);
    b->GetYaxis()->SetRangeUser(extrema.first,extrema.second);
    
    // if(title.Contains("average")){
    //   a->GetYaxis()->SetRangeUser(0.85,1.25);
    //   b->GetYaxis()->SetRangeUser(0.85,1.25);
    // } else {
    //   a->GetYaxis()->SetRangeUser(0.1,0.2);
    //   b->GetYaxis()->SetRangeUser(0.1,0.2);
    // }

    a->GetXaxis()->SetNdivisions(305);
    b->GetXaxis()->SetNdivisions(305);
    
    /*
      a->SetFillStyle(3003);
      b->SetFillStyle(3003);
    */

    beautify(a);
    beautify(b);

    if(!((TString)a->GetName()).Contains("h2")){

      a->SetMarkerStyle(kFullCircle);
      b->SetMarkerStyle(kOpenSquare);

      a->SetMarkerSize(2);
      b->SetMarkerSize(2);

      a->SetLineStyle(1);
      b->SetLineStyle(7);

      a->SetLineWidth(2);
      b->SetLineWidth(2);

      a->Draw("E1");
      b->Draw("E1same");

      a->Draw("Psame");
      b->Draw("Psame");

    } else {

     
      /*
	a->SetFillColorAlpha(kRed,  0.30);
	b->SetFillColorAlpha(kBlue, 0.30);
	//a->GetYaxis()->SetRangeUser(0.6,1.4);
	//b->GetYaxis()->SetRangeUser(0.6,1.4);
	a->Draw("box");
	b->Draw("boxsame");
      */	


      auto pair = createRebinnedHistograms((TH2F*)a,(TH2F*)b);

      beautify(pair.first);
      beautify(pair.second);

      pair.first->GetXaxis()->SetNdivisions(305);
      pair.second->GetXaxis()->SetNdivisions(305);

      pair.first->SetFillColorAlpha(kRed,0.30);
      pair.second->SetFillColorAlpha(kBlue,0.30);

      pair.first->SetFillStyle(3005);
      pair.second->SetFillStyle(3004);

      pair.first->SetMarkerColor(kRed);
      pair.second->SetMarkerColor(kBlue);

      pair.first->SetMarkerStyle(kFullCircle);
      pair.second->SetMarkerColor(kOpenSquare);

      pair.first->SetLineColor(kRed);
      pair.second->SetLineColor(kBlue);

      pair.first->GetYaxis()->SetRangeUser(0.3,1.7);
      pair.second->GetYaxis()->SetRangeUser(0.3,1.7);

      pair.first->Draw("candle3");
      pair.second->Draw("candle3same");
      
      auto h_a_pfx_tmp = (TProfile*)(((TH2F*)a)->ProfileX("_apfx",1,-1,"o"));
      h_a_pfx_tmp->SetName(TString(a->GetName())+"_pfx");
      h_a_pfx_tmp->SetStats(kFALSE);
      h_a_pfx_tmp->SetMarkerColor(kRed);
      h_a_pfx_tmp->SetLineColor(kRed);
      h_a_pfx_tmp->SetLineWidth(2); 
      h_a_pfx_tmp->SetMarkerSize(2); 
      h_a_pfx_tmp->SetMarkerStyle(kFullCircle);

      auto h_b_pfx_tmp = (TProfile*)(((TH2F*)b)->ProfileX("_bpfx",1,-1,"o"));
      h_b_pfx_tmp->SetName(TString(b->GetName())+"_pfx");
      h_b_pfx_tmp->SetStats(kFALSE);
      h_b_pfx_tmp->SetMarkerColor(kBlue);
      h_b_pfx_tmp->SetLineColor(kBlue);
      h_b_pfx_tmp->SetMarkerSize(2);
      h_b_pfx_tmp->SetLineWidth(2);  
      h_b_pfx_tmp->SetMarkerStyle(kOpenSquare);

      //h_a_pfx_tmp->Draw("PLsame");
      //h_b_pfx_tmp->Draw("PLsame");
      
    }

    lego->Draw("same");

    static const Int_t nIOV_a = a->GetNbinsX () ;
    static const Int_t nIOV_b = b->GetNbinsX () ;

    //std::cout<< nIOV_a << " " << nIOV_b << std::endl;
    
    Double_t ax[nIOV_a],ay[nIOV_a];
    Double_t bx[nIOV_b],by[nIOV_b];

    TArrow* a_lines[nIOV_a];
    TArrow* b_lines[nIOV_b];

    c1[iGraph]->cd();
    c1[iGraph]->Update();

    for(Int_t IOV_ofA=0;IOV_ofA<nIOV_a;IOV_ofA++){
      ax[IOV_ofA] = a->GetXaxis()->GetBinLowEdge(IOV_ofA+1);
      ay[IOV_ofA] = a->GetBinContent(IOV_ofA+1);
      a_lines[IOV_ofA] = new TArrow(ax[IOV_ofA],(c1[iGraph]->GetUymin()),ax[IOV_ofA],(c1[iGraph]->GetUymin()+0.2*(c1[iGraph]->GetUymax()-c1[iGraph]->GetUymin()) ),0.5,"|>");
      a_lines[IOV_ofA]->SetLineColor(kRed);
      a_lines[IOV_ofA]->SetLineStyle(1);
      a_lines[IOV_ofA]->SetLineWidth(1);
      a_lines[IOV_ofA]->Draw("same");
    }

    for(Int_t IOV_ofB=0;IOV_ofB<nIOV_b;IOV_ofB++){
      bx[IOV_ofB] = b->GetXaxis()->GetBinLowEdge(IOV_ofB+1);
      by[IOV_ofB] = b->GetBinContent(IOV_ofB+1);
      b_lines[IOV_ofB] = new TArrow(bx[IOV_ofB],c1[iGraph]->GetUymin(),bx[IOV_ofB],(c1[iGraph]->GetUymin()+0.1*(c1[iGraph]->GetUymax()-c1[iGraph]->GetUymin())),0.5,"|>");
      b_lines[IOV_ofB]->SetLineColor(kBlue);
      b_lines[IOV_ofB]->SetLineStyle(1);
      b_lines[IOV_ofB]->SetLineWidth(1);
      b_lines[IOV_ofB]->Draw("same");
    }

    TPaveText* runnumbers[nIOV_a];

    /*
      Double_t range = c1[iGraph]->GetUxmax() - c1[iGraph]->GetUxmin();
      std::cout<<c1[iGraph]->GetUxmax()<<" "<<c1[iGraph]->GetUxmin()<<std::endl;
    */

    for(Int_t IOV_ofA=0;IOV_ofA<nIOV_a;IOV_ofA++){
   
      Int_t ix1;
      Int_t ix2;
      Int_t iw = gPad->GetWw();
      Int_t ih = gPad->GetWh();
      Double_t x1p,y1p,x2p,y2p;
      gPad->GetPadPar(x1p,y1p,x2p,y2p);
      ix1 = (Int_t)(iw*x1p);
      ix2 = (Int_t)(iw*x2p);
      Double_t wndc  = TMath::Min(1.,(Double_t)iw/(Double_t)ih);
      Double_t rw    = wndc/(Double_t)iw;
      Double_t x1ndc = (Double_t)ix1*rw;
      Double_t x2ndc = (Double_t)ix2*rw;
      Double_t rx1,ry1,rx2,ry2;
      gPad->GetRange(rx1,ry1,rx2,ry2);
      Double_t rx = (x2ndc-x1ndc)/(rx2-rx1);
      Double_t _sx;
      _sx = rx*(ax[IOV_ofA]-rx1)+x1ndc+0.005; //-0.05;
      Double_t _dx = _sx+0.05;
     
      Int_t index;
      if(IOV_ofA<5) 
	index=IOV_ofA;
      else{
	index=IOV_ofA-5;
      }

      runnumbers[IOV_ofA] = new TPaveText(_sx,0.14+(0.03*index),_dx,(0.17+0.03*index),"NDC");
      runnumbers[IOV_ofA]->SetFillColor(0);
      runnumbers[IOV_ofA]->SetLineColor(kRed);
      runnumbers[IOV_ofA]->SetLineWidth(2);
      runnumbers[IOV_ofA]->SetTextColor(kRed);
      runnumbers[IOV_ofA]->SetTextColor(1);
      runnumbers[IOV_ofA]->SetTextFont(42);
      //runnumbers[IOV_ofA]->SetTextAlign(11);
      TText *textRun = runnumbers[IOV_ofA]->AddText(Form("%i",int(ax[IOV_ofA])));
      textRun->SetTextSize(0.028);
      textRun->SetTextColor(kRed);
      
      runnumbers[IOV_ofA]->Draw("same");
    }
  
    TArrow* theIOV = new TArrow(theIOVNumber,c1[iGraph]->GetUymin(),theIOVNumber,c1[iGraph]->GetUymax());
    theIOV->SetLineColor(kMagenta);
    theIOV->SetLineStyle(1);
    theIOV->SetLineWidth(3);
    theIOV->Draw("same");

    Int_t ix1;
    Int_t ix2;
    Int_t iw = gPad->GetWw();
    Int_t ih = gPad->GetWh();
    Double_t x1p,y1p,x2p,y2p;
    gPad->GetPadPar(x1p,y1p,x2p,y2p);
    ix1 = (Int_t)(iw*x1p);
    ix2 = (Int_t)(iw*x2p);
    Double_t wndc  = TMath::Min(1.,(Double_t)iw/(Double_t)ih);
    Double_t rw    = wndc/(Double_t)iw;
    Double_t x1ndc = (Double_t)ix1*rw;
    Double_t x2ndc = (Double_t)ix2*rw;
    Double_t rx1,ry1,rx2,ry2;
    gPad->GetRange(rx1,ry1,rx2,ry2);
    Double_t rx = (x2ndc-x1ndc)/(rx2-rx1);
    Double_t _sx;
    _sx = rx*(theIOVNumber-rx1)+x1ndc+0.015; //-0.05;
    Double_t _dx = _sx+0.1;
    
    TPaveText *theIOVtext = new TPaveText(_sx,0.84,_dx,0.87,"NDC");
    theIOVtext->SetFillColor(kYellow);
    theIOVtext->SetLineColor(kMagenta);
    theIOVtext->SetLineWidth(2);
    theIOVtext->SetTextColor(kMagenta);
    theIOVtext->SetTextColor(1);
    theIOVtext->SetTextFont(42);
    //theIOVtext->SetTextAlign(11);
    TText *textRun = theIOVtext->AddText(Form("%i",theIOVNumber));
    textRun->SetTextSize(0.040);
    textRun->SetTextColor(kMagenta);
      
    theIOVtext->Draw("same");


    TString canvasName  = listOfGraphs[iGraph]+".pdf";
    TString canvasName2 = listOfGraphs[iGraph]+".png";

    canvasName.ReplaceAll(" ","_");
    canvasName2.ReplaceAll(" ","_");

    c1[iGraph]->SaveAs(canvasName);
    c1[iGraph]->SaveAs(canvasName2);
    //delete c1;
    //delete a;
    //delete b;
  }

}
