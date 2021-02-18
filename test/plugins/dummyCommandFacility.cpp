/**
 * @file dummy_command_facility_.cxx Dummy command facility
 * implementation of the CommandFacility interface
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include "cmdlib/CommandFacility.hpp"

#include <ers/ers.h>
#include <nlohmann/json.hpp>
#include <cetlib/BasicPluginFactory.h>

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

  void run(std::atomic<bool>& end_marker) {
    ERS_INFO("Going for a run...");
    auto democmd = nlohmann::json::parse("{\"happy\": true, \"pi\": 3.141 }");
    auto slowcmd = nlohmann::json::parse("{\"asd\": true}");

    bool once = true;
    while (end_marker) {
      if (once) {
        // execute 10 quick commands
        for (auto i=0; i<1000; ++i) {
          inherited::execute_command(democmd, cmdmeta_t());
        }

        // execute 1 slow command
        inherited::execute_command(slowcmd, cmdmeta_t());

        // execute again 10 quick command   
        for (auto i=0; i<1000; ++i) {
          inherited::execute_command(democmd, cmdmeta_t());
        }
        once = false;
      }
    }
    ERS_INFO("Finished.");
  }

protected:
  typedef CommandFacility inherited;

  void completion_callback(const cmdobj_t& cmd, cmdmeta_t& meta) {
    ERS_INFO("Command " << cmd << "\nexecution resulted with: " << meta["result"]);
  }

};

extern "C" {
  std::shared_ptr<dunedaq::cmdlib::CommandFacility> make(std::string uri) {
    return std::shared_ptr<dunedaq::cmdlib::CommandFacility>(new dummyCommandFacility(uri));
  }
}
