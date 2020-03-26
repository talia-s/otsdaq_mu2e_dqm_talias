//Author: S Middleton
//Date: 2020
//Purpose: prototype Analyzer fir DQM trigger rates. Based on Offline module Trigger/srs/ReadTriggerInfo. Much of the code was copied over from that module.

//Art:
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "canvas/Utilities/InputTag.h"

#include "artdaq-core/Data/ContainerFragment.hh"
#include "artdaq-core/Data/Fragment.hh"
#include "artdaq/DAQdata/Globals.hh"

#include "cetlib_except/exception.h"

//OTS:
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

//DQM:
#include "/home/mu2etrig/test_stand/ots_agg/srcs/otsdaq_mu2e_dqm/otsdaq-dqm/DataProcessorPlugins/DQMMu2eHistoConsumer.h"
#include "/home/mu2etrig/test_stand/ots_agg/srcs/otsdaq_mu2e_dqm/otsdaq-dqm/FEInterfaces/FEHistosMakerInterface.h"

//ROOT:
#include "art/Framework/Services/Optional/TFileService.h"
#include "art_root_io/TFileService.h" 
#include <TH1F.h>
#include <TH1.h>
#include <TProfile.h>

//Offline:
#include "ConditionsService/inc/AcceleratorParams.hh"
#include "ConditionsService/inc/ConditionsHandle.hh"


//C++:
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace ots
{
    class TriggerRates : public art::EDAnalyzer
    {
      public:
	    explicit TriggerRates(fhicl::ParameterSet const& p);
	    virtual ~TriggerRates() = default;

	    void analyze(art::Event const& e) override;
	    void beginRun(art::Run const&) override;
        void beginJob() override;
        void endJob() override;
      private:
	    art::RunNumber_t current_run_;

        enum {
            kNTrigInfo     = 40
        };

        struct  summaryInfoHist_  {
            TH1F *_hTrigInfo  [kNTrigInfo];
            TH2F *_h2DTrigInfo[kNTrigInfo];
            TH1F *_hTrigBDW   [kNTrigInfo];

            summaryInfoHist_() {
            for (int i=0; i<kNTrigInfo; ++i){ 
            _hTrigInfo  [i] = NULL;
            _h2DTrigInfo[i] = NULL;
            _hTrigBDW   [i] = NULL;
            }
          }
        };

        struct  trigInfo_ {
            int           counts;
            int           exclusive_counts;
            std::string   label;             
            trigInfo_ ():counts(0), exclusive_counts(0){}
        };

        std::vector<trigInfo_>    _trigAll;	     
        std::vector<trigInfo_>    _trigFinal;    
        std::vector<trigInfo_>    _trigCaloOnly; 
        std::vector<trigInfo_>    _trigCaloCalib;
        std::vector<trigInfo_>    _trigTrack;    
        std::vector<trigInfo_>    _trigHelix;    
        std::vector<trigInfo_>    _trigEvtPS;  
  
        summaryInfoHist_ _sumHist;

        art::RunNumber_t current_run_;
        std::string outputFileName_;

        bool writeOutput_;
        bool overwrite_mode_;
        size_t _nMaxTrig;    
        int _nTrackTrig;
        int _nCaloTrig;
        float _nProcess;
        void PlotRate(art::Event const& e);
        //void BookCanvas( std::string name, DQMHistos& container, std::string keySuffix, std::string titleSuffix);
        void BookHistos();
        void BookTriggerHistos(art::ServiceHandle<art::TFileService> & Tfs, summaryInfoHist_ &Hist);
        void FillHistos();
        //void Write(int event, int run, DQMHistos& container);
        void evalTriggerRate();
    };
}

