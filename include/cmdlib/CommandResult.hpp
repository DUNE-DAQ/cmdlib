/**
 * @file CommandResult.hpp
 *
 * CommandResult is a simple struct to hold the data about the 
 * request's success and to whom to respond with the result.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef CMDLIB_INCLUDE_CMDLIB_COMMANDRESULT_HPP_ 
#define CMDLIB_INCLUDE_CMDLIB_COMMANDRESULT_HPP_ 

#include <string>

namespace dune {
namespace daq {
namespace ccm { 

struct CommandResult
{
  CommandResult(const std::string& cliaddr, int cliport, const std::string& result)
    : m_answer_addr{cliaddr}, m_answer_port(cliport), m_result{result}
  {}
  std::string m_answer_addr = "unset"; // caller address
  int m_answer_port = -999;    // caller's requested port to answer
  std::string m_result = "unset";      // result content
};

} // namespace ccm
} // namespace daq
} // namespace dune

#endif // CMDLIB_INCLUDE_CMDLIB_COMMANDRESULT_HPP_ 
