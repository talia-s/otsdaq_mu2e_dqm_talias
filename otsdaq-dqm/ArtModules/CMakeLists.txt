art_make( BASENAME_ONLY
 LIBRARY_NAME otsdaq-dqm_ArtModules
 SUBDIRS detail
 NO_PLUGINS
 DICT_DICTIONARIES
 ${ART_PERSISTENCY_PROVENANCE}
 ${CANVAS_PERSISTENCY_PROVENANCE}
 LIB_LIBRARIES
canvas
 otsdaq-dqm_Overlays
 artdaq-core_Utilities
 artdaq_ArtModules
 artdaq-core_Data
 artdaq-core_Core
  art_Framework_Core
  art_Framework_IO_Sources
  art_Framework_Principal
  art_Persistency_Provenance
  MF_MessageLogger
  ${ROOT_CINTEX}
  ${ROOT_RIO}
  ${ROOT_CORE}
  ${ROOT_NET}
  ${ROOT_REFLEX}
  cetlib
  cetlib_except
  DTCInterface
)

# simple_plugin(WireChamberDQM "module"
# 	otsdaq-dqm_Overlays
#         ECLWriterCore
#   NetworkUtilities
# 	artdaq-core_Data
# 	${CETLIB_EXCEPT}
# 	${ROOT_GPAD}
# 	${ROOT_HIST}
# 	${ROOT_GUI}
# 	${ROOT_RIO}
#   ${ROOT_NET}
# #	MessageFacility
# 	${MF_MESSAGELOGGER}
# 	${MF_UTILITIES}
# 	${XMLRPC_SERVER_ABYSSPP}
# 	${XMLRPC_SERVER_ABYSS}
# 	${XMLRPC_SERVERPP}
# 	${XMLRPC_CLIENT}
# 	${XMLRPC_CLIENTPP}
# 	${XMLRPC_SERVER}
# 	${XMLRPC_ABYSS}
# 	${XMLRPCPP}
# 	${XMLRPC}
# 	${XMLRPC_UTIL}
# 	${XMLRPC_UTILPP}
# 	${XMLRPC_XMLPARSE}
# 	${XMLRPC_XMLTOK}
# 	curl
# 	pthread
# )
# simple_plugin(WireChamberDQMECL "module"
# 	otsdaq-trigger_Overlays
#         ECLWriterCore
#   NetworkUtilities
# 	artdaq-core_Data
# 	${CETLIB_EXCEPT}
# 	${ROOT_GPAD}
# 	${ROOT_HIST}
# 	${ROOT_GUI}
# 	${ROOT_RIO}
#   ${ROOT_NET}
# #	MessageFacility
# 	${MF_MESSAGELOGGER}
# 	${MF_UTILITIES}
# 	${XDAQ_XERCES-C}
# 	pthread
# )


#simple_plugin(TriggerCounter "module"
#        artdaq-core_Utilities
#	otsdaq-dqm_Overlays
#	NetworkUtilities
#	artdaq-core_Data
#	 artdaq-core_Core
#	${CETLIB_EXCEPT}
#	${ROOT_GPAD}
#	${ROOT_HIST}
#	${ROOT_GUI}
#	${ROOT_RIO}
#  ${ROOT_NET}
#	MessageFacility
#	${MF_MESSAGELOGGER}
#	${MF_UTILITIES}
#)