ots::TriggerRates::TriggerRates(fhicl::ParameterSet const& ps)
    : art::EDAnalyzer(ps)
    current_run_(0),
    outputFileName_(ps.get<std::string>("fileName", "otsdaqExampleDQM.root")),
    writeOutput_(ps.get<bool>("write_to_file", true)),
    overwrite_mode_(ps.get<bool>("overwrite_output_file", true)),
    _nMaxTrig      (pset.get<size_t>("nFilters", 70)),
    _nTrackTrig    (pset.get<size_t>("nTrackTriggers", 4)),
    _nCaloTrig     (pset.get<size_t>("nCaloTriggers", 4)),
    _nProcess      (pset.get<float> ("nEventsProcessed", 1.))
{
	TLOG_INFO("TriggerRates") << "TriggerRate Plotter construction is beginning " << TLOG_ENDL;
    _trigAll.      resize(_nMaxTrig);	     
    _trigFinal.    resize(_nMaxTrig);    
    _trigCaloOnly. resize(_nMaxTrig); 
    _trigCaloCalib.resize(_nMaxTrig);
    _trigTrack.    resize(_nMaxTrig);    
    _trigHelix.    resize(_nMaxTrig);    
    _trigEvtPS.    resize(_nMaxTrig); 
	TLOG_DEBUG("TriggerRates") << "TriggerRat Plotter construction complete" << TLOG_ENDL;
}

void ots::TriggerRates::beginRun(art::Run const& e)
{
	if(e.run() == current_run_)
		return;
	current_run_ = e.run();
}

void ots::TriggerRates::beginJob(){

}

void ots::TriggerRates::analyze(art::Event const& e)
{
	TLOG_INFO("TriggerRate - Plotter")
	    << "TriggerRate Plotting Module is Analyzing Event #  " << e.event() << TLOG_ENDL;
    BookHistos();
    FillHitos();//TODO
}

void  ots::TriggerRates::BookHistos(){
     art::ServiceHandle<art::TFileService> tfs;
     BookTriggerHistos(Tfs, Examples);
}


