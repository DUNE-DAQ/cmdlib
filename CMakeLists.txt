cmake_minimum_required(VERSION 3.12)
project(cmdlib VERSION 0.0.0)

find_package(daq-buildtools 1.2.1 REQUIRED)

daq_setup_environment()

find_package(cetlib REQUIRED)
find_package(tbb REQUIRED)
find_package(ers REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(TBB REQUIRED)

<<<<<<< HEAD
set(CMDLIB_DEPENDENCIES ${CETLIB} ${CETLIB_EXCEPT} ers::ers nlohmann_json::nlohmann_json ${TBB_IMPORTED_TARGETS})
=======
set(CMDLIB_DEPENDENCIES ${CETLIB} ${CETLIB_EXCEPT} ${TBB} ers::ers nlohmann_json::nlohmann_json)
>>>>>>> dune-daq/master

##############################################################################
# Main library

daq_add_library(CommandFacility.cpp LINK_LIBRARIES ${CMDLIB_DEPENDENCIES})

##############################################################################
# Plugins

daq_add_plugin(stdinCommandFacility duneCommandFacility LINK_LIBRARIES cmdlib)


##############################################################################
# Integration tests

daq_add_plugin(dummyCommandFacility duneCommandFacility TEST LINK_LIBRARIES cmdlib)

daq_add_application(cmdlib_test_dummy_app test_dummy_app.cxx TEST LINK_LIBRARIES cmdlib)
daq_add_application(cmdlib_test_stdin_app test_stdin_app.cxx TEST LINK_LIBRARIES cmdlib)

##############################################################################
# Unit tests:


##############################################################################

daq_install()
