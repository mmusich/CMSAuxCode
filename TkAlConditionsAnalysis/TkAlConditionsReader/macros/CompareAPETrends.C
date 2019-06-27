#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1.h"
#include "TObjArray.h"
#include "TMath.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TPad.h"
#include "TColor.h"
#include "iostream"
#include "TPaveText.h"

/*--------------------------------------------------------------------*/
  std::pair<std::pair<Double_t,Double_t>,std::pair<Double_t,Double_t> > getExtrema(TObjArray *array){
/*--------------------------------------------------------------------*/

  TGraph* g0 = static_cast<TGraph*>(array->At(0));

  Double_t theYMaximum = TMath::MinElement(g0->GetN(),g0->GetY()); 
  Double_t theYMinimum = TMath::MaxElement(g0->GetN(),g0->GetY()); 

  Double_t theXMaximum = TMath::MinElement(g0->GetN(),g0->GetX()); 
  Double_t theXMinimum = TMath::MaxElement(g0->GetN(),g0->GetX());

  //cout << theMinimum << " " << theMaximum << endl;
  
  for(Int_t i = 0; i< array->GetSize(); i++){

    TGraph* gi = static_cast<TGraph*>(array->At(i));
    Double_t tempYMax = TMath::MaxElement(gi->GetN(),gi->GetY());
    Double_t tempYMin = TMath::MinElement(gi->GetN(),gi->GetY());
    
    Double_t tempXMax = TMath::MaxElement(gi->GetN(),gi->GetX());
    Double_t tempXMin = TMath::MinElement(gi->GetN(),gi->GetX());

    if( tempYMax > theYMaximum){
      theYMaximum = tempYMax;
      //cout<<"i= "<<i<<" theMaximum="<<theMaximum<<endl;
    }

    if( tempYMin < theYMinimum){
      theYMinimum = tempYMin;
      //cout<<"i= "<<i<<" theMinimum="<<theMaximum<<endl;
    }

    
    if( tempXMax > theXMaximum){
      theXMaximum = tempXMax;
      //cout<<"i= "<<i<<" theMaximum="<<theMaximum<<endl;
    }

    if( tempXMin < theXMinimum){
      theXMinimum = tempXMin;
      //cout<<"i= "<<i<<" theMinimum="<<theMaximum<<endl;
    }

  }
  
  return std::make_pair(std::make_pair(0.8*theYMinimum,1.3*theYMaximum),std::make_pair(theXMinimum,theXMaximum));
}



void beautify(TGraph *g){
  g->GetXaxis()->SetLabelFont(42);
  g->GetYaxis()->SetLabelFont(42);
  g->GetYaxis()->SetLabelSize(.05);
  g->GetXaxis()->SetLabelSize(.05);
  g->GetYaxis()->SetTitleSize(.05);
  g->GetXaxis()->SetTitleSize(.05);
  g->GetXaxis()->SetTitleOffset(1.1);
  g->GetYaxis()->SetTitleOffset(1.2);
  g->GetXaxis()->SetTitleFont(42);
  g->GetYaxis()->SetTitleFont(42);
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
  gStyle->SetOptStat("e");
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

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

}

