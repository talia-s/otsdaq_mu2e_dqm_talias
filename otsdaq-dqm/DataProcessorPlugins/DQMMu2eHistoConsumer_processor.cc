#include "otsdaq-dqm/DataProcessorPlugins/DQMMu2eHistoConsumer.h"
#include "otsdaq/MessageFacility/MessageFacility.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/Macros/ProcessorPluginMacros.h"
#include <chrono>
#include <thread>
#include <TDirectory.h>
#include <TFile.h>
#include <TH1F.h>
#include <TTree.h>

using namespace ots;

//========================================================================================================================
	DQMMu2eHistoConsumer::DQMMu2eHistoConsumer(std::string supervisorApplicationUID, std::string bufferUID, std::string processorUID, const 		ConfigurationTree& theXDAQContextConfigTree, const std::string& configurationPath)
		: WorkLoop             (processorUID)
		, DQMHistosConsumerBase(supervisorApplicationUID, bufferUID, processorUID, LowConsumerPriority)
		, Configurable         (theXDAQContextConfigTree, configurationPath)
		, saveFile_            (theXDAQContextConfigTree.getNode(configurationPath).getNode("SaveFile").getValue<bool>())
		, filePath_            (theXDAQContextConfigTree.getNode(configurationPath).getNode("FilePath").getValue<std::string>())
		, radixFileName_       (theXDAQContextConfigTree.getNode(configurationPath).getNode("RadixFileName").getValue<std::string>())

	{
		std::cout<<"[In DQMMu2eHistoConsumer () ] Initiating ..."<<std::endl;
	}

//========================================================================================================================
	DQMMu2eHistoConsumer::~DQMMu2eHistoConsumer(void)
	{
		DQMHistosBase::closeFile();
	}
//========================================================================================================================
	void DQMMu2eHistoConsumer::startProcessingData(std::string runNumber)
		{
		  std::cout << __PRETTY_FUNCTION__ << filePath_ + "/" + radixFileName_ + "_Run" + runNumber + ".root" << std::endl;
			DQMHistosBase::openFile(filePath_ + "/" + radixFileName_ + "_Run" + runNumber + ".root");//filePath_ + radixFileName_
			DQMHistosBase::myDirectory_ = DQMHistosBase::theFile_->mkdir("Mu2eHistos", "Mu2eHistos");
			DQMHistosBase::myDirectory_->cd();
			DQMHistosBase::myDirectory_->mkdir("ExtraDir", "ExtraDir");
			hGauss_ = new TH1F("Gaussian", "Gaussian", 100, 0, 100);
			testTree_ = new TTree("tree", "tree");
			int value;
			testTree_->Branch("Value", &value);
			value = 7;
			testTree_->Fill();
			value = 8;
			testTree_->Fill();

			std::cout << __PRETTY_FUNCTION__ << "Starting!" << std::endl;
			DataConsumer::startProcessingData(runNumber);
			std::cout << __PRETTY_FUNCTION__ << "Started!" << std::endl;
		}

//========================================================================================================================
	void DQMMu2eHistoConsumer::stopProcessingData(void)
	{
		std::cout<<"[In DQMMu2eHistoConsumer () ] Stopping ..."<<std::endl;
	
		DataConsumer::stopProcessingData();
		if(saveFile_)
		{
			std::cout<<"[In DQMMu2eHistoConsumer () ] Saving ..."<<std::endl;
			DQMHistosBase::save();
		}
		closeFile();
	}

//========================================================================================================================
	void DQMMu2eHistoConsumer::pauseProcessingData(void)
	{
		std::cout<<"[In DQMMu2eHistoConsumer () ] Pausing ..."<<std::endl;
		DataConsumer::stopProcessingData();
	}

//========================================================================================================================
	void DQMMu2eHistoConsumer::resumeProcessingData(void)
	{
		std::cout<<"[In DQMMu2eHistoConsumer () ] Resuming ..."<<std::endl;
		DataConsumer::startProcessingData("");
	}

//========================================================================================================================
	bool DQMMu2eHistoConsumer::workLoopThread(toolbox::task::WorkLoop* workLoop)
	{
		// std::cout<<"[In DQMMu2eHistoConsumer () ] CallingFastRead ..."<<std::endl;
		fastRead();
		return WorkLoop::continueWorkLoop_;
	}

//========================================================================================================================
	void DQMMu2eHistoConsumer::fastRead(void)
	{
		// std::cout<<"[In DQMMu2eHistoConsumer () ] FastRead ..."<<std::endl;
		if(DataConsumer::read(dataP_, headerP_) < 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));//10
			return;
		}
		double value = 0;
		memcpy(&value, &dataP_->at(0), sizeof(double));
	//	std::cout << __PRETTY_FUNCTION__ << " Value: " << value << std::endl;
		// std::cout<<"[In DQMMu2eHistoConsumer () ] Filling ..."<<std::endl;
		hGauss_->Fill(value);
		DataConsumer::setReadSubBuffer<std::string, std::map<std::string, std::string>>();
	}

DEFINE_OTS_PROCESSOR(DQMMu2eHistoConsumer)
