
# cet_build_plugin(Occupancy art::module LIBRARIES REG
#   )

# cet_build_plugin(TrackerDQM art::module LIBRARIES REG
#   )

cet_build_plugin(CaloDQM art::module LIBRARIES REG
art_root_io::TFileService_service
artdaq_core_mu2e::artdaq-core-mu2e_Data
otsdaq_mu2e::otsdaq-mu2e_ArtModules
otsdaq::NetworkUtilities
Offline::RecoDataProducts
ROOT::Hist
ROOT::Tree
ROOT::Core
ROOT::RIO
ROOT::Gui
)

cet_build_plugin(IntensityInfoDQM art::module LIBRARIES REG
art_root_io::TFileService_service
artdaq_core_mu2e::artdaq-core-mu2e_Data
otsdaq_mu2e::otsdaq-mu2e_ArtModules
otsdaq::NetworkUtilities
Offline::RecoDataProducts
ROOT::Hist
ROOT::Tree
ROOT::Core
ROOT::RIO
ROOT::Gui
)

#cet_build_plugin(TrackerDQM art::module LIBRARIES REG
#art_root_io::TFileService_service
#artdaq_core_mu2e::artdaq-core-mu2e_Data
#otsdaq_mu2e::otsdaq-mu2e_ArtModules
#otsdaq::NetworkUtilities
#ROOT::Hist
#ROOT::Tree
#ROOT::Core
#ROOT::RIO
#ROOT::Gui
#)

# BTrk and KinKal have non-standard Find*.cmake...
include_directories($ENV{KINKAL_INC})
include_directories($ENV{BTRK_INC})
link_directories($ENV{KINKAL_LIB})
link_directories($ENV{BTRK_LIB})
cet_build_plugin(TriggerDQM art::module LIBRARIES REG
art_root_io::TFileService_service
artdaq_core_mu2e::artdaq-core-mu2e_Data
canvas::canvas
otsdaq_mu2e::otsdaq-mu2e_ArtModules
otsdaq::NetworkUtilities
Offline::Mu2eUtilities
ROOT::Hist
ROOT::Tree
ROOT::Core
ROOT::RIO
ROOT::Gui
)

cet_build_plugin(ReadTriggerCounts art::module LIBRARIES REG
art_root_io::TFileService_service
artdaq_core_mu2e::artdaq-core-mu2e_Data
	artdaq::ArtModules
Offline::BFieldGeom
      Offline::ConditionsService
      Offline::DataProducts
      Offline::GeometryService
      Offline::GlobalConstantsService
      Offline::Mu2eUtilities
      Offline::RecoDataProducts
      Offline::TrackerGeom
	)



install_headers()
install_source()
install_fhicl(SUBDIRS fcl)
