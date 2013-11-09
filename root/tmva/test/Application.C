/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TTreeFormula.h"
#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif



using namespace TMVA;

void Application( ) 
{   

TString myMethodList = "BDT" ;


#ifdef __CINT__
   gROOT->ProcessLine( ".O0" ); // turn off optimization in CINT
#endif

TString *filenameOut;

filenameOut = new TString("TMVA-RESULT.root");
   //---------------------------------------------------------------

   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   // --- Cut optimisation
   Use["Cuts"]            = 1;
   Use["CutsD"]           = 1;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   // 
   // --- 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 1;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 1; // the "PCA" extension indicates PCA-trans formed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   //
   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 1;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDEFoam"]         = 1;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 1; // k-nearest neighbour method
   //
   // --- Linear "Discriminant Analysis
   Use["LD"]              = 1; // Linear "Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // --- Function "Discriminant analysis
   Use["FDA_GA"]          = 1; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   //
   // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBNN"]          = 1; // Recommended ANN with BFGS training method and bayesian regulator
   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
   //
   // --- Support Vector Machine 
   Use["SVM"]             = 1;
   // 
   // --- Boosted Decision Trees
   Use["BDT"]             = 1; // uses Adaptive Boost
   Use["BDTG"]            = 0; // uses Gradient Boost
   Use["BDTB"]            = 0; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   // 
   // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 1;
   // ---------------------------------------------------------------
   Use["Plugin"]          = 0;
   Use["Category"]        = 0;
   Use["SVM_Gauss"]       = 0;
   Use["SVM_Poly"]        = 0;
   Use["SVM_Lin"]         = 0;


   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod 
                      << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
               std::cout << it->first << " ";
            }
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Create the Reader object

   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used

float BDT,logreg,alchemy_category, alchemy_category_score,avglinksize,commonLinkRatio_1,commonLinkRatio_2,commonLinkRatio_3,commonLinkRatio_4,compression_ratio,embed_ratio,frameTagRatio,hasDomainLink,html_ratio,image_ratio,is_news,lengthyLinkDomain,linkwordscore,news_front_page,non_markup_alphanum_characters,numberOfLinks,numwords_in_url,parametrizedLinkRatio,spelling_errors_ratio;

 reader->AddVariable("logreg",&logreg);
 reader->AddVariable("alchemy_category",&alchemy_category);
 reader->AddVariable("alchemy_category_score",&alchemy_category_score);
 reader->AddVariable("avglinksize",&avglinksize);
 reader->AddVariable("commonLinkRatio_1",&commonLinkRatio_1);
 reader->AddVariable("commonLinkRatio_2",&commonLinkRatio_2);
 reader->AddVariable("commonLinkRatio_3",&commonLinkRatio_3);
 reader->AddVariable("commonLinkRatio_4",&commonLinkRatio_4);
 reader->AddVariable("compression_ratio",&compression_ratio);
 reader->AddVariable("embed_ratio",&embed_ratio);
// reader->AddVariable("frameBased",&frameBased);
 reader->AddVariable("frameTagRatio",&frameTagRatio);
 reader->AddVariable("hasDomainLink",&hasDomainLink);
 reader->AddVariable("html_ratio",&html_ratio);
 reader->AddVariable("image_ratio",&image_ratio);
 reader->AddVariable("is_news",&is_news);
 reader->AddVariable("lengthyLinkDomain",&lengthyLinkDomain);
 reader->AddVariable("linkwordscore",&linkwordscore);
 reader->AddVariable("news_front_page",&news_front_page);
 reader->AddVariable("non_markup_alphanum_characters",&non_markup_alphanum_characters);
 reader->AddVariable("numberOfLinks",&numberOfLinks);
 reader->AddVariable("numwords_in_url",&numwords_in_url);
 reader->AddVariable("parametrizedLinkRatio",&parametrizedLinkRatio);
 reader->AddVariable("spelling_errors_ratio",&spelling_errors_ratio);




   Float_t Category_cat1, Category_cat2, Category_cat3;
   if (Use["Category"]){
      // Add artificial spectators for distinguishing categories
      reader->AddSpectator( "Category_cat1 := var3<=0",             &Category_cat1 );
      reader->AddSpectator( "Category_cat2 := (var3>0)&&(var4<0)",  &Category_cat2 );
      reader->AddSpectator( "Category_cat3 := (var3>0)&&(var4>=0)", &Category_cat3 );
   }

   // --- Book the MVA methods

   TString dir;
