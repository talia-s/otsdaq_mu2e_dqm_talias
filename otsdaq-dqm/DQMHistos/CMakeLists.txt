cet_make(LIBRARY_NAME TestDQMHistos
		 LIBRARIES
		 TableCore
		 ConfigurationInterface
		 NetworkUtilities
		 DataDecoders
		 Macros
		 ${CETLIB}
		 ${ROOT_BASIC_LIB_LIST}
		 MF_MessageLogger
		 # ${MF_MESSAGELOGGER}
)

install_headers()
install_source()
