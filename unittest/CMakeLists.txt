add_executable(cmdlib_boost_unittest test_commandfacility_boost.cxx)

target_link_libraries(cmdlib_boost_unittest
  PRIVATE
    cmdlib
    Boost::unit_test_framework
)

target_compile_definitions(cmdlib_boost_unittest PRIVATE BOOST_TEST_DYN_LINK)

add_test(NAME cmdlib_boost_unittest COMMAND cmdlib_boost_unittest)