/*
   if(BINNO == 2)  dir  = "weightC/";
   TString prefix = "TMVAClassification";

   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = TString(it->first) + TString(" method");
         TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
         reader->BookMVA( methodName, weightfile ); 
      }
   }
  */

   TString weightfile;

//if(BINNO == 0) weightfile = "CMSSW_5_3_3_patch2/src/UserCode/oldVHbb/data/MVA_RMed_ZH125_oct.weights.xml";
//if(BINNO == 1) weightfile = "CMSSW_5_3_3_patch2/src/UserCode/oldVHbb/data/MVA_RTight_ZH125_oct.weights.xml";


    weightfile = "weights/TMVAClassification_BDT.weights.xml";


   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = TString(it->first) + TString(" method");
//         TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
         reader->BookMVA( methodName, weightfile ); 
      }
   }
  

   // Book output histograms
   UInt_t nbin = 100;
   TH1F   *histLk(0), *histLkD(0), *histLkPCA(0), *histLkKDE(0), *histLkMIX(0), *histPD(0), *histPDD(0);
   TH1F   *histPDPCA(0), *histPDEFoam(0), *histPDEFoamErr(0), *histPDEFoamSig(0), *histKNN(0), *histHm(0);
   TH1F   *histFi(0), *histFiG(0), *histFiB(0), *histLD(0), *histNn(0),*histNnbfgs(0),*histNnbnn(0);
   TH1F   *histNnC(0), *histNnT(0), *histBdt(0), *histBdtG(0), *histBdtD(0), *histRf(0), *histSVMG(0);
   TH1F   *histSVMP(0), *histSVML(0), *histFDAMT(0), *histFDAGA(0), *histCat(0), *histPBdt(0);

   if (Use["Likelihood"])    histLk      = new TH1F( "MVA_Likelihood",    "MVA_Likelihood",    nbin, -1, 1 );
   if (Use["LikelihoodD"])   histLkD     = new TH1F( "MVA_LikelihoodD",   "MVA_LikelihoodD",   nbin, -1, 0.9999 );
   if (Use["LikelihoodPCA"]) histLkPCA   = new TH1F( "MVA_LikelihoodPCA", "MVA_LikelihoodPCA", nbin, -1, 1 );
   if (Use["LikelihoodKDE"]) histLkKDE   = new TH1F( "MVA_LikelihoodKDE", "MVA_LikelihoodKDE", nbin,  -0.00001, 0.99999 );
   if (Use["LikelihoodMIX"]) histLkMIX   = new TH1F( "MVA_LikelihoodMIX", "MVA_LikelihoodMIX", nbin,  0, 1 );
   if (Use["PDERS"])         histPD      = new TH1F( "MVA_PDERS",         "MVA_PDERS",         nbin,  0, 1 );
   if (Use["PDERSD"])        histPDD     = new TH1F( "MVA_PDERSD",        "MVA_PDERSD",        nbin,  0, 1 );
   if (Use["PDERSPCA"])      histPDPCA   = new TH1F( "MVA_PDERSPCA",      "MVA_PDERSPCA",      nbin,  0, 1 );
   if (Use["KNN"])           histKNN     = new TH1F( "MVA_KNN",           "MVA_KNN",           nbin,  0, 1 );
   if (Use["HMatrix"])       histHm      = new TH1F( "MVA_HMatrix",       "MVA_HMatrix",       nbin, -0.95, 1.55 );
   if (Use["Fisher"])        histFi      = new TH1F( "MVA_Fisher",        "MVA_Fisher",        nbin, -4, 4 );
   if (Use["FisherG"])       histFiG     = new TH1F( "MVA_FisherG",       "MVA_FisherG",       nbin, -1, 1 );
   if (Use["BoostedFisher"]) histFiB     = new TH1F( "MVA_BoostedFisher", "MVA_BoostedFisher", nbin, -2, 2 );
   if (Use["LD"])            histLD      = new TH1F( "MVA_LD",            "MVA_LD",            nbin, -2, 2 );
   if (Use["MLP"])           histNn      = new TH1F( "MVA_MLP",           "MVA_MLP",           nbin, -1.25, 1.5 );
   if (Use["MLPBFGS"])       histNnbfgs  = new TH1F( "MVA_MLPBFGS",       "MVA_MLPBFGS",       nbin, -1.25, 1.5 );
   if (Use["MLPBNN"])        histNnbnn   = new TH1F( "MVA_MLPBNN",        "MVA_MLPBNN",        nbin, -1.25, 1.5 );
   if (Use["CFMlpANN"])      histNnC     = new TH1F( "MVA_CFMlpANN",      "MVA_CFMlpANN",      nbin,  0, 1 );
   if (Use["TMlpANN"])       histNnT     = new TH1F( "MVA_TMlpANN",       "MVA_TMlpANN",       nbin, -1.3, 1.3 );
   if (Use["BDT"])           histBdt     = new TH1F( "MVA_BDT",           "MVA_BDT",           nbin, -0.8, 0.8 );
   if (Use["BDTD"])          histBdtD    = new TH1F( "MVA_BDTD",          "MVA_BDTD",          nbin, -0.8, 0.8 );
   if (Use["BDTG"])          histBdtG    = new TH1F( "MVA_BDTG",          "MVA_BDTG",          nbin, -1.0, 1.0 );
   if (Use["RuleFit"])       histRf      = new TH1F( "MVA_RuleFit",       "MVA_RuleFit",       nbin, -2.0, 2.0 );
   if (Use["SVM_Gauss"])     histSVMG    = new TH1F( "MVA_SVM_Gauss",     "MVA_SVM_Gauss",     nbin,  0.0, 1.0 );
   if (Use["SVM_Poly"])      histSVMP    = new TH1F( "MVA_SVM_Poly",      "MVA_SVM_Poly",      nbin,  0.0, 1.0 );
   if (Use["SVM_Lin"])       histSVML    = new TH1F( "MVA_SVM_Lin",       "MVA_SVM_Lin",       nbin,  0.0, 1.0 );
   if (Use["FDA_MT"])        histFDAMT   = new TH1F( "MVA_FDA_MT",        "MVA_FDA_MT",        nbin, -2.0, 3.0 );
   if (Use["FDA_GA"])        histFDAGA   = new TH1F( "MVA_FDA_GA",        "MVA_FDA_GA",        nbin, -2.0, 3.0 );
   if (Use["Category"])      histCat     = new TH1F( "MVA_Category",      "MVA_Category",      nbin, -2., 2. );
   if (Use["Plugin"])        histPBdt    = new TH1F( "MVA_PBDT",          "MVA_BDT",           nbin, -0.8, 0.8 );

   // PDEFoam also returns per-event error, fill in histogram, and also fill significance
   if (Use["PDEFoam"]) {
      histPDEFoam    = new TH1F( "MVA_PDEFoam",       "MVA_PDEFoam",              nbin,  0, 1 );
      histPDEFoamErr = new TH1F( "MVA_PDEFoamErr",    "MVA_PDEFoam error",        nbin,  0, 1 );
      histPDEFoamSig = new TH1F( "MVA_PDEFoamSig",    "MVA_PDEFoam significance", nbin,  0, 10 );
   }

   // Book example histogram for probability (the other methods are done similarly)
   TH1F *probHistFi(0), *rarityHistFi(0);
   if (Use["Fisher"]) {
      probHistFi   = new TH1F( "MVA_Fisher_Proba",  "MVA_Fisher_Proba",  nbin, 0, 1 );
      rarityHistFi = new TH1F( "MVA_Fisher_Rarity", "MVA_Fisher_Rarity", nbin, 0, 1 );
   }

   // Prepare input tree ithis must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //   
   TFile *input(0);
   TString fname;

