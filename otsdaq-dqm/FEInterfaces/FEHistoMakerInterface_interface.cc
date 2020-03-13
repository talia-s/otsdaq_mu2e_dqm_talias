#include "otsdaq/MessageFacility/MessageFacility.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/Macros/InterfacePluginMacros.h"
#include "otsdaq-dqm/FEInterfaces/FEHistoMakerInterface.h"

#include <chrono>
#include <thread>

#include <iostream>


using namespace ots;

//========================================================================================================================
FEHistoMakerInterface::FEHistoMakerInterface(const std::string& interfaceUID, const ConfigurationTree& theXDAQContextConfigTree, const std::string& configurationPath)
: FEVInterface    (interfaceUID, theXDAQContextConfigTree, configurationPath)
, TCPPublishServer(theXDAQContextConfigTree.getNode(configurationPath).getNode("ServerPort").getValue<unsigned int>(), 1)
, distribution_   (50,10)
{
	TCPPublishServer::startAccept();
}

//========================================================================================================================
FEHistoMakerInterface::~FEHistoMakerInterface (void)
{
}

//========================================================================================================================
void FEHistoMakerInterface::configure(void)
{
	std::cout << __PRETTY_FUNCTION__ << "ConfigureDone!" << std::endl;
}

//========================================================================================================================
void FEHistoMakerInterface::halt (void)
{
}

//========================================================================================================================
void FEHistoMakerInterface::pause (void)
{
} 

//========================================================================================================================
void FEHistoMakerInterface::resume (void)
{
}

//========================================================================================================================
void FEHistoMakerInterface::start (std::string runNumber)
{
}

//========================================================================================================================
//The running state is a thread
bool FEHistoMakerInterface::running(void)
{
    double value = distribution_(generator_);
	std::string buffer(8,'0');
	memcpy(&buffer.at(0), &value, sizeof(double));
	//std::cout << __PRETTY_FUNCTION__ << " Value: " << value << " Buffer: " << buffer << std::endl;

	TCPPublishServer::broadcast(buffer);
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	return WorkLoop::continueWorkLoop_;//otherwise it stops!!!!!
}

//========================================================================================================================
void FEHistoMakerInterface::stop (void)
{
}

DEFINE_OTS_INTERFACE(FEHistoMakerInterface)