void   ots::TriggerRates::findTrigIndex(std::vector<trigInfo_> &Vec, std::string& ModuleLabel, int &Index){
    
    Index = 0;
    for (size_t i=0; i<Vec.size(); ++i){
      if (Vec[i].label == ModuleLabel) { 
	Index = i;
	break;
      }else if (Vec[i].label != ""){
	Index = i+1;
      }
    }
  }

  void ots::TriggerRates::BookTriggerHistos(art::ServiceHandle<art::TFileService> & Tfs, summaryInfoHist_  &Hist){
    art::TFileDirectory trigInfoDir = Tfs->mkdir("trigInfo");

    Hist._hTrigInfo[0]   = trigInfoDir.make<TH1F>("hTrigInfo_global", "Global Trigger rejection", (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    Hist._hTrigInfo[1]   = trigInfoDir.make<TH1F>("hTrigInfo_track", "Calo-only Triggers rejection", (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    Hist._hTrigInfo[2]   = trigInfoDir.make<TH1F>("hTrigInfo_calo", "Track Triggers rejection", (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    Hist._hTrigInfo[3]   = trigInfoDir.make<TH1F>("hTrigInfo_evtPS", "Event prescaler Trigger bits distribution", (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    Hist._hTrigInfo[4]   = trigInfoDir.make<TH1F>("hTrigInfo_helix", "HelixSeed Triggers rejection", (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    Hist._hTrigInfo[5]   = trigInfoDir.make<TH1F>("hTrigInfo_caloCalib", "Calo Calibration rejection", (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    Hist._hTrigInfo[6]   = trigInfoDir.make<TH1F>("hTrigInfo_final"     , "Global Trigger rejection of the paths"      , (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       

    Hist._hTrigInfo[10]  = trigInfoDir.make<TH1F>("hTrigInfo_unique_all", "Events found only by each Trig path"        , (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    Hist._hTrigInfo[11]  = trigInfoDir.make<TH1F>("hTrigInfo_unique"    , "Events found only by each Trig path"        , (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       

    Hist._hTrigInfo[15]  = trigInfoDir.make<TH1F>("hTrigInfo_paths"     , "Rejection of all the Trigger paths"         , (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       


    Hist._h2DTrigInfo[0] = trigInfoDir.make<TH2F>("h2DTrigInfo_map_all" , "Trigger correlation map from all filters"   , (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5), (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    Hist._h2DTrigInfo[1] = trigInfoDir.make<TH2F>("h2DTrigInfo_map"     , "Trigger correlation map"                    , (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5), (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));   

    art::TFileDirectory trigBDWDir = Tfs->mkdir("trigBDW");

    Hist._hTrigBDW[0]   = trigBDWDir.make<TH1F>("hTrigBDW_global"    , "Trigger bandwidth; ; rate [Hz]"                   , (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    Hist._hTrigBDW[1]   = trigBDWDir.make<TH1F>("hTrigBDW_cumulative", "Cumulative Trigger bandwidth; ; rate [Hz]"        , (_nMaxTrig+2), -0.5, (_nMaxTrig+1.5));       
    
  }


void ots::TriggerRates::endJob(){
    
    int    indexTrigInfo11(0);
    for (size_t i=0; i<_trigAll.size(); ++i ){
      _sumHist._hTrigInfo  [0]->GetXaxis()->SetBinLabel(i+1, _trigAll[i].label.c_str());
      _sumHist._h2DTrigInfo[0]->GetXaxis()->SetBinLabel(i+1, _trigAll[i].label.c_str());

      if (_trigAll[i].counts > 0) {
	    _sumHist._hTrigInfo[0]->SetBinContent(i+1, _nProcess/_trigAll[i].counts);
	    for (size_t j=0; j<_trigAll.size(); ++j ){
	      _sumHist._h2DTrigInfo[0]->GetYaxis()->SetBinLabel(j+1, _trigAll[j].label.c_str());
	    }
      }

      _sumHist._hTrigInfo[1]->GetXaxis()->SetBinLabel(i+1, _trigTrack[i].label.c_str());
      if (_trigTrack[i].counts > 0) _sumHist._hTrigInfo[1]->SetBinContent(i+1, _nProcess/_trigTrack[i].counts);

      _sumHist._hTrigInfo[2]->GetXaxis()->SetBinLabel(i+1, _trigCaloOnly[i].label.c_str());
      if (_trigCaloOnly[i].counts > 0) _sumHist._hTrigInfo[2]->SetBinContent(i+1, _nProcess/_trigCaloOnly[i].counts);

      _sumHist._hTrigInfo[3]->GetXaxis()->SetBinLabel(i+1, _trigEvtPS[i].label.c_str());
      if (_trigEvtPS[i].counts > 0) _sumHist._hTrigInfo[3]->SetBinContent(i+1, _trigEvtPS[i].counts);

      _sumHist._hTrigInfo[4]->GetXaxis()->SetBinLabel(i+1, _trigHelix[i].label.c_str());
      if (_trigHelix[i].counts > 0) _sumHist._hTrigInfo[4]->SetBinContent(i+1, _trigHelix[i].counts);

      _sumHist._hTrigInfo[5]->GetXaxis()->SetBinLabel(i+1, _trigCaloCalib[i].label.c_str());
      if (_trigCaloCalib[i].counts > 0) _sumHist._hTrigInfo[5]->SetBinContent(i+1, _trigCaloCalib[i].counts);

      if (_trigFinal[i].counts > 0) {
	    _sumHist._hTrigInfo  [6]->GetXaxis()->SetBinLabel(i+1, _trigFinal[i].label.c_str());
	    _sumHist._hTrigInfo  [6]->SetBinContent(i+1, _nProcess/_trigFinal[i].counts);
      }

    //fill  the histograms that shows how many events were found exclusively by each trigger path
    _sumHist._hTrigInfo  [10]->GetXaxis()->SetBinLabel(i+1, _trigAll[i].label.c_str());
    double    content_trigInfo11 = _sumHist._hTrigInfo [10]->GetBinContent(i+1);
    if (content_trigInfo11>0){
        _sumHist._hTrigInfo  [11]->GetXaxis()->SetBinLabel(indexTrigInfo11 +1, _trigAll[i].label.c_str());
        _sumHist._hTrigInfo  [11]->SetBinContent(indexTrigInfo11 +1, content_trigInfo11);
        ++indexTrigInfo11;
    }

    }

    int                nbinsx = _sumHist._h2DTrigInfo[0]->GetNbinsX();
    int                nbinsy = _sumHist._h2DTrigInfo[0]->GetNbinsY();
    std::vector<int>   binsToSkip;

    for (int i=0; i<nbinsx; ++i){
        bool used(false);

        for (int j=0; j<nbinsy; ++j){
            if (_sumHist._h2DTrigInfo[0]->GetBinContent(i+1, j+1) > 0) {
                used = true;
                break;
            }
        }
        if (!used) binsToSkip.push_back(i);
    }

    int   index_x(0);
    for (int i=0; i<nbinsx; ++i){
        int    counts = std::count(binsToSkip.begin(), binsToSkip.end(), i);
        if (counts >= 1) continue;
        _sumHist._h2DTrigInfo[1]->GetXaxis()->SetBinLabel(index_x+1, _trigAll[i].label.c_str());

        int    index_y(0);

        for (int j=0; j<nbinsy; ++j){
            counts = std::count(binsToSkip.begin(), binsToSkip.end(), j);
            if (counts >= 1)continue;
            double  content =  _sumHist._h2DTrigInfo[0]->GetBinContent(i+1, j+1);
            _sumHist._h2DTrigInfo[1]->SetBinContent(index_x+1, index_y+1, content);

            //set the label
            if (index_x == 0){
            _sumHist._h2DTrigInfo[1]->GetYaxis()->SetBinLabel(index_y+1, _trigAll[j].label.c_str());
            }

	        ++index_y;
      }
    ++index_x;
    }
    
    evalTriggerRate();
  }

void   ots::TriggerRates::evalTriggerRate(){
    
    std::sort(_trigFinal.begin(), _trigFinal.end(), [](const auto a, const auto b) {return a.counts < b.counts; });
    
    ConditionsHandle<AcceleratorParams> accPar("ignored");
    double    mbtime         = accPar->deBuncherPeriod;
    double    mean_mb_rate   = 1./(mbtime/CLHEP::s)*_duty_cycle;

    bool      isFirst(true);
    int       index(0);
   
    std::vector<string>    labels_by_rate;

    for (size_t i=0; i< _trigFinal.size(); ++i){
        double  nEvents = (double)_trigFinal[i].counts;
        if ( nEvents <= 1e-3) continue;

        labels_by_rate.push_back(_trigFinal[i].label);

        double  eff   = nEvents/_nProcess;
        double  rate  = mean_mb_rate*eff;
        _sumHist._hTrigBDW[0]->GetXaxis()->SetBinLabel(index+1, _trigFinal[i].label.c_str());
        _sumHist._hTrigBDW[1]->GetXaxis()->SetBinLabel(index+1, _trigFinal[i].label.c_str());
        _sumHist._hTrigBDW[0]->SetBinContent(index+1, rate);

        if (isFirst) {
            _sumHist._hTrigBDW[1]->SetBinContent(index+1, rate);
          	isFirst = false;
        }else{
	        double    nCorrelated(0);
	        findCorrelatedEvents(labels_by_rate, nCorrelated);

	        rate = _sumHist._hTrigBDW[1]->GetBinContent(index) + (nEvents-nCorrelated)/(double)_nProcess*mean_mb_rate;
              	_sumHist._hTrigBDW[1]->SetBinContent(index+1, rate);
      }

      ++index;
    }

  }

DEFINE_ART_MODULE(ots::TriggerRates)
