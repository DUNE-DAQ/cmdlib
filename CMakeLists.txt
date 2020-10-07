cmake_minimum_required(VERSION 3.12)
project(cmdlib VERSION 0.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../daq-buildtools/cmake ${CMAKE_MODULE_PATH})
include(DAQ)

daq_setup_environment()

find_package(cetlib REQUIRED)
find_package(ers REQUIRED)

set(CMDLIB_DEPENDENCIES ${CETLIB} ${CETLIB_EXCEPT} ers::ers)

##############################################################################
daq_point_build_to( src )

add_library(cmdlib src/CommandFacility.cpp)
target_link_libraries(cmdlib ${CMDLIB_DEPENDENCIES})

add_library(cmdlib_stdinCommandFacility_duneCommandFacility src/stdinCommandFacility.cpp)
target_link_libraries(cmdlib_stdinCommandFacility_duneCommandFacility cmdlib)

##############################################################################
daq_point_build_to( test )

add_library(cmdlib_dummyCommandFacility_duneCommandFacility test/dummy_command_facility.cpp)
target_link_libraries(cmdlib_dummyCommandFacility_duneCommandFacility cmdlib)

add_executable(cmdlib_test_dummy_app test/test_dummy_app.cxx)
target_link_libraries(cmdlib_test_dummy_app cmdlib)

add_executable(cmdlib_test_stdin_app test/test_stdin_app.cxx)
target_link_libraries(cmdlib_test_stdin_app cmdlib)

##############################################################################
daq_point_build_to( unittest )

daq_install(TARGETS cmdlib_test_dummy_app cmdlib_test_stdin_app)