fname = "test.root";



std::cout << fname.Data() <<std::endl;
   if (!gSystem->AccessPathName( fname )) 
      input = TFile::Open( fname ); // check if file in local directory exists
   else    
      input = TFile::Open( "http://root.cern.ch/files/tmva_class_example.root"); // if not: download from ROOT server
   
   if (!input) {
      
      exit(1);
   }
   
   // --- Event loop

   // Prepare the event tree
   // - here the variable names have to corresponds to your tree
   // - you can use the same variables as above which is slightly faster,
   //   but of course you can use different ones and copy the values inside the event loop


   TTree* theTree = (TTree*)input->Get("tree");
  
   theTree->SetBranchAddress("logreg",&logreg);
   theTree->SetBranchAddress("alchemy_category",&alchemy_category);
   theTree->SetBranchAddress("alchemy_category_score",&alchemy_category_score);
   theTree->SetBranchAddress("avglinksize",&avglinksize);
   theTree->SetBranchAddress("commonLinkRatio_1",&commonLinkRatio_1);
   theTree->SetBranchAddress("commonLinkRatio_2",&commonLinkRatio_2);
   theTree->SetBranchAddress("commonLinkRatio_3",&commonLinkRatio_3);
   theTree->SetBranchAddress("commonLinkRatio_4",&commonLinkRatio_4);
   theTree->SetBranchAddress("compression_ratio",&compression_ratio);
   theTree->SetBranchAddress("embed_ratio",&embed_ratio);
  // theTree->SetBranchAddress("frameBased",&frameBased);
   theTree->SetBranchAddress("frameTagRatio",&frameTagRatio);
   theTree->SetBranchAddress("hasDomainLink",&hasDomainLink);
   theTree->SetBranchAddress("html_ratio",&html_ratio);
   theTree->SetBranchAddress("image_ratio",&image_ratio);
   theTree->SetBranchAddress("is_news",&is_news);
   theTree->SetBranchAddress("lengthyLinkDomain",&lengthyLinkDomain);
   theTree->SetBranchAddress("linkwordscore",&linkwordscore);
   theTree->SetBranchAddress("news_front_page",&news_front_page);
   theTree->SetBranchAddress("non_markup_alphanum_characters",&non_markup_alphanum_characters);
   theTree->SetBranchAddress("numberOfLinks",&numberOfLinks);
   theTree->SetBranchAddress("numwords_in_url",&numwords_in_url);
   theTree->SetBranchAddress("parametrizedLinkRatio",&parametrizedLinkRatio);
   theTree->SetBranchAddress("spelling_errors_ratio",&spelling_errors_ratio);



   // Efficiency calculator for cut method
   Int_t    nSelCutsGA = 0;
   Double_t effS       = 0.7;

   std::vector<Float_t> vecVar(4); // vector for EvaluateMVA tests

   TFile *target  = new TFile( filenameOut->Data(),"RECREATE" );
   TTree *tree = new TTree("tree", "myTree");

   tree->Branch("BDT"  	        ,  &BDT            ,  "BDT/F"       );            	
   tree->Branch("logreg",&logreg,"logreg/F");
   tree->Branch("alchemy_category",&alchemy_category,"alchemy_category/F");
   tree->Branch("alchemy_category_score",&alchemy_category_score,"alchemy_category_score/F");
   tree->Branch("avglinksize",&avglinksize,"avglinksize/F");
   tree->Branch("commonLinkRatio_1",&commonLinkRatio_1,"commonLinkRatio_1/F");
   tree->Branch("commonLinkRatio_2",&commonLinkRatio_2,"commonLinkRatio_2/F");
   tree->Branch("commonLinkRatio_3",&commonLinkRatio_3,"commonLinkRatio_3/F");
   tree->Branch("commonLinkRatio_4",&commonLinkRatio_4,"commonLinkRatio_4/F");
   tree->Branch("compression_ratio",&compression_ratio,"compression_ratio/F");
   tree->Branch("embed_ratio",&embed_ratio,"embed_ratio/F");
  // tree->Branch("frameBased",&frameBased,"frameBased/F");
   tree->Branch("frameTagRatio",&frameTagRatio,"frameTagRatio/F");
   tree->Branch("hasDomainLink",&hasDomainLink,"hasDomainLink/F");
   tree->Branch("html_ratio",&html_ratio,"html_ratio/F");
   tree->Branch("image_ratio",&image_ratio,"image_ratio/F");
   tree->Branch("is_news",&is_news,"is_news/F");
   tree->Branch("lengthyLinkDomain",&lengthyLinkDomain,"lengthyLinkDomain/F");
   tree->Branch("linkwordscore",&linkwordscore,"linkwordscore/F");
   tree->Branch("news_front_page",&news_front_page,"news_front_page/F");
   tree->Branch("non_markup_alphanum_characters",&non_markup_alphanum_characters,"non_markup_alphanum_characters/F");
   tree->Branch("numberOfLinks",&numberOfLinks,"numberOfLinks/F");
   tree->Branch("numwords_in_url",&numwords_in_url,"numwords_in_url/F");
   tree->Branch("parametrizedLinkRatio",&parametrizedLinkRatio,"parametrizedLinkRatio/F");
   tree->Branch("spelling_errors_ratio",&spelling_errors_ratio,"spelling_errors_ratio/F");
  

   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {

      theTree->GetEntry(ievt);

      // --- Return the MVA outputs and fill into histograms

      
     
  
      if (Use["CutsGA"]) {
         // Cuts is a special case: give the desired signal efficienciy
         Bool_t passed = reader->EvaluateMVA( "CutsGA method", effS );
         if (passed) nSelCutsGA++;
      }

      if (Use["Likelihood"   ])   histLk     ->Fill( reader->EvaluateMVA( "Likelihood method"    ) ,1);
      if (Use["LikelihoodD"  ])   histLkD    ->Fill( reader->EvaluateMVA( "LikelihoodD method"   ) ,1);
      if (Use["LikelihoodPCA"])   histLkPCA  ->Fill( reader->EvaluateMVA( "LikelihoodPCA method" ) ,1);
      if (Use["LikelihoodKDE"])   histLkKDE  ->Fill( reader->EvaluateMVA( "LikelihoodKDE method" ) ,1);
      if (Use["LikelihoodMIX"])   histLkMIX  ->Fill( reader->EvaluateMVA( "LikelihoodMIX method" ) ,1);
      if (Use["PDERS"        ])   histPD     ->Fill( reader->EvaluateMVA( "PDERS method"         ) ,1);
      if (Use["PDERSD"       ])   histPDD    ->Fill( reader->EvaluateMVA( "PDERSD method"        ) ,1);
      if (Use["PDERSPCA"     ])   histPDPCA  ->Fill( reader->EvaluateMVA( "PDERSPCA method"      ) ,1);
      if (Use["KNN"          ])   histKNN    ->Fill( reader->EvaluateMVA( "KNN method"           ) ,1);
      if (Use["HMatrix"      ])   histHm     ->Fill( reader->EvaluateMVA( "HMatrix method"       ) ,1);
      if (Use["Fisher"       ])   histFi     ->Fill( reader->EvaluateMVA( "Fisher method"        ) ,1);
      if (Use["FisherG"      ])   histFiG    ->Fill( reader->EvaluateMVA( "FisherG method"       ) ,1);
      if (Use["BoostedFisher"])   histFiB    ->Fill( reader->EvaluateMVA( "BoostedFisher method" ) ,1);
      if (Use["LD"           ])   histLD     ->Fill( reader->EvaluateMVA( "LD method"            ) ,1);
      if (Use["MLP"          ])   histNn     ->Fill( reader->EvaluateMVA( "MLP method"           ) ,1);
      if (Use["MLPBFGS"      ])   histNnbfgs ->Fill( reader->EvaluateMVA( "MLPBFGS method"       ) ,1);
      if (Use["MLPBNN"       ])   histNnbnn  ->Fill( reader->EvaluateMVA( "MLPBNN method"        ) ,1);
      if (Use["CFMlpANN"     ])   histNnC    ->Fill( reader->EvaluateMVA( "CFMlpANN method"      ) ,1);
      if (Use["TMlpANN"      ])   histNnT    ->Fill( reader->EvaluateMVA( "TMlpANN method"       ) ,1);
      if (Use["BDT"          ])   histBdt    ->Fill( reader->EvaluateMVA( "BDT method"           ) ,1);
      if (Use["BDTD"         ])   histBdtD   ->Fill( reader->EvaluateMVA( "BDTD method"          ) ,1);
      if (Use["BDTG"         ])   histBdtG   ->Fill( reader->EvaluateMVA( "BDTG method"          ) ,1);
      if (Use["RuleFit"      ])   histRf     ->Fill( reader->EvaluateMVA( "RuleFit method"       ) ,1);
      if (Use["SVM_Gauss"    ])   histSVMG   ->Fill( reader->EvaluateMVA( "SVM_Gauss method"     ) ,1);
      if (Use["SVM_Poly"     ])   histSVMP   ->Fill( reader->EvaluateMVA( "SVM_Poly method"      ) ,1);
      if (Use["SVM_Lin"      ])   histSVML   ->Fill( reader->EvaluateMVA( "SVM_Lin method"       ) ,1);
      if (Use["FDA_MT"       ])   histFDAMT  ->Fill( reader->EvaluateMVA( "FDA_MT method"        ) ,1);
      if (Use["FDA_GA"       ])   histFDAGA  ->Fill( reader->EvaluateMVA( "FDA_GA method"        ) ,1);
      if (Use["Category"     ])   histCat    ->Fill( reader->EvaluateMVA( "Category method"      ) ,1);
      if (Use["Plugin"       ])   histPBdt   ->Fill( reader->EvaluateMVA( "P_BDT method"         ) ,1);
     
      if (Use["BDT"          ])  {BDT = reader->EvaluateMVA("BDT method"); BDT = (BDT + 0.906) / (0.906+0.8); tree->Fill(); std::cout<< BDT << std::endl;}
      if (Use["BDTG"          ])  {BDT = reader->EvaluateMVA("BDTG method"); tree->Fill();}
      if (Use["BDTD"         ])   {BDT =  reader->EvaluateMVA( "BDTD method"); tree->Fill();}
      if (Use["Fisher"       ])   {BDT =  reader->EvaluateMVA( "Fisher method");tree->Fill();}



      // Retrieve also per-event error
      if (Use["PDEFoam"]) {
         Double_t val = reader->EvaluateMVA( "PDEFoam method" );
         Double_t err = reader->GetMVAError();
         histPDEFoam   ->Fill( val );
         histPDEFoamErr->Fill( err );         
         if (err>1.e-50) histPDEFoamSig->Fill( val/err );
      }         

      // Retrieve probability instead of MVA output
      if (Use["Fisher"])   {
         probHistFi  ->Fill( reader->GetProba ( "Fisher method" ) );
         rarityHistFi->Fill( reader->GetRarity( "Fisher method" ) );
      }
   }

   // Get elapsed time
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   // Get efficiency for cuts classifier
   if (Use["CutsGA"]) std::cout << "--- Efficiency for CutsGA method: " << double(nSelCutsGA)/theTree->GetEntries()
                                << " (for a required signal efficiency of " << effS << ")" << std::endl;

   if (Use["CutsGA"]) {

      // test: retrieve cuts for particular signal efficiency
      // CINT ignores dynamic_casts so we have to use a cuts-secific Reader function to acces the pointer  
      TMVA::MethodCuts* mcuts = reader->FindCutsMVA( "CutsGA method" ) ;

      if (mcuts) {      
         std::vector<Double_t> cutsMin;
         std::vector<Double_t> cutsMax;
         mcuts->GetCuts( 0.7, cutsMin, cutsMax );
         std::cout << "--- -------------------------------------------------------------" << std::endl;
         std::cout << "--- Retrieve cut values for signal efficiency of 0.7 from Reader" << std::endl;
         for (UInt_t ivar=0; ivar<cutsMin.size(); ivar++) {
            std::cout << "... Cut: " 
                      << cutsMin[ivar] 
                      << " < \"" 
                      << mcuts->GetInputVar(ivar)
                      << "\" <= " 
                      << cutsMax[ivar] << std::endl;
         }
         std::cout << "--- -------------------------------------------------------------" << std::endl;
      }
   }

   // --- Write histograms


   if (Use["Likelihood"   ])   histLk     ->Write();
   if (Use["LikelihoodD"  ])   histLkD    ->Write();
   if (Use["LikelihoodPCA"])   histLkPCA  ->Write();
   if (Use["LikelihoodKDE"])   histLkKDE  ->Write();
   if (Use["LikelihoodMIX"])   histLkMIX  ->Write();
   if (Use["PDERS"        ])   histPD     ->Write();
   if (Use["PDERSD"       ])   histPDD    ->Write();
   if (Use["PDERSPCA"     ])   histPDPCA  ->Write();
   if (Use["KNN"          ])   histKNN    ->Write();
   if (Use["HMatrix"      ])   histHm     ->Write();
   if (Use["Fisher"       ]) {  histFi     ->Write();tree->Write();}
   if (Use["FisherG"      ])   histFiG    ->Write();
   if (Use["BoostedFisher"])   histFiB    ->Write();
   if (Use["LD"           ])   histLD     ->Write();
   if (Use["MLP"          ])   histNn     ->Write();
   if (Use["MLPBFGS"      ])   histNnbfgs ->Write();
   if (Use["MLPBNN"       ])   histNnbnn  ->Write();
   if (Use["CFMlpANN"     ])   histNnC    ->Write();
   if (Use["TMlpANN"      ])   histNnT    ->Write();
   if (Use["BDT"          ])   {histBdt    ->Write(); tree->Write();}
   if (Use["BDTD"         ])   {histBdtD   ->Write(); tree->Write();}
   if (Use["BDTG"         ])   {histBdtG   ->Write();tree->Write();} 
   if (Use["RuleFit"      ])   histRf     ->Write();
   if (Use["SVM_Gauss"    ])   histSVMG   ->Write();
   if (Use["SVM_Poly"     ])   histSVMP   ->Write();
   if (Use["SVM_Lin"      ])   histSVML   ->Write();
   if (Use["FDA_MT"       ])   histFDAMT  ->Write();
   if (Use["FDA_GA"       ])   histFDAGA  ->Write();
   if (Use["Category"     ])   histCat    ->Write();
   if (Use["Plugin"       ])   histPBdt   ->Write();

   // Write also error and significance histos
   if (Use["PDEFoam"]) { histPDEFoam->Write(); histPDEFoamErr->Write(); histPDEFoamSig->Write(); }

   // Write also probability hists
   if (Use["Fisher"]) { if (probHistFi != 0) probHistFi->Write(); if (rarityHistFi != 0) rarityHistFi->Write(); }
   target->Close();

   std::cout << "--- Created root file: " << filenameOut->Data() << " containing the MVA output histograms" << std::endl;
  
   delete reader;
   std::cout << "==> TMVAClassificationApplication is done!" << endl << std::endl;



}
