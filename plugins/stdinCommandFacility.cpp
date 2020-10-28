/**
 * @file stdinCommandFacility.cpp CommandFacility implementation
 * that reads commands from std::cin
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include "cmdlib/CommandFacility.hpp"
#include "cmdlib/Issues.hpp"

#include <ers/ers.h>
#include <nlohmann/json.hpp>
#include <cetlib/BasicPluginFactory.h>

#include <thread>
#include <chrono>
#include <memory>
#include <string>

using namespace dunedaq::cmdlib;
using namespace std::chrono_literals;

class stdinCommandFacility : public CommandFacility
{
public:
  explicit stdinCommandFacility(std::string uri) : CommandFacility(uri) { 
    // Allocate resources as needed
  }

  // Implementation of the runner
  void run(std::atomic<bool>& end_marker) {
    ERS_INFO("Entered commands will be launched on CommandedObject...");

    std::string cmd;
    while (end_marker) { //until runmarker
      // feed commands from cin
      std::cin >> cmd;

      try {
        // Try to parse
        auto jstr = nlohmann::json::parse(cmd);
        // exercice base launch (deferred)
        inherited::executeCommand(jstr);
      }
      catch (const nlohmann::json::parse_error& err) {
        ers::error(dunedaq::cmdlib::CommandParserError(ERS_HERE, err.what()));
      }

    }
    ERS_INFO("Command handling stopped.");
  }

protected:
  typedef CommandFacility inherited;

  // Implementation of completionHandler interface
  void completionCallback(const std::string& result) {
    ERS_INFO("Command execution resulted with: " << result);
  }

};

extern "C" {
    std::shared_ptr<dunedaq::cmdlib::CommandFacility> make(std::string uri) {
        return std::shared_ptr<dunedaq::cmdlib::CommandFacility>(new stdinCommandFacility(uri));
    }
}
