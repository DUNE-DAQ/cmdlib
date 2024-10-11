/**
 * @file dummy_command_facility_.cxx Dummy command facility
 * implementation of the CommandFacility interface
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include "cmdlib/CommandFacility.hpp"

#include "logging/Logging.hpp"
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
  explicit dummyCommandFacility(std::string uri, int n) :
  CommandFacility(uri),
  n_command{n} {
  }

  void run(std::atomic<bool>& end_marker) {
    TLOG() << "Going for a run... " << n_command;

    bool once = true;
    while (end_marker) {
      if (once) {
        // execute n_command quick commands
        for (auto i=0; i<n_command; ++i) {
          auto democmd = nlohmann::json::parse("{\"happy\": true, \"pi\": 3.141, \"count\": " + std::to_string(i) + "}");
          inherited::execute_command(democmd, cmd::CommandReply());
        }

        // execute 1 slow command
        auto slowcmd = nlohmann::json::parse("{\"asd\": true}");
        inherited::execute_command(slowcmd, cmd::CommandReply());

        // execute again n_command quick command
        for (auto i=0; i<n_command; ++i) {
          auto democmd = nlohmann::json::parse("{\"happy\": true, \"pi\": 3.141, \"count\": " + std::to_string(i) + "}");
          inherited::execute_command(democmd, cmd::CommandReply());
        }
        once = false;
      }
    }

    TLOG_DEBUG(1) <<"Finished.";
  }

protected:
  typedef CommandFacility inherited;
  int n_command;

  void completion_callback(const cmdobj_t& cmd, cmd::CommandReply& meta) {
    TLOG() << "Command " << cmd << "\nexecution resulted with: " << meta.result;
  }

};

extern "C" {
  std::shared_ptr<dunedaq::cmdlib::CommandFacility> make(std::string uri, int n) {
    return std::shared_ptr<dunedaq::cmdlib::CommandFacility>(new dummyCommandFacility(uri, n));
  }
}