void CompareAPETrends(TString file1, TString file2){
  
  setStyle();

  TFile *f1 = TFile::Open(file1);
  TFile *f2 = TFile::Open(file2);

  TString label1 = file1.ReplaceAll("APETrend_","").ReplaceAll(".root","");
  TString label2 = file2.ReplaceAll("APETrend_","").ReplaceAll(".root","");

  TString listOfGraphs[70] = {
    "g_average_APE_BPixL1o", 
    "g_average_APE_BPixL1i", 
    "g_average_APE_BPixL2o", 
    "g_average_APE_BPixL2i", 
    "g_average_APE_BPixL3o", 
    "g_average_APE_BPixL4i", 
    "g_average_APE_BPixL4o", 
    "g_average_APE_BPixL3i", 
    "g_average_APE_FPixmL1", 
    "g_average_APE_FPixmL2", 
    "g_average_APE_FPixmL3", 
    "g_average_APE_FPixpL1", 
    "g_average_APE_FPixpL2", 
    "g_average_APE_FPixpL3", 
    "g_average_APE_TIBL1Ro", 
    "g_average_APE_TIBL1Ri", 
    "g_average_APE_TIBL1So", 
    "g_average_APE_TIBL1Si", 
    "g_average_APE_TIBL2Ro", 
    "g_average_APE_TIBL2Ri", 
    "g_average_APE_TIBL2So", 
    "g_average_APE_TIBL2Si", 
    "g_average_APE_TIBL3o",  
    "g_average_APE_TIBL3i",  
    "g_average_APE_TIBL4o",  
    "g_average_APE_TIBL4i",  
    "g_average_APE_TOBL1Ro", 
    "g_average_APE_TOBL1Ri", 
    "g_average_APE_TOBL1So", 
    "g_average_APE_TOBL1Si", 
    "g_average_APE_TOBL2Ro", 
    "g_average_APE_TOBL2Ri", 
    "g_average_APE_TOBL2So", 
    "g_average_APE_TOBL2Si", 
    "g_average_APE_TOBL3o",  
    "g_average_APE_TOBL3i",  
    "g_average_APE_TOBL4o",  
    "g_average_APE_TOBL4i",  
    "g_average_APE_TOBL5o",  
    "g_average_APE_TOBL5i",  
    "g_average_APE_TOBL6o",  
    "g_average_APE_TOBL6i",  
    "g_average_APE_TIDmR1R", 
    "g_average_APE_TIDmR1S", 
    "g_average_APE_TIDmR2R", 
    "g_average_APE_TIDmR2S", 
    "g_average_APE_TIDmR3",  
    "g_average_APE_TIDpR1R", 
    "g_average_APE_TIDpR1S", 
    "g_average_APE_TIDpR2R", 			     
    "g_average_APE_TIDpR2S", 			      
    "g_average_APE_TIDpR3",
    "g_average_APE_TECmR1R",
    "g_average_APE_TECmR1S",
    "g_average_APE_TECmR2R",
    "g_average_APE_TECmR2S",
    "g_average_APE_TECmR3", 
    "g_average_APE_TECmR4", 
    "g_average_APE_TECmR5", 
    "g_average_APE_TECmR6", 
    "g_average_APE_TECmR7", 
    "g_average_APE_TECpR1R",
    "g_average_APE_TECpR1S",
    "g_average_APE_TECpR2R",
    "g_average_APE_TECpR2S",
    "g_average_APE_TECpR3", 
    "g_average_APE_TECpR4", 
    "g_average_APE_TECpR5", 
    "g_average_APE_TECpR6", 
    "g_average_APE_TECpR7" 
  };

  TCanvas *c1[69]; 

  for(int iGraph=0;iGraph<69;iGraph++){
    
    std::cout<<listOfGraphs[iGraph]<<std::endl;
    c1[iGraph] = new TCanvas(listOfGraphs[iGraph],listOfGraphs[iGraph],1200,800);
    TGraphErrors  *a = (TGraphErrors*)f1->Get("TrackerAPEReader/"+listOfGraphs[iGraph]);
    TGraphErrors  *b = (TGraphErrors*)f2->Get("TrackerAPEReader/"+listOfGraphs[iGraph]);

    c1[iGraph]->cd();

    a->SetLineColor(kRed);
    b->SetLineColor(kBlue);
    
    a->SetFillColorAlpha(kRed-1,  0.35);
    b->SetFillColorAlpha(kBlue-1, 0.35);

    /*
      a->SetFillColor(kRed);
      b->SetFillColor(kBlue);
    */

    a->SetMarkerColor(kRed);
    b->SetMarkerColor(kBlue);

    a->SetMarkerStyle(20);
    b->SetMarkerStyle(21);

    a->SetMarkerSize(2);
    b->SetMarkerSize(2);

    a->SetLineStyle(9);
    b->SetLineStyle(9);

    TLegend *lego = new TLegend(0.75,0.80,0.90,0.90);
    lego->SetLineWidth(0);
    lego->SetBorderSize(0);
    lego->SetFillColor(0);
    lego->AddEntry(a,label1,"PF");
    lego->AddEntry(b,label2,"P");

    TString title = a->GetName();

    TObjArray *array = new TObjArray(2); 
    array->Add(a);
    array->Add(b);

    std::pair<std::pair<Double_t,Double_t>,std::pair<Double_t,Double_t> >  extrema =  getExtrema(array);
 
    delete array;

    a->GetYaxis()->SetRangeUser(extrema.first.first,extrema.first.second);
    b->GetYaxis()->SetRangeUser(extrema.first.first,extrema.first.second);

    a->GetXaxis()->SetRangeUser(extrema.second.first,extrema.second.second);
    b->GetXaxis()->SetRangeUser(extrema.second.first,extrema.second.second);

    //    std::cout<<"xmin: "<<extrema.second.first<<" xmax:"<<extrema.second.second<<std::endl;

    /*
    if(title.Contains("average")){
      a->GetYaxis()->SetRangeUser(0.,40);
      b->GetYaxis()->SetRangeUser(0.,40);
    } else {
      a->GetYaxis()->SetRangeUser(0.,2);
      b->GetYaxis()->SetRangeUser(0.,2);
    }
    */

    a->GetXaxis()->SetNdivisions(305);
    b->GetXaxis()->SetNdivisions(305);
    
    /*
      a->SetFillStyle(3003);
      b->SetFillStyle(3003);
    */

    beautify(a);
    beautify(b);

    //a->Draw("3A");
    //b->Draw("3same");

    b->Draw("APLsame");
    a->Draw("PLsame");


    lego->Draw("same");

    static const Int_t nIOV_a = a->GetN();
    static const Int_t nIOV_b = b->GetN();

    //std::cout<< nIOV_a << " " << nIOV_b << std::endl;
    
    Double_t ax[nIOV_a],ay[nIOV_a];
    Double_t bx[nIOV_b],by[nIOV_b];

    TArrow* a_lines[nIOV_a];
    TArrow* b_lines[nIOV_b];

    c1[iGraph]->cd();
    c1[iGraph]->Update();

    for(Int_t IOV_ofA=0;IOV_ofA<nIOV_a;IOV_ofA++){
      a->GetPoint(IOV_ofA,ax[IOV_ofA],ay[IOV_ofA]); 
      a_lines[IOV_ofA] = new TArrow(ax[IOV_ofA],(c1[iGraph]->GetUymin()),ax[IOV_ofA],(c1[iGraph]->GetUymin()+0.2*(c1[iGraph]->GetUymax()-c1[iGraph]->GetUymin()) ),0.5,"|>");
      a_lines[IOV_ofA]->SetLineColor(kRed);
      a_lines[IOV_ofA]->SetLineStyle(1);
      a_lines[IOV_ofA]->SetLineWidth(1);
      a_lines[IOV_ofA]->Draw("same");
    }

    for(Int_t IOV_ofB=0;IOV_ofB<nIOV_b;IOV_ofB++){
      b->GetPoint(IOV_ofB,bx[IOV_ofB],by[IOV_ofB]); 
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
