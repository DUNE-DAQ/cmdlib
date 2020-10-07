/**
 * @file dummy_command_facility_.cxx Dummy command facility
 * implementation of the CommandFacility interface
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include "cmdlib/CommandFacility.hpp"
#include "ers/ers.h"

#include <cetlib/BasicPluginFactory.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>

using namespace dunedaq::cmdlib;
using namespace std::chrono_literals;

class dummyCommandFacility : public CommandFacility
{
public:
  explicit dummyCommandFacility(std::string uri) : CommandFacility(uri) { 
  }

  virtual ~dummyCommandFacility() { 
  }

  void run(std::atomic<bool>& end_marker) {
    ERS_INFO("Going for a run...");
    std::string slowcmd("asd");

    bool once = true;
    while (end_marker) {
      if (once) {
        // execute 10 quick commands
        for (auto i=0; i<1000; ++i) {
          inherited::executeCommand(std::to_string(i));
        }

        // execute 1 slow command
        inherited::executeCommand(slowcmd);

        // execute again 10 quick command   
        for (auto i=0; i<1000; ++i) {
          inherited::executeCommand(std::to_string(i));
        }
        once = false;
      }
    }
    ERS_INFO("Finished.");
  }

protected:
  typedef CommandFacility inherited;

  void completionCallback(const std::string& result) {
    ERS_INFO("Dummy handler just prints out result of cmd: " << result);
  }

};

extern "C" {
  std::shared_ptr<dunedaq::cmdlib::CommandFacility> make(std::string uri) {
    return std::shared_ptr<dunedaq::cmdlib::CommandFacility>(new dummyCommandFacility(uri));
  }
}
