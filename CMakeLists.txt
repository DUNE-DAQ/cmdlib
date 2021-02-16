cmake_minimum_required(VERSION 3.12)
project(cmdlib VERSION 1.0.2)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

find_package(cetlib REQUIRED)
find_package(TBB REQUIRED)      # TBB should be capitalized here
find_package(ers REQUIRED)
find_package(nlohmann_json REQUIRED)

set(CMDLIB_DEPENDENCIES ${CETLIB} ${CETLIB_EXCEPT} ${TBB} ${TBB_IMPORTED_TARGETS} ers::ers nlohmann_json::nlohmann_json)

daq_codegen_schema(*.jsonnet TEMPLATES Structs.hpp Nljs.hpp )

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
# No unit tests written

##############################################################################

daq_install()